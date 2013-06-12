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
#include <gtkmm.h>

#include <exception>
#include <sstream>

#include "db/dbhandler.h"
#include "visitswindow.h"

using namespace std;
using namespace Gtk;
using namespace Glib;

VisitsWindow::VisitsWindow(int personID)
	: m_hyper(0), m_chol(0), m_trigl(0), m_diabetes(0), m_personID(personID)
{
	RefPtr<Builder> builder = Builder::create_from_file("src/ui/new-visit-last.glade");
	TreeViewColumn *col;
	DateTime tm = DateTime::create_now_utc();
	
	builder->get_widget("VisitsWindow", m_win);
	builder->get_widget("tbAlergiesAdd", m_tbAlergiesAdd);
	builder->get_widget("txtComplaint", m_txtComplaint);
	builder->get_widget("txtAnamnesis", m_txtAnamnesis);
	builder->get_widget("txtSleepiness", m_txtSleepiness);
	builder->get_widget("txtWeight", m_txtWeight);
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
	builder->get_widget("chkAnxiety", m_chkAnxiety);
	builder->get_widget("chkIrrt", m_chkIrrt);
	builder->get_widget("chkFrustration", m_chkFrustration);
	builder->get_widget("chkCry", m_chkCry);
	builder->get_widget("chkVerm", m_chkVerm);
	builder->get_widget("chkVed", m_chkVed);
	builder->get_widget("chkBra", m_chkBra);
	builder->get_widget("chkPrt", m_chkPrt);
	builder->get_widget("chkAml", m_chkAml);
	builder->get_widget("chkAlg", m_chkAlg);
	builder->get_widget("chkIrritable", m_chkIrritable);
	builder->get_widget("chkSad", m_chkSad);
	builder->get_widget("chkMed", m_chkMed);
	builder->get_widget("chkMelan", m_chkMelan);
	builder->get_widget("cmbAppearance", m_cmbAppearance);
	builder->get_widget("cmbMovement", m_cmbMovement);
	builder->get_widget("cmbHearing", m_cmbHearing);
	builder->get_widget("cmbThroat", m_cmbThroat) ;
	builder->get_widget("cmbScent", m_cmbScent); 
	builder->get_widget("cmbVision", m_cmbVision);
	builder->get_widget("txtVoice", m_txtVoice);
	builder->get_widget("txtSmell", m_txtSmell);
	builder->get_widget("txtFatigue", m_txtFatigue);
	builder->get_widget("txtSexualActivity", m_txtSexualActivity);
	builder->get_widget("txtBody", m_txtBody);
	builder->get_widget("txtAbdomen", m_txtAbdomen);
	builder->get_widget("txtHead", m_txtHead);
	builder->get_widget("txtCirculation", m_txtCirculation);
	builder->get_widget("txtTranspiration", m_txtTranspiration);
	builder->get_widget("txtDehydration", m_txtDehydration);


	m_wincal = new Widgets::CalendarWindow(*((Window*)this), *((Widget*)m_txtDate));

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
	
	((Image*)m_btnHyper->get_image())->set_from_icon_name("list-remove-symbolic", IconSize(16));
	((Image*)m_btnChol->get_image())->set_from_icon_name("list-remove-symbolic", IconSize(16));
	((Image*)m_btnTrigl->get_image())->set_from_icon_name("list-remove-symbolic", IconSize(16));
	((Image*)m_btnDiabetes->get_image())->set_from_icon_name("list-remove-symbolic", IconSize(16));
	m_txtDate->set_icon_from_icon_name("x-office-calendar-symbolic", ENTRY_ICON_SECONDARY);
	m_txtDate->set_icon_tooltip_text("Escolher data da consulta", ENTRY_ICON_SECONDARY);
	m_txtDate->set_text(tm.format("%d/%m/%Y"));

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
	m_txtDate->signal_icon_press().connect(sigc::mem_fun(*this, &VisitsWindow::on_txtDate_iconPress));
	m_win->signal_apply().connect(sigc::mem_fun(*this, &VisitsWindow::on_apply));
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
	if(m_wincal != NULL)
		delete m_wincal;
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

void VisitsWindow::setPersonID(int personID)
{
	m_personID = personID;
}

void VisitsWindow::on_txtDate_iconPress(Gtk::EntryIconPosition ipos, const GdkEventButton*ev)
{
	int x, y;

	get_window()->get_position(x, y);
	m_wincal->popup(x + 225, y + 51);
}

void VisitsWindow::on_apply()
{
	try {
		DBHandler::get_instance().open();
		DBHandler::get_instance().visit_insert(*((VisitInterface*)this));
		DBHandler::get_instance().close();
	} catch(exception& ex)
	{ }
	m_win->hide();
}

/*****************************************
 *           Interface methods           *
 ****************************************/

int VisitsWindow::getPersonID()
{
	return m_personID;
}

ustring VisitsWindow::getComplaint()
{
	return m_txtComplaint->get_text();
}
ustring VisitsWindow::getAnamnesis()
{
	return m_txtAnamnesis->get_buffer()->get_text(false);
}
Date VisitsWindow::getDate()
{
	Date dt;
	dt.set_parse(m_txtDate->get_text());
	return dt;
}
float VisitsWindow::getWeight()
{
	return (float)m_txtWeight->get_value();
}

ustring VisitsWindow::getAppearance()
{
	return m_cmbAppearance->get_active_text();
}
ustring VisitsWindow::getMovement()
{
	return m_cmbMovement->get_active_text();
}
ustring VisitsWindow::getVoice()
{
	return m_txtVoice->get_text();
}
ustring VisitsWindow::getSmell()
{
	return m_txtSmell->get_text();
}

int VisitsWindow::getHypertension()
{
	return m_hyper;
}
int VisitsWindow::getCholesterol()
{
	return m_chol;
}
int VisitsWindow::getTriglyceride()
{
	return m_trigl;
}
int VisitsWindow::getDiabetes()
{
	return m_diabetes;
}

ustring VisitsWindow::getSleepiness()
{
	return m_txtSleepiness->get_text();
}
ustring VisitsWindow::getTranspiration()
{
	return m_txtTranspiration->get_text();
}
ustring VisitsWindow::getDehydration()
{
	return m_txtDehydration->get_text();
}

int VisitsWindow::isAnxiety()
{
	return m_chkAnxiety->get_active();
}
int VisitsWindow::isIrrt()
{
	return m_chkIrrt->get_active();
}
int VisitsWindow::isFrustration()
{
	return m_chkFrustration->get_active();
}
int VisitsWindow::isCry()
{
	return m_chkCry->get_active();
}
int VisitsWindow::isVerm()
{
	return m_chkVerm->get_active();
}
int VisitsWindow::isVed()
{
	return m_chkVed->get_active();
}
int VisitsWindow::isBrad()
{
	return m_chkBra->get_active();
}
int VisitsWindow::isPrt()
{
	return m_chkPrt->get_active();
}
int VisitsWindow::isAml()
{
	return m_chkAml->get_active();
}
int VisitsWindow::isAlg()
{
	return m_chkAlg->get_active();
}
int VisitsWindow::isIrritable()
{
	return m_chkIrritable->get_active();
}
int VisitsWindow::isSad()
{
	return m_chkSad->get_active();
}
int VisitsWindow::isMed()
{
	return m_chkMed->get_active();
}
int VisitsWindow::isMelan()
{
	return m_chkMelan->get_active();
}
ustring VisitsWindow::getHearing()
{
	return m_cmbHearing->get_active_text();
}
ustring VisitsWindow::getThroat()
{
	return m_cmbThroat->get_active_text();
}
ustring VisitsWindow::getScent()
{
	return m_cmbScent->get_active_text();
}
ustring VisitsWindow::getVision()
{
	return m_cmbVision->get_active_text();
}
ustring VisitsWindow::getFatigue()
{
	return m_txtFatigue->get_text();
}
ustring VisitsWindow::getSexualActivity()
{
	return m_txtSexualActivity->get_text();
}
ustring VisitsWindow::getBody()
{
	return m_txtBody->get_text();
}
ustring VisitsWindow::getAbdomen()
{
	return m_txtAbdomen->get_text();
}
ustring VisitsWindow::getHead()
{
	return m_txtHead->get_text();
}
ustring VisitsWindow::getCirculation()
{
	return m_txtCirculation->get_text();
}