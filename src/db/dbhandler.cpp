/*
 * DBHandler.cpp
 *
 *  Created on: Aug 25, 2012
 *      Author: ilan
 */

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>
#include <sstream>

#include <cstring>
#include <cstdlib>

#include "util.h"
#include "dbhandler.h"
#include "../exceptions/sql-connection.h"

using namespace Glib;
using namespace std;

DBHandler *DBHandler::m_handle = NULL;

DBHandler::DBHandler(const string& dbname, bool use_fk_constraint) : m_dbname(dbname), m_db(NULL), m_fkConstraint(use_fk_constraint)
{
}

DBHandler& DBHandler::get_instance(const string& dbpath)
{
	if(!m_handle) {
		m_handle = new DBHandler(dbpath, true);
	}
	return *DBHandler::m_handle;
}

DBHandler& DBHandler::get_instance()
{
	if(!m_handle) {
		//throw (std::exception((string)"enable to get instance"));
		throw (std::exception());
	}
	return *m_handle;
}
void DBHandler::finalize(void)
{
	if(m_handle != NULL) {
		m_handle->close();
		delete m_handle;
		m_handle = NULL;
	} else
		//throw (std::exception((string)"The singleton class has not been initialized."));
		throw (std::exception());
}

bool DBHandler::open(void)
{
	int res;

	if(m_db != NULL)
		throw(SqlConnectionOpenedException());

	res = sqlite3_open(m_dbname.c_str(), &m_db);
	if(res == SQLITE_OK) {
		// Activating Foreign Key Constraints
		ustring sql("PRAGMA foreign_keys=ON;");
		sqlite3_stmt *stmt;

		if(sqlite3_prepare_v2(m_db, sql.c_str(), sql.bytes(), &stmt, NULL) == SQLITE_OK) {
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}
	return res == SQLITE_OK;
}

void DBHandler::close(void)
{
	if(m_db != NULL) {
		if(sqlite3_close(m_db) == SQLITE_OK) {  
			m_db = NULL;
		}
		else
			std::cout<< "Erro while closing the database..."<< sqlite3_errmsg(m_db)<< std::endl;
	} else
		throw(SqlConnectionClosedException());
}

const ustring& DBHandler::get_database_path(void) const
{
	return m_dbname;
}

int DBHandler::person_insert(const Person& p) const
{
	int res = 0;
	bool shouldRollback = false;

	/*TODO: Handle the errors correctly */
	if(!p.validate())
		throw std::invalid_argument("Os dados da pessoa são inválidos.");

	if(m_db != NULL) {
		stringstream ss;
		ss << p.get_height();

		string qBegin = "BEGIN IMMEDIATE TRANSACTION;";
		string query = "INSERT INTO Person( " \
						"Name, Address, Zip1, Zip2, Location, Sex, Height, Birthday, " \
						"Birthplace, Nationality, Profession, TaxNumber, Referer, " \
						"Email, RefMaritalStatusID, RefBloodTypeID) " \
						"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
		string qPhones = "INSERT INTO Contact(ContactNumber, RefPersonID, RefNumberTypeID, RefCountryCode) VALUES(?,?,?,?)";
		string qFinish = "COMMIT TRANSACTION;";

		sqlite3_stmt *stmt, *stmtB, *stmtP, *stmtE;

		if(sqlite3_prepare_v2(m_db, qBegin.c_str(), -1, &stmtB, NULL) == SQLITE_OK && sqlite3_step(stmtB) != SQLITE_DONE) {
			//Error could start transaction...
			if(stmtB != NULL)
				sqlite3_finalize(stmtB);
			return -1;
		}

		if((res = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, NULL)) == SQLITE_OK) {
			guint32 pid = 0;
			guint16 zip1, zip2;
			guint32 phones[2] = {p.get_phone(), p.get_cellphone()};

			p.get_zip(zip1, zip2);	
			sqlite3_bind_text(stmt, 1, p.get_name().c_str(), p.get_name().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 2, p.get_address().c_str(), p.get_address().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 3, zip1);
			sqlite3_bind_int(stmt, 4, zip2);
			sqlite3_bind_text(stmt, 5, p.get_locality().c_str(), p.get_locality().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 6, p.get_sex() ? 1:0);
			sqlite3_bind_double(stmt, 7, p.get_height());
			sqlite3_bind_text(stmt, 8, p.get_birthday().format_string((ustring)"%d/%m/%Y").c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 9, p.get_birthplace().c_str(), p.get_birthplace().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 10, p.get_nationality().c_str(), p.get_nationality().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 11, p.get_profession().c_str(), p.get_profession().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 12, p.get_tax_number());
			if(p.get_referer().length()>0)
				sqlite3_bind_text(stmt, 13, p.get_referer().c_str(), p.get_referer().bytes(), SQLITE_TRANSIENT);
			else
				sqlite3_bind_text(stmt, 13, NULL, -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 14, p.get_email().c_str(), p.get_email().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 15, p.get_marital_status());
			sqlite3_bind_int(stmt, 16, p.get_blood_type());

			if(sqlite3_step(stmt) == SQLITE_DONE) {
				res = 1;

				pid = sqlite3_last_insert_rowid(m_db);

				if(sqlite3_prepare_v2(m_db, qPhones.c_str(), -1, &stmtP, NULL) == SQLITE_OK) {
					int errCode;
					for(int i=0; i< 2; i++) {
						sqlite3_bind_int(stmtP, 1, phones[i]);
						sqlite3_bind_int(stmtP, 2, pid);
						sqlite3_bind_int(stmtP, 3, i + 1);
						sqlite3_bind_int(stmtP, 4, 351);
						if((errCode = sqlite3_step(stmtP)) == SQLITE_DONE)
							sqlite3_reset(stmtP);
						else if(errCode == SQLITE_ERROR) {
							shouldRollback = true;
							break;
						}
					}
					sqlite3_finalize(stmtP);
				} else {
					shouldRollback = true;
					std::cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
					sqlite3_finalize(stmtP);
				}

				sqlite3_finalize(stmt);
			} else {
				shouldRollback = true;
				std::cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
				//sqlite3_finalize(stmt);
			}

			if(shouldRollback)
				qFinish = "ROLLBACK TRANSACTION;";
			else
				m_signal_person_added.emit(pid, p.get_name(), p.get_tax_number());

			// End the transaction... either by issuing a rollback or commit.
			if(sqlite3_prepare_v2(m_db, qFinish.c_str(), -1, &stmtE, NULL) == SQLITE_OK && sqlite3_step(stmtE) != SQLITE_DONE) {
				//Error could start transaction...
				if(stmtB != NULL)
					sqlite3_finalize(stmtB); // clean up
				return -1;
			}

			//cleanup
			sqlite3_finalize(stmtB);
			sqlite3_finalize(stmtE);
		} else {
			std::cout<< "Error (" << res <<") while inserting..."<< std::endl<< "'" << query <<"'"<<endl;
			sqlite3_finalize(stmtB);
		}
	} else
		throw (SqlConnectionClosedException());

	return res;
}

int DBHandler::visit_insert(VisitInterface& v) const
{
	int res = 0;
	bool shouldRollback = false;

	/*TODO: Handle the errors correctly */
	//if(!v.validate())
	//	throw std::invalid_argument("Os dados da visita são inválidos.");

	if(m_db != NULL) {
		int visitID;
		string qBegin = "BEGIN IMMEDIATE TRANSACTION;";
		string query = "INSERT INTO Visits( " \
						"RefPersonID, Complaint, Anamnesis, VisitDate, Weight, PhysicalAppearance, " \
						"Movement, Voice, Smell, Hypertension, Cholesterol, Triglyceride, " \
						"Diabetes, Sleepiness, Transpiration, Dehydration, Anxiety, Irrt, " \
						"Frustration, Cry, Verm, Ved, Brad, Prt, Aml, Alg, Irritable, Sad, " \
						"Med, Melan, Hearing, Throat, Scent, Vision, Fatigue, SexualActivity, " \
						"Body, Abdomen, Head, Circulation, EatingHabits," \
						"Menstruation,Pregnancy,Surgery," \
						"PreviousTreatment,Prostheses,Weight2,Urine,Faeces,Tongue," \
						"PulseD,PulseE,Apal,Exams,ClinicalAnalysis,Color," \
						"Escle,Observations,MedTxt,Medication,Treatment) " \
						"VALUES(?, ?, ?, ?, ?, ?, " \
								"?, ?, ?, ?, ?, ?, ?, ?, " \
								"?, ?, ?, ?, ?, ?, ?, ?, " \
								"?, ?, ?, ?, ?, ?, ?, ?, " \
								"?, ?, ?, ?, ?, ?, ?, ?, " \
								"?, ?, ?, ?, ?, ?, ?, ?, " \
								"?, ?, ?, ?, ?, ?, ?, ?, " \
								"?, ?, ?, ?, ?, ?, ?); ";
		string queryPain = "INSERT INTO Pain(Type,Since,Observation,RefVisitID)" \
						" VALUES(?, ?, ?, ?);";
		string queryBP = "INSERT INTO BloodPressure(High, Low, BPM, RefVisitID)" \
						" VALUES(?, ?, ?, ?);";
		string qFinish = "COMMIT TRANSACTION;";

		sqlite3_stmt *stmt, *stmtB, *stmtE;

		if(sqlite3_prepare_v2(m_db, qBegin.c_str(), -1, &stmtB, NULL) == SQLITE_OK && sqlite3_step(stmtB) != SQLITE_DONE) {
			//Error could start transaction...
			if(stmtB != NULL)
				sqlite3_finalize(stmtB);
			return -1;
		}

		if((res = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, NULL)) == SQLITE_OK) {
			
			sqlite3_bind_int(stmt, 1, v.getPersonID());
			sqlite3_bind_text(stmt, 2, v.getComplaint().c_str(), v.getComplaint().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 3, v.getAnamnesis().c_str(), v.getAnamnesis().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 4, v.getDate().c_str(), v.getDate().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_double(stmt, 5, v.getWeight());

			sqlite3_bind_text(stmt, 6, v.getAppearance().c_str(), v.getAppearance().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 7, v.getMovement().c_str(), v.getMovement().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 8, v.getVoice().c_str(), v.getVoice().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 9, v.getSmell().c_str(), v.getSmell().bytes(), SQLITE_TRANSIENT);

			sqlite3_bind_int(stmt, 10, v.getHypertension());
			sqlite3_bind_int(stmt, 11, v.getCholesterol());
			sqlite3_bind_int(stmt, 12, v.getTriglyceride());
			sqlite3_bind_int(stmt, 13, v.getDiabetes());

			sqlite3_bind_text(stmt, 14, v.getSleepiness().c_str(), v.getSleepiness().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 15, v.getTranspiration().c_str(), v.getTranspiration().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 16, v.getDehydration().c_str(), v.getDehydration().bytes(), SQLITE_TRANSIENT);

			sqlite3_bind_int(stmt, 17, v.isAnxiety());
			sqlite3_bind_int(stmt, 18, v.isIrrt());
			sqlite3_bind_int(stmt, 19, v.isFrustration());
			sqlite3_bind_int(stmt, 20, v.isCry());
			sqlite3_bind_int(stmt, 21, v.isVerm());
			sqlite3_bind_int(stmt, 22, v.isVed());
			sqlite3_bind_int(stmt, 23, v.isBrad());
			sqlite3_bind_int(stmt, 24, v.isPrt());
			sqlite3_bind_int(stmt, 25, v.isAml());
			sqlite3_bind_int(stmt, 26, v.isAlg());
			sqlite3_bind_int(stmt, 27, v.isIrritable());
			sqlite3_bind_int(stmt, 28, v.isSad());
			sqlite3_bind_int(stmt, 29, v.isMed());
			sqlite3_bind_int(stmt, 30, v.isMelan());

			sqlite3_bind_text(stmt, 31, v.getHearing().c_str(), v.getHearing().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 32, v.getThroat().c_str(), v.getThroat().bytes(), SQLITE_TRANSIENT); 
			sqlite3_bind_text(stmt, 33, v.getScent().c_str(), v.getScent().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 34, v.getVision().c_str(), v.getVision().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 35, v.getFatigue().c_str(), v.getFatigue().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 36, v.getSexualActivity().c_str(), v.getSexualActivity().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 37, v.getBody().c_str(), v.getBody().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 38, v.getAbdomen().c_str(), v.getAbdomen().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 39, v.getHead().c_str(), v.getHead().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 40, v.getCirculation().c_str(), v.getCirculation().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 41, v.getEatingHabits().c_str(), v.getEatingHabits().bytes(), SQLITE_TRANSIENT);

			sqlite3_bind_text(stmt, 42, v.getMenstruation().c_str(), v.getMenstruation().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 43, v.getPregnancy().c_str(), v.getPregnancy().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 44, v.getSurgery().c_str(), v.getSurgery().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 45, v.getPreviousTreatment().c_str(), v.getPreviousTreatment().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 46, v.getProstheses());
			sqlite3_bind_int(stmt, 47, v.getWeightBool());
			sqlite3_bind_text(stmt, 48, v.getUrine().c_str(), v.getUrine().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 49, v.getFaeces().c_str(), v.getFaeces().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 50, v.getTongue().c_str(), v.getTongue().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 51, v.getPulseD().c_str(), v.getPulseD().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 52, v.getPulseE().c_str(), v.getPulseE().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 53, v.getApal().c_str(), v.getApal().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 54, v.getExams().c_str(), v.getExams().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 55, v.getClinicalAnalysis().c_str(), v.getClinicalAnalysis().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 56, v.getColor().c_str(), v.getColor().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 57, v.getEscle().c_str(), v.getEscle().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 58, v.getObservations().c_str(), v.getObservations().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 59, v.getMed().c_str(), v.getMed().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 60, v.getMedication().c_str(), v.getMedication().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 61, v.getTreatment().c_str(), v.getTreatment().bytes(), SQLITE_TRANSIENT);

			if(sqlite3_step(stmt) == SQLITE_DONE) {
				res = 1;
			} else {
				shouldRollback = true;
				std::cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
			}
			sqlite3_finalize(stmt);

			visitID = get_visit_from_person(v.getPersonID());

			/************************ inserting pain info ************************/
			if((res = sqlite3_prepare_v2(m_db, queryPain.c_str(), -1, &stmt, NULL)) == SQLITE_OK) {
				sqlite3_bind_text(stmt, 1, v.getPain().c_str(), v.getPain().bytes(), SQLITE_TRANSIENT);
				sqlite3_bind_text(stmt, 2, v.getPainSince().c_str(), v.getPainSince().bytes(), SQLITE_TRANSIENT);
				sqlite3_bind_text(stmt, 3, v.getPainObs().c_str(), v.getPainObs().bytes(), SQLITE_TRANSIENT);
				sqlite3_bind_int(stmt, 4, visitID);
				if(sqlite3_step(stmt) == SQLITE_DONE)
					res = 1;
				else
					shouldRollback = true;
				sqlite3_finalize(stmt);
			} else {
				shouldRollback = true;
				std::cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
				sqlite3_finalize(stmt);
			}
			
			/************************  end of pain info  ************************/

			/************************ inserting blood pressure info ************************/
			if((res = sqlite3_prepare_v2(m_db, queryBP.c_str(), -1, &stmt, NULL)) == SQLITE_OK) {
				sqlite3_bind_int(stmt, 1, v.getBPMax());
				sqlite3_bind_int(stmt, 2, v.getBPMin());
				sqlite3_bind_int(stmt, 3, v.getBPM());
				sqlite3_bind_int(stmt, 4, visitID);
				if(sqlite3_step(stmt) == SQLITE_DONE)
					res = 1;
				else {
					std::cerr<< "Error executing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
					shouldRollback = true;
				}
				sqlite3_finalize(stmt);
			} else {
				shouldRollback = true;
				std::cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
				sqlite3_finalize(stmt);
			}
			
			/************************  end of blood pressure info  ************************/


			if(shouldRollback)
				qFinish = "ROLLBACK TRANSACTION;";
			else 
				m_signal_visit_added.emit(visitID, v.getComplaint(), v.getDate());
			// End the transaction... either by issuing a rollback or commit.
			if(sqlite3_prepare_v2(m_db, qFinish.c_str(), -1, &stmtE, NULL) == SQLITE_OK && sqlite3_step(stmtE) != SQLITE_DONE) {
				//Error could start transaction...
				if(stmtB != NULL)
					sqlite3_finalize(stmtB); // clean up
				return -1;
			}

			//cleanup
			sqlite3_finalize(stmtB);
			sqlite3_finalize(stmtE);
		} else {
			std::cout<< "Error (" << res <<") while inserting" <<endl <<"Message: "<< sqlite3_errmsg(m_db) << std::endl<< "'" << query <<"'"<<endl;
			sqlite3_finalize(stmtB);
		}
	} else
		throw (SqlConnectionClosedException());

	return res;
}

int DBHandler::person_update(const Person& p) const
{
	int res = 0;
	bool shouldRollback = false;

	/*TODO: Handle the errors correctly */
	if(!p.validate())
		throw std::invalid_argument("Os dados da pessoa são inválidos.");

	if(m_db != NULL) {
		stringstream ss;
		ss << p.get_height();

		string qBegin = "BEGIN IMMEDIATE TRANSACTION;";
		string query = "UPDATE Person " \
						"SET Name = ?, Address = ?, Zip1 = ?, Zip2 = ?, Location = ?, Sex = ?, Height = ?, Birthday = ?, " \
						"Birthplace = ?, Nationality = ?, Profession = ?, TaxNumber = ?, Referer = ?, " \
						"Email = ?, RefMaritalStatusID = ?, RefBloodTypeID = ? WHERE PersonID = ?;' ";
		string qPhones = "UPDATE Contact SET ContactNumber = ? WHERE RefPersonID = ? AND RefNumberTypeID = ?;";
		string qFinish = "COMMIT TRANSACTION;";

		sqlite3_stmt *stmt, *stmtB, *stmtP, *stmtE;

		if(sqlite3_prepare_v2(m_db, qBegin.c_str(), -1, &stmtB, NULL) == SQLITE_OK && sqlite3_step(stmtB) != SQLITE_DONE) {
			//TODO: Throw exception error could start transaction...
			if(stmtB != NULL)
				sqlite3_finalize(stmtB);
			return -1;
		}

		if((res = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, NULL)) == SQLITE_OK) {
			guint32 phones[2] = {p.get_phone(), p.get_cellphone()};
			guint16 zip1, zip2;

			p.get_zip(zip1, zip2);
			
			sqlite3_bind_text(stmt, 1, p.get_name().c_str(), p.get_name().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 2, p.get_address().c_str(), p.get_address().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 3, zip1);
			sqlite3_bind_int(stmt, 4, zip2);
			sqlite3_bind_text(stmt, 5, p.get_locality().c_str(), p.get_locality().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 6, p.get_sex() ? 1:0);
			sqlite3_bind_double(stmt, 7, p.get_height());
			sqlite3_bind_text(stmt, 8, p.get_birthday().format_string((ustring)"%d/%m/%Y").c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 9, p.get_birthplace().c_str(), p.get_birthplace().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 10, p.get_nationality().c_str(), p.get_nationality().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 11, p.get_profession().c_str(), p.get_profession().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 12, p.get_tax_number());
			if(p.get_referer().length()>0)
				sqlite3_bind_text(stmt, 13, p.get_referer().c_str(), p.get_referer().bytes(), SQLITE_TRANSIENT);
			else
				sqlite3_bind_text(stmt, 13, NULL, -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 14, p.get_email().c_str(), p.get_email().bytes(), SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 15, p.get_marital_status());
			sqlite3_bind_int(stmt, 16, p.get_blood_type());

			sqlite3_bind_int(stmt, 17, p.get_id());

			if(sqlite3_step(stmt) == SQLITE_DONE)
				res = 1;
			else
				shouldRollback = true;
			sqlite3_finalize(stmt);

			if(user_has_contact(p)) {
				if(!shouldRollback && sqlite3_prepare_v2(m_db, qPhones.c_str(), -1, &stmtP, NULL) == SQLITE_OK) {
					int errCode;
					for(int i = 1; i <= 2; i++) {
						sqlite3_bind_int(stmtP, 1, phones[i-1]);
						sqlite3_bind_int(stmtP, 2, p.get_id());
						sqlite3_bind_int(stmtP, 3, i);
						if((errCode = sqlite3_step(stmtP)) == SQLITE_DONE)
							sqlite3_reset(stmtP);
						else if(errCode == SQLITE_ERROR) {
							shouldRollback = true;
							break;
						}
					}
					sqlite3_finalize(stmtP);
				} else {
					shouldRollback = true;
					cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<endl;
					sqlite3_finalize(stmt);
				}
			} else {
				qPhones = "INSERT INTO Contact(ContactNumber, RefPersonID, RefNumberTypeID, RefCountryCode) " \
						  "VALUES(?,?,?,?)";

				if(sqlite3_prepare_v2(m_db, qPhones.c_str(), -1, &stmtP, NULL) == SQLITE_OK) {
					res = 1;
					
					int errCode;
					for(int i=0; i< 2; i++) {
						sqlite3_bind_int(stmtP, 1, phones[i]);
						sqlite3_bind_int(stmtP, 2, p.get_id());
						sqlite3_bind_int(stmtP, 3, i + 1);
						sqlite3_bind_int(stmtP, 4, 351);
						if((errCode = sqlite3_step(stmtP)) == SQLITE_DONE)
							sqlite3_reset(stmtP);
						else if(errCode == SQLITE_ERROR) {
							shouldRollback = true;
							break;
						}
					}
					sqlite3_finalize(stmtP);
				} else {
					shouldRollback = true;
					std::cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
					sqlite3_finalize(stmtP);
				}
			}


			if(shouldRollback) {
				cerr<< "Warning: something went wrong, rolling back the transaction."<<
						endl<< "\tLast database error: "<< sqlite3_errmsg(m_db);
				qFinish = "ROLLBACK TRANSACTION;";
			}

			// End the transaction... either by issuing a rollback or commit.
			if((res = (sqlite3_prepare_v2(m_db, qFinish.c_str(), qFinish.size(), &stmtE, NULL) == SQLITE_OK && sqlite3_step(stmtE) != SQLITE_DONE))) {
				//Error could start transaction...
				if(stmtB != NULL)
					sqlite3_finalize(stmtB); // clean up
				return -1;
			}

			//cleanup
			sqlite3_finalize(stmtB);
			sqlite3_finalize(stmtE);

			if(!shouldRollback && !res)
				m_signal_person_edit(p);
		} else {
			std::cout<< "Error (" << res <<") while updating..."<< std::endl<< "'" << query <<"'"<<endl;
			sqlite3_finalize(stmtB);
		}
	} else
		throw (SqlConnectionClosedException());

	return res;
}

bool DBHandler::get_person(const guint32 id, Person& p) const
{
	bool res = false;
	sqlite3_stmt *stmt;
	ustring tmp;

	if(m_db != NULL) {
		string query = "SELECT Name, Address, Zip1, Zip2, Location, Sex, Height, Birthday, Birthplace, Nationality, " \
					"Profession, TaxNumber, Referer, Email, RefBloodTypeID, RefMaritalStatusID FROM Person WHERE PersonID=?";

		if(sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL) == SQLITE_OK) {
			int val(SQLITE_ROW);
			guint16 zip1, zip2;
			sqlite3_bind_int(stmt, 1, id);

			while(val == SQLITE_ROW) {
				val = sqlite3_step(stmt);
				switch(val) {
				case SQLITE_ROW: {
					p.set_id(id);
					p.set_name(ustring((const char*)sqlite3_column_text(stmt, 0)));
					p.set_address(ustring((const char*)sqlite3_column_text(stmt, 1)));
					zip1 = sqlite3_column_int(stmt, 2);
					zip2 = sqlite3_column_int(stmt, 3);
					p.set_zip(zip1, zip2);
					p.set_locality(ustring((const char*)sqlite3_column_text(stmt, 4)));
					p.set_sex((bool)sqlite3_column_int(stmt, 5));
					p.set_height((float)sqlite3_column_double(stmt, 6));
					p.set_birthday(Util::parse_date(string((const char*)sqlite3_column_text(stmt, 7))));
					p.set_birthplace(ustring((const char*)sqlite3_column_text(stmt, 8)));
					p.set_nationality(ustring((const char*)sqlite3_column_text(stmt, 9)));
					p.set_profession(ustring((const char*)sqlite3_column_text(stmt, 10)));
					p.set_tax_number((guint32)sqlite3_column_int(stmt, 11));
					if(sqlite3_column_text(stmt, 12) != NULL)
						p.set_referer(ustring((const char*)sqlite3_column_text(stmt, 12)));
					p.set_email(ustring((const char *)sqlite3_column_text(stmt, 13)));
					p.set_blood_type(sqlite3_column_int(stmt, 14));
					p.set_marital_status(sqlite3_column_int(stmt, 15));
					break;
				}
				case SQLITE_DONE: {
					res = true;
					break;
				}
				case SQLITE_ERROR:
					res = false;
					break;
				}
			}
			sqlite3_finalize(stmt);
			query = "SELECT ContactNumber FROM Contact WHERE RefPersonID = ? ORDER BY RefNumberTypeID";
			if(sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL) == SQLITE_OK) {
				int val(SQLITE_ROW);
				short phone=1;
				sqlite3_bind_int(stmt, 1, id);

				while(val == SQLITE_ROW) {
					val = sqlite3_step(stmt);
					switch(val) {
					case SQLITE_ROW: {
						if(phone){
							p.set_phone((guint32)sqlite3_column_int(stmt, 0));
							phone = 0;
						} else
							p.set_cellphone((guint32)sqlite3_column_int(stmt, 0));
						break;
					}
					case SQLITE_DONE: {
						res = true;
						break;
					}
					case SQLITE_ERROR:
						res = false;
						break;
					}
				}
				sqlite3_finalize(stmt);
			}
		}
	} else
		throw (SqlConnectionClosedException());

	return res;
}

bool DBHandler::person_remove(unsigned int id) const
{
	bool res = false;

	/*TODO: Handle errors correctly */

	if(m_db != NULL) {
		sqlite3_stmt *stmt;
		string query = "DELETE FROM Person WHERE (PersonID = ?)";


		if(sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(stmt, 1, id);

			if(sqlite3_step(stmt) == SQLITE_DONE)
				res = true;
			sqlite3_finalize(stmt);
		}
	} else
		throw (SqlConnectionClosedException());

	return res;
}

bool DBHandler::visit_remove(unsigned int id) const
{
	bool res = false;

	/*TODO: Handle errors correctly */

	if(m_db != NULL) {
		sqlite3_stmt *stmt;
		string queryTransact = "BEGIN IMMEDIATE TRANSACTION;";
		string queryPain = "DELETE FROM Pain WHERE (RefVisitID = ?);";
		string queryBP = "DELETE FROM BloodPressure WHERE (RefVisitID = ?);";
		string query = "DELETE FROM Visits WHERE (VisitID = ?);";

		if(sqlite3_prepare_v2(m_db, queryTransact.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
			if(sqlite3_step(stmt) == SQLITE_DONE)
				res = true;
			sqlite3_finalize(stmt);
		} else {
			std::cout<< "Error while removing (init transaction) " <<endl <<"Message: "<< sqlite3_errmsg(m_db) << std::endl<< "'" << query <<"'"<<endl;
			return false;
		}
		if(sqlite3_prepare_v2(m_db, queryPain.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(stmt, 1, id);

			if(sqlite3_step(stmt) == SQLITE_DONE)
				res = true;
			sqlite3_finalize(stmt);
		} else {
			std::cout<< "Error while removing" <<endl <<"Message: "<< sqlite3_errmsg(m_db) << std::endl<< "'" << query <<"'"<<endl;
			goto cleanup;
		}

		if(sqlite3_prepare_v2(m_db, queryBP.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(stmt, 1, id);

			if(sqlite3_step(stmt) == SQLITE_DONE)
				res = true;
			sqlite3_finalize(stmt);
		} else {
			std::cout<< "Error while removing" <<endl <<"Message: "<< sqlite3_errmsg(m_db) << std::endl<< "'" << query <<"'"<<endl;
			goto cleanup;
		}

		if(sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(stmt, 1, id);

			if(sqlite3_step(stmt) == SQLITE_DONE)
				res = true;
			sqlite3_finalize(stmt);
		} else {
			std::cout<< "Error while removing" <<endl <<"Message: "<< sqlite3_errmsg(m_db) << std::endl<< "'" << query <<"'"<<endl;
			goto cleanup;
		}

cleanup:
		if(!res)
			queryTransact = "rollback transaction;";
		else
			queryTransact = "commit transaction;";

		if(sqlite3_prepare_v2(m_db, queryTransact.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}

	} else
		throw (SqlConnectionClosedException());

	return res;
}

void DBHandler::get_patients(const ustring *name) const
{
	/*TODO: throw exception if db is not opened*/
	ustring query = "SELECT PersonID, Name, TaxNumber FROM Person";

	if(m_db != NULL) {
		sqlite3_stmt *stmt;
		ustring t_name("%");

		if(name != NULL) {
			query += " WHERE Name LIKE ? ORDER BY Name ASC;";
		} else
			query += " ORDER BY Name ASC;";

		if(sqlite3_prepare_v2(m_db, query.c_str(), query.bytes(), &stmt, NULL) == SQLITE_OK) {
			int res;

			if(name != NULL) {
				t_name += *name + "%";
				sqlite3_bind_text(stmt, 1, t_name.c_str(), t_name.bytes(), SQLITE_TRANSIENT);
			}

			while(true)
				if(sqlite3_step(stmt) == SQLITE_ROW) {
					guint32 id = sqlite3_column_int(stmt, 0);
					char *name = (char *)sqlite3_column_text(stmt, 1);
					guint32 nif = sqlite3_column_int(stmt, 2);

					m_signal_person_added.emit(id, (ustring)name, nif);
				} else
					break; // Nothing else to do.
			sqlite3_finalize(stmt);
		}
	} else
		throw (SqlConnectionClosedException());
}

void DBHandler::get_visits(guint32 personID) const
{
	/*TODO: throw exception if db is not opened*/
	ustring query = "SELECT VisitID, Complaint, VisitDate FROM Visits ";

	if(m_db != NULL) {
		sqlite3_stmt *stmt;
		
		query += " WHERE RefPersonID = ? ORDER BY VisitDate ASC;";
		
		if(sqlite3_prepare_v2(m_db, query.c_str(), query.bytes(), &stmt, NULL) == SQLITE_OK) {
			int res;

			sqlite3_bind_int(stmt, 1, personID);

			while(true)
				if(sqlite3_step(stmt) == SQLITE_ROW) {
					guint32 id = sqlite3_column_int(stmt, 0);
					ustring complaint = ustring((char *)sqlite3_column_text(stmt, 1));
					ustring date = ustring((char *)sqlite3_column_text(stmt, 2));

					m_signal_visit_added.emit(id, complaint, date);
				} else
					break; // Nothing else to do.
			sqlite3_finalize(stmt);
		} else {
			cout<< "Error: " << sqlite3_errmsg(m_db) <<endl;
		}
	} else
		throw (SqlConnectionClosedException());
}

bool DBHandler::get_visit(int id, VisitInterface &v) const
{
	bool res = false;
	sqlite3_stmt *stmt;
	ustring tmp;

	if(m_db != NULL) {
		string query = "SELECT Complaint, Anamnesis, VisitDate, Weight, PhysicalAppearance, " \
						"Movement, Voice, Smell, Hypertension, Cholesterol, Triglyceride, " \
						"Diabetes, Sleepiness, Transpiration, Dehydration, Anxiety, Irrt, " \
						"Frustration, Cry, Verm, Ved, Brad, Prt, Aml, Alg, Irritable, Sad, " \
						"Med, Melan, Hearing, Throat, Scent, Vision, Fatigue, SexualActivity, " \
						"Body, Abdomen, Head, Circulation, EatingHabits, Type, Since, " \
						"Observation, Surgery, Weight2, Prostheses, PreviousTreatment, " \
						"Menstruation, Pregnancy, Urine, Faeces, Tongue, PulseD, PulseE, " \
						"Apal, Exams, ClinicalAnalysis, Color, Escle, Observations, " \
						"MedTxt, Medication, Treatment, High, Low, BPM " \
						"FROM Visits V INNER JOIN Pain P ON P.RefVisitID = V.VisitID " \
						"INNER JOIN BloodPressure BP ON BP.RefVisitID = V.VisitID " \
						"WHERE V.VisitID = ?";
		if(sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL) == SQLITE_OK) {
			int val(SQLITE_ROW);
			guint16 zip1, zip2;
			sqlite3_bind_int(stmt, 1, id);

			while(val == SQLITE_ROW) {
				val = sqlite3_step(stmt);
				switch(val) {
				case SQLITE_ROW: {

					v.setComplaint(ustring((const char*)sqlite3_column_text(stmt, 0)));
					v.setAnamnesis(ustring((const char*)sqlite3_column_text(stmt, 1)));
					v.setDate(ustring((const char*)sqlite3_column_text(stmt, 2)));
					v.setWeight(sqlite3_column_double(stmt, 3));
					v.setAppearance(ustring((const char*)sqlite3_column_text(stmt, 4)));
					v.setMovement(ustring((const char*)sqlite3_column_text(stmt, 5)));
					v.setVoice(ustring((const char*)sqlite3_column_text(stmt, 6)));
					v.setSmell(ustring((const char*)sqlite3_column_text(stmt, 7)));
					v.setHypertension(sqlite3_column_int(stmt, 8));
					v.setCholesterol(sqlite3_column_int(stmt, 9));
					v.setTriglyceride(sqlite3_column_int(stmt, 10));
					v.setDiabetes(sqlite3_column_int(stmt, 11));
					v.setSleepiness(ustring((const char*)sqlite3_column_text(stmt, 12)));
					v.setTranspiration(ustring((const char*)sqlite3_column_text(stmt, 13)));
					v.setDehydration(ustring((const char*)sqlite3_column_text(stmt, 14)));
					v.setAnxiety(sqlite3_column_int(stmt, 15));
					v.setIrrt(sqlite3_column_int(stmt, 16));
					v.setFrustration(sqlite3_column_int(stmt, 17));
					v.setCry(sqlite3_column_int(stmt, 18));
					v.setVerm(sqlite3_column_int(stmt, 19));
					v.setVed(sqlite3_column_int(stmt, 20));
					v.setBrad(sqlite3_column_int(stmt, 21));
					v.setPrt(sqlite3_column_int(stmt, 22));
					v.setAml(sqlite3_column_int(stmt, 23));
					v.setAlg(sqlite3_column_int(stmt, 24));
					v.setIrritable(sqlite3_column_int(stmt, 25));
					v.setSad(sqlite3_column_int(stmt, 26));
					v.setMed(sqlite3_column_int(stmt, 27));
					v.setMelan(sqlite3_column_int(stmt, 28));
					v.setHearing(ustring((const char*)sqlite3_column_text(stmt, 29)));
					v.setThroat(ustring((const char*)sqlite3_column_text(stmt, 30)));
					v.setScent(ustring((const char*)sqlite3_column_text(stmt, 31)));
					v.setVision(ustring((const char*)sqlite3_column_text(stmt, 32)));
					v.setFatigue(ustring((const char*)sqlite3_column_text(stmt, 33)));
					v.setSexualActivity(ustring((const char*)sqlite3_column_text(stmt, 34)));
					v.setBody(ustring((const char*)sqlite3_column_text(stmt, 35)));
					v.setAbdomen(ustring((const char*)sqlite3_column_text(stmt, 36)));
					v.setHead(ustring((const char*)sqlite3_column_text(stmt, 37)));
					v.setCirculation(ustring((const char*)sqlite3_column_text(stmt, 38)));
					v.setEatingHabits(ustring((const char*)sqlite3_column_text(stmt, 39)));
					v.setPain(ustring((const char*)sqlite3_column_text(stmt, 40)));
					v.setPainSince(ustring((const char*)sqlite3_column_text(stmt, 41)));
					v.setPainObs(ustring((const char*)sqlite3_column_text(stmt, 42)));
					v.setSurgery(ustring((const char*)sqlite3_column_text(stmt, 43)));
					v.setWeightBool((bool)sqlite3_column_int(stmt, 44));
					v.setProstheses((bool)sqlite3_column_int(stmt, 45));
					v.setPreviousTreatment(ustring((const char*)sqlite3_column_text(stmt, 46)));

					v.setMenstruation(ustring((const char*)sqlite3_column_text(stmt, 47)));
					v.setPregnancy(ustring((const char*)sqlite3_column_text(stmt, 48)));
					v.setUrine(ustring((const char*)sqlite3_column_text(stmt, 49)));
					v.setFaeces(ustring((const char*)sqlite3_column_text(stmt, 50)));
					v.setTongue(ustring((const char*)sqlite3_column_text(stmt, 51)));
					v.setPulseD(ustring((const char*)sqlite3_column_text(stmt, 52)));
					v.setPulseE(ustring((const char*)sqlite3_column_text(stmt, 53)));
					v.setApal(ustring((const char*)sqlite3_column_text(stmt, 54)));
					v.setExams(ustring((const char*)sqlite3_column_text(stmt, 55)));
					v.setClinicalAnalysis(ustring((const char*)sqlite3_column_text(stmt, 56)));
					v.setColor(ustring((const char*)sqlite3_column_text(stmt, 57)));
					v.setEscle(ustring((const char*)sqlite3_column_text(stmt, 58)));
					v.setObservations(ustring((const char*)sqlite3_column_text(stmt, 59)));
					v.setMed(ustring((const char*)sqlite3_column_text(stmt, 60)));
					v.setMedication(ustring((const char*)sqlite3_column_text(stmt, 61)));
					v.setTreatment(ustring((const char*)sqlite3_column_text(stmt, 62)));
					v.setBloodPressure(sqlite3_column_int(stmt, 63), sqlite3_column_int(stmt, 64), sqlite3_column_int(stmt, 65));

					break;
				}
				case SQLITE_DONE: {
					res = true;
					break;
				}
				case SQLITE_ERROR:
					cerr<< "Error executing the statement: "<< sqlite3_errmsg(m_db)<<endl;
					res = false;
					break;
				}
			}
			sqlite3_finalize(stmt);
		} else {
			cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<endl;
		}
	} else
		throw (SqlConnectionClosedException());

	return res;
}


sigc::signal<void, guint32, const ustring&, guint32>& DBHandler::signal_person_added()
{
	return m_signal_person_added;
}

sigc::signal<void, guint32, const ustring&, const ustring&>& DBHandler::signal_visit_added()
{
	return m_signal_visit_added;
}

sigc::signal<void, const Person&> DBHandler::signal_person_edited()
{
	return m_signal_person_edit;
}


inline bool DBHandler::user_has_contact(const Person& p) const
/* Protected methods */
{
	sqlite3_stmt *stmt;
	bool hasContactInfo = false;

	if(m_db != NULL) {
		string sqlStr = "SELECT ContactNumber FROM Contact WHERE RefPersonID = ? ORDER BY RefNumberTypeID";
		if(sqlite3_prepare_v2(m_db, sqlStr.c_str(), sqlStr.size(), &stmt, NULL) == SQLITE_OK) {
			int val(SQLITE_ROW);
			short phone=1;
			sqlite3_bind_int(stmt, 1, p.get_id());

			while(val == SQLITE_ROW) {
				val = sqlite3_step(stmt);
				switch(val) {
				case SQLITE_ROW: {
					hasContactInfo = true;
					break;
				}
				case SQLITE_DONE: {
					break;
				}
				case SQLITE_ERROR:
					break;
				}
			}
		}
	
		sqlite3_finalize(stmt);
	} else
		throw (SqlConnectionClosedException());

	return hasContactInfo;
}

guint DBHandler::get_visit_from_person(gint PersonID) const
{
	string query = "select VisitID from Visits where RefPersonID = ? ORDER BY ROWID Desc  limit 1";
	sqlite3_stmt *stmt;
	int res = -1;

	if(m_db != NULL) {
		if(sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL) == SQLITE_OK) {
			int val(SQLITE_ROW);
	
			sqlite3_bind_int(stmt, 1, PersonID);
			while(val == SQLITE_ROW) {
				val = sqlite3_step(stmt);
				switch(val) {
				case SQLITE_ROW: {
					res = sqlite3_column_int(stmt, 0);
					break;
				}
				case SQLITE_DONE: {
					break;
				}
				case SQLITE_ERROR:
					break;
				}
			}
			sqlite3_finalize(stmt);
		} else {
			cerr<< "Warning: something went wrong."<<
				endl<< "\tLast database error: "<< sqlite3_errmsg(m_db);
		}
	} else
		throw (SqlConnectionClosedException());

	return res;
}

void DBHandler::configuration_update(Configuration& c)
{
	string query = "update configuration set Value = ? where Key = ? ";

	sqlite3_stmt *stmt;
	int posx, posy, width, height;
	string tmp, tmp2;
	int i = 0, val;

	if(m_db != NULL) {
		c.get_window_size(width, height);
		c.get_window_position(posx, posy);
		if(sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL) == SQLITE_OK) {
			while(i < 5) {
				switch(i) {
					case 0:
						tmp = std::to_string(posx);
						sqlite3_bind_text(stmt, 1, tmp.c_str(), tmp.size(), SQLITE_TRANSIENT);
						tmp2 = "PosX";
						sqlite3_bind_text(stmt, 2, tmp2.c_str(), tmp2.size(), SQLITE_TRANSIENT);
						break;
					case 1:
						tmp = std::to_string(posy);
						sqlite3_bind_text(stmt, 1, tmp.c_str(), tmp.size(), SQLITE_TRANSIENT);
						tmp2 = "PosY";
						sqlite3_bind_text(stmt, 2, tmp2.c_str(), tmp2.size(), SQLITE_TRANSIENT);
						break;
					case 2:
						tmp = std::to_string(width);
						sqlite3_bind_text(stmt, 1, tmp.c_str(), tmp.size(), SQLITE_TRANSIENT);
						tmp2 = "Width";
						sqlite3_bind_text(stmt, 2, tmp2.c_str(), tmp2.size(), SQLITE_TRANSIENT);
						break;
					case 3:
						tmp = std::to_string(height);
						sqlite3_bind_text(stmt, 1, tmp.c_str(), tmp.size(), SQLITE_TRANSIENT);
						tmp2 = "Height";
						sqlite3_bind_text(stmt, 2, tmp2.c_str(), tmp2.size(), SQLITE_TRANSIENT);
						break;
					case 4:
						tmp = std::to_string(c.get_window_maximized());
						sqlite3_bind_text(stmt, 1, tmp.c_str(), tmp.size(), SQLITE_TRANSIENT);
						tmp2 = "Maximized";
						sqlite3_bind_text(stmt, 2, tmp2.c_str(), tmp2.size(), SQLITE_TRANSIENT);
						break;
				}
				val = sqlite3_step(stmt);
				switch(val)
				{
					case SQLITE_DONE:
						i++;
						break;
					case SQLITE_ERROR:
						cout<< "Erro while updating..."<< endl;
						i=5;
						break;
				}
				sqlite3_reset(stmt);
			}
			sqlite3_finalize(stmt);
		}
		
	} else
		throw (SqlConnectionClosedException());
}

void DBHandler::get_configuration(Configuration& c)
{
	string query = "select Key, Value from configuration ";
	sqlite3_stmt *stmt;
	string tmp, tmp2;
	int i = 0, val;
	int posx(0);
	int posy(0);
	int width(0);
	int height(0);
	int maximized(0);

	if(m_db != NULL) {
		if(sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL) == SQLITE_OK) {

			val = sqlite3_step(stmt);
			while(val == SQLITE_ROW) {
				const char *key = (const char*)sqlite3_column_text(stmt, 0);
				const char *value = (const char*)sqlite3_column_text(stmt, 1);

				if(!strcmp(key, "PosX")) {
					stringstream ss;
					ss << value;
					ss >> posx;
				} else if(!strcmp(key, "PosY")) {
					stringstream ss;
					ss << value;
					ss >> posy;
				} else if(!strcmp(key, "Width")) {
					stringstream ss;
					ss << value;
					ss >> width;
				} else if(!strcmp(key, "Height")) {
					stringstream ss;
					ss << value;
					ss >> height;
				} else if(!strcmp(key, "Maximized")) {
					stringstream ss;
					ss << value;
					ss >> maximized;
				}
				val = sqlite3_step(stmt);
			}
			//c.set_window_move(posx, posy);
			c.set_window_resize(width, height);
			c.set_window_maximized(maximized);
			sqlite3_finalize(stmt);
		}
		
	} else
		throw (SqlConnectionClosedException());
}