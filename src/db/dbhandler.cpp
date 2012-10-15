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
#include <sstream>

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
				for(int i=0; i< 2; i++) {
					sqlite3_bind_int(stmtP, 1, phones[i]);
					sqlite3_bind_int(stmtP, 2, pid);
					sqlite3_bind_int(stmtP, 3, i + 1);
					sqlite3_bind_int(stmtP, 4, 351);
					if(sqlite3_step(stmtP) == SQLITE_DONE)
						sqlite3_reset(stmtP);
					else {
						std::cerr<< "Error inserting phones: "<< sqlite3_errmsg(m_db)<<std::endl;
					}
				}
				sqlite3_finalize(stmtP);
			} else {
				std::cerr<< "Error preparing the statement: "<< sqlite3_errmsg(m_db)<<std::endl;
			}

			// End the transaction...
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

bool DBHandler::person_remove(unsigned int id) const
{
	bool res = false;

	/*TODO: Handle errors correctly */

	if(m_db != NULL) {
		stringstream ss;
		sqlite3_stmt *stmt;
		string query = "DELETE FROM Person WHERE PersonID = ";

		ss<< id;
		query += ss.str();

		if(sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
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

