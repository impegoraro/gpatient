/*
 * DBHandler.cpp
 *
 *  Created on: Aug 25, 2012
 *      Author: ilan
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>

#include "util.h"
#include "dbhandler.h"

using namespace Glib;
using namespace std;

DBHandler::DBHandler(const string& dbname) : m_dbname(dbname), m_db(NULL)
{
}

bool DBHandler::open(void)
{
	//TODO: throw already open exception
	//if(m_isOpen())
		//throw
	return (sqlite3_open(m_dbname.c_str(), &m_db) == SQLITE_OK);
}

void DBHandler::close(void)
{
	if(m_db != NULL)
		sqlite3_close(m_db);
}

const ustring& DBHandler::get_database_path(void)
{
	return m_dbname;
}

int DBHandler::person_insert(const Person& p) const
{
	int res = 0;
	bool shouldRollback = false;

	/*TODO: Handle the errors correctly */
	if(p.get_name().length() == 0) {
		throw std::exception();
	}

	if(m_db != NULL) {
		stringstream ss;
		ss << p.get_height();

		string qBegin = "BEGIN IMMEDIATE TRANSACTION;";
		string query = "INSERT INTO Person( " \
						"Name, Address, Zip, Location, Sex, Height, Birthday, " \
						"Birthplace, Nationality, Profession, TaxNumber, Referer, " \
						"Email, RefMaritalStatusID, RefBloodTypeID) " \
						"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
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
			guint32 phones[2] = {p.get_phone(), p.get_cellphone()};

			sqlite3_bind_text(stmt, 1, p.get_name().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 2, p.get_address().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 3, p.get_zip(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 4, p.get_locality().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 5, p.get_sex() ? 1:0);
			sqlite3_bind_double(stmt, 6, p.get_height());
			sqlite3_bind_text(stmt, 7, p.get_birthday().format_string((ustring)"%d/%m/%Y").c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 8, p.get_birthplace().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 9, p.get_nationality().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 10, p.get_profession().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 11, p.get_tax_number());
			if(p.get_referer().length()>0)
				sqlite3_bind_text(stmt, 12, p.get_referer().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 13, p.get_email().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 14, p.get_marital_status());
			sqlite3_bind_int(stmt, 15, p.get_blood_type());

			if(sqlite3_step(stmt) == SQLITE_DONE)
				res = 1;
			sqlite3_finalize(stmt);

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

			if(shouldRollback)
				qFinish = "ROLLBACK TRANSACTION;";

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
	}

	return res;
}

bool DBHandler::get_person(const guint32 id, Person& p) const
{
	bool res = false;
	sqlite3_stmt *stmt;

	if(m_db != NULL) {
		string query = "SELECT Name, Address, Zip, Location, Sex, Height, Birthday, Birthplace, Nationality, " \
					"Profession, TaxNumber, Referer, Email, RefBloodTypeID, RefMaritalStatusID FROM Person WHERE PersonID=?";

		if(sqlite3_prepare_v2(m_db, query.c_str(), query.size(), &stmt, NULL) == SQLITE_OK) {
			int val(SQLITE_ROW);
			sqlite3_bind_int(stmt, 1, id);

			while(val == SQLITE_ROW) {
				val = sqlite3_step(stmt);
				switch(val) {
				case SQLITE_ROW: {
					char *zip1, *zip2;

					p.set_id(id);
					p.set_name(ustring((const char*)sqlite3_column_text(stmt, 0)));
					p.set_address(ustring((const char*)sqlite3_column_text(stmt, 1)));
					zip1 = strdup((const char*)sqlite3_column_text(stmt, 2));
					zip2 = strchr(zip1, '-');
					if(zip2 != NULL) {
						*zip2 = '\0';
						zip2++;
					}
					p.set_zip(zip1, zip2);
					free(zip1);
					p.set_locality(ustring((const char*)sqlite3_column_text(stmt,3)));
					p.set_sex((bool)sqlite3_column_int(stmt, 4));
					p.set_height((float)sqlite3_column_double(stmt, 5));
					p.set_birthday(Util::parse_date(string((const char*)sqlite3_column_text(stmt, 6))));
					p.set_birthplace(ustring((const char*)sqlite3_column_text(stmt, 7)));
					p.set_nationality(ustring((const char*)sqlite3_column_text(stmt, 8)));
					p.set_profession(ustring((const char*)sqlite3_column_text(stmt, 9)));
					p.set_tax_number((guint32)sqlite3_column_int(stmt, 10));
					if(sqlite3_column_text(stmt, 11) != NULL)
						p.set_referer(ustring((const char*)sqlite3_column_text(stmt, 11)));
					p.set_email(ustring((const char *)sqlite3_column_text(stmt, 12)));
					p.set_blood_type(sqlite3_column_int(stmt, 13));
					p.set_marital_status(sqlite3_column_int(stmt, 14));
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
	}

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
	}

	return res;
}

void DBHandler::get_patients(void) const
{
	/*TODO: throw exception if db is not opened*/
	const char *query = "SELECT PersonID, Name FROM Person ORDER BY Name ASC";

	if(m_db != NULL) {
		sqlite3_stmt *stmt;

		if(sqlite3_prepare_v2(m_db, query, -1, &stmt, NULL) == SQLITE_OK) {
			int res;

			while(true)
				if(sqlite3_step(stmt) == SQLITE_ROW) {
					guint32 id = sqlite3_column_int(stmt, 0);
					char *name = (char *)sqlite3_column_text(stmt, 1);

					signal_person_added(id, (ustring)name);
				} else
					break; // Nothing else to do.
			sqlite3_finalize(stmt);
		}
	}
}

