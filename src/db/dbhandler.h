/*
 * DBHandler.h
 *
 *  Created on: Aug 25, 2012
 *      Author: ilan
 */

#ifndef DBHANDLER_H_
#define DBHANDLER_H_

#include <string>
#include <vector>
#include <sqlite3.h>

class DBHandler
{
protected:
	std::string m_dbname;

	sqlite3 *m_db;


public:
	DBHandler(const std::string& dbname);

	std::vector<std::string>* get_patients(void) const;

	bool open(void);
	void close(void);
};

#endif /* DBHANDLER_H_ */
