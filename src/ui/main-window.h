/*
 * main-window.h
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <gtkmm.h>

#include "patient-window.h"
#include "view-patient.h"
#include "../person.h"
#include "../db/dbhandler.h"
#include "visitswindow.h"

#define GLADE_VISITS "src/ui/main-visit.glade"

class MainWindow : public Gtk::Window
{
protected:
	class ListPatientsCols : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListPatientsCols()
		{
			add(m_col_id);
			add(m_col_name);
			add(m_col_nif);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<int> m_col_nif;
	};
	
	class ListVisitsCols : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListVisitsCols()
		{
			add(m_col_id);
			add(m_col_complaint);
			add(m_col_date);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_complaint;
		Gtk::TreeModelColumn<Glib::ustring> m_col_date;
	};

	Glib::RefPtr<Gtk::UIManager> m_uiman;
	Glib::RefPtr<Gtk::ActionGroup> m_actionGroup;
	
	Gtk::Toolbar m_mainToolbar;
	Gtk::ToolButton m_mtbAdd;
	Gtk::ToolButton m_mtbEdit;
	Gtk::ToolButton m_mtbRemove;
	Gtk::ToolItem m_mtbExpander;
	Gtk::ToolItem m_mtbEntrySearch;
	Gtk::ToolButton m_mtbAddVisit;
	
	Gtk::Notebook m_nb;
	
	Gtk::Label m_lblPatients;
	Gtk::Entry m_entryPatients;
	Gtk::TreeView m_treePatients;

	Gtk::Toolbar m_tbVisits;

	ListPatientsCols m_lpCols;
	ListVisitsCols m_lvCols;

	// Visit widgets
	Gtk::TreeView *m_treeVisits;
	Gtk::Button *m_btnBack;

	// Selected patient information
	Gtk::Label *m_lblPName;
	Gtk::Label *m_lblPBloodtype;
	Gtk::Label *m_lblPHeight;
	Gtk::Label *m_lblPSex;
	Gtk::Label *m_lblPAge;

	Gtk::Button *m_btnViewPatient;
	Gtk::Grid *m_gridVisits;

	Glib::RefPtr<Gtk::TreeModelFilter> m_treeFilter;
	Glib::RefPtr<Gtk::ListStore> m_modelPatients;
	Glib::RefPtr<Gtk::ListStore> m_modelVisits;

	
	//Glib::SignalTimeout m_searchTimeout;
	sigc::connection m_connSearch;
	Glib::Timer m_timerSearch;

	bool m_entryPatientStatus;
	bool m_fireSearch;
	Glib::RefPtr<Gtk::Application> m_app;

	ViewPatientWindow *m_vp;
	PatientWindow *m_pw;
	VisitsWindow *m_vw;
	
public:

	MainWindow(const Glib::ustring& title, Glib::RefPtr<Gtk::Application>& app);
	virtual ~MainWindow();
	/* Helper Functions */
	void hlpr_append_patient(guint32 id, const Glib::ustring& name, guint32 nif);
	void hlpr_append_visit(guint32 id, const Glib::ustring& complaint, const Glib::ustring& date);

protected:
	void get_visits_widgets(void);

	/* Signal Handlers */
	virtual bool on_delete_event(GdkEventAny *);

	void on_btnBack_clicked(void);
	void on_btnShPatient_clicked(void);
	void on_btnToolAdd_clicked(void);
	void on_btnToolAddVisit_clicked(void);
	void on_btnToolEdit_clicked(void);
	void on_btnToolRemove_clicked(void);
	void on_window_show(void);

	bool on_entryPatient_focusIn(GdkEventFocus *focus); 
	bool on_entryPatient_focusOut(GdkEventFocus *focus);

	void on_treePatients_activated(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
	//void on_treePatients_selected();
	void on_txtSearch_changed();
	void on_db_person_edited(const Person& p);
	void on_mhAbout_activate(void);
	bool handler_timeout_search();

	void patient_window_add(PatientWindow &);

	bool filter_patient_by_name(const Gtk::TreeModel::const_iterator&);
};

#endif
