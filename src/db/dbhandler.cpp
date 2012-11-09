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

DBHandler::DBHandler(const string& dbname, bool use_fk_constraint) : m_dbname(dbname), m_db(NULL), m_fkConstraint(use_fk_constraint)
{
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
		sqlite3_close(m_db);
		m_db = NULL;
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
				}

				sqlite3_finalize(stmt);
			} else {
				shouldRollback = true;
				std::cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
			}

			if(shouldRollback)
				qFinish = "ROLLBACK TRANSACTION;";
			else
				m_signal_person_added.emit(pid, p.get_name());

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
			}

			if(shouldRollback) {
				cerr<< "Warning: something went wrong, rolling back the transaction."<<endl<< "\tLast database error: "<< sqlite3_errmsg(m_db);
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
		}
	} else
		throw (SqlConnectionClosedException());

	return res;
}

bool DBHandler::get_person(const guint32 id, Person& p) const
{
	bool res = false;
	sqlite3_stmt *stmt;

	if(m_db != NULL) {
		string query = "SELECT Name, Address, Zip1, Zip2, Location, Sex, Height, Birthday, Birthplace, Nationality, " \
					"Profession, TaxNumber, Referer, Email, RefBloodTypeID, RefMaritalStatusID FROM Person WHERE PersonID=?";

		if(sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL) == SQLITE_OK) {
			int val(SQLITE_ROW);
			sqlite3_bind_int(stmt, 1, id);

			while(val == SQLITE_ROW) {
				val = sqlite3_step(stmt);
				switch(val) {
				case SQLITE_ROW: {
					p.set_id(id);
					p.set_name(ustring((const char*)sqlite3_column_text(stmt, 0)));
					p.set_address(ustring((const char*)sqlite3_column_text(stmt, 1)));
					p.set_zip((guint16)sqlite3_column_int(stmt, 2), (guint16)sqlite3_column_int(stmt, 3));
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

void DBHandler::get_patients(const ustring *name) const
{
	/*TODO: throw exception if db is not opened*/
	ustring query = "SELECT PersonID, Name FROM Person";

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

					m_signal_person_added.emit(id, (ustring)name);
				} else
					break; // Nothing else to do.
			sqlite3_finalize(stmt);
		}
	} else
		throw (SqlConnectionClosedException());
}

sigc::signal<void, guint32, const ustring&>& DBHandler::signal_person_added()
{
	return m_signal_person_added;
}

sigc::signal<void, const Person&> DBHandler::signal_person_edited()
{
	return m_signal_person_edit;
}
