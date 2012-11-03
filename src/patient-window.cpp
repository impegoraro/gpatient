/*
 * patient-window.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <gtkmm.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "patient-window.h"
#include "util.h"

using namespace std;
using namespace Glib;
using namespace Gtk;

static bool helper_entry_focusIn(NumericEntry& entry, bool& value);
static bool helper_entry_focusOut(NumericEntry& entry, bool& value, char *text);
static void inline helper_entry_set_state(NumericEntry& entry, bool state = true);

PatientWindow::PatientWindow(Gtk::Window& parent, const std::string& title, PatientWindowType type) :
	Dialog((ustring)title, parent, true), m_type(type),
	m_lblTitle("<big><b>Ficha de inscrição de novo paciente</b></big>"),
	m_lblName("_Nome:", true), m_lblHeight("_Altura:", true),
	m_lblBlood("_Tipo de Sangue:", true), m_lblSex("_Sexo:", true),
	m_txtHeight(*manage(new Adjustment(1.0, 0.0, 3.0, 0.01, 0.05)), 0.1, 2),
	m_grpSex(), m_rbMale(m_grpSex, "_Masculino", true), m_rbFemale(m_grpSex, "_Femenino", true),
	m_lblBirthday("_Data de Nascimento:", true), m_lblBirthplace("_Local de Nascimento:", true),
	m_lblNacionality("Na_cionalidade:", true), m_lblProfession("_Profissão:", true),
	m_lblTaxNumber("Nº _Identificação Fiscal:", true), m_lblMaritalStatus("Es_tado Civil:", true),
	m_lblAddress("_Morada:", true), m_lblLocation("_Localidade:", true),
	m_lblZip("-"), m_lblContact("_Contactos:", true), m_lblReferer("_Enviado por:", true),
	m_lblEmail("_Email:", true), m_cellphoneStatus(false), m_phoneStatus(false), m_dateStatus(false), m_wincal(*this)
{
	Box *box1, *box2, *innerContacts, *innerPersonal;
	Frame *frPersonal = manage(new Frame("<b>Dados Pessoais</b>"));
	Frame *frContacts = manage(new Frame("<b>Morada e Contacto</b>"));
	Box *boxPhones = manage(new HBox(true, 2));
	Table *tbPersonal = manage(new Table(9, 3, false));
	Table *tbContacts = manage(new Table(5, 5, false));

	box1 = manage(new VBox(false, 4));
	box2 = manage(new HBox(true, 4));

	innerPersonal = manage(new HBox(true, 4));
	innerContacts = manage(new HBox(true, 4));

	innerPersonal->pack_start(*tbPersonal, true, true, 1);
	innerContacts->pack_start(*tbContacts, true, true, 1);
	frPersonal->add(*innerPersonal);
	frContacts->add(*innerContacts);

	tbPersonal->attach(m_lblName, 0, 1, 0, 1, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_txtName, 1, 3, 0, 1, FILL | SHRINK | EXPAND, FILL, 2, 0);
	tbPersonal->attach(m_lblHeight, 0, 1, 1, 2, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_txtHeight, 1, 3, 1, 2, FILL | SHRINK | EXPAND, FILL, 2, 0);
	tbPersonal->attach(m_lblSex, 0, 1, 2, 3, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_rbMale, 1, 2, 2, 3, FILL | SHRINK, FILL, 0, 0);
	tbPersonal->attach(m_rbFemale, 2, 3, 2, 3, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_lblNacionality, 0, 1, 3, 4, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_txtNacionality, 1, 3, 3, 4, FILL | SHRINK | EXPAND, FILL, 2, 0);
	tbPersonal->attach(m_lblBirthday, 0, 1, 4, 5, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_txtBirthday, 1, 3, 4, 5, FILL | SHRINK | EXPAND, FILL, 2, 0);
	tbPersonal->attach(m_lblBirthplace, 0, 1, 5, 6, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_txtBirthplace, 1, 3, 5, 6, FILL | SHRINK | EXPAND, FILL, 2, 0);
	tbPersonal->attach(m_lblProfession, 0, 1, 6, 7, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_txtProfession, 1, 3, 6, 7, FILL | SHRINK | EXPAND, FILL, 2, 0);
	tbPersonal->attach(m_lblBlood, 0, 1, 7, 8, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_cmbBlood, 1, 3, 7, 8, FILL | SHRINK | EXPAND, FILL, 2, 0);
	tbPersonal->attach(m_lblTaxNumber, 0, 1, 8, 9, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_txtTaxNumber, 1, 3, 8, 9, FILL | SHRINK | EXPAND, FILL, 2, 0);
	tbPersonal->attach(m_lblMaritalStatus, 0, 1, 9, 10, FILL | SHRINK, FILL, 2, 0);
	tbPersonal->attach(m_cmbMaritalStatus, 1, 3, 9, 10, FILL | SHRINK | EXPAND, FILL, 2, 0);

	boxPhones->pack_start(m_txtPhone, true, true, 1);
	boxPhones->pack_start(m_txtCellphone, true, true, 1);
	tbContacts->attach(m_lblAddress, 0, 1, 0, 1, FILL | SHRINK, FILL, 2, 0);
	tbContacts->attach(m_txtAddress, 1, 5, 0, 1, FILL | SHRINK | EXPAND, FILL, 0, 0);
	tbContacts->attach(m_lblLocation, 0, 1, 1, 2, FILL | SHRINK, FILL, 2, 0);
	tbContacts->attach(m_txtLocation, 1, 2, 1, 2, FILL | SHRINK | EXPAND, FILL, 0, 0);
	tbContacts->attach(m_txtZip1, 2, 3, 1, 2, FILL | SHRINK | EXPAND, FILL, 0, 0);
	tbContacts->attach(m_lblZip, 3, 4, 1, 2, FILL, FILL, 0, 0);
	tbContacts->attach(m_txtZip2, 4, 5, 1, 2, FILL | EXPAND | EXPAND, FILL, 0, 0);
	tbContacts->attach(m_lblContact, 0, 1, 2, 3, FILL | SHRINK, FILL, 2, 0);
	tbContacts->attach(*boxPhones, 1, 5, 2, 3, FILL | SHRINK | EXPAND, FILL, 0, 0);
	tbContacts->attach(m_lblEmail, 0, 1, 3, 4, FILL | SHRINK, FILL, 2, 0);
	tbContacts->attach(m_txtEmail, 1, 5, 3, 4, FILL | SHRINK | EXPAND, FILL, 0, 0);
	tbContacts->attach(m_lblReferer, 0, 1, 4, 5, FILL | EXPAND | SHRINK, FILL, 2, 0);
	tbContacts->attach(m_txtReferer, 1, 5, 4, 5, FILL | SHRINK | EXPAND, FILL, 0, 0);

	box1->pack_start(m_lblTitle, false, true, 8);
	box1->pack_start(*box2, true, true, 2);

	box2->pack_start(*frPersonal, true, true, 2);
	box2->pack_start(*frContacts, true, true, 2);


	/* TODO: Remove deprecated code... Using it for the sake of windows compatibility */
	m_cmbBlood.append("A"); m_cmbBlood.append("A+"); m_cmbBlood.append("A-");
	m_cmbBlood.append("B"); m_cmbBlood.append("B+"); m_cmbBlood.append("B-");
	m_cmbBlood.append("O"); m_cmbBlood.append("O+"); m_cmbBlood.append("O-");
	m_cmbBlood.append("AB"); m_cmbBlood.append("AB+"); m_cmbBlood.append("AB-");
	m_cmbBlood.set_active(0);

	m_cmbMaritalStatus.append("Casado(a)");
	m_cmbMaritalStatus.append("Divorciado(a)");
	m_cmbMaritalStatus.append("Solteiro(a)");
	m_cmbMaritalStatus.append("Viúvo(a)");
	m_cmbMaritalStatus.append("Outro...");
	m_cmbMaritalStatus.set_active(2);

	m_lblTitle.set_alignment(0.00f, 0.5f);
	m_lblTitle.set_use_markup();
	m_lblName.set_alignment(1.0f, 0.5f);
	m_lblName.set_mnemonic_widget(m_txtName);
	m_lblHeight.set_alignment(1.0f, 0.5f);
	m_lblHeight.set_mnemonic_widget(m_txtHeight);
	m_txtHeight.set_size_request(80, -1);
	m_lblBlood.set_alignment(1.0f, 0.5f);
	m_lblBlood.set_mnemonic_widget(m_cmbBlood);
	m_lblSex.set_alignment(1.0f, 0.5f);
	m_lblSex.set_mnemonic_widget(m_rbMale);
	m_lblNacionality.set_alignment(1.0f, 0.5f);
	m_lblNacionality.set_mnemonic_widget(m_txtNacionality);
	m_lblBirthday.set_alignment(1.0f, 0.5f);
	m_lblBirthday.set_mnemonic_widget(m_txtBirthday);
	m_lblBirthplace.set_alignment(1.0f, 0.5f);
	m_lblBirthplace.set_mnemonic_widget(m_txtBirthday);
	m_lblProfession.set_alignment(1.0f, 0.5f);
	m_lblProfession.set_mnemonic_widget(m_txtProfession);
	m_lblTaxNumber.set_alignment(1.0f, 0.5f);
	m_lblTaxNumber.set_mnemonic_widget(m_txtTaxNumber);
	m_lblMaritalStatus.set_alignment(1.0f, 0.5f);
	m_lblMaritalStatus.set_mnemonic_widget(m_cmbMaritalStatus);

	m_lblAddress.set_alignment(1.0f, 0.5f);
	m_lblAddress.set_use_markup();
	m_lblAddress.set_mnemonic_widget(m_txtAddress);
	//m_txtLocation.set_size_request(95,-1);
	m_lblLocation.set_alignment(1.0f, 0.5f);
	m_lblLocation.set_use_markup();
	m_lblLocation.set_mnemonic_widget(m_txtLocation);
	m_lblContact.set_alignment(1.0f, 0.5f);
	m_lblContact.set_use_markup();
	m_lblContact.set_mnemonic_widget(m_txtPhone);
	m_lblEmail.set_alignment(1.0f, 0.5f);
	m_lblEmail.set_use_markup();
	m_lblEmail.set_mnemonic_widget(m_txtEmail);
	m_lblReferer.set_alignment(1.0f, 0.5f);
	m_lblReferer.set_use_markup();
	m_lblReferer.set_mnemonic_widget(m_txtReferer);
	m_lblZip.set_padding(0, 0);
	box1->show_all();

	tbPersonal->set_row_spacings(5);
	tbContacts->set_row_spacings(5);
	m_txtName.set_max_length(70);
	m_txtAddress.set_max_length(100);
	m_txtNacionality.set_max_length(30);
	m_txtBirthday.set_max_length(10);
	m_txtBirthplace.set_max_length(30);
	m_txtProfession.set_max_length(30);
	m_txtLocation.set_max_length(50);
	m_txtTaxNumber.set_max_length(9);
	m_txtPhone.set_size_request(75, -1);
	m_txtPhone.set_max_length(9);
	m_txtReferer.set_max_length(70);
	m_txtEmail.set_max_length(255);
	m_txtCellphone.set_max_length(9);
	m_txtCellphone.set_size_request(75, -1);
	m_txtZip1.set_size_request(44, -1);
	m_txtZip1.set_max_length(4);
	m_txtZip2.set_size_request(32, -1);
	m_txtZip2.set_max_length(3);
	m_txtBirthday.set_editable(false);
	frPersonal->set_shadow_type(ShadowType::SHADOW_OUT);
	frContacts->set_shadow_type(ShadowType::SHADOW_OUT);
	((Label*) frPersonal->get_label_widget())->set_use_markup();
	((Label*) frContacts->get_label_widget())->set_use_markup();

	helper_entry_focusOut(m_txtPhone, m_phoneStatus, (char*)"Telefone...");
	helper_entry_focusOut(m_txtCellphone, m_cellphoneStatus, (char*)"Telemóvel...");

	m_txtName.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtName));
	m_txtAddress.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtAddress));
	m_txtNacionality.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtNacionality));
	m_txtLocation.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtLocation));
	m_txtBirthplace.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtBirthplace));
	m_txtProfession.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtProfession));
	m_txtEmail.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtEmail));
	m_txtReferer.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtReferer));
	m_txtPhone.signal_focus_in_event().connect(sigc::mem_fun(*this, &PatientWindow::on_PhoneFocusIn));
	m_txtPhone.signal_focus_out_event().connect(sigc::mem_fun(*this, &PatientWindow::on_PhoneFocusOut));
	m_txtCellphone.signal_focus_in_event().connect(sigc::mem_fun(*this, &PatientWindow::on_CellphoneFocusIn));
	m_txtCellphone.signal_focus_out_event().connect(sigc::mem_fun(*this, &PatientWindow::on_CellphoneFocusOut));
	m_txtBirthday.signal_focus_in_event().connect(sigc::mem_fun(*this, &PatientWindow::on_focusIn_show_calendar));

	set_skip_pager_hint();
	set_skip_taskbar_hint();
	get_vbox()->pack_start(*box1);

	// Set differences
	if(type == PW_TYPE_ADD) {
		add_button(Stock::ADD, RESPONSE_ACCEPT);
	} else {
		add_button(Stock::EDIT, RESPONSE_ACCEPT);
		m_lblTitle.set_text("<b><big>Ficha do paciente</big></b>");
		m_lblTitle.set_use_markup();
	}
	add_button(Stock::CANCEL, RESPONSE_CANCEL);
	//set_size_request(638,375);
	set_resizable(false);
}

guint32 PatientWindow::get_id(void) const
{
	return m_id;
}

void PatientWindow::set_person(const Person& p)
{
	char *str, *zip2;

	m_id = p.get_id();
	m_txtName.set_text(p.get_name());
	m_txtHeight.set_value(p.get_height());
	m_rbFemale.set_active(!p.get_sex());
	str = strdup(p.get_zip());
	zip2 = strchr(str, '-');
	if(zip2 != NULL) {
		*zip2 = '\0';
		zip2++;
		m_txtZip1.set_text(str);
		m_txtZip2.set_text(zip2);
	}
	free(str);
	m_txtNacionality.set_text(p.get_nationality());
	str = (char*)calloc(sizeof(char), 11);
	sprintf(str, "%u/%u/%u", p.get_birthday().get_day(), p.get_birthday().get_month(), p.get_birthday().get_year());
	m_txtBirthday.set_text(str);

	m_txtBirthplace.set_text(p.get_birthplace());
	m_txtProfession.set_text(p.get_profession());
	m_cmbBlood.set_active(p.get_blood_type() - 1);
	m_txtAddress.set_text(p.get_address());
	m_cmbMaritalStatus.set_active(p.get_marital_status() - 1);
	m_txtLocation.set_text(p.get_locality());
	sprintf(str, "%u", p.get_phone());
	m_txtPhone.set_text((ustring)str);
	sprintf(str, "%u", p.get_cellphone());
	m_txtCellphone.set_text((ustring)str);
	m_txtEmail.set_text(p.get_email());
	m_txtReferer.set_text(p.get_referer());
	sprintf(str, "%u", p.get_tax_number());
	m_txtTaxNumber.set_text((ustring)str);
	free(str);
	m_cellphoneStatus = m_phoneStatus=false;
	helper_entry_set_state(m_txtPhone, false);
	helper_entry_set_state(m_txtCellphone, false);
}

void PatientWindow::get_person(Person& p) const
{
	p.set_name(m_txtName.get_text());
	p.set_height((float)m_txtHeight.get_value());
	p.set_sex(m_rbMale.get_active());
	p.set_zip(m_txtZip1.get_text().c_str(), m_txtZip2.get_text().c_str());
	p.set_nationality(m_txtNacionality.get_text());
	p.set_birthday(Util::parse_date((string) m_txtBirthday.get_text()));
	p.set_birthplace(m_txtBirthplace.get_text());
	p.set_profession(m_txtProfession.get_text());
	p.set_blood_type(m_cmbBlood.get_active_row_number() + 1);
	p.set_address(m_txtAddress.get_text());
	p.set_marital_status(m_cmbMaritalStatus.get_active_row_number() + 1);
	p.set_locality(m_txtLocation.get_text());
	p.set_phone(atoi(m_txtPhone.get_text().c_str()));
	p.set_cellphone(atoi(m_txtCellphone.get_text().c_str()));
	p.set_email(m_txtEmail.get_text());
	p.set_referer(m_txtReferer.get_text());
	p.set_tax_number(atoi(m_txtTaxNumber.get_text().c_str()));
}

bool PatientWindow::on_PhoneFocusIn(GdkEventFocus *focus)
{
	return helper_entry_focusIn(m_txtPhone, m_phoneStatus);
}
bool PatientWindow::on_PhoneFocusOut(GdkEventFocus *focus)
{
	return helper_entry_focusOut(m_txtPhone, m_phoneStatus, (char*)"Telefone...");
}
bool PatientWindow::on_CellphoneFocusIn(GdkEventFocus *focus)
{
	return helper_entry_focusIn(m_txtCellphone, m_cellphoneStatus);
}
bool PatientWindow::on_CellphoneFocusOut(GdkEventFocus *focus)
{
	return helper_entry_focusOut(m_txtCellphone, m_cellphoneStatus, (char*)"Telemóvel...");
}

bool helper_entry_focusIn(NumericEntry& entry, bool& value)
{
	if(value) {
		helper_entry_set_state(entry, false);
		entry.set_allow_alphanumeric();
		entry.set_text("");
		entry.set_allow_alphanumeric(false);
		entry.set_max_length(9);
		value = false;
	}

	return true;
}

bool helper_entry_focusOut(NumericEntry& entry, bool& value, char *text)
{
	if(!value && entry.get_text_length() == 0) {
		helper_entry_set_state(entry, true);
		entry.set_allow_alphanumeric();
		entry.set_text((ustring)text);
		entry.set_allow_alphanumeric(false);
		value = true;
	}

	return true;
}

static void inline helper_entry_set_state(NumericEntry& entry, bool state)
{
	if(state) {
		entry.set_max_length(14);
		entry.modify_text(STATE_NORMAL, Gdk::Color(ustring("Grey")));
	} else {
		entry.unset_text(STATE_NORMAL);
		entry.set_max_length(9);
	}
}

void NumericEntry::on_insert_text(const Glib::ustring& text, int *position)
{
	bool allow(true);

	for(int i=0; i < text.length(); i++) {
		if(text.c_str()[i] < '0' || text.c_str()[i] > '9' ) {
			allow = false;
			break;
		}
	}
	if(allow || m_allow_alphanumeric)
		Gtk::Entry::on_insert_text(text, position);
}

void DateEntry::on_insert_text(const Glib::ustring& text, int *position)
{
	bool allow(true);

	for(int i=0; i < text.length(); i++) {
		if(text.c_str()[i] < '0' || text.c_str()[i] > '9' ) {
			allow = false;
			break;
		}
	}
	if(allow || m_allow_alphanumeric)
		Gtk::Entry::on_insert_text(text, position);
}

bool PatientWindow::on_focusOut_trim(GdkEventFocus *event, Entry* entry)
{
	ustring str(entry->get_text());

	entry->set_text(Util::string_trim(str));
	return true;
}

bool PatientWindow::on_delete_event(GdkEventAny *event)
{
	std::cout<< "on delete event"<<std::endl;
	return true;
}

bool PatientWindow::on_focusIn_show_calendar(GdkEventFocus *focus)
{
	int x, y;

	get_window()->get_position(x, y);
	m_wincal.popup(m_txtBirthday, x, y);
	m_txtBirthplace.grab_focus();
	return true;
}

/* Calendar Window */
CalendarWindow::CalendarWindow(Window& win) : Dialog("", false, false), m_wDate(NULL)
{
	get_vbox()->pack_start(m_cal, true, true);
	set_decorated(false);
	m_cal.signal_day_selected_double_click().connect(sigc::mem_fun(*this, &CalendarWindow::on_selected_day));
}

void CalendarWindow::selected_date(Date& date) const
{
	m_cal.get_date(date);
}

void CalendarWindow::popup(Entry& widget, unsigned int x, unsigned int y)
{
	int tmpx, tmpy;
	m_wDate = &widget;
	Date tmp;

	widget.get_window()->get_position(tmpx, tmpy);
	move(x + tmpx, tmpy + y + widget.get_height());

	if(m_wDate->get_text().length() > 0) {
		tmp = Util::parse_date((string)m_wDate->get_text());
		m_cal.select_month(tmp.get_month()-1, tmp.get_year());
		m_cal.select_day(tmp.get_day());
	}
	m_cal.show();
	run();
}

void CalendarWindow::on_selected_day(void)
{
	Date tmp;
	m_cal.get_date(tmp);
	m_wDate->set_text(tmp.format_string((ustring)"%d/%m/%Y"));
	response(RESPONSE_ACCEPT);
	hide();
}

bool CalendarWindow::on_focus_out_event(GdkEventFocus *focus)
{
	Date tmp;
	m_cal.get_date(tmp);
	m_wDate->set_text(tmp.format_string((ustring)"%d/%m/%Y"));
	response(RESPONSE_ACCEPT);
	hide();
	return true;
}
