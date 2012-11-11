/*
 * CalendarWindow.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: ilan
 */

#include <iostream>
#include <string>
#include "calendar-window.h"
#include "../../util.h"


using namespace Gtk;
using namespace Glib;

/* Calendar Window */
Widgets::CalendarWindow::CalendarWindow(Window& win) : Dialog("", false), m_wDate(NULL)
{
	get_vbox()->pack_start(m_cal, true, true);
	set_decorated(false);

	m_cal.signal_day_selected_double_click().connect(sigc::mem_fun(*this, &CalendarWindow::on_selected_day));
}

void Widgets::CalendarWindow::selected_date(Date& date) const
{
	m_cal.get_date(date);
}

void Widgets::CalendarWindow::popup(Entry& widget, unsigned int x, unsigned int y)
{
	m_wDate = &widget;
	Date tmp;

	move(x, y);
	if(m_wDate->get_text().length() > 0) {
		tmp = Util::parse_date((std::string)m_wDate->get_text());
		m_cal.select_month(tmp.get_month()-1, tmp.get_year());
		m_cal.select_day(tmp.get_day());
	}
	m_cal.show();
	run();
}

void Widgets::CalendarWindow::on_selected_day(void)
{
	Date tmp;

	m_cal.get_date(tmp);
	m_wDate->set_text(tmp.format_string((ustring)"%d/%m/%Y"));
	response(RESPONSE_ACCEPT);
	hide();
}

bool Widgets::CalendarWindow::on_focus_out_event(GdkEventFocus *focus)
{
	Date tmp;

	m_cal.get_date(tmp);
	m_wDate->set_text(tmp.format_string((ustring)"%d/%m/%Y"));
	response(RESPONSE_ACCEPT);
	hide();

	return true;
}
