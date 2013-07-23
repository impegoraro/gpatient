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

#include "util.h"
#include "ui/widgets/numeric-entry.h"
#include "db/dbhandler.h"
#include "visitswindow.h"

using namespace std;
using namespace Gtk;
using namespace Glib;

VisitsWindow::VisitsWindow(Window& win, int personID)
	: m_hyper(0), m_chol(0), m_trigl(0), m_diabetes(0), m_personID(personID)
{
	RefPtr<Builder> builder = Builder::create_from_file("src/ui/new-visit-last.glade");
	TreeViewColumn *col;

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
	//builder->get_widget("cmbHearing", m_cmbHearing);
	//builder->get_widget("cmbThroat", m_cmbThroat) ;
	//builder->get_widget("cmbScent", m_cmbScent); 
	//builder->get_widget("cmbVision", m_cmbVision);
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
	//builder->get_widget("txtBPMax", m_txtBPMax);
	//builder->get_widget("txtBPMin", m_txtBPMin);
	//builder->get_widget("txtBPM", m_txtBPM);
	builder->get_widget("txtApal", m_txtApal);
	builder->get_widget("txtExams", m_txtExams);
	builder->get_widget("txtClinicalAnalysis", m_txtClinicalAnalysis);
	builder->get_widget("txtColor", m_txtColor);
	builder->get_widget("txtEscle", m_txtEscle);
	builder->get_widget("txtObservations", m_txtObservations);
	builder->get_widget("txtMed", m_txtMed);
	builder->get_widget("txtMedication", m_txtMedication);
	builder->get_widget("txtTreatment", m_txtTreatment);
	
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
	
	((Image*)m_btnHyper->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnChol->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnTrigl->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	((Image*)m_btnDiabetes->get_image())->set_from_icon_name("list-remove", ICON_SIZE_BUTTON);
	m_txtDate->set_icon_from_icon_name("x-office-calendar", ENTRY_ICON_SECONDARY);
	m_txtDate->set_icon_tooltip_text("Escolher data da consulta", ENTRY_ICON_SECONDARY);

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
	m_txtComplaint->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtComplaint));
	m_txtDate->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtDate));
	m_txtSleepiness->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtSleepiness));
	m_txtEatingHabits->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtEatingHabits));
	m_txtAnamnesis->get_buffer()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check_textview), m_txtAnamnesis));
	m_txtTranspiration->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtTranspiration));
	m_txtDehydration->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtDehydration));
	m_txtVoice->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtVoice));
	m_txtSmell->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtSmell));
	m_txtFatigue->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtFatigue));
	m_txtSexualActivity->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtSexualActivity));
	m_txtBody->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtBody));
	m_txtAbdomen->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtAbdomen));
	m_txtHead->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtHead));
	m_txtCirculation->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtCirculation));
	m_cmbHearing->get_entry()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_cmbHearing->get_entry()));
	m_cmbThroat->get_entry()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_cmbThroat->get_entry()));
	m_cmbScent->get_entry()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_cmbScent->get_entry()));
	m_cmbVision->get_entry()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_cmbVision->get_entry()));
	m_txtPainSince->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtPainSince));
	m_txtPainObs->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtPainObs));
	m_txtSurgery->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtSurgery));
	m_txtPreviousTreatment->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtPreviousTreatment));
	m_txtMenstruation->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtMenstruation));
	m_txtPregnancy->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtPregnancy));
	m_txtUrine->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtUrine));
	m_txtFaeces->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtFaeces));
	m_txtTongue->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtTongue));
	m_txtPulseD->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtPulseD));
	m_txtPulseE->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtPulseE));
	m_txtBPMax->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtBPMax));
	m_txtBPMin->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtBPMin));
	m_txtBPM->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtBPM));
	m_txtApal->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtApal));
	m_txtExams->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtExams));
	m_txtClinicalAnalysis->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtClinicalAnalysis));
	m_txtColor->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtColor));
	m_txtEscle->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtEscle));
	m_txtObservations->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtObservations));
	m_txtMed->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check), m_txtMed));
	m_txtMedication->get_buffer()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check_textview), m_txtMedication));
	m_txtTreatment->get_buffer()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &VisitsWindow::on_widget_check_textview), m_txtTreatment));

	m_win->set_parent(win);
	m_win->set_modal(true);
}

const Gtk::Window* VisitsWindow::get_window()
{
	return m_win;
}

void VisitsWindow::show()
{
	clear();
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
	{ std::cout<< "Could not add the visit"<< std::endl; }
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

	m_txtComplaint->set_text("");
	m_txtDate->set_text(tm.format("%d/%m/%Y"));
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
	m_chkBra->set_active(0);
	m_chkPrt->set_active(0);
	m_chkAml->set_active(0);
	m_chkAlg->set_active(0);
	m_chkIrritable->set_active(0);
	m_chkSad->set_active(0);
	m_chkMed->set_active(0);
	m_chkMelan->set_active(0);
	m_chkProstheses->set_active(0);
	m_chkWeight->set_active(0);

	allergies = RefPtr<ListStore>::cast_dynamic(m_treeAlergies->get_model());
	hereditary = RefPtr<ListStore>::cast_dynamic(m_treeHereditary->get_model());
	allergies->clear();
	hereditary->clear();
}


void VisitsWindow::on_widget_check(Entry* entry)
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

void VisitsWindow::on_widget_check_textview(TextView* textview)
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
ustring VisitsWindow::getDate()
{
	return m_txtDate->get_text();
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
ustring  VisitsWindow::getEatingHabits()
{
	return m_txtEatingHabits->get_text();
}

ustring VisitsWindow::getMenstruation()
{
	return m_txtMenstruation->get_text();
}
ustring VisitsWindow::getPregnancy()
{
	return m_txtPregnancy->get_text();
}
ustring VisitsWindow::getPain()
{
	return m_cmbPain->get_active_text();
}
ustring VisitsWindow::getPainSince()
{
	return m_txtPainSince->get_text();
}
ustring VisitsWindow::getPainObs()
{
	return m_txtPainObs->get_text();
}
ustring VisitsWindow::getSurgery()
{
	return m_txtSurgery->get_text();
}
ustring VisitsWindow::getPreviousTreatment()
{
	return m_txtPreviousTreatment->get_text();
}
bool VisitsWindow::getProstheses()
{
	return m_chkProstheses->get_active();
}
bool VisitsWindow::getWeightBool()
{
	return m_chkWeight->get_active();
}
ustring VisitsWindow::getUrine()
{
	return m_txtUrine->get_text();
}
ustring VisitsWindow::getFaeces()
{
	return m_txtFaeces->get_text();
}
ustring VisitsWindow::getTongue()
{
	return m_txtTongue->get_text();
}
ustring VisitsWindow::getPulseD()
{
	return m_txtPulseD->get_text();
}
ustring VisitsWindow::getPulseE()
{
	return m_txtPulseE->get_text();
}
gint16 VisitsWindow::getBPMax()
{
	gint16 val;
	std::stringstream ss;
	ss<< m_txtBPMax->get_text().raw();
	ss>>val;
	return val;
}
gint16 VisitsWindow::getBPMin()
{
	gint16 val;
	std::stringstream ss;
	ss<< m_txtBPMin->get_text().raw();
	ss>>val;
	return val;
}
gint16 VisitsWindow::getBPM()
{
	gint16 val;
	std::stringstream ss;
	ss<< m_txtBPM->get_text().raw();
	ss>>val;
	return val;
}
ustring VisitsWindow::getApal()
{
	return m_txtApal->get_text();
}
ustring VisitsWindow::getExams()
{
	return m_txtExams->get_text();
}
ustring VisitsWindow::getClinicalAnalysis()
{
	return m_txtClinicalAnalysis->get_text();
}
ustring VisitsWindow::getColor()
{
	return m_txtColor->get_text();
}
ustring VisitsWindow::getEscle()
{
	return m_txtEscle->get_text();
}
ustring VisitsWindow::getObservations()
{
	return m_txtObservations->get_text();
}
ustring VisitsWindow::getMed()
{
	return m_txtMed->get_text();
}
ustring VisitsWindow::getMedication()
{
	return m_txtMedication->get_buffer()->get_text(false);
}
ustring VisitsWindow::getTreatment()
{
	return m_txtTreatment->get_buffer()->get_text(false);
}

/***********************************
 *             Setters             *
***********************************/
void VisitsWindow::setComplaint(const Glib::ustring& val)
{
}
void VisitsWindow::setAnamnesis(const Glib::ustring& val)
{
}
void VisitsWindow::setDate(const Glib::ustring& val)
{
}
void VisitsWindow::setWeight(float val)
{
}
void VisitsWindow::setAppearance(const Glib::ustring& val)
{
}
void VisitsWindow::setMovement(const Glib::ustring& val)
{
}
void VisitsWindow::setVoice(const Glib::ustring& val)
{
}
void VisitsWindow::setSmell(const Glib::ustring& val)
{
}
void VisitsWindow::setHypertension(int val)
{
}
void VisitsWindow::setCholesterol(int val)
{
}
void VisitsWindow::setTriglyceride(int val)
{
}
void VisitsWindow::setDiabetes(int val)
{
}
void VisitsWindow::setSleepiness(const Glib::ustring& val)
{
}
void VisitsWindow::setTranspiration(const Glib::ustring& val)
{
}
void VisitsWindow::setDehydration(const Glib::ustring& val)
{
}
void VisitsWindow::setAnxiety(int val)
{
}
void VisitsWindow::setIrrt(int val)
{
}
void VisitsWindow::setFrustration(int val)
{
}
void VisitsWindow::setCry(int val)
{
}
void VisitsWindow::setVerm(int val)
{
}
void VisitsWindow::setVed(int val)
{
}
void VisitsWindow::setBrad(int val)
{
}
void VisitsWindow::setPrt(int val)
{
}
void VisitsWindow::setAml(int val)
{
}
void VisitsWindow::setAlg(int val)
{
}
void VisitsWindow::setIrritable(int val)
{
}
void VisitsWindow::setSad(int val)
{
}
void VisitsWindow::setMed(int val)
{
}
void VisitsWindow::setMelan(int val)
{
}
void VisitsWindow::setHearing(const Glib::ustring& val)
{
}
void VisitsWindow::setThroat(const Glib::ustring& val)
{
}
void VisitsWindow::setScent(const Glib::ustring& val)
{
}
void VisitsWindow::setVision(const Glib::ustring& val)
{
}
void VisitsWindow::setFatigue(const Glib::ustring& val)
{
}
void VisitsWindow::setSexualActivity(const Glib::ustring& val)
{
}
void VisitsWindow::setBody(const Glib::ustring& val)
{
}
void VisitsWindow::setAbdomen(const Glib::ustring& val)
{
}
void VisitsWindow::setHead(const Glib::ustring& val)
{
}
void VisitsWindow::setCirculation(const Glib::ustring& val)
{
}
void VisitsWindow::setEatingHabits(const Glib::ustring& val)
{
}

void VisitsWindow::setMenstruation(const Glib::ustring& val)
{
}
void VisitsWindow::setPregnancy(const Glib::ustring& val)
{
}
void VisitsWindow::setPain(const Glib::ustring& val)
{
}
void VisitsWindow::setPainSince(const Glib::ustring& val)
{
}
void VisitsWindow::setPainObs(const Glib::ustring& val)
{
}
void VisitsWindow::setSurgery(const Glib::ustring& val)
{
}
void VisitsWindow::setPreviousTreatment(const Glib::ustring& val)
{
}
void VisitsWindow::setProstheses(bool val)
{
}
void VisitsWindow::setWeightBool(bool val)
{
}
void VisitsWindow::setUrine(const Glib::ustring& val)
{
}
void VisitsWindow::setFaeces(const Glib::ustring& val)
{
}
void VisitsWindow::setTongue(const Glib::ustring& val)
{
}
void VisitsWindow::setPulseD(const Glib::ustring& val)
{
}
void VisitsWindow::setPulseE(const Glib::ustring& val)
{
}
void VisitsWindow::setBloodPressure(gint16 high, gint16 low, gint16 bpm)
{
}
void VisitsWindow::setApal(const Glib::ustring& val)
{
}
void VisitsWindow::setExams(const Glib::ustring& val)
{
}
void VisitsWindow::setClinicalAnalysis(const Glib::ustring& val)
{
}
void VisitsWindow::setColor(const Glib::ustring& val)
{
}
void VisitsWindow::setEscle(const Glib::ustring& val)
{
}
void VisitsWindow::setObservations(const Glib::ustring& val)
{
}
void VisitsWindow::setMed(const Glib::ustring& val)
{
}
void VisitsWindow::setMedication(const Glib::ustring& val)
{
}
void VisitsWindow::setTreatment(const Glib::ustring& val)
{
}