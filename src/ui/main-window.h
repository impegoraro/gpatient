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
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};

	Glib::RefPtr<Gtk::UIManager> m_uiman;
	Glib::RefPtr<Gtk::ActionGroup> m_actionGroup;
	
	Gtk::Toolbar m_mainToolbar;
	Gtk::ToolButton m_mtbAdd;
	Gtk::ToolButton m_mtbEdit;
	Gtk::ToolButton m_mtbRemove;

	Gtk::Notebook m_nb;
	
	Gtk::Label m_lblPatients;
	Gtk::Entry m_entryPatients;
	Gtk::TreeView m_treePatients;

	Gtk::Toolbar m_tbVisits;
	Gtk::TreeView m_treeVisits;
	Gtk::Label m_lblsugestions;

	ListPatientsCols m_lpCols;

	Glib::RefPtr<Gtk::TreeModelFilter> m_treeFilter;
	Glib::RefPtr<Gtk::ListStore> m_modelPatients;

	// Selected patient information
	Gtk::Frame m_frpinfo;
	Gtk::Label m_lblpname;
	Gtk::Label m_lblpbloodtype;
	Gtk::Label m_lblpheight;
	Gtk::Label m_lblpage;
	Gtk::Label m_lblpsex;
	Gtk::LinkButton m_btnShPatient;

	Gtk::Button m_btnBack;
	
	//Glib::SignalTimeout m_searchTimeout;
	sigc::connection m_connSearch;
	Glib::Timer m_timerSearch;

	bool m_entryPatientStatus;
	bool m_fireSearch;
	Glib::RefPtr<Gtk::Application> m_app;

	ViewPatientWindow *m_vp;
	PatientWindow *m_pw;
public:

	MainWindow(const Glib::ustring& title, Glib::RefPtr<Gtk::Application>& app);
	virtual ~MainWindow();
	/* Helper Functions */
	void hlpr_append_patient(guint32 id, const Glib::ustring& name);

protected:
	/* Signal Handlers */
	virtual bool on_delete_event(GdkEventAny *);

	void on_btnBack_clicked(void);
	void on_btnShPatient_clicked(void);
	void on_btnToolAdd_clicked(void);
	void on_btnToolEdit_clicked(void);
	void on_btnToolRemove_clicked(void);
	void on_window_show(void);

	bool on_entryPatient_focusIn(GdkEventFocus *focus); 
	bool on_entryPatient_focusOut(GdkEventFocus *focus);

	void on_treePatients_activated(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
	void on_treePatients_selected();
	void on_txtSearch_changed();
	void on_db_person_edited(const Person& p);
	void on_mhAbout_activate(void);
	bool handler_timeout_search();

	void patient_window_add(PatientWindow &);

	bool filter_patient_by_name(const Gtk::TreeModel::const_iterator&);
};

#endif
