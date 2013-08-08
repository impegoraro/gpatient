#include <glibmm/ustring.h>
#include "allergy.h"

Allergy::Allergy(guint32 id, guint32 status, Glib::ustring& name, Glib::ustring& obs) : m_id(id), m_status(status), m_name(name), m_obs(obs)
{
}

Allergy::Allergy(guint32 id, Glib::ustring& name, Glib::ustring& obs) : m_id(id), m_status(LIST_STATUS_NORMAL), m_name(name), m_obs(obs) 
{
}