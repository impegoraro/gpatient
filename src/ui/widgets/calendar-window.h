/*
 * CalendarWindow.h
 *
 *  Created on: Nov 9, 2012
 *      Author: ilan
 */

#ifndef CALENDARWINDOW_H_
#define CALENDARWINDOW_H_

#include <gtkmm.h>

namespace Widgets
{

class CalendarWindow : public Gtk::Dialog
{

public:
	CalendarWindow(Gtk::Window&);

	void selected_date(Glib::Date&) const;
	void popup(Gtk::Entry&, unsigned int x, unsigned int y);

protected:
	Gtk::Calendar m_cal;
	Gtk::Entry *m_wDate;

	void on_selected_day(void);

	virtual bool on_focus_out_event(GdkEventFocus *);
};

} /* namespace Widgets */
#endif /* CALENDARWINDOW_H_ */
