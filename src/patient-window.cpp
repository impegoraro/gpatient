/*
 * patient-window.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#include <gtkmm.h>

#include "patient-window.h"

using namespace Glib;
using namespace Gtk;

PatientWindow::PatientWindow(const std::string& title, PatientWindowType type) :
	Dialog(title, true, false), m_type(type),
	m_lblTitle("<big><b>Dados do Paciente</b></big>"),
	m_lblName("_Nome:", true), m_lblHeight("_Altura", true),
	m_lblBlood("_Tipo de Sangue:", true), m_lblSex("_Sexo:", true),
	m_entryHeight(*(manage(new Adjustment(1.0, 0.0, 3.0, 0.01, 0.05))), 0.1, 2),
	m_grpSex(), m_rbMale(m_grpSex, "_Masculino", true), m_rbFemale(m_grpSex, "_Femenino", true)
{
	Table *tbmain = manage(new Table(5, 2, false));
	Frame *frSex = manage(new Frame("Sexo"));
	Box *bSex = manage(new HBox(true, 2));
	Button *btn;

	tbmain->attach(m_lblTitle, 0, 2, 0, 1, FILL | EXPAND, FILL, 4, 30);
	tbmain->attach(m_lblName, 0, 1, 1, 2, FILL, FILL, 4, 0);
	tbmain->attach(m_entryName, 1, 2, 1, 2, FILL | EXPAND, FILL, 0, 0);
	tbmain->attach(m_lblHeight, 0, 1, 2, 3, FILL, FILL, 4, 0);
	tbmain->attach(m_entryHeight, 1, 2, 2, 3, FILL | EXPAND, FILL, 0, 0);
	tbmain->attach(m_lblBlood, 0, 1, 3, 4, FILL, FILL, 4, 0);
	tbmain->attach(m_cmbBlood, 1, 2, 3, 4, FILL | EXPAND, FILL, 0, 0);
	bSex->pack_start(m_rbMale);
	bSex->pack_start(m_rbFemale);
	frSex->add(*bSex);
	tbmain->attach(*frSex, 0, 2, 4, 5, FILL | EXPAND, FILL, 0, 0);

	m_cmbBlood.append("A");
	m_cmbBlood.append("B");
	m_cmbBlood.append("O");
	m_cmbBlood.append("AB");
	m_cmbBlood.set_active(0);

	m_lblTitle.set_alignment(0.00f, 0.5f);
	m_lblTitle.set_use_markup();
	m_lblName.set_alignment(1.0f, 0.5f);
	m_lblName.set_mnemonic_widget(m_entryName);
	m_lblHeight.set_alignment(1.0f, 0.5f);
	m_lblHeight.set_mnemonic_widget(m_entryHeight);
	m_lblHeight.set_alignment(1.0f, 0.5f);
	m_lblBlood.set_mnemonic_widget(m_cmbBlood);

	tbmain->show_all();
	tbmain->set_row_spacings(4);
	tbmain->set_col_spacings(2);

	btn = add_button(((type == PW_TYPE_ADD) ? Stock::ADD : Stock::EDIT), RESPONSE_ACCEPT);
	add_button(Stock::CANCEL, RESPONSE_CANCEL);

	m_entryName.signal_activate().connect(sigc::mem_fun(*btn, &Button::clicked));
	m_entryHeight.signal_activate().connect(sigc::mem_fun(*btn, &Button::clicked));

	set_skip_pager_hint();
	set_skip_taskbar_hint();

	get_vbox()->pack_start(*tbmain);
	set_size_request(375,380);
	set_resizable(false);
}

const ustring PatientWindow::get_patient_name(void) const
{
	return m_entryName.get_text();
}

