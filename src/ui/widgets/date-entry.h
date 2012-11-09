/*
 * DateEntry.h
 *
 *  Created on: Nov 9, 2012
 *      Author: ilan
 */

#ifndef DATEENTRY_H_
#define DATEENTRY_H_

#include <gtkmm.h>

namespace Widgets
{

class DateEntry : public Gtk::Entry
{
public:
	DateEntry();
	void set_allow_alphanumeric(bool allow=true);
protected:
	bool m_allow_alphanumeric;
	virtual void on_insert_text(const Glib::ustring& text, int *position);
};

} /* namespace Widgets */


#endif /* DATEENTRY_H_ */
