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
#include <sigc++/sigc++.h>
#include "../person.h"
#include "../visit.h"
#include "subvisit-interface.h"
#include "visit-interface.h"
#include "../configuration.h"

typedef sigc::slot<void, const Allergy&, const Glib::Date&> SlotAppendAllergy;

/*
 * This class is a singleton to the proxy that interacts with the database.
 */
class DBHandler
{
protected:
	bool m_fkConstraint;
	std::string m_dbname;

	sqlite3 *m_db;

	// singleton instance
	static DBHandler *m_handle;

	DBHandler(const std::string& dbname, bool use_fk_constraint = true);

	inline bool user_has_contact(const Person& p) const;
	inline guint get_visit_from_person(gint PersonID) const;

	sigc::signal<void, guint32, const Glib::ustring&, guint32> m_signal_person_added;
	sigc::signal<void, guint32, const Glib::ustring&, const Glib::ustring&> m_signal_visit_added;
	sigc::signal<void, const VisitInterface&> m_signal_visit_edited;
	sigc::signal<void, const Allergy&, const Glib::Date&> m_signal_allergies;
	sigc::signal<void, const Person&> m_signal_person_edit;
public:

	static DBHandler& get_instance(const std::string& dbpath);
	static DBHandler& get_instance();
	static void finalize(void);

	int person_insert(const Person& p) const;
	int person_update(const Person& p) const;
	bool person_remove(unsigned int id) const;
	void get_patients(const Glib::ustring *name) const;
	bool get_person(const guint32 id, Person& p) const;
	bool exists_person_by_identification_card(const guint32 ic, guint32 *personID = NULL) const;
	bool exists_person_by_tax_number(const guint32 tax, guint32 *personID = NULL) const;
	
	bool visit_remove(unsigned int id) const;
	int visit_insert(VisitInterface& visit) const;
	int visit_update(VisitInterface& v) const;
	int subvisit_insert(SubVisitInterface& v) const;
	void get_visits(guint32 personID) const;
	bool get_visit(int id, VisitInterface &v) const;
	
	void get_person_allergies(guint32 personID, const Glib::Date& date, SlotAppendAllergy) const;
	
	const Glib::ustring& get_database_path(void) const;

	bool open(void);
	void close(void);

	sigc::signal<void, guint32, const Glib::ustring&, guint32>& signal_person_added();
	sigc::signal<void, guint32, const Glib::ustring&, const Glib::ustring&>& signal_visit_added();
	sigc::signal<void, const VisitInterface&>& signal_visit_edited();
	sigc::signal<void, const Person&>& signal_person_edited();
	sigc::signal<void, const Allergy&, const Glib::Date&>& signal_allergies();
};

#endif /* DBHANDLER_H_ */
