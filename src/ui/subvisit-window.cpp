#include "subvisit-window.h"

#include <iostream>
#include <exception>

#include "../db/dbhandler.h"
#include "../util.h"

using namespace Gtk;
using namespace Glib;


#define GLADE_FILE_UI "src/ui/subvisits-window.glade"

SubVisitWindow::SubVisitWindow(Window& parent)
{
	RefPtr<Builder> builder = Builder::create_from_file(GLADE_FILE_UI);
	Box *pboxBP;

	builder->get_widget("boxBloodPressure", pboxBP);
	builder->get_widget("win", m_win);
	builder->get_widget("txtVisitDate", m_txtVisitDate);
	builder->get_widget("txtSleepiness", m_txtSleepiness);
	builder->get_widget("txtFatigue", m_txtFatigue);
	builder->get_widget("txtHead", m_txtHead);
	builder->get_widget("txtTongue", m_txtTongue);
	builder->get_widget("txtUrine", m_txtUrine);
	builder->get_widget("txtFaeces", m_txtFaeces);
	builder->get_widget("txtMenstruation", m_txtMenstruation);
	builder->get_widget("lblMenstruation", m_lblMenstruation);
	builder->get_widget("txtPulseD", m_txtPulseD);
	builder->get_widget("txtPulseE", m_txtPulseE);
	builder->get_widget("txtApal", m_txtApal);
	builder->get_widget("txtObservations", m_txtObservations);

	m_wincal = new Widgets::CalendarWindow(*m_win, *((Widget*)m_txtVisitDate));

	m_txtBPMax = manage(new Widgets::NumericEntry());
	m_txtBPMax->set_width_chars(3);
	m_txtBPMax->set_max_length(3);
	m_txtBPMin = manage(new Widgets::NumericEntry());
	m_txtBPMin->set_width_chars(3);
	m_txtBPMin->set_max_length(3);
	m_txtBPM = manage(new Widgets::NumericEntry());
	m_txtBPM->set_width_chars(3);
	m_txtBPM->set_max_length(3);
	pboxBP->pack_start(*m_txtBPMax, false, true);
	pboxBP->pack_start(*m_txtBPMin, false, true);
	pboxBP->pack_start(*m_txtBPM, false, true);
	pboxBP->reorder_child(*m_txtBPMax, 0);
	pboxBP->reorder_child(*m_txtBPMin, 2);
	pboxBP->reorder_child(*m_txtBPM, 4);

	m_txtVisitDate->set_icon_tooltip_text("Escolher data da consulta", ENTRY_ICON_SECONDARY);
	m_txtVisitDate->set_icon_from_icon_name("x-office-calendar", ENTRY_ICON_SECONDARY);

	m_win->signal_apply().connect(sigc::mem_fun(*this, &SubVisitWindow::on_apply));
	m_txtVisitDate->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtSleepiness->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtFatigue->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtHead->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtTongue->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtUrine->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtFaeces->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtMenstruation->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtPulseD->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtPulseE->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtApal->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtObservations->get_buffer()->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtBPMax->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtBPMin->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_txtBPM->signal_changed().connect(sigc::mem_fun(*this, &SubVisitWindow::on_widget_check));
	m_win->signal_key_press_event().connect(sigc::mem_fun(*this, &SubVisitWindow::close_on_esc));
	m_win->signal_cancel().connect(sigc::mem_fun(*m_win, &Widget::hide));
	m_txtVisitDate->signal_icon_press().connect(sigc::mem_fun(*this, &SubVisitWindow::on_txtDate_iconPress));

	m_win->set_transient_for(parent);
	m_win->add_events(Gdk::KEY_PRESS_MASK);
	m_win->set_modal(true);
}

SubVisitWindow::~SubVisitWindow()
{
	delete m_wincal;
	if(m_win != NULL)
		delete m_win;
}

void SubVisitWindow::show()
{
	m_txtFatigue->grab_focus();
	m_win->show();
}

void SubVisitWindow::show_all()
{
	m_txtFatigue->grab_focus();
	m_win->show_all();
}

bool SubVisitWindow::close_on_esc(GdkEventKey* event)
{
	if (event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Escape) {
		m_win->hide();
    	return true;
  	}
  		return false;
}

Window* SubVisitWindow::get_window(void)
{
	return m_win;
}

void SubVisitWindow::clean(void)
{
	DateTime tm = DateTime::create_now_utc();

	m_personID = m_visitID = m_parentVisitID = 0;	
	m_txtVisitDate->set_text(tm.format("%Y-%m-%d"));
	m_txtSleepiness->set_text("");
	m_txtFatigue->set_text("");
	m_txtHead->set_text("");
	m_txtTongue->set_text("");
	m_txtUrine->set_text("");
	m_txtFaeces->set_text("");
	m_txtMenstruation->set_text("");
	m_txtPulseD->set_text("");
	m_txtPulseE->set_text("");
	m_txtApal->set_text("");
	m_txtBPMax->set_text("");
	m_txtBPMin->set_text("");
	m_txtBPM->set_text("");
	m_txtObservations->get_buffer()->set_text("");
}

void SubVisitWindow::on_txtDate_iconPress(Gtk::EntryIconPosition ipos, const GdkEventButton*ev)
{
	int x, y;

	m_win->get_position(x, y);
	m_wincal->popup(x + 25, y + 30);
}

void SubVisitWindow::on_widget_check()
{
	bool complete = false;

	switch(m_win->get_current_page())
	{
		case 0:
			complete = ((m_txtVisitDate->get_text_length() > 0) &&
						(m_txtSleepiness->get_text_length() > 0) &&
						(m_txtFatigue->get_text_length() > 0) &&
						(m_txtHead->get_text_length() > 0) &&
						(m_txtTongue->get_text_length() > 0) &&
						(m_txtUrine->get_text_length() > 0) &&
						(m_txtFaeces->get_text_length() > 0) &&
						(m_txtBPMax->get_text_length() > 0) &&
						(m_txtBPMin->get_text_length() > 0) &&
						(m_txtBPM->get_text_length() > 0) &&
						(m_txtMenstruation->get_text_length() > 0) &&
						(m_txtPulseD->get_text_length() > 0) &&
						(m_txtPulseE->get_text_length() > 0) &&
						(m_txtApal->get_text_length() > 0));
						//(m_txtObservations->get_buffer()->size() > 0));
			break;	
	}

	m_win->set_page_complete(*m_win->get_nth_page(m_win->get_current_page()), complete);
}

void SubVisitWindow::on_apply()
{
	DBHandler::get_instance().open();
	try {
		//if(m_type == WINDOW_TYPE_ADD) {
			DBHandler::get_instance().subvisit_insert(*((SubVisitInterface*)this));
		//} else {
		//	DBHandler::get_instance().visit_update(*((VisitInterface*)this));
		//}

	} catch(std::exception& ex)
	{
		std::cout<< "Could not add the subvisit"<< std::endl;
	}
	DBHandler::get_instance().close();
	m_win->hide();
}




/************************* Setters *************************/
guint32 SubVisitWindow::getPersonID()
{
	return m_personID;
}
guint32 SubVisitWindow::getPersonID() const
{
	return m_personID;
}
guint32 SubVisitWindow::getVisitID()
{
	return m_visitID;
}
guint32 SubVisitWindow::getVisitID() const
{
	return m_visitID;
}
guint32 SubVisitWindow::getParentVisitID()
{
	return m_parentVisitID;
}
guint32 SubVisitWindow::getParentVisitID() const
{
	return m_parentVisitID;
}
const Glib::Date SubVisitWindow::getDate()
{
	return Util::parse_date(m_txtVisitDate->get_text());
}
const Glib::Date SubVisitWindow::getDate() const
{
	return Util::parse_date(m_txtVisitDate->get_text());
}
const Glib::ustring SubVisitWindow::getSleepiness()
{
	return m_txtSleepiness->get_text();
}
const Glib::ustring SubVisitWindow::getSleepiness() const
{
	return m_txtSleepiness->get_text();
}
const Glib::ustring SubVisitWindow::getFatigue()
{
	return m_txtFatigue->get_text();
}
const Glib::ustring SubVisitWindow::getFatigue() const
{
	return m_txtFatigue->get_text();
}
const Glib::ustring SubVisitWindow::getHead()
{
	return m_txtHead->get_text();
}
const Glib::ustring SubVisitWindow::getHead() const
{
	return m_txtHead->get_text();
}
const Glib::ustring SubVisitWindow::getTongue()
{
	return m_txtTongue->get_text();
}
const Glib::ustring SubVisitWindow::getTongue() const
{
	return m_txtTongue->get_text();
}
const Glib::ustring SubVisitWindow::getUrine()
{
	return m_txtUrine->get_text();
}
const Glib::ustring SubVisitWindow::getUrine() const
{
	return m_txtUrine->get_text();
}
const Glib::ustring SubVisitWindow::getFaeces()
{
	return m_txtFaeces->get_text();
}
const Glib::ustring SubVisitWindow::getFaeces() const
{
	return m_txtFaeces->get_text();
}
const Glib::ustring SubVisitWindow::getMenstruation()
{
	return m_txtMenstruation->get_text();
}
const Glib::ustring SubVisitWindow::getMenstruation() const
{
	return m_txtMenstruation->get_text();
}
const Glib::ustring SubVisitWindow::getPulseD()
{
	return m_txtPulseD->get_text();
}
const Glib::ustring SubVisitWindow::getPulseD() const
{
	return m_txtPulseD->get_text();
}
const Glib::ustring SubVisitWindow::getPulseE()
{
	return m_txtPulseE->get_text();
}
const Glib::ustring SubVisitWindow::getPulseE() const
{
	return m_txtPulseE->get_text();
}
const Glib::ustring SubVisitWindow::getApal()
{
	return m_txtApal->get_text();
}
const Glib::ustring SubVisitWindow::getApal() const
{
	return m_txtApal->get_text();
}
const Glib::ustring SubVisitWindow::getObservations()
{
	return m_txtObservations->get_buffer()->get_text();
}
const Glib::ustring SubVisitWindow::getObservations() const
{
	return m_txtObservations->get_buffer()->get_text();
}
const void SubVisitWindow::getBloodPressure(guint16& max, guint16& min, guint16& bpm)
{
	max = m_txtBPMax->get_value();
	min = m_txtBPMin->get_value();
	bpm = m_txtBPM->get_value();
}
const void SubVisitWindow::getBloodPressure(guint16& max, guint16& min, guint16& bpm) const
{
	max = m_txtBPMax->get_value();
	min = m_txtBPMin->get_value();
	bpm = m_txtBPM->get_value();
}


void SubVisitWindow::setPersonID(guint32 val)
{
	m_personID = val;
}
void SubVisitWindow::setVisitID(guint32 val)
{
	m_visitID = val;
}
void SubVisitWindow::setParentVisitID(guint32 val)
{
	m_parentVisitID = val;
}
void SubVisitWindow::setDate(const Glib::Date& val)
{
	m_txtVisitDate->set_text(val.format_string("%Y-%m-%d"));
}
void SubVisitWindow::setSleepiness(const Glib::ustring& val)
{
	m_txtSleepiness->set_text(val);
}
void SubVisitWindow::setFatigue(const Glib::ustring& val)
{
	m_txtFatigue->set_text(val);
}
void SubVisitWindow::setHead(const Glib::ustring& val)
{
	m_txtHead->set_text(val);
}
void SubVisitWindow::setTongue(const Glib::ustring& val)
{
	m_txtTongue->set_text(val);
}
void SubVisitWindow::setUrine(const Glib::ustring& val)
{
	m_txtUrine->set_text(val);
}
void SubVisitWindow::setFaeces(const Glib::ustring& val)
{
	m_txtFaeces->set_text(val);
}
void SubVisitWindow::setMenstruation(const Glib::ustring& val)
{
	m_txtMenstruation->set_text(val);
}
void SubVisitWindow::setPulseD(const Glib::ustring& val)
{
	m_txtPulseD->set_text(val);
}
void SubVisitWindow::setPulseE(const Glib::ustring& val)
{
	m_txtPulseE->set_text(val);
}
void SubVisitWindow::setApal(const Glib::ustring& val)
{
	m_txtApal->set_text(val);
}
void SubVisitWindow::setObservations(const Glib::ustring& val)
{
	m_txtObservations->get_buffer()->set_text(val);
}
void SubVisitWindow::setBloodPressure(guint16 max, guint16 min, guint16 bpm)
{
	m_txtBPMax->set_text(max);
	m_txtBPMin->set_text(min);
	m_txtBPM->set_text(bpm);
}



