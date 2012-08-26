#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <gtkmm.h>
#include <iostream>

using namespace std;
using namespace Gtk;

class MainWindow : public Window
{
protected:
	class ListPatientsCols : public TreeModel::ColumnRecord
	{
	public:

		ListPatientsCols()
		{ add(m_col_id);add(m_col_name); }

		TreeModelColumn<unsigned int> m_col_id;
		TreeModelColumn<Glib::ustring> m_col_name;
	};

	MenuBar m_mainMenu;
	Menu m_filemenu;
	MenuItem m_mFile;
	ImageMenuItem m_mfQuit;

	Menu m_helpmenu;
	MenuItem m_mHelp;
	ImageMenuItem m_mhAbout;


	Toolbar m_mainToolbar;
	ToolButton m_mtbAdd;
	ToolButton m_mtbRemove;
	Paned m_paned1;
	Label m_lblPatients;
	Entry m_entryPatients;
	TreeView m_treePatients;

	Toolbar m_tbVisits;
	TreeView m_treeVisits;

	ListPatientsCols m_lpCols;

	Glib::RefPtr<ListStore> m_modelPatients;
	bool m_entryPatientStatus;
public:

	MainWindow(string title);

	/* Helper Functions */
	void hlpr_append_patient(unsigned int id, const Glib::ustring& name);

	/* Signal Handlers */
	void on_btnToolAdd_clicked(void);
	void on_btnToolRemove_clicked(void);

	void on_window_show(void);

	bool on_entryPatient_focusIn(GdkEventFocus *focus);
	bool on_entryPatient_focusOut(GdkEventFocus *focus);

};

#endif
