/*
 * NumericEntry.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: ilan
 */

#include "numeric-entry.h"

Widgets::NumericEntry::NumericEntry() : m_allow_alphanumeric(false)
{
}

void Widgets::NumericEntry::set_allow_alphanumeric(bool allow)
{
	this->m_allow_alphanumeric = allow;
}

void Widgets::NumericEntry::set_text(const Glib::ustring& text)
{
	Gtk::Entry::set_text(text);
}

void Widgets::NumericEntry::set_text(gint32 val)
{
	Gtk::Entry::set_text(Glib::ustring::compose((Glib::ustring)"%1", val));
}

void Widgets::NumericEntry::on_insert_text(const Glib::ustring& text, int *position)
{
	bool allow(true);

	for(int i=0; i < text.length(); i++) {
		if(text.c_str()[i] < '0' || text.c_str()[i] > '9' ) {
			allow = false;
			break;
		}
	}
	if(allow || m_allow_alphanumeric)
		Gtk::Entry::on_insert_text(text, position);
}
