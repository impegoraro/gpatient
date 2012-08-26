/*
 * DBHandler.cpp
 *
 *  Created on: Aug 25, 2012
 *      Author: ilan
 */

#include <vector>
#include <string>

#include "dbhandler.h"

using namespace std;

typedef std::vector<std::string> ArrayString;

static int callback(void *tmp, int argc, char **argv, char **azColName)
{
	int i;
	ArrayString *vector;
	vector = (ArrayString *) tmp;
	for(i = 0; i < argc; i++) {
		vector->push_back(string(argv[0]));
	}

	return 0;
}

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

vector<string>* DBHandler::get_patients(void) const
{
	/*TODO: throw exception if db is not opened*/
	int rc;
	char *zErrMsg;
	vector<string> *tmp = new vector<string>;

	if(m_db != NULL) {
		rc = sqlite3_exec(m_db, "select Name from Person", callback, tmp, &zErrMsg);
		if(rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			return NULL;
		}
		return tmp;
	}

	return NULL;
}

