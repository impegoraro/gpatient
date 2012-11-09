/*
 * NumericEntry.h
 *
 *  Created on: Nov 9, 2012
 *      Author: ilan
 */

#ifndef NUMERICENTRY_H_
#define NUMERICENTRY_H_

#include <gtkmm.h>

namespace Widgets
{

class NumericEntry : public Gtk::Entry
{
public:
	NumericEntry();
	void set_allow_alphanumeric(bool allow =  true);
	void set_text(const Glib::ustring& text);
	void set_text(gint32 val);

protected:
	bool m_allow_alphanumeric;
	virtual void on_insert_text(const Glib::ustring& text, int *position);
};

} /* namespace Widgets */
#endif /* NUMERICENTRY_H_ */
