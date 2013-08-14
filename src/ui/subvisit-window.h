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
public:
	enum WindowType
	{
		WINDOW_TYPE_ADD,
		WINDOW_TYPE_EDIT
	};

private:
	WindowType m_type;
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
	void set_sex_widgets(bool sex);
	void set_window_type(SubVisitWindow::WindowType type, guint32 visitID = -1);

	Gtk::Window* get_window(void);



	/********** Setters **********/
	virtual guint32 getPersonID();
	virtual guint32 getPersonID() const;
	virtual guint32 getSubVisitID();
	virtual guint32 getSubVisitID() const;
	virtual guint32 getParentVisitID();
	virtual guint32 getParentVisitID() const;
	virtual const Glib::Date getSubVisitDate();
	virtual const Glib::Date getSubVisitDate() const;
	virtual const Glib::ustring getSubVisitSleepiness();
	virtual const Glib::ustring getSubVisitSleepiness() const;
	virtual const Glib::ustring getSubVisitFatigue();
	virtual const Glib::ustring getSubVisitFatigue() const;
	virtual const Glib::ustring getSubVisitHead();
	virtual const Glib::ustring getSubVisitHead() const;
	virtual const Glib::ustring getSubVisitTongue();
	virtual const Glib::ustring getSubVisitTongue() const;
	virtual const Glib::ustring getSubVisitUrine();
	virtual const Glib::ustring getSubVisitUrine() const;
	virtual const Glib::ustring getSubVisitFaeces();
	virtual const Glib::ustring getSubVisitFaeces() const;
	virtual const Glib::ustring getSubVisitMenstruation();
	virtual const Glib::ustring getSubVisitMenstruation() const;
	virtual const Glib::ustring getSubVisitPulseD();
	virtual const Glib::ustring getSubVisitPulseD() const;
	virtual const Glib::ustring getSubVisitPulseE();
	virtual const Glib::ustring getSubVisitPulseE() const;
	virtual const Glib::ustring getSubVisitApal();
	virtual const Glib::ustring getSubVisitApal() const;
	virtual const Glib::ustring getSubVisitObservations();
	virtual const Glib::ustring getSubVisitObservations() const;
	virtual const void getSubVisitBloodPressure(guint16& max, guint16& min, guint16& bpm);
	virtual const void getSubVisitBloodPressure(guint16& max, guint16& min, guint16& bpm) const;

	virtual void setPersonID(guint32 val);
	virtual void setParentVisitID(guint32 val);
	virtual void setSubVisitID(guint32 val);
	virtual void setSubVisitDate(const Glib::Date& val);
	virtual void setSubVisitSleepiness(const Glib::ustring& val);
	virtual void setSubVisitFatigue(const Glib::ustring& val);
	virtual void setSubVisitHead(const Glib::ustring& val);
	virtual void setSubVisitTongue(const Glib::ustring& val);
	virtual void setSubVisitUrine(const Glib::ustring& val);
	virtual void setSubVisitFaeces(const Glib::ustring& val);
	virtual void setSubVisitMenstruation(const Glib::ustring& val);
	virtual void setSubVisitPulseD(const Glib::ustring& val);
	virtual void setSubVisitPulseE(const Glib::ustring& val);
	virtual void setSubVisitApal(const Glib::ustring& val);
	virtual void setSubVisitObservations(const Glib::ustring& val);
	virtual void setSubVisitBloodPressure(guint16 max, guint16 min, guint16 bpm);


};

#endif