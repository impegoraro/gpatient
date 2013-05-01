/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gestao-herb
 * Copyright (C) 2013 Ilan Pegoraro <iemoreirap@gmail.com>
 * 
gestao-herb is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gestao-herb is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _VISITSWINDOW_H_
#define _VISITSWINDOW_H_

#include <gtkmm/assistant.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/treeview.h>
#include <gtkmm/textview.h>
#include <gtkmm/builder.h>
#include <gtkmm/liststore.h>


class VisitsWindow 
{
public:
	VisitsWindow();
	~VisitsWindow();

	const Gtk::Window* get_window();
	
	void show();
private:
	class ListAlergies : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListAlergies()
		{
			add(m_col_id);
			add(m_col_name);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};
	class ListHereditary : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListHereditary()
		{
			add(m_col_id);
			add(m_col_parent);
			add(m_col_name);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_parent;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};

	enum BasicInfoButtons
	{
		Hypertension,
		Cholesterol,
		Triglycerides,
		Diabetes
	};
	
	
	Gtk::Assistant *m_win;

	Gtk::Button *m_btnHyper;
	int m_hyper;
	Gtk::Button *m_btnChol;
	int m_chol;
	Gtk::Button *m_btnTrigl;
	int m_trigl;
	Gtk::Button *m_btnDiabetes;
	int m_diabetes;
	
	Gtk::ToolButton *m_tbAlergiesAdd;
	Gtk::ToolButton *m_tbAlergiesRemove;
	Gtk::TreeView *m_treeAlergies;

	Gtk::ToolButton *m_tbHereditaryAdd;
	Gtk::ToolButton *m_tbHereditaryRemove;
	Gtk::TreeView *m_treeHereditary;
	
	Gtk::Entry *m_txtDate;

	ListAlergies m_la;
	ListHereditary m_lhd;
	
	bool on_window_closing(GdkEventAny*);
	void on_btnChangeState(BasicInfoButtons);
	void hlpr_list_alergies_add();
	void hlpr_list_hereditary_add();
	void hlpr_list_remove(Gtk::TreeView *);
};

#endif // _VISITSWINDOW_H_

