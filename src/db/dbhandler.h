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

#include "../person.h"

class DBHandler
{
protected:
	std::string m_dbname;

	sqlite3 *m_db;
public:
	// PersonID, Person Name
	sigc::signal<void, guint32, const Glib::ustring&> signal_person_added;

	DBHandler(const std::string& dbname);

	int person_insert(const Person& p) const;
	bool person_remove(unsigned int id) const;
	void get_patients(void) const;
	bool get_person(const guint32 id, Person& p) const;
	const Glib::ustring& get_database_path(void);
	bool open(void);
	void close(void);
};

#endif /* DBHANDLER_H_ */
