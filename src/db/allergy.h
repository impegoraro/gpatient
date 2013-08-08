#ifndef ALLERGY_H_
#define ALLERGY_H_

#include <glibmm/ustring.h>
#include "list-status.h"

class Allergy
{
public:
	Allergy(guint32 id, guint32 status, Glib::ustring& name, Glib::ustring& obs);
	Allergy(guint32 id, Glib::ustring& name, Glib::ustring& obs);
	
	guint32 m_id;
	guint32 m_status;
	Glib::ustring m_name;
	Glib::ustring m_obs;
};

#endif