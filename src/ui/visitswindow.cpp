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

#include <exception>
#include <sstream>

#include "util.h"
#include "ui/widgets/numeric-entry.h"
#include "db/dbhandler.h"
#include "visitswindow.h"
#include "db/list-status.h"

using namespace std;
using namespace Gtk;
using namespace Glib;

//m_txtSleepiness->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtTranspiration->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtDehydration->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtEatingHabits->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtVoice->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtSmell->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtFatigue->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtSexualActivity->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtBody->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtAbdomen->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtHead->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtCirculation->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtMenstruation->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtPregnancy->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtPainSince->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtPainObs->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtSurgery->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtPreviousTreatment->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtUrine->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtFaeces->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtTongue->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtPulseD->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtPulseE->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtBPMax->set_text("103");
//m_txtBPMin->set_text("71");
//m_txtBPM->set_text("111");
//m_txtApal->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtExams->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtClinicalAnalysis->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtColor->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtEscle->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtObservations->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//m_txtMed->set_text("sadadasdsadsadsad bvfkjhshkjfsjlk");
//
//m_txtAnamnesis->get_buffer()->set_text("hhjlkasjhfdshjgsskbgkfdmhbkd lkmlk");
//m_txtMedication->get_buffer()->set_text("hhjlkasjhfdshjgsskbgkfdmhbkd lkmlk");
//m_txtTreatment->get_buffer()->set_text("hhjlkasjhfdshjgsskbgkfdmhbkd lkmlk");
//
//m_cmbAppearance->set_active(0);
//m_cmbMovement->set_active(0);
//m_cmbHearing->set_active(0);
//m_cmbThroat->set_active(0);
//m_cmbScent->set_active(0);
//m_cmbVision->set_active(0);
//m_cmbPain->set_active(0);
//
//m_txtWeight->set_value(0.0);
//
//m_chkAnxiety->set_active(0);
//m_chkIrrt->set_active(0);
//m_chkFrustration->set_active(0);
//m_chkCry->set_active(1);
//m_chkVerm->set_active(0);
//m_chkVed->set_active(1);
//m_chkBrad->set_active(0);
//m_chkPrt->set_active(1);
//m_chkAml->set_active(1);
//m_chkAlg->set_active(0);
//m_chkIrritable->set_active(0);
//m_chkSad->set_active(0);
//m_chkMed->set_active(0);
//m_chkMelan->set_active(0);
//m_chkProstheses->set_active(0);
//m_chkWeight->set_active(0);
//
//m_win->set_page_complete(*m_win->get_nth_page(0), true);
//m_win->set_page_complete(*m_win->get_nth_page(1), true);
//m_win->set_page_complete(*m_win->get_nth_page(2), true);
//m_win->set_page_complete(*m_win->get_nth_page(3), true);


VisitsWindow::VisitsWindow(Window& win, int personID)
	: m_hyper(0), m_chol(0), m_trigl(0), m_diabetes(0), m_personID(personID), m_visitID(0), m_type(VisitsWindow::WINDOW_TYPE_ADD)
{
	RefPtr<Builder> builder = Builder::create_from_file("src/ui/new-visit-last.glade");
	TreeViewColumn *col;

	builder->get_widget("VisitsWindow", m_win);
	builder->get_widget("tbAllergiesAdd", m_tbAllergiesAdd);
	builder->get_widget("txtComplaint", m_txtComplaint);
	builder->get_widget("txtAnamnesis", m_txtAnamnesis);
	builder->get_widget("txtSleepiness", m_txtSleepiness);
	builder->get_widget("txtWeight", m_txtWeight);
	builder->get_widget("tbAllergiesRemove", m_tbAllergiesRemove);
	builder->get_widget("tbHereditaryAdd", m_tbHereditaryAdd);
	builder->get_widget("tbHereditaryRemove", m_tbHereditaryRemove);
	builder->get_widget("treeAllergies", m_treeAllergies);
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
	builder->get_widget("chkBrad", m_chkBrad);
	builder->get_widget("chkPrt", m_chkPrt);
	builder->get_widget("chkAml", m_chkAml);
	builder->get_widget("chkAlg", m_chkAlg);
	builder->get_widget("chkIrritable", m_chkIrritable);
	builder->get_widget("chkSad", m_chkSad);
	builder->get_widget("chkMed", m_chkMed);
	builder->get_widget("chkMelan", m_chkMelan);
	builder->get_widget("cmbAppearance", m_cmbAppearance);
	builder->get_widget("cmbMovement", m_cmbMovement);
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
	builder->get_widget("txtEatingHabits", m_txtEatingHabits);
	builder->get_widget("txtMenstruation", m_txtMenstruation);
	builder->get_widget("txtPregnancy", m_txtPregnancy);
	builder->get_widget("lblMenstruation", m_lblMenstruation);
	builder->get_widget("lblPregnancy", m_lblPregnancy);
	builder->get_widget("cmbPain", m_cmbPain);
	builder->get_widget("txtPainSince", m_txtPainSince);
	builder->get_widget("txtPainObs", m_txtPainObs);
	builder->get_widget("txtSurgery", m_txtSurgery);
	builder->get_widget("txtPreviousTreatment", m_txtPreviousTreatment);
	builder->get_widget("chkProstheses", m_chkProstheses);
	builder->get_widget("chkWeight", m_chkWeight);
	builder->get_widget("txtUrine", m_txtUrine);
	builder->get_widget("txtFaeces", m_txtFaeces);
	builder->get_widget("txtTongue", m_txtTongue);
	builder->get_widget("txtPulseD", m_txtPulseD);
	builder->get_widget("txtPulseE", m_txtPulseE);
	builder->get_widget("txtApal", m_txtApal);
	builder->get_widget("txtExams", m_txtExams);
	builder->get_widget("txtClinicalAnalysis", m_txtClinicalAnalysis);
	builder->get_widget("txtColor", m_txtColor);
	builder->get_widget("txtEscle", m_txtEscle);
	builder->get_widget("txtObservations", m_txtObservations);
	builder->get_widget("txtMed", m_txtMed);
	builder->get_widget("txtMedication", m_txtMedication);
	builder->get_widget("txtTreatment", m_txtTreatment);
	builder->get_widget("lblTitle", m_lblTitle);

	Gtk::Box *pboxBP;
	builder->get_widget("boxBloodPressure", pboxBP);

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
	
	Grid *grid5;
	Label *lblTmp;

	builder->get_widget("grid5", grid5);
	builder->get_widget("lblHearing", lblTmp);
	m_cmbHearing = manage(new ComboBoxText(true));
	m_cmbHearing->set_entry_text_column(0);
	m_cmbHearing->append("Bem");
	m_cmbHearing->append("Mal");
	m_cmbHearing->get_entry()->set_max_length(45);
	grid5->attach_next_to(*m_cmbHearing, *lblTmp, POS_RIGHT, 1, 1);
	
	builder->get_widget("lblThroat", lblTmp);
	m_cmbThroat = manage(new ComboBoxText(true));
	m_cmbThroat->set_entry_text_column(0);
	m_cmbThroat->append("Tosse");
	m_cmbThroat->append("Muco");
	m_cmbThroat->get_entry()->set_max_length(45);
	grid5->attach_next_to(*m_cmbThroat, *lblTmp, POS_RIGHT, 1, 1);

	builder->get_widget("lblScent", lblTmp);
	m_cmbScent = manage(new ComboBoxText(true));
	m_cmbScent->set_entry_text_column(0);
	m_cmbScent->append("Não tem");
	m_cmbScent->append("Normal");
	m_cmbScent->get_entry()->set_max_length(45);
	grid5->attach_next_to(*m_cmbScent, *lblTmp, POS_RIGHT, 1, 1);

	builder->get_widget("lblVision", lblTmp);
	m_cmbVision = manage(new ComboBoxText(true));
	m_cmbVision->set_entry_text_column(0);
	m_cmbVision->append("Má");
	m_cmbVision->append("Turva");
	m_cmbVision->get_entry()->set_max_length(45);
	grid5->attach_next_to(*m_cmbVision, *lblTmp, POS_RIGHT, 1, 1);
	

	m_wincal = new Widgets::CalendarWindow(*m_win, *((Widget*)m_txtDate));
	ListAllergies la;
	/* Tree allergies configuration */
	m_treeAllergies->set_model(ListStore::create(la));
	col = m_treeAllergies->get_column(m_treeAllergies->append_column_numeric("ID", la.m_col_id, "%u")-1);
	col->set_visible(false);
	col = m_treeAllergies->get_column(m_treeAllergies->append_column_editable("Nome", la.m_col_name)-1);
	col->set_resizable();
	col = m_treeAllergies->get_column(m_treeAllergies->append_column_editable("Observações", la.m_col_obs)-1);
	col->set_resizable();
	col = m_treeAllergies->get_column(m_treeAllergies->append_column_numeric("Status", la.m_col_status,"%d")-1);
	col->set_visible(true);


	/* Tree Hereditary Diseases configuration */
	m_treeHereditary->set_model(ListStore::create(m_lhd));
	col = m_treeHereditary->get_column(m_treeHereditary->append_column_numeric("ID", m_lhd.m_col_id, "%u")-1);
	col->set_visible(false);
	col = m_treeHereditary->get_column(m_treeHereditary->append_column_editable("Parente", m_lhd.m_col_parent)-1);
	col->set_resizable();
	col = m_treeHereditary->get_column(m_treeHereditary->append_column_editable("Nome", m_lhd.m_col_name)-1);
	col->set_resizable();
	col = m_treeHereditary->get_column(m_treeHereditary->append_column_editable("Observações", m_lhd.m_col_obs)-1);
	col->set_resizable();
	col = m_treeHereditary->get_column(m_treeHereditary->append_column_numeric("Status", m_lhd.m_col_status, "%u")-1);
	col->set_visible(false);

	((Image*)m_btnHyper->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnChol->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnTrigl->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnDiabetes->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	m_txtDate->set_icon_from_icon_name("x-office-calendar", ENTRY_ICON_SECONDARY);
	m_txtDate->set_icon_tooltip_text("Escolher data da consulta", ENTRY_ICON_SECONDARY);

	m_win->signal_prepare().connect(sigc::mem_fun(*this, &VisitsWindow::next_page));
	m_win->signal_cancel().connect(sigc::mem_fun(*m_win, &Widget::hide));
	m_win->signal_delete_event().connect(sigc::mem_fun(*this, &VisitsWindow::on_window_closing));
	//m_tbAllergiesAdd->signal_clicked().connect(sigc::mem_fun(*this, &VisitsWindow::hlpr_list_allergies_add));
	//m_tbAllergiesRemove->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::hlpr_list_remove), 0));
	//m_tbHereditaryAdd->signal_clicked().connect(sigc::mem_fun(*this, &VisitsWindow::hlpr_list_hereditary_add));
	//m_tbHereditaryRemove->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::hlpr_list_remove), 1));
	m_btnHyper->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_btnChangeState), BasicInfoButtons::Hypertension));
	m_btnChol->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_btnChangeState), BasicInfoButtons::Cholesterol));
	m_btnTrigl->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_btnChangeState), BasicInfoButtons::Triglycerides));
	m_btnDiabetes->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_btnChangeState), BasicInfoButtons::Diabetes));
	m_txtDate->signal_icon_press().connect(sigc::mem_fun(*this, &VisitsWindow::on_txtDate_iconPress));
	m_win->signal_apply().connect(sigc::mem_fun(*this, &VisitsWindow::on_apply));
	m_txtComplaint->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtDate->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtSleepiness->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtEatingHabits->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtAnamnesis->get_buffer()->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtTranspiration->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtDehydration->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtVoice->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtSmell->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtFatigue->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtSexualActivity->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtBody->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtAbdomen->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtHead->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtCirculation->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_cmbHearing->get_entry()->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_cmbThroat->get_entry()->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_cmbScent->get_entry()->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_cmbVision->get_entry()->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtPainSince->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtPainObs->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtSurgery->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtPreviousTreatment->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtMenstruation->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtPregnancy->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtUrine->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtFaeces->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtTongue->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtPulseD->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtPulseE->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtBPMax->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtBPMin->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtBPM->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtApal->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtExams->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtClinicalAnalysis->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtColor->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtEscle->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtObservations->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtMed->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtMedication->get_buffer()->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_txtTreatment->get_buffer()->signal_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_widget_check));
	m_treeAllergies->get_model()->signal_row_changed().connect(sigc::mem_fun(*this, &VisitsWindow::on_listAllergies_row_changed));
	m_win->signal_key_press_event().connect(sigc::mem_fun(*this, &VisitsWindow::close_on_esc));
	m_win->add_events(Gdk::KEY_PRESS_MASK);


	m_cmbAppearance->set_active(0);
	m_cmbMovement->set_active(0);
	m_cmbHearing->set_active(0);
	m_cmbThroat->set_active(0);
	m_cmbScent->set_active(0);
	m_cmbVision->set_active(0);
	m_cmbPain->set_active(0);

	m_win->set_transient_for(win);
	m_win->set_modal(true);
}

bool VisitsWindow::close_on_esc(GdkEventKey* event)
{
	if (event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Escape) {
		clear();
		m_win->hide();
    	return true;
  	}
  		return false;
}

const Gtk::Window* VisitsWindow::get_window()
{
	return m_win;
}

void VisitsWindow::show()
{
	DateTime tm = DateTime::create_now_utc();;
	m_txtDate->set_text(tm.format("%Y-%m-%d"));

	m_win->set_page_complete(*m_win->get_nth_page(0), !(m_type == WINDOW_TYPE_ADD));
	m_win->set_page_complete(*m_win->get_nth_page(1), !(m_type == WINDOW_TYPE_ADD));
	m_win->set_page_complete(*m_win->get_nth_page(2), !(m_type == WINDOW_TYPE_ADD));
	m_win->set_page_complete(*m_win->get_nth_page(3), !(m_type == WINDOW_TYPE_ADD));
	m_txtComplaint->grab_focus();
	
	//get_db_data();

	m_win->show_all();
}

void VisitsWindow::get_db_data()
{
	DBHandler db = DBHandler::get_instance();
	Date date;
	auto allergies = RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model());
	auto hereditary = RefPtr<ListStore>::cast_dynamic(m_treeHereditary->get_model());	
	m_wincal->selected_date(date);

	allergies->clear();
	hereditary->clear();

	db.open();
	//db.get_person_allergies(m_personID, date, sigc::mem_fun(*this, &VisitsWindow::hlpr_append_allergy));

	db.close();
}

void VisitsWindow::next_page(Widget* page)
{
	//get_db_data();
}

VisitsWindow::~VisitsWindow()
{
	delete m_wincal;
	if(m_win != NULL)
		delete m_win;
}

bool VisitsWindow::on_window_closing(GdkEventAny* event)
{
	clear();
	m_win->hide();
	return false;
}

void VisitsWindow::hlpr_list_allergies_add()
{
	//RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model());
	//ListAllergies la;
	//TreeIter iter = model->append();
	//TreeModel::Row row = *iter;

	//row[la.m_col_status] = LIST_STATUS_ADDED;
	//row[la.m_col_id] = -1;
	//row[la.m_col_name] = "<Escreva o nome da alergia>";
	//m_treeAllergies->get_selection()->select(iter);
	//m_treeAllergies->activate();
}

void VisitsWindow::on_listAllergies_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter)
{
	//TreeModel::Row row = *iter;
	//ListAllergies la;
	//if(row[la.m_col_status] == LIST_STATUS_MODIFIED || row[la.m_col_status] == LIST_STATUS_NORMAL) {
	//	row[la.m_col_status] = LIST_STATUS_MODIFIED;
	//}
}

void VisitsWindow::hlpr_list_hereditary_add()
{
	//RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(m_treeHereditary->get_model());
	//TreeIter iter = model->append();
	//TreeModel::Row row = *iter;

	//row[m_lhd.m_col_id] = -1;
	//row[m_lhd.m_col_parent] = "";
	//row[m_lhd.m_col_name] = "<Escreva o nome da doença>";
	//row[m_lhd.m_col_status] = LIST_STATUS_ADDED;
	//m_treeHereditary->get_selection()->select(iter);
}

void VisitsWindow::hlpr_list_remove(int val)
{
	//if(val == 0) {
	//	// Allergies' list
	//	ListAllergies la;
	//	RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model());
	//	TreeIter iter = m_treeAllergies->get_selection()->get_selected();
	//	TreeModel::Row row = *iter;

	//	if(model->iter_is_valid(iter)) 
	//		if(row[la.m_col_status] == LIST_STATUS_ADDED)
	//			model->erase(iter);
	//		else
	//			row[la.m_col_status] = LIST_STATUS_REMOVED;
	//	
	//} else {
	//	// Hereditary deceases' list
	//	/*RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model());
	//	TreeIter iter = m_treeAllergies->get_selection()->get_selected();
	//	TreeModel::Row row = *iter;

	//	if(model->iter_is_valid(iter))
	//		row[m_la.m_col_status] = LIST_STATUS_REMOVED;*/
	//}
		
}

void VisitsWindow::set_window_type(VisitsWindow::WindowType type, guint32 visitID)
{
	m_type = type;
	m_visitID = visitID;
	if(m_type == WINDOW_TYPE_ADD) {
		m_lblTitle->set_markup("<span size=\"xx-large\"><b><i>Nova Visita</i></b></span>");
		m_win->set_title("Nova Visita");
	} else {
		m_lblTitle->set_markup("<span size=\"xx-large\"><b><i>Editar Visita</i></b></span>");
		m_win->set_title("Editar Visita");
	}
}

int VisitsWindow::get_window_type()
{
	return m_type;	
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
	
	img = (Image*)btn->get_image();;
	switch(*val) {
		case 0:
			*val = 1;
			img->set_from_icon_name("go-up", ICON_SIZE_BUTTON);
			break;
		case 1:
			*val = 2;
			img->set_from_icon_name("go-down", ICON_SIZE_BUTTON);
			break;
		default:
			*val = 0;
			img->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
			break;
	}
}

void VisitsWindow::set_button_state(VisitsWindow::BasicInfoButtons type)
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
	
	img = (Image*)btn->get_image();;
	switch(*val) {
		case 0:
			img->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
			break;
		case 1:
			img->set_from_icon_name("go-up", ICON_SIZE_BUTTON);
			break;
		default:
			img->set_from_icon_name("go-down", ICON_SIZE_BUTTON);
			break;
	}
}

void VisitsWindow::hlpr_append_allergy(const Allergy& allergy, const Glib::Date& date)
{
//	RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model());
//	TreeIter iter = model->append();
//	VisitsWindow::ListAllergies la;
//
//	if(iter) {
//		TreeModel::Row row = *iter;
//	
//		row[la.m_col_status] = LIST_STATUS_NORMAL;
//		row[la.m_col_id] = allergy.m_id;
//		row[la.m_col_name] = allergy.m_name;
//		row[la.m_col_obs] = allergy.m_obs;
//		//row[la.m_col_status] = allergy.m_status;
//	}
}



void VisitsWindow::setPersonID(int personID)
{
	m_personID = personID;
}

void VisitsWindow::on_txtDate_iconPress(Gtk::EntryIconPosition ipos, const GdkEventButton*ev)
{
	int x, y;
	 Gdk::ModifierType mtype;

	//m_win->get_position(x, y);
	m_win->get_screen()->get_root_window()->get_pointer(x, y, mtype);
	m_wincal->popup(x, y);
}

void VisitsWindow::on_apply()
{
	DBHandler::get_instance().open();
	try {
		if(m_type == WINDOW_TYPE_ADD) {
			DBHandler::get_instance().visit_insert(*((VisitInterface*)this));
		} else {
			DBHandler::get_instance().visit_update(*((VisitInterface*)this));
		}

	} catch(exception& ex)
	{
		std::cout<< "Could not add the visit"<< std::endl;
	}
	DBHandler::get_instance().close();
	clear();
	m_win->hide();
}

void VisitsWindow::set_sex_widgets(bool sex)
{
	m_txtPregnancy->set_visible(sex);
	m_txtMenstruation->set_visible(sex);
	m_lblMenstruation->set_visible(sex);
	m_lblPregnancy->set_visible(sex);

	m_txtPregnancy->set_no_show_all(!sex);
	m_txtMenstruation->set_no_show_all(!sex);
	m_lblMenstruation->set_no_show_all(!sex);
	m_lblPregnancy->set_no_show_all(!sex);
}

void VisitsWindow::clear()
{
	DateTime tm = DateTime::create_now_utc();
	RefPtr<ListStore> allergies, hereditary;

	m_trigl = m_hyper = m_chol = m_diabetes = m_personID = m_visitID = 0;	
	m_txtComplaint->set_text("");
	m_txtDate->set_text(tm.format("%Y-%m-%d"));
	m_txtSleepiness->set_text("");
	m_txtTranspiration->set_text("");
	m_txtDehydration->set_text("");
	m_txtEatingHabits->set_text("");
	m_txtVoice->set_text("");
	m_txtSmell->set_text("");
	m_txtFatigue->set_text("");
	m_txtSexualActivity->set_text("");
	m_txtBody->set_text("");
	m_txtAbdomen->set_text("");
	m_txtHead->set_text("");
	m_txtCirculation->set_text("");
	m_txtMenstruation->set_text("");
	m_txtPregnancy->set_text("");
	m_txtPainSince->set_text("");
	m_txtPainObs->set_text("");
	m_txtSurgery->set_text("");
	m_txtPreviousTreatment->set_text("");
	m_txtUrine->set_text("");
	m_txtFaeces->set_text("");
	m_txtTongue->set_text("");
	m_txtPulseD->set_text("");
	m_txtPulseE->set_text("");
	m_txtBPMax->set_text("");
	m_txtBPMin->set_text("");
	m_txtBPM->set_text("");
	m_txtApal->set_text("");
	m_txtExams->set_text("");
	m_txtClinicalAnalysis->set_text("");
	m_txtColor->set_text("");
	m_txtEscle->set_text("");
	m_txtObservations->set_text("");
	m_txtMed->set_text("");

	((Image*)m_btnHyper->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnChol->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnTrigl->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnDiabetes->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);

	m_txtAnamnesis->get_buffer()->erase(m_txtAnamnesis->get_buffer()->begin(), m_txtAnamnesis->get_buffer()->end());
	m_txtMedication->get_buffer()->erase(m_txtMedication->get_buffer()->begin(), m_txtMedication->get_buffer()->end());
	m_txtTreatment->get_buffer()->erase(m_txtTreatment->get_buffer()->begin(), m_txtTreatment->get_buffer()->end());

	m_cmbAppearance->set_active(0);
	m_cmbMovement->set_active(0);
	m_cmbHearing->set_active(0);
	m_cmbThroat->set_active(0);
	m_cmbScent->set_active(0);
	m_cmbVision->set_active(0);
	m_cmbPain->set_active(0);

	m_txtWeight->set_value(0.0);

	m_chkAnxiety->set_active(0);
	m_chkIrrt->set_active(0);
	m_chkFrustration->set_active(0);
	m_chkCry->set_active(0);
	m_chkVerm->set_active(0);
	m_chkVed->set_active(0);
	m_chkBrad->set_active(0);
	m_chkPrt->set_active(0);
	m_chkAml->set_active(0);
	m_chkAlg->set_active(0);
	m_chkIrritable->set_active(0);
	m_chkSad->set_active(0);
	m_chkMed->set_active(0);
	m_chkMelan->set_active(0);
	m_chkProstheses->set_active(0);
	m_chkWeight->set_active(0);

	allergies = RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model());
	hereditary = RefPtr<ListStore>::cast_dynamic(m_treeHereditary->get_model());
	allergies->clear();
	hereditary->clear();

	m_win->set_page_complete(*m_win->get_nth_page(0), false);
	m_win->set_page_complete(*m_win->get_nth_page(1), false);
	m_win->set_page_complete(*m_win->get_nth_page(2), false);
	m_win->set_page_complete(*m_win->get_nth_page(3), false);
}


void VisitsWindow::on_widget_check()
{
	bool complete = false;

	switch(m_win->get_current_page())
	{
		case 0:
			complete = ((m_txtComplaint->get_text_length() > 0) &&
				(m_txtDate->get_text_length() > 0) &&
				(m_txtSleepiness->get_text_length() > 0) &&
				(m_txtEatingHabits->get_text_length() > 0) &&
				(m_txtAnamnesis->get_buffer()->size() > 0));
			break;	
		case 1:
			complete = ((m_txtTranspiration->get_text_length() > 0) &&
				(m_txtDehydration->get_text_length() > 0) &&
				(m_txtVoice->get_text_length() > 0) &&
				(m_txtSmell->get_text_length() > 0) &&
				(m_txtFatigue->get_text_length() > 0) &&
				(m_txtSexualActivity->get_text_length() > 0) &&
				(m_txtBody->get_text_length() > 0) &&
				(m_txtAbdomen->get_text_length() > 0) &&
				(m_txtHead->get_text_length() > 0) &&
				(m_txtCirculation->get_text_length() > 0) &&
				(m_cmbHearing->get_entry()->get_text_length() > 0) &&
				(m_cmbThroat->get_entry()->get_text_length() > 0) &&
				(m_cmbScent->get_entry()->get_text_length() > 0) &&
				(m_cmbVision->get_entry()->get_text_length() > 0));
			break;	
		case 2:
			complete = ((m_txtPainSince->get_text_length() > 0) &&
				(m_txtPainObs->get_text_length() > 0) &&
				(m_txtSurgery->get_text_length() > 0) &&
				(m_txtPreviousTreatment->get_text_length() > 0));
			break;	
		case 3:
			complete = ((m_txtMenstruation->get_visible() && m_txtMenstruation->get_text_length() > 0) || !m_txtMenstruation->get_visible()) &&
				((m_txtPregnancy->get_visible() && m_txtPregnancy->get_text_length() > 0) || !m_txtPregnancy->get_visible()) &&
				(m_txtUrine->get_text_length() > 0) &&
				(m_txtFaeces->get_text_length() > 0) &&
				(m_txtTongue->get_text_length() > 0) &&
				(m_txtPulseD->get_text_length() > 0) &&
				(m_txtPulseE->get_text_length() > 0) &&
				(m_txtBPMax->get_text_length() > 0) &&
				(m_txtBPMin->get_text_length() > 0) &&
				(m_txtBPM->get_text_length() > 0) &&
				(m_txtApal->get_text_length() > 0) &&
				(m_txtExams->get_text_length() > 0) &&
				(m_txtClinicalAnalysis->get_text_length() > 0) &&
				(m_txtColor->get_text_length() > 0) &&
				(m_txtEscle->get_text_length() > 0) &&
				(m_txtObservations->get_text_length() > 0) &&
				(m_txtMed->get_text_length() > 0) &&
				(m_txtMedication->get_buffer()->size() > 0)&&
				(m_txtTreatment->get_buffer()->size() > 0);
			break;
	}

	m_win->set_page_complete(*m_win->get_nth_page(m_win->get_current_page()), complete);
}

/*****************************************
 *           Interface methods           *
 ****************************************/

guint32 VisitsWindow::getPersonID() const
{
	return m_personID;
}

guint32 VisitsWindow::getVisitID() const
{
	return m_visitID;
}

ustring VisitsWindow::getComplaint() const
{
	return m_txtComplaint->get_text();
}
ustring VisitsWindow::getAnamnesis() const
{
	return m_txtAnamnesis->get_buffer()->get_text(false);
}
ustring VisitsWindow::getDate() const
{
	Date date;
	m_wincal->selected_date(date);
	return date.format_string("%Y-%m-%d");
}
float VisitsWindow::getWeight() const
{
	return (float)m_txtWeight->get_value();
}

ustring VisitsWindow::getAppearance() const
{
	return m_cmbAppearance->get_active_text();
}
ustring VisitsWindow::getMovement() const
{
	return m_cmbMovement->get_active_text();
}
ustring VisitsWindow::getVoice() const
{
	return m_txtVoice->get_text();
}
ustring VisitsWindow::getSmell() const
{
	return m_txtSmell->get_text();
}

int VisitsWindow::getHypertension() const
{
	return m_hyper;
}
int VisitsWindow::getCholesterol() const
{
	return m_chol;
}
int VisitsWindow::getTriglyceride() const
{
	return m_trigl;
}
int VisitsWindow::getDiabetes() const
{
	return m_diabetes;
}

ustring VisitsWindow::getSleepiness() const
{
	return m_txtSleepiness->get_text();
}
ustring VisitsWindow::getTranspiration() const
{
	return m_txtTranspiration->get_text();
}
ustring VisitsWindow::getDehydration() const
{
	return m_txtDehydration->get_text();
}

int VisitsWindow::isAnxiety() const
{
	return m_chkAnxiety->get_active();
}
int VisitsWindow::isIrrt() const
{
	return m_chkIrrt->get_active();
}
int VisitsWindow::isFrustration() const
{
	return m_chkFrustration->get_active();
}
int VisitsWindow::isCry() const
{
	return m_chkCry->get_active();
}
int VisitsWindow::isVerm() const
{
	return m_chkVerm->get_active();
}
int VisitsWindow::isVed() const
{
	return m_chkVed->get_active();
}
int VisitsWindow::isBrad() const
{
	return m_chkBrad->get_active();
}
int VisitsWindow::isPrt() const
{
	return m_chkPrt->get_active();
}
int VisitsWindow::isAml() const
{
	return m_chkAml->get_active();
}
int VisitsWindow::isAlg() const
{
	return m_chkAlg->get_active();
}
int VisitsWindow::isIrritable() const
{
	return m_chkIrritable->get_active();
}
int VisitsWindow::isSad() const
{
	return m_chkSad->get_active();
}
int VisitsWindow::isMed() const
{
	return m_chkMed->get_active();
}
int VisitsWindow::isMelan() const
{
	return m_chkMelan->get_active();
}
ustring VisitsWindow::getHearing() const
{
	return m_cmbHearing->get_active_text();
}
ustring VisitsWindow::getThroat() const
{
	return m_cmbThroat->get_active_text();
}
ustring VisitsWindow::getScent() const
{
	return m_cmbScent->get_active_text();
}
ustring VisitsWindow::getVision() const
{
	return m_cmbVision->get_active_text();
}
ustring VisitsWindow::getFatigue() const
{
	return m_txtFatigue->get_text();
}
ustring VisitsWindow::getSexualActivity() const
{
	return m_txtSexualActivity->get_text();
}
ustring VisitsWindow::getBody() const
{
	return m_txtBody->get_text();
}
ustring VisitsWindow::getAbdomen() const
{
	return m_txtAbdomen->get_text();
}
ustring VisitsWindow::getHead() const
{
	return m_txtHead->get_text();
}
ustring VisitsWindow::getCirculation() const
{
	return m_txtCirculation->get_text();
}
ustring  VisitsWindow::getEatingHabits() const
{
	return m_txtEatingHabits->get_text();
}

ustring VisitsWindow::getMenstruation() const
{
	return m_txtMenstruation->get_text();
}
ustring VisitsWindow::getPregnancy() const
{
	return m_txtPregnancy->get_text();
}
ustring VisitsWindow::getPain() const
{
	return m_cmbPain->get_active_text();
}
ustring VisitsWindow::getPainSince() const
{
	return m_txtPainSince->get_text();
}
ustring VisitsWindow::getPainObs() const
{
	return m_txtPainObs->get_text();
}
ustring VisitsWindow::getSurgery() const
{
	return m_txtSurgery->get_text();
}
ustring VisitsWindow::getPreviousTreatment() const
{
	return m_txtPreviousTreatment->get_text();
}
bool VisitsWindow::getProstheses() const
{
	return m_chkProstheses->get_active();
}
bool VisitsWindow::getWeightBool() const
{
	return m_chkWeight->get_active();
}
ustring VisitsWindow::getUrine() const
{
	return m_txtUrine->get_text();
}
ustring VisitsWindow::getFaeces() const
{
	return m_txtFaeces->get_text();
}
ustring VisitsWindow::getTongue() const
{
	return m_txtTongue->get_text();
}
ustring VisitsWindow::getPulseD() const
{
	return m_txtPulseD->get_text();
}
ustring VisitsWindow::getPulseE() const
{
	return m_txtPulseE->get_text();
}
gint16 VisitsWindow::getBPMax() const
{
	gint16 val;
	std::stringstream ss;
	ss<< m_txtBPMax->get_text().raw();
	ss>>val;
	return val;
}
gint16 VisitsWindow::getBPMin() const
{
	gint16 val;
	std::stringstream ss;
	ss<< m_txtBPMin->get_text().raw();
	ss>>val;
	return val;
}
gint16 VisitsWindow::getBPM() const
{
	gint16 val;
	std::stringstream ss;
	ss<< m_txtBPM->get_text().raw();
	ss>>val;
	return val;
}
ustring VisitsWindow::getApal() const
{
	return m_txtApal->get_text();
}
ustring VisitsWindow::getExams() const
{
	return m_txtExams->get_text();
}
ustring VisitsWindow::getClinicalAnalysis() const
{
	return m_txtClinicalAnalysis->get_text();
}
ustring VisitsWindow::getColor() const
{
	return m_txtColor->get_text();
}
ustring VisitsWindow::getEscle() const
{
	return m_txtEscle->get_text();
}
ustring VisitsWindow::getObservations() const
{
	return m_txtObservations->get_text();
}
ustring VisitsWindow::getMed() const
{
	return m_txtMed->get_text();
}
ustring VisitsWindow::getMedication() const
{
	return m_txtMedication->get_buffer()->get_text(false);
}
ustring VisitsWindow::getTreatment() const
{
	return m_txtTreatment->get_buffer()->get_text(false);
}

TreeModel::Children VisitsWindow::getAllergies() const
{
	TreeNodeChildren ch = m_treeAllergies->get_model()->children();
	return ch;
}

/***********************************
 *             Setters             *
***********************************/

void VisitsWindow::setVisitID(const guint32 val)
{
	m_visitID = val;
}

void VisitsWindow::setComplaint(const Glib::ustring& val)
{
	m_txtComplaint->set_text(val);
}
void VisitsWindow::setAnamnesis(const Glib::ustring& val)
{
	m_txtAnamnesis->get_buffer()->set_text(val);
}
void VisitsWindow::setDate(const Glib::ustring& val)
{
	m_txtDate->set_text(val);
}
void VisitsWindow::setWeight(float val)
{
	m_txtWeight->set_value(val);
}
void VisitsWindow::setAppearance(const Glib::ustring& val)
{
	m_cmbAppearance->set_active_text(val);
}
void VisitsWindow::setMovement(const Glib::ustring& val)
{
	m_cmbMovement->set_active_text(val);
}
void VisitsWindow::setVoice(const Glib::ustring& val)
{
	m_txtVoice->set_text(val);
}
void VisitsWindow::setSmell(const Glib::ustring& val)
{
	m_txtSmell->set_text(val);
}
void VisitsWindow::setHypertension(int val)
{
	m_hyper = val;
	set_button_state(BasicInfoButtons::Hypertension);
}
void VisitsWindow::setCholesterol(int val)
{
	m_chol = val;
	set_button_state(BasicInfoButtons::Cholesterol);
}
void VisitsWindow::setTriglyceride(int val)
{
	m_trigl = val;
	set_button_state(BasicInfoButtons::Triglycerides);
}
void VisitsWindow::setDiabetes(int val)
{
	m_diabetes = val;
	set_button_state(BasicInfoButtons::Diabetes);
}
void VisitsWindow::setSleepiness(const Glib::ustring& val)
{
	m_txtSleepiness->set_text(val);
}
void VisitsWindow::setTranspiration(const Glib::ustring& val)
{
	m_txtTranspiration->set_text(val);
}
void VisitsWindow::setDehydration(const Glib::ustring& val)
{
	m_txtDehydration->set_text(val);
}
void VisitsWindow::setAnxiety(int val)
{
	m_chkAnxiety->set_active(val);
}
void VisitsWindow::setIrrt(int val)
{
	m_chkIrrt->set_active(val);
}
void VisitsWindow::setFrustration(int val)
{
	m_chkFrustration->set_active(val);
}
void VisitsWindow::setCry(int val)
{
	m_chkCry->set_active(val);
}
void VisitsWindow::setVerm(int val)
{
	m_chkVerm->set_active(val);
}
void VisitsWindow::setVed(int val)
{
	m_chkVed->set_active(val);
}
void VisitsWindow::setBrad(int val)
{
	m_chkBrad->set_active(val);
}
void VisitsWindow::setPrt(int val)
{
	m_chkPrt->set_active(val);
}
void VisitsWindow::setAml(int val)
{
	m_chkAml->set_active(val);
}
void VisitsWindow::setAlg(int val)
{
	m_chkAlg->set_active(val);
}
void VisitsWindow::setIrritable(int val)
{
	m_chkIrritable->set_active(val);
}
void VisitsWindow::setSad(int val)
{
	m_chkSad->set_active(val);
}
void VisitsWindow::setMed(int val)
{
	m_chkMed->set_active(val);
}
void VisitsWindow::setMelan(int val)
{
	m_chkMelan->set_active(val);
}
void VisitsWindow::setHearing(const Glib::ustring& val)
{
	m_cmbHearing->get_entry()->set_text(val);
}
void VisitsWindow::setThroat(const Glib::ustring& val)
{
	m_cmbThroat->get_entry()->set_text(val);
}
void VisitsWindow::setScent(const Glib::ustring& val)
{
	m_cmbScent->get_entry()->set_text(val);
}
void VisitsWindow::setVision(const Glib::ustring& val)
{
	m_cmbVision->get_entry()->set_text(val);
}
void VisitsWindow::setFatigue(const Glib::ustring& val)
{
	m_txtFatigue->set_text(val);
}
void VisitsWindow::setSexualActivity(const Glib::ustring& val)
{
	m_txtSexualActivity->set_text(val);
}
void VisitsWindow::setBody(const Glib::ustring& val)
{
	m_txtBody->set_text(val);
}
void VisitsWindow::setAbdomen(const Glib::ustring& val)
{
	m_txtAbdomen->set_text(val);
}
void VisitsWindow::setHead(const Glib::ustring& val)
{
	m_txtHead->set_text(val);
}
void VisitsWindow::setCirculation(const Glib::ustring& val)
{
	m_txtCirculation->set_text(val);
}
void VisitsWindow::setEatingHabits(const Glib::ustring& val)
{
	m_txtEatingHabits->set_text(val);
}

void VisitsWindow::setMenstruation(const Glib::ustring& val)
{
	m_txtMenstruation->set_text(val);
}
void VisitsWindow::setPregnancy(const Glib::ustring& val)
{
	m_txtPregnancy->set_text(val);
}
void VisitsWindow::setPain(const Glib::ustring& val)
{
	m_cmbPain->set_active_text(val);
}
void VisitsWindow::setPainSince(const Glib::ustring& val)
{
	m_txtPainSince->set_text(val);
}
void VisitsWindow::setPainObs(const Glib::ustring& val)
{
	m_txtPainObs->set_text(val);
}
void VisitsWindow::setSurgery(const Glib::ustring& val)
{
	m_txtSurgery->set_text(val);
}
void VisitsWindow::setPreviousTreatment(const Glib::ustring& val)
{
	m_txtPreviousTreatment->set_text(val);
}
void VisitsWindow::setProstheses(bool val)
{
	m_chkProstheses->set_active(val);
}
void VisitsWindow::setWeightBool(bool val)
{
	m_chkWeight->set_active(val);
}
void VisitsWindow::setUrine(const Glib::ustring& val)
{
	m_txtUrine->set_text(val);
}
void VisitsWindow::setFaeces(const Glib::ustring& val)
{
	m_txtFaeces->set_text(val);
}
void VisitsWindow::setTongue(const Glib::ustring& val)
{
	m_txtTongue->set_text(val);
}
void VisitsWindow::setPulseD(const Glib::ustring& val)
{
	m_txtPulseD->set_text(val);
}
void VisitsWindow::setPulseE(const Glib::ustring& val)
{
	m_txtPulseE->set_text(val);
}
void VisitsWindow::setBloodPressure(gint16 high, gint16 low, gint16 bpm)
{
	m_txtBPMax->set_text(to_string(high));
	m_txtBPMin->set_text(to_string(low));
	m_txtBPM->set_text(to_string(bpm));
}
void VisitsWindow::setApal(const Glib::ustring& val)
{
	m_txtApal->set_text(val);
}
void VisitsWindow::setExams(const Glib::ustring& val)
{
	m_txtExams->set_text(val);
}
void VisitsWindow::setClinicalAnalysis(const Glib::ustring& val)
{
	m_txtClinicalAnalysis->set_text(val);
}
void VisitsWindow::setColor(const Glib::ustring& val)
{
	m_txtColor->set_text(val);
}
void VisitsWindow::setEscle(const Glib::ustring& val)
{
	m_txtEscle->set_text(val);
}
void VisitsWindow::setObservations(const Glib::ustring& val)
{
	m_txtObservations->set_text(val);
}
void VisitsWindow::setMed(const Glib::ustring& val)
{
	m_txtMed->set_text(val);
}
void VisitsWindow::setMedication(const Glib::ustring& val)
{
	m_txtMedication->get_buffer()->set_text(val);
}
void VisitsWindow::setTreatment(const Glib::ustring& val)
{
	m_txtTreatment->get_buffer()->set_text(val);
}