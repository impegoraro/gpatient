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
Widgets::CalendarWindow::CalendarWindow(Window& win, Widget& widget) : 
	Dialog("", false), m_wDate(NULL),
	m_btnPrevMonth("<"), m_btnMonth(""), m_btnNextMonth(">"),
	m_btnPrevYear("<"), m_btnYear(""), m_btnNextYear(">"),
	m_txtYear(Adjustment::create(1990, 1910, 9999), 1), shoulClose(true)
{
	Grid *pGrid = manage(new Grid());

	ustring ui_info = 
		"<ui>"
        "  <popup name='PopupMenu'>"
        "    <menuitem action='Janeiro'/>"
        "    <menuitem action='Fevereiro'/>"
        "    <menuitem action='Marco'/>"
		"    <menuitem action='Abril'/>"
		"    <menuitem action='Maio'/>"
		"    <menuitem action='Junho'/>"
		"    <menuitem action='Julho'/>"
		"    <menuitem action='Agosto'/>"
		"    <menuitem action='Setembro'/>"
		"    <menuitem action='Outubro'/>"
		"    <menuitem action='Novembro'/>"
		"    <menuitem action='Dezembro'/>"
        "  </popup>"
        "</ui>";

	m_uiman = UIManager::create();
	m_actionGrp = ActionGroup::create();

	m_actionGrp->add(Action::create("PopupMenu", "Popup Menu"));
	m_actionGrp->add(Action::create("Janeiro", "Janeiro"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 0));
	m_actionGrp->add(Action::create("Fevereiro", "Fevereiro"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 1));
	m_actionGrp->add(Action::create("Marco", "Marco"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 2));
	m_actionGrp->add(Action::create("Abril", "Abril"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 3));
	m_actionGrp->add(Action::create("Maio", "Maio"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 4));
	m_actionGrp->add(Action::create("Junho", "Junho"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 5));
	m_actionGrp->add(Action::create("Julho", "Julho"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 6));
	m_actionGrp->add(Action::create("Agosto", "Agosto"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 7));
	m_actionGrp->add(Action::create("Setembro", "Setembro"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 8));
	m_actionGrp->add(Action::create("Outubro", "Outubro"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 9));
	m_actionGrp->add(Action::create("Novembro", "Novembro"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 10));
	m_actionGrp->add(Action::create("Dezembro", "Dezembro"), sigc::bind<guint8>(sigc::mem_fun(*this, &CalendarWindow::change_month_to), 11));


	m_uiman->insert_action_group(m_actionGrp);

	try {
		m_uiman->add_ui_from_string(ui_info);
	} catch (Glib::MarkupError ex) {
		std::cout<< "Warning: error while parsing calendar poupup menu. Cause: "<< ex.what()<< std::endl;
	}
	//set_parent(win);
	set_type_hint(Gdk::WINDOW_TYPE_HINT_POPUP_MENU);
		
	pGrid->attach(m_btnPrevMonth, 0, 0, 1, 1);
	pGrid->attach_next_to(m_btnMonth, m_btnPrevMonth, POS_RIGHT, 1, 1);
	pGrid->attach_next_to(m_btnNextMonth, m_btnMonth, POS_RIGHT, 1, 1);

	pGrid->attach_next_to(m_btnPrevYear, m_btnNextMonth, POS_RIGHT, 1, 1);
	pGrid->attach_next_to(m_txtYear, m_btnPrevYear, POS_RIGHT, 1, 1);
	pGrid->attach_next_to(m_btnYear, m_txtYear, POS_RIGHT, 1, 1);
	pGrid->attach_next_to(m_btnNextYear, m_btnYear, POS_RIGHT, 1, 1);

	pGrid->attach(m_cal, 0, 1, 7, 1);

	m_btnPrevMonth.signal_clicked().connect(sigc::bind<bool>(sigc::mem_fun(*this, &CalendarWindow::change_month), false));
    m_btnNextMonth.signal_clicked().connect(sigc::bind<bool>(sigc::mem_fun(*this, &CalendarWindow::change_month), true));
	m_btnPrevYear.signal_clicked().connect(sigc::bind<bool>(sigc::mem_fun(*this, &CalendarWindow::change_year), false));
    m_btnNextYear.signal_clicked().connect(sigc::bind<bool>(sigc::mem_fun(*this, &CalendarWindow::change_year), true));
	m_btnYear.signal_clicked().connect(sigc::bind<bool>(sigc::mem_fun(*this, &CalendarWindow::select_year), true));
	m_cal.signal_day_selected_double_click().connect(sigc::mem_fun(*this, &CalendarWindow::on_selected_day));
	m_cal.signal_month_changed().connect(sigc::mem_fun(*this, &CalendarWindow::on_cal_monthChanged));
	m_txtYear.signal_focus_out_event().connect(sigc::mem_fun(*this, &CalendarWindow::on_btnYear_focusOut));
	m_txtYear.signal_changed().connect(sigc::mem_fun(*this, &CalendarWindow::on_btnYear_changed));
	m_btnMonth.signal_button_press_event().connect(sigc::mem_fun(*this, &CalendarWindow::on_btnMonth_buttonPressed), false);
	m_wDate = &widget;

	m_btnPrevMonth.set_relief(RELIEF_NONE);
	m_btnMonth.set_relief(RELIEF_NONE);
	m_btnMonth.set_size_request(85, -1);
	m_btnNextMonth.set_relief(RELIEF_NONE);
	m_btnPrevYear.set_relief(RELIEF_NONE);
	m_btnYear.set_relief(RELIEF_NONE);
	m_btnNextYear.set_relief(RELIEF_NONE);


	m_btnPrevMonth.set_halign(ALIGN_CENTER);
	m_btnMonth.set_halign(ALIGN_FILL);
	m_btnMonth.set_hexpand();
	m_btnNextMonth.set_halign(ALIGN_CENTER);
	m_btnPrevYear.set_halign(ALIGN_CENTER);
	m_btnYear.set_halign(ALIGN_FILL);
	m_btnYear.set_hexpand();
	m_btnNextYear.set_halign(ALIGN_CENTER);
	
	set_decorated(false);
	set_attached_to(widget);
	m_cal.set_display_options(CALENDAR_SHOW_DAY_NAMES);


	m_btnMonth.add_events(Gdk::BUTTON_PRESS_MASK);
	get_vbox()->pack_start(*pGrid, true, true, 0);

	set_border_width(10);
	on_cal_monthChanged();

	m_txtYear.set_no_show_all(true);
}

void Widgets::CalendarWindow::selected_date(Date& date) const
{
	m_cal.get_date(date);
}

void Widgets::CalendarWindow::popup(unsigned int x, unsigned int y)
{
	Date tmp;

	move(x, y);
	if(((Entry*)m_wDate)->get_text().length() > 0) {
		tmp = Util::parse_date(((Entry*)m_wDate)->get_text().raw());
		m_cal.select_month(tmp.get_month()-1, tmp.get_year());
		m_cal.select_day(tmp.get_day());
	}
	response(RESPONSE_ACCEPT);
	present();
	show_all();
	run();
}

void Widgets::CalendarWindow::on_selected_day(void)
{
	Date tmp;

	m_cal.get_date(tmp);
	((Entry*)m_wDate)->set_text(tmp.format_string((ustring)"%d/%m/%Y"));

	response(RESPONSE_ACCEPT);
	hide();
}

bool Widgets::CalendarWindow::on_focus_out_event(GdkEventFocus *focus)
{
	Date tmp;

	m_cal.get_date(tmp);
	((Entry*)m_wDate)->set_text(tmp.format_string((ustring)"%d/%m/%Y"));
	/*response(RESPONSE_ACCEPT);

	hide();*/

	return true;
}

void Widgets::CalendarWindow::change_month(bool forward)
{
	Date dt;

	m_cal.get_date(dt);

	if(forward)
		dt.add_months(1);
	else
		dt.subtract_months(1);

	m_cal.select_month(dt.get_month()-1, dt.get_year());
	helper_set_month_year(dt);
}

void Widgets::CalendarWindow::change_year(bool forward)
{
	Date dt;

	m_cal.get_date(dt);

	if(forward)
		dt.add_years(1);
	else
		dt.subtract_years(1);

	m_cal.select_month(dt.get_month()-1, dt.get_year());
	helper_set_month_year(dt);
}

void Widgets::CalendarWindow::change_month_to(guint8 month)
{
	Date dt;

	m_cal.get_date(dt);

	m_cal.select_month(month % 12, dt.get_year());
}

void Widgets::CalendarWindow::select_year(bool visibility)
{
	m_btnYear.set_visible(!visibility);
	m_txtYear.set_visible(visibility);

	if(visibility)
		m_txtYear.grab_focus();
}

bool Widgets::CalendarWindow::on_btnYear_focusOut(GdkEventFocus *event)
{
	select_year(false);
	
	return false;
}

void Widgets::CalendarWindow::on_btnYear_changed()
{
	Date dt;
	guint16 year = m_txtYear.get_value();

	m_cal.get_date(dt);
	m_cal.select_month(dt.get_month() - 1, year);

	dt.set_year(year);

	helper_set_month_year(dt);
}

void Widgets::CalendarWindow::on_cal_monthChanged()
{
	Date dt;
	m_cal.get_date(dt);
	if(dt.valid())
		helper_set_month_year(dt);
}

bool Widgets::CalendarWindow::on_btnMonth_buttonPressed(GdkEventButton* event)
{
	Menu *mp;

	std::cout<< "callback"<<std::endl;	
	if(event->type == GDK_BUTTON_PRESS) {
		mp = dynamic_cast<Menu*>(m_uiman->get_widget("/PopupMenu"));
		if(mp) {
			mp->popup(event->button, event->time);
			std::cout<< "showing menu"<<std::endl;
		}
		return false;
	}
	return false;
}