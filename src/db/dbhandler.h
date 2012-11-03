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
	bool m_fkConstraint;
	std::string m_dbname;

	sqlite3 *m_db;

	sigc::signal<void, guint32, const Glib::ustring&> m_signal_person_added;
public:
	// PersonID, Person Name


	DBHandler(const std::string& dbname, bool use_fk_constraint = true);

	int person_insert(const Person& p) const;
	int person_update(const Person& p) const;
	bool person_remove(unsigned int id) const;

	void get_patients(const Glib::ustring *name) const;
	bool get_person(const guint32 id, Person& p) const;

	const Glib::ustring& get_database_path(void) const;

	bool open(void);
	void close(void);

	sigc::signal<void, guint32, const Glib::ustring&>& signal_person_added();
};

#endif /* DBHANDLER_H_ */
