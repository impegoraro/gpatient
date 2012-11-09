/*
 * DateEntry.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: ilan
 */

#include "date-entry.h"

Widgets::DateEntry::DateEntry() : m_allow_alphanumeric(false)
{
}

void Widgets::DateEntry::set_allow_alphanumeric(bool allow)
{
	this->m_allow_alphanumeric;
}

void Widgets::DateEntry::on_insert_text(const Glib::ustring& text, int *position)
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
