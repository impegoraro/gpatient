#ifndef SUBVISIT_WINDOW_H_
#define SUBVISIT_WINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/assistant.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/treeview.h>
#include <gtkmm/textview.h>
#include <gtkmm/builder.h>
#include <gtkmm/liststore.h>
#include <gtkmm/combobox.h>
#include <gtkmm/comboboxtext.h>

#include "../db/subvisit-interface.h"
#include "ui/widgets/widgets.h"


class SubVisitWindow : public SubVisitInterface
{
private:
	guint32 m_visitID;
	guint32 m_parentVisitID;
	guint32 m_personID;
	
	Gtk::Assistant *m_win;
	Gtk::Entry *m_txtVisitDate;
	Gtk::Entry *m_txtSleepiness;
	Gtk::Entry *m_txtFatigue;
	Gtk::Entry *m_txtHead;
	Gtk::Entry *m_txtTongue;
	Gtk::Entry *m_txtUrine;
	Gtk::Entry *m_txtFaeces;
	Gtk::Entry *m_txtMenstruation;
	Gtk::Label *m_lblMenstruation;
	Gtk::Entry *m_txtPulseD;
	Gtk::Entry *m_txtPulseE;
	Gtk::Entry *m_txtApal;
	Gtk::TextView *m_txtObservations;
	Widgets::NumericEntry *m_txtBPMax;
	Widgets::NumericEntry *m_txtBPMin;
	Widgets::NumericEntry *m_txtBPM;

	Widgets::CalendarWindow *m_wincal;

	bool close_on_esc(GdkEventKey* event);
	void on_txtDate_iconPress(Gtk::EntryIconPosition ipos, const GdkEventButton*ev);
	void on_widget_check();
	void on_apply();
public:
	SubVisitWindow(Gtk::Window&);
	~SubVisitWindow();

	void show();
	void show_all();
	void clean(void);

	Gtk::Window* get_window(void);



	/********** Setters **********/
	virtual guint32 getPersonID();
	virtual guint32 getPersonID() const;
	virtual guint32 getVisitID();
	virtual guint32 getVisitID() const;
	virtual guint32 getParentVisitID();
	virtual guint32 getParentVisitID() const;
	virtual const Glib::Date getDate();
	virtual const Glib::Date getDate() const;
	virtual const Glib::ustring getSleepiness();
	virtual const Glib::ustring getSleepiness() const;
	virtual const Glib::ustring getFatigue();
	virtual const Glib::ustring getFatigue() const;
	virtual const Glib::ustring getHead();
	virtual const Glib::ustring getHead() const;
	virtual const Glib::ustring getTongue();
	virtual const Glib::ustring getTongue() const;
	virtual const Glib::ustring getUrine();
	virtual const Glib::ustring getUrine() const;
	virtual const Glib::ustring getFaeces();
	virtual const Glib::ustring getFaeces() const;
	virtual const Glib::ustring getMenstruation();
	virtual const Glib::ustring getMenstruation() const;
	virtual const Glib::ustring getPulseD();
	virtual const Glib::ustring getPulseD() const;
	virtual const Glib::ustring getPulseE();
	virtual const Glib::ustring getPulseE() const;
	virtual const Glib::ustring getApal();
	virtual const Glib::ustring getApal() const;
	virtual const Glib::ustring getObservations();
	virtual const Glib::ustring getObservations() const;
	virtual const void getBloodPressure(guint16& max, guint16& min, guint16& bpm);
	virtual const void getBloodPressure(guint16& max, guint16& min, guint16& bpm) const;


	virtual void setPersonID(guint32 val);
	virtual void setVisitID(guint32 val);
	virtual void setParentVisitID(guint32 val);
	virtual void setDate(const Glib::Date& val);
	virtual void setSleepiness(const Glib::ustring& val);
	virtual void setFatigue(const Glib::ustring& val);
	virtual void setHead(const Glib::ustring& val);
	virtual void setTongue(const Glib::ustring& val);
	virtual void setUrine(const Glib::ustring& val);
	virtual void setFaeces(const Glib::ustring& val);
	virtual void setMenstruation(const Glib::ustring& val);
	virtual void setPulseD(const Glib::ustring& val);
	virtual void setPulseE(const Glib::ustring& val);
	virtual void setApal(const Glib::ustring& val);
	virtual void setObservations(const Glib::ustring& val);
	virtual void setBloodPressure(guint16 max, guint16 min, guint16 bpm);


};

#endif