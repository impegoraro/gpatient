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

#include <iostream>
#include <glibmm/datetime.h>

#include "visitswindow.h"

using namespace Gtk;
using namespace Glib;

VisitsWindow::VisitsWindow()
	: m_hyper(0), m_chol(0), m_trigl(0), m_diabetes(0)
{
	RefPtr<Builder> builder = Builder::create_from_file("src/ui/new-visit-alternative.glade");
	TreeViewColumn *col;
	DateTime tm = DateTime::create_now_utc();
	
	builder->get_widget("VisitsWindow", m_win);
	builder->get_widget("tbAlergiesAdd", m_tbAlergiesAdd);
	builder->get_widget("tbAlergiesRemove", m_tbAlergiesRemove);
	builder->get_widget("tbHereditaryAdd", m_tbHereditaryAdd);
	builder->get_widget("tbHereditaryRemove", m_tbHereditaryRemove);
	builder->get_widget("treeAlergies", m_treeAlergies);
	builder->get_widget("treeHereditary", m_treeHereditary);
	builder->get_widget("btnHypertension", m_btnHyper);
	builder->get_widget("btnChol", m_btnChol);
	builder->get_widget("btnTrigl", m_btnTrigl);
	builder->get_widget("btnDiabetes", m_btnDiabetes);
	builder->get_widget("txtDate", m_txtDate);

	/* Tree Alergies configuration */
	m_treeAlergies->set_model(ListStore::create(m_la));
	col = m_treeAlergies->get_column(m_treeAlergies->append_column_numeric("ID", m_la.m_col_id, "%d")-1);
	col->set_visible(false);
	col = m_treeAlergies->get_column(m_treeAlergies->append_column_editable("Nome", m_la.m_col_name)-1);
	
	/* Tree Hereditary Diseases configuration */
	m_treeHereditary->set_model(ListStore::create(m_lhd));
	col = m_treeHereditary->get_column(m_treeHereditary->append_column_numeric("ID", m_lhd.m_col_id, "%d")-1);
	col->set_visible(false);
	col = m_treeHereditary->get_column(m_treeHereditary->append_column_editable("Parente", m_lhd.m_col_parent)-1);
	col = m_treeHereditary->get_column(m_treeHereditary->append_column_editable("Nome", m_lhd.m_col_name)-1);
	
	
	m_win->signal_cancel().connect(sigc::mem_fun(*m_win, &Widget::hide));
	m_win->signal_delete_event().connect(sigc::mem_fun(*this, &VisitsWindow::on_window_closing));
	m_tbAlergiesAdd->signal_clicked().connect(sigc::mem_fun(*this, &VisitsWindow::hlpr_list_alergies_add));
	m_tbAlergiesRemove->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::hlpr_list_remove), m_treeAlergies));
	m_tbHereditaryAdd->signal_clicked().connect(sigc::mem_fun(*this, &VisitsWindow::hlpr_list_hereditary_add));
	m_tbHereditaryRemove->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::hlpr_list_remove), m_treeHereditary));
	m_btnHyper->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_btnChangeState), BasicInfoButtons::Hypertension));
	m_btnChol->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_btnChangeState), BasicInfoButtons::Cholesterol));
	m_btnTrigl->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_btnChangeState), BasicInfoButtons::Triglycerides));
	m_btnDiabetes->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_btnChangeState), BasicInfoButtons::Diabetes));

	((Image*)m_btnHyper->get_image())->set_from_icon_name("list-remove-symbolic", IconSize(16));
	((Image*)m_btnChol->get_image())->set_from_icon_name("list-remove-symbolic", IconSize(16));
	((Image*)m_btnTrigl->get_image())->set_from_icon_name("list-remove-symbolic", IconSize(16));
	((Image*)m_btnDiabetes->get_image())->set_from_icon_name("list-remove-symbolic", IconSize(16));
	
	m_txtDate->set_text(tm.format("%d/%m/%Y"));
}

const Gtk::Window* VisitsWindow::get_window()
{
	return m_win;
}

void VisitsWindow::show()
{
	m_win->show_all();
}

VisitsWindow::~VisitsWindow()
{
	if(m_win != NULL)
		delete m_win; 
}

bool VisitsWindow::on_window_closing(GdkEventAny* event)
{
	m_win->hide();
	return false;
}

void VisitsWindow::hlpr_list_alergies_add()
{
	RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(m_treeAlergies->get_model());
	TreeIter iter = model->append();
	TreeModel::Row row = *iter;

	row[m_la.m_col_id] = -1;
	row[m_la.m_col_name] = "<Escreva o nome da alergia>";
	m_treeAlergies->get_selection()->select(iter);
	m_treeAlergies->activate();
}

void VisitsWindow::hlpr_list_hereditary_add()
{
	RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(m_treeHereditary->get_model());
	TreeIter iter = model->append();
	TreeModel::Row row = *iter;

	row[m_lhd.m_col_id] = -1;
	row[m_lhd.m_col_parent] = "";
	row[m_lhd.m_col_name] = "<Escreva o nome da doença>";
	m_treeHereditary->get_selection()->select(iter);
}

void VisitsWindow::hlpr_list_remove(TreeView *tree)
{
	RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(tree->get_model());
	TreeIter iter = tree->get_selection()->get_selected();
	TreeModel::Row row = *iter;

	if(model->iter_is_valid(iter))
		model->erase(iter);
	
}

void VisitsWindow::on_btnChangeState(VisitsWindow::BasicInfoButtons type)
{
	Button *btn;
	Image *img;
	int *val;

	if(type == BasicInfoButtons::Hypertension) {
		btn = m_btnHyper;
		val = &m_hyper;
	} else if(type == BasicInfoButtons::Cholesterol) {
		btn = m_btnChol;
		val = &m_chol;
	} else if (type == BasicInfoButtons::Triglycerides) {
		btn = m_btnTrigl;
		val = &m_trigl;
	} else if(type == BasicInfoButtons::Diabetes) {
		btn = m_btnDiabetes;
		val = &m_diabetes;
	} else
		return;
	
	img= (Image*)btn->get_image();;
	
	switch(*val) {
		case 0:
			*val = 1;
			img->set_from_icon_name("go-up-symbolic", IconSize(16));
			break;
		case 1:
			*val = 2;
			img->set_from_icon_name("go-down-symbolic", IconSize(16));
			break;
		default:
			*val = 0;
			img->set_from_icon_name("list-remove-symbolic", IconSize(16));
			break;
	}
}