/*
 * main-window.h
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <gtkmm.h>

#include "person.h"
#include "db/dbhandler.h"

class MainWindow : public Gtk::Window
{
protected:
	class ListPatientsCols : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListPatientsCols()
		{ add(m_col_id); add(m_col_name); }

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};

	Gtk::MenuBar m_mainMenu;
	Gtk::Menu m_filemenu;
	Gtk::MenuItem m_mFile;
	Gtk::ImageMenuItem m_mfQuit;

	Gtk::Menu m_helpmenu;
	Gtk::MenuItem m_mHelp;
	Gtk::ImageMenuItem m_mhAbout;


	Gtk::Toolbar m_mainToolbar;
	Gtk::ToolButton m_mtbAdd;
	Gtk::ToolButton m_mtbEdit;
	Gtk::ToolButton m_mtbRemove;
	Gtk::Paned m_paned1;
	Gtk::Label m_lblPatients;
	Gtk::Entry m_entryPatients;
	Gtk::TreeView m_treePatients;

	Gtk::Toolbar m_tbVisits;
	Gtk::TreeView m_treeVisits;

	ListPatientsCols m_lpCols;

	Glib::RefPtr<Gtk::ListStore> m_modelPatients;

	// Selected patient information
	Gtk::Frame m_frpinfo;
	Gtk::Label m_lblpname;
	Gtk::Label m_lblpbloodtype;
	Gtk::Label m_lblpheight;
	Gtk::Label m_lblpage;
	Gtk::Label m_lblpsex;


	DBHandler m_db;
	bool m_entryPatientStatus;
public:

	MainWindow(const Glib::ustring& title, const Glib::ustring& dbpath);

	/* Helper Functions */
	void hlpr_append_patient(guint32 id, const Glib::ustring& name);

protected:
	/* Signal Handlers */
	void on_btnToolAdd_clicked(void);
	void on_btnToolEdit_clicked(void);
	void on_btnToolRemove_clicked(void);

	void on_window_show(void);

	bool on_entryPatient_focusIn(GdkEventFocus *focus);
	bool on_entryPatient_focusOut(GdkEventFocus *focus);

	void on_treePatients_selected();

	void on_mhAbout_activate(void);
};

#endif
