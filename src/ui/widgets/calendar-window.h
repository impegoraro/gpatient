/*
 * CalendarWindow.h
 *
 *  Created on: Nov 9, 2012
 *      Author: ilan
 */

#ifndef CALENDARWINDOW_H_
#define CALENDARWINDOW_H_

#include <sigc++/sigc++.h>
#include <gdkmm/event.h>
#include <gdkmm/window.h>
#include <glibmm/date.h>
#include <gtkmm/frame.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/menu.h>
#include <gtkmm/grid.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/calendar.h>
#include <gtkmm/entry.h>

namespace Widgets
{

class CalendarWindow : public Gtk::Dialog
{

public:
	CalendarWindow(Gtk::Window&, Gtk::Widget&);
	
	void selected_date(Glib::Date&) const;
	void popup(unsigned int x, unsigned int y);

protected:
	bool shoulClose;
	Gtk::Window *parent;
	Gtk::Calendar m_cal;
	Gtk::Widget *m_wDate;
	Gtk::Button m_btnPrevMonth;
	Gtk::Button m_btnMonth;
	Gtk::Button m_btnNextMonth;

	Gtk::Button m_btnPrevYear;
	Gtk::Button m_btnYear;
	Gtk::Button m_btnNextYear;

	Gtk::SpinButton m_txtYear;

	Glib::RefPtr<Gtk::UIManager> m_uiman;
	Glib::RefPtr<Gtk::ActionGroup> m_actionGrp;


	inline void helper_set_month_year(Glib::Date& dt)
	{
		m_btnMonth.set_label(dt.format_string("%B"));
		m_btnYear.set_label(dt.format_string("%Y"));
		m_txtYear.set_value(dt.get_year());
	}

	void on_selected_day(void);
	void change_month(bool);
	void change_month_to(guint8);
	void change_year(bool);
	void select_year(bool);
	void on_cal_monthChanged();
	void on_btnYear_changed();
	bool on_btnMonth_buttonPressed(GdkEventButton* event);
	virtual bool on_btnYear_focusOut(GdkEventFocus *);
	virtual bool on_focus_out_event(GdkEventFocus *);
};

} /* namespace Widgets */
#endif /* CALENDARWINDOW_H_ */
