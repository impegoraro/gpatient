/*
 * patient-window.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <cstring>
#include <cstdlib>
#include <iostream>

#include "../db/dbhandler.h"
#include "widgets/widgets.h"
#include "patient-window.h"
#include "util.h"
#include "main.h"

using namespace std;
using namespace Glib;
using namespace Gtk;
using namespace Widgets;


static bool helper_entry_focusIn(NumericEntry& entry, bool& value);
static bool helper_entry_focusOut(NumericEntry& entry, bool& value, char *text);
static void inline helper_entry_set_state(NumericEntry& entry, bool state = true);

PatientWindow::PatientWindow(Gtk::Window& parent, const std::string& title, PatientWindowType type) :
	Window(WINDOW_TOPLEVEL), m_type(type),
	m_lblTitle("<big><b>Ficha de inscrição de novo paciente</b></big>"),
	m_lblName("<b>_Nome:</b>", true), m_lblHeight("<b>_Altura:</b>", true),
	m_lblBlood("<b>_Tipo de Sangue:</b>", true), m_lblSex("<b>_Sexo:</b>", true),
	m_txtHeight(Adjustment::create(1.0, 0.0, 3.0, 0.01, 0.05), 0.1, 2),
	m_grpSex(), m_rbMale(m_grpSex, "_Masculino", true), m_rbFemale(m_grpSex, "_Feminino", true),
	m_lblBirthday("<b>_Data de Nascimento:</b>", true), m_lblBirthplace("<b>_Local de Nascimento:</b>", true),
	m_lblNacionality("<b>Na_cionalidade:</b>", true), m_lblProfession("<b>_Profissão:</b>", true),
	m_lblIdentificationCard("B. I_dentidade", true),
	m_lblTaxNumber("Nº _Identificação Fiscal:", true), m_lblMaritalStatus("<b>Es_tado Civil:</b>", true),
	m_lblAddress("<b>_Morada:</b>", true), m_lblLocation("<b>_Localidade:</b>", true),
	m_lblZip("-"), m_lblContact("<b>_Contactos:</b>", true), m_lblReferer("_Enviado por:", true),
	m_lblEmail("<b>_Email:</b>", true), m_cellphoneStatus(false), m_phoneStatus(false), m_dateStatus(false), 
	m_wincal(*this, m_txtBirthday),m_tnUnique(false), m_icUnique(false),
	m_btnAccept(type == PW_TYPE_ADD? Stock::ADD:Stock::EDIT), m_btnCancel(Stock::CANCEL)
{
	Frame *frPersonal = manage(new Frame("<b>Dados Pessoais</b>"));
	Frame *frContacts = manage(new Frame("<b>Morada e Contacto</b>"));
	Box *bZip = manage(new HBox(false, 0)), *bPhones = manage(new HBox(true, 0));
	Grid *tbPersonal = manage(new Grid());
	Grid *tbContacts = manage(new Grid());
	Grid *mGrid = manage(new Grid());
	ButtonBox *btnBox = manage(new ButtonBox());
	set_title((ustring)title);
	set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
	set_transient_for(parent);
	set_modal();
	
	frPersonal->add(*tbPersonal);
	frContacts->add(*tbContacts);
	frContacts->set_size_request(100,-1);

	tbPersonal->attach(m_lblName, 0, 0, 1, 1);
	tbPersonal->attach_next_to(m_txtName, m_lblName, POS_RIGHT, 3, 1);
	tbPersonal->attach(m_lblHeight, 0, 1, 1, 1);
	tbPersonal->attach_next_to(m_txtHeight, m_lblHeight, POS_RIGHT, 3, 1);
	tbPersonal->attach(m_lblSex, 0, 2, 1, 1);
	tbPersonal->attach_next_to(m_rbMale, m_lblSex, POS_RIGHT, 1, 1);
	tbPersonal->attach_next_to(m_rbFemale, m_rbMale, POS_RIGHT, 1, 1);
	tbPersonal->attach(m_lblNacionality, 0, 3, 1, 1);
	tbPersonal->attach_next_to(m_txtNationality, m_lblNacionality, POS_RIGHT, 3, 1);
	tbPersonal->attach(m_lblBirthday, 0, 4, 1, 1);
	tbPersonal->attach_next_to(m_txtBirthday, m_lblBirthday, POS_RIGHT, 3, 1);
	tbPersonal->attach(m_lblBirthplace, 0, 5, 1, 1);
	tbPersonal->attach_next_to(m_txtBirthplace, m_lblBirthplace, POS_RIGHT, 3, 1);
	tbPersonal->attach(m_lblProfession, 0, 6, 1, 1);
	tbPersonal->attach_next_to(m_txtProfession, m_lblProfession, POS_RIGHT, 3, 1);
	tbPersonal->attach(m_lblTaxNumber, 0, 7, 1, 1);
	tbPersonal->attach(m_lblIdentificationCard, 0, 8, 1, 1);
	tbPersonal->attach_next_to(m_txtIdentificationCard, m_lblIdentificationCard, POS_RIGHT, 3, 1);
	tbPersonal->attach_next_to(m_txtTaxNumber, m_lblTaxNumber, POS_RIGHT, 3, 1);
	tbPersonal->attach(m_lblMaritalStatus, 0, 9, 1, 1);
	tbPersonal->attach_next_to(m_cmbMaritalStatus, m_lblMaritalStatus, POS_RIGHT, 3, 1);
	tbPersonal->attach(m_lblBlood, 0, 10, 1, 1);
	tbPersonal->attach_next_to(m_cmbBlood, m_lblBlood, POS_RIGHT, 3, 1);
	
	bZip->pack_start(m_txtZip1, false, true, 0);
	bZip->pack_start(m_lblZip, false, true, 0);
	bZip->pack_start(m_txtZip2, false, true, 0);

	bPhones->pack_start(m_txtPhone, true, true, 0);
	bPhones->pack_start(m_txtCellphone, true, true, 0);

	tbContacts->attach(m_lblAddress, 0, 0, 1, 2);
	tbContacts->attach(m_txtAddress, 1, 0, 2, 1);
	tbContacts->attach(m_lblLocation, 0, 2, 1, 1);
	tbContacts->attach(m_txtLocation, 1, 2, 1, 1);
	tbContacts->attach_next_to(*bZip, m_txtLocation, Gtk::PositionType::POS_RIGHT, 1, 1);
	tbContacts->attach(m_lblContact, 0, 3, 1, 1);
	tbContacts->attach(*bPhones, 1, 3, 2, 1);
	tbContacts->attach(m_lblEmail, 0, 4, 1, 1);
	tbContacts->attach(m_txtEmail, 1, 4, 2, 1);
	tbContacts->attach(m_lblReferer, 0, 5, 1, 1);
	tbContacts->attach(m_txtReferer, 1, 5, 2, 1);

	btnBox->add(m_btnAccept);
	btnBox->add(m_btnCancel);

	mGrid->attach(m_lblTitle, 0, 1, 1, 1);
	mGrid->attach(*frPersonal, 0, 2, 1, 1);
	mGrid->attach(*frContacts, 1, 2, 1, 1);
	mGrid->attach_next_to(*btnBox, *frContacts, PositionType::POS_BOTTOM, 1, 1);

	add(*mGrid);

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

	tbPersonal->set_row_spacing(8);
	tbPersonal->set_column_spacing(2);
	tbContacts->set_row_spacing(8);
	tbContacts->set_column_spacing(2);

	btnBox->set_layout(BUTTONBOX_END);

	m_lblTitle.set_margin_top(5);
	m_lblTitle.set_margin_left(15);
	m_lblTitle.set_margin_bottom(15);
	m_lblTitle.set_alignment(0.00f, 0.5f);
	m_lblTitle.set_use_markup();
	m_lblName.set_alignment(1.0f, 0.5f);
	m_lblName.set_use_markup();
	m_lblName.set_mnemonic_widget(m_txtName);
	m_lblHeight.set_alignment(1.0f, 0.5f);
	m_lblHeight.set_mnemonic_widget(m_txtHeight);
	m_lblHeight.set_use_markup();
	m_txtHeight.set_size_request(80, -1);
	m_lblBlood.set_alignment(1.0f, 0.5f);
	m_lblBlood.set_use_markup();
	m_lblBlood.set_mnemonic_widget(m_cmbBlood);
	m_lblSex.set_alignment(1.0f, 0.5f);
	m_lblSex.set_use_markup();
	m_lblSex.set_mnemonic_widget(m_rbMale);
	m_lblNacionality.set_alignment(1.0f, 0.5f);
	m_lblNacionality.set_use_markup();
	m_lblNacionality.set_mnemonic_widget(m_txtNationality);
	m_lblBirthday.set_alignment(1.0f, 0.5f);
	m_lblBirthday.set_mnemonic_widget(m_txtBirthday);
	m_lblBirthday.set_use_markup();
	m_txtBirthday.set_icon_from_icon_name("x-office-calendar", ENTRY_ICON_SECONDARY);
	m_txtBirthday.set_icon_tooltip_text("Escolher data de nascimento", ENTRY_ICON_SECONDARY);
	m_lblBirthplace.set_alignment(1.0f, 0.5f);
	m_lblBirthplace.set_use_markup();
	m_lblBirthplace.set_mnemonic_widget(m_txtBirthday);
	m_lblProfession.set_alignment(1.0f, 0.5f);
	m_lblProfession.set_use_markup();
	m_lblProfession.set_mnemonic_widget(m_txtProfession);
	m_lblTaxNumber.set_alignment(1.0f, 0.5f);
	m_lblTaxNumber.set_use_markup();
	m_lblTaxNumber.set_mnemonic_widget(m_txtTaxNumber);
	m_lblMaritalStatus.set_alignment(1.0f, 0.5f);
	m_lblMaritalStatus.set_use_markup();
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
	m_lblIdentificationCard.set_alignment(1.0f, 0.5f);
	m_lblIdentificationCard.set_use_markup();
	m_lblIdentificationCard.set_mnemonic_widget(m_txtIdentificationCard);

	m_txtName.set_max_length(70);
	m_txtAddress.set_max_length(100);
	m_txtNationality.set_max_length(30);
	m_txtBirthday.set_max_length(10);
	m_txtBirthplace.set_max_length(30);
	m_txtProfession.set_max_length(30);
	m_txtLocation.set_max_length(50);
	m_txtTaxNumber.set_max_length(9);
	m_txtIdentificationCard.set_max_length(9);
	m_txtPhone.set_max_length(9);
	m_txtPhone.set_width_chars(13);
	m_txtReferer.set_max_length(70);
	m_txtEmail.set_max_length(255);
	m_txtCellphone.set_max_length(9);
	m_txtCellphone.set_width_chars(13);
	m_txtZip1.set_max_length(4);
	m_txtZip1.set_hexpand(false);
	m_txtZip1.set_width_chars(4);
	m_txtZip2.set_max_length(3);
	m_txtZip2.set_width_chars(3);
	m_txtZip2.set_hexpand(false);
	m_txtBirthday.set_editable(false);
	frPersonal->set_shadow_type(ShadowType::SHADOW_OUT);
	frContacts->set_shadow_type(ShadowType::SHADOW_OUT);
	((Label*) frPersonal->get_label_widget())->set_use_markup();
	((Label*) frContacts->get_label_widget())->set_use_markup();
	
	m_txtPhone.set_margin_right(5);
	
	tbPersonal->set_margin_left(5);
	tbPersonal->set_margin_right(5);
	tbPersonal->set_margin_top(5);
	tbPersonal->set_margin_bottom(5);
	tbContacts->set_margin_left(5);
	tbContacts->set_margin_right(5);
	tbContacts->set_margin_top(5);
	tbContacts->set_margin_bottom(5);

	frPersonal->set_margin_left(12);
	frPersonal->set_margin_left(5);
	frContacts->set_margin_left(12);
	frContacts->set_margin_right(10);

	btnBox->set_spacing(5);
	btnBox->set_margin_top(5);
	btnBox->set_margin_bottom(12);
	btnBox->set_margin_right(10);

	helper_entry_focusOut(m_txtPhone, m_phoneStatus, (char*)"Telefone...");
	helper_entry_focusOut(m_txtCellphone, m_cellphoneStatus, (char*)"Telemóvel...");

	m_txtName.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtName));
	m_txtAddress.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtAddress));
	m_txtNationality.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtNationality));
	m_txtLocation.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtLocation));
	m_txtBirthplace.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtBirthplace));
	m_txtProfession.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtProfession));
	m_txtEmail.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtEmail));
	m_txtReferer.signal_focus_out_event().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::on_focusOut_trim), &m_txtReferer));
	m_txtPhone.signal_focus_in_event().connect(sigc::mem_fun(*this, &PatientWindow::on_PhoneFocusIn));
	m_txtPhone.signal_focus_out_event().connect(sigc::mem_fun(*this, &PatientWindow::on_PhoneFocusOut));
	m_txtCellphone.signal_focus_in_event().connect(sigc::mem_fun(*this, &PatientWindow::on_CellphoneFocusIn));
	m_txtCellphone.signal_focus_out_event().connect(sigc::mem_fun(*this, &PatientWindow::on_CellphoneFocusOut));
	m_txtTaxNumber.signal_focus_out_event().connect(sigc::mem_fun(*this, &PatientWindow::on_taxNumber_focus_out));
	m_txtIdentificationCard.signal_focus_out_event().connect(sigc::mem_fun(*this, &PatientWindow::on_identificationCard_focus_out));
	//m_txtBirthday.signal_focus_in_event().connect(sigc::mem_fun(*this, &PatientWindow::on_focusIn_show_calendar));
	m_txtBirthday.signal_icon_press().connect(sigc::mem_fun(*this, &PatientWindow::on_txtBirthday_iconPress));
	m_btnCancel.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::activate_close), true));
	m_btnAccept.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &PatientWindow::activate_close), false));
	signal_key_press_event().connect(sigc::mem_fun(*this, &PatientWindow::close_on_esc));
	add_events(Gdk::KEY_PRESS_MASK);
	
	set_skip_pager_hint();
	set_skip_taskbar_hint();

	// Set differences
	if(type == PW_TYPE_EDIT) {
		m_lblTitle.set_text("<b><big>Ficha do paciente</big></b>");
		m_lblTitle.set_use_markup();
	}

	//set_size_request(638,375);
	set_resizable(false);
	mGrid->show_all();
}

PatientWindow::~PatientWindow()
{
}

guint32 PatientWindow::get_id(void) const
{
	return m_id;
}

bool PatientWindow::close_on_esc(GdkEventKey* event)
{
	if (event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Escape) {
		clean();
		hide();
    	return true;
  	}
  		return false;
}

void PatientWindow::set_person(const Person& p)
{
	guint16 zip1, zip2;

	m_id = p.get_id();
	m_txtName.set_text(p.get_name());
	m_txtHeight.set_value(p.get_height());
	m_rbFemale.set_active(!p.get_sex());
	p.get_zip(zip1, zip2);
	m_txtZip1.set_text(zip1);
	m_txtZip2.set_text(zip2);
	m_txtNationality.set_text(p.get_nationality());
	m_txtBirthday.set_text(p.get_birthday().format_string("%Y-%m-%d"));
	m_txtBirthplace.set_text(p.get_birthplace());
	m_txtProfession.set_text(p.get_profession());
	m_cmbBlood.set_active(p.get_blood_type() - 1);
	m_txtAddress.set_text(p.get_address());
	m_cmbMaritalStatus.set_active(p.get_marital_status() - 1);
	m_txtLocation.set_text(p.get_locality());
	if(p.get_phone() == 0)
		m_txtPhone.set_text("");
	else
		m_txtPhone.set_text(p.get_phone());
	if(p.get_cellphone() == 0)
		m_txtCellphone.set_text("");
	else
		m_txtCellphone.set_text(p.get_cellphone());
	m_txtEmail.set_text(p.get_email());
	m_txtReferer.set_text(p.get_referer());
	if(p.get_tax_number() == 0)
		m_txtTaxNumber.set_text("");
	else	
		m_txtTaxNumber.set_text(p.get_tax_number());
	m_cellphoneStatus = m_phoneStatus = false;
	if(p.get_identification_card() == 0)
		m_txtIdentificationCard.set_text("");
	else
		m_txtIdentificationCard.set_text(p.get_identification_card());
	helper_entry_set_state(m_txtPhone, false);
	helper_entry_set_state(m_txtCellphone, false);
}

void PatientWindow::get_person(Person& p) const
{
	ustring tmp;
	guint32 val;
	
	p.set_id(m_id);
	p.set_name(m_txtName.get_text());
	p.set_height((float)m_txtHeight.get_value());
	p.set_sex(m_rbMale.get_active());
	//p.set_zip((guint16) atoi(m_txtZip1.get_text().c_str()), (guint16) atoi(m_txtZip2.get_text().c_str()));
	tmp = m_txtZip1.get_text() + "-" + m_txtZip2.get_text();
	p.set_zip(tmp);
	p.set_nationality(m_txtNationality.get_text());
	p.set_birthday(Util::parse_date((string) m_txtBirthday.get_text()));
	p.set_birthplace(m_txtBirthplace.get_text());
	p.set_profession(m_txtProfession.get_text());
	p.set_blood_type(m_cmbBlood.get_active_row_number() + 1);
	p.set_address(m_txtAddress.get_text());
	p.set_marital_status(m_cmbMaritalStatus.get_active_row_number() + 1);
	p.set_locality(m_txtLocation.get_text());
	val = (guint32)m_txtPhone.get_value();
	p.set_phone(val);
	p.set_cellphone((guint32)m_txtCellphone.get_value());
	p.set_email(m_txtEmail.get_text());
	p.set_referer(m_txtReferer.get_text());
	p.set_tax_number((guint32)m_txtTaxNumber.get_value());
	p.set_identification_card((guint32) m_txtIdentificationCard.get_value());
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

	return false;
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

	return false;
}

static void inline helper_entry_set_state(NumericEntry& entry, bool state)
{
	if(state) {
		entry.set_max_length(14);
		entry.override_color(Gdk::RGBA("Grey"), STATE_FLAG_NORMAL);
	} else {
		entry.override_color(Gdk::RGBA("Black"), STATE_FLAG_NORMAL);
		entry.set_max_length(9);
	}
}

bool PatientWindow::on_focusOut_trim(GdkEventFocus *event, Entry* entry)
{
	ustring str(entry->get_text());

	entry->set_text(Util::string_trim(str));
	return false;
}

void PatientWindow::on_txtBirthday_iconPress(Gtk::EntryIconPosition, const GdkEventButton*)
{
	int x, y;

	get_window()->get_position(x, y);
	m_wincal.popup(x + 125, y + 200);
}

bool PatientWindow::on_delete_event(GdkEventAny *event)
{
	hide();
	clean();
	return true;
}

void PatientWindow::clean()
{
	m_id = 0;

	m_txtName.set_text("");
	m_txtHeight.set_value(1.00f);
	m_rbMale.set_active();
	m_txtZip1.set_text("");
	m_txtZip2.set_text("");
	m_txtNationality.set_text("");
	m_txtBirthday.set_text("");
	m_txtBirthplace.set_text("");
	m_txtProfession.set_text("");
	m_cmbBlood.set_active(0);
	m_txtAddress.set_text("");
	m_cmbMaritalStatus.set_active(2);
	m_txtLocation.set_text("");
	m_cellphoneStatus = m_phoneStatus = false;
	m_txtPhone.set_text("");
	m_txtCellphone.set_text("");
	m_txtIdentificationCard.set_text("");
	helper_entry_focusOut(m_txtPhone, m_phoneStatus, (char*)"Telefone...");
	helper_entry_focusOut(m_txtCellphone, m_cellphoneStatus, (char*)"Telemóvel...");
	m_txtEmail.set_text("");
	m_txtReferer.set_text("");
	m_txtTaxNumber.set_text("");
	m_txtIdentificationCard.set_text("");

	m_txtName.unset_background_color(STATE_FLAG_NORMAL);
	m_txtName.unset_color(STATE_FLAG_NORMAL);
	m_txtName.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtHeight.unset_background_color(STATE_FLAG_NORMAL);
	m_txtHeight.unset_color(STATE_FLAG_NORMAL);
	m_txtHeight.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtZip1.unset_background_color(STATE_FLAG_NORMAL);
	m_txtZip1.unset_color(STATE_FLAG_NORMAL);
	m_txtZip1.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtZip2.unset_background_color(STATE_FLAG_NORMAL);
	m_txtZip2.unset_color(STATE_FLAG_NORMAL);
	m_txtZip2.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtNationality.unset_background_color(STATE_FLAG_NORMAL);
	m_txtNationality.unset_color(STATE_FLAG_NORMAL);
	m_txtNationality.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtBirthday.unset_background_color(STATE_FLAG_NORMAL);
	m_txtBirthday.unset_color(STATE_FLAG_NORMAL);
	m_txtBirthplace.unset_background_color(STATE_FLAG_NORMAL);
	m_txtBirthplace.unset_color(STATE_FLAG_NORMAL);
	m_txtBirthplace.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtProfession.unset_background_color(STATE_FLAG_NORMAL);
	m_txtProfession.unset_color(STATE_FLAG_NORMAL);
	m_txtProfession.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtAddress.unset_background_color(STATE_FLAG_NORMAL);
	m_txtAddress.unset_color(STATE_FLAG_NORMAL);
	m_txtAddress.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtLocation.unset_background_color(STATE_FLAG_NORMAL);
	m_txtLocation.unset_color(STATE_FLAG_NORMAL);
	m_txtLocation.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtPhone.unset_background_color(STATE_FLAG_NORMAL);
	m_txtPhone.unset_color(STATE_FLAG_NORMAL);
	m_txtPhone.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtCellphone.unset_background_color(STATE_FLAG_NORMAL);
	m_txtCellphone.unset_color(STATE_FLAG_NORMAL);
	m_txtCellphone.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtIdentificationCard.unset_background_color(STATE_FLAG_NORMAL);
	m_txtIdentificationCard.unset_color(STATE_FLAG_NORMAL);
	m_txtIdentificationCard.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtEmail.unset_background_color(STATE_FLAG_NORMAL);
	m_txtEmail.unset_color(STATE_FLAG_NORMAL);
	m_txtEmail.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtReferer.unset_background_color(STATE_FLAG_NORMAL);
	m_txtReferer.unset_color(STATE_FLAG_NORMAL);
	m_txtReferer.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtTaxNumber.unset_background_color(STATE_FLAG_NORMAL);
	m_txtTaxNumber.unset_color(STATE_FLAG_NORMAL);
	m_txtTaxNumber.unset_icon(ENTRY_ICON_SECONDARY);
	m_txtIdentificationCard.unset_background_color(STATE_FLAG_NORMAL);
	m_txtIdentificationCard.unset_color(STATE_FLAG_NORMAL);
	m_txtIdentificationCard.unset_icon(ENTRY_ICON_SECONDARY);
}

void PatientWindow::show()
{
	m_txtName.grab_focus();
	return Window::show();
}

void PatientWindow::show_all()
{
	m_txtName.grab_focus();
	return Window::show_all();
}

sigc::signal<void, PatientWindow &>& PatientWindow::signal_add()
{
	return m_signal_add;
}

void PatientWindow::activate_close(bool val)
{
	if(val){
		clean();
		hide();
	} else {
		guint32 phone = 0, cell = 0;
		
		if(!m_phoneStatus) {
			std::stringstream ss;
			ss<< m_txtPhone.get_text().raw();
			ss>>phone;
		}
		if(!m_cellphoneStatus) {
			std::stringstream ss;
			ss<< m_txtCellphone.get_text().raw();
			ss>>cell;
		}
		
		if(m_txtName.get_text_length() > 0 && m_txtNationality.get_text_length() > 0 && m_txtBirthday.get_text_length() > 0 && m_txtProfession.get_text_length() > 0 &&
				m_txtBirthplace.get_text_length() > 0 && (m_txtTaxNumber.get_text_length() > 0 || m_txtIdentificationCard.get_text_length() > 0) && m_txtAddress.get_text_length() > 0 &&
				m_txtZip1.get_text_length() > 0 && m_txtLocation.get_text_length() > 0 && m_txtZip2.get_text_length() > 0 && phone > 0 && cell > 0 && m_txtEmail.get_text_length() > 0 && 
				(m_tnUnique || m_icUnique)) {
			m_signal_add(*this);
			hide();
			clean();
		} else {
			MessageDialog msgbox("Não é possível adicionar o novo paciente.", true, MESSAGE_ERROR, BUTTONS_OK, true);
			validate_entry(m_txtName);
			validate_entry(m_txtNationality);
			validate_entry(m_txtBirthday, false);
			validate_entry(m_txtProfession);
			validate_entry(m_txtBirthplace);
			validate_entry(m_txtAddress);
			validate_entry(m_txtLocation);
			validate_entry(m_txtZip1);
			validate_entry(m_txtZip2);
			validate_entry(m_txtCellphone, cell);
			validate_entry(m_txtPhone, phone);
			validate_entry(m_txtEmail);
			validate_entry(m_txtTaxNumber, true, &m_txtIdentificationCard);
			validate_entry(m_txtIdentificationCard, true, &m_txtTaxNumber);

			msgbox.set_title("Validação dos dados");
			//msgbox.set_secondary_text("Todos os campos são de preenchimento obrigatório, com a excepção do campo <i>Enviado Por</i>.", true);
			msgbox.set_secondary_text("Falta preencher os campos asinalados.\nO bilhete de identidade só é obrigatório se a pessoa não tiver NIF.", true);
			msgbox.run();
		}
	}
}

void PatientWindow::set_window_type(PatientWindow::PatientWindowType type)
{
	if(m_type == type) return;  //Do nothing if the same type

	Image *img = dynamic_cast<Image*>(m_btnAccept.get_image());
	if(img != NULL)
		delete img;

	m_type = type;
	switch(m_type) {
		case PW_TYPE_EDIT: {
			//Image img(Stock::EDIT, Gtk::ICON_SIZE_BUTTON);
			img = new Image(Stock::EDIT, Gtk::ICON_SIZE_BUTTON);;
			img->set(Stock::EDIT, Gtk::ICON_SIZE_BUTTON);
			m_btnAccept.set_image(*img);
			//m_btnAccept.set_text("");
			m_lblTitle.set_text("<b><big>Ficha clinica do paciente</big></b>");
			m_btnAccept.set_label("_Edit");
			m_btnAccept.set_visible(true);
			m_txtName.set_editable(true);
			m_txtHeight.set_editable(true);
			//m_cmbBlood.set_editable(true);
			//m_rbMale.set_editable(true);
			//m_rbFemale.set_editable(true);
			m_txtBirthday.set_editable(true);
			m_txtBirthplace.set_editable(true);
			m_txtNationality.set_editable(true);
			m_txtProfession.set_editable(true);
			m_txtTaxNumber.set_editable(true);
			//m_cmbMaritalStatus.set_editable(true);
			m_txtAddress.set_editable(true);
			m_txtLocation.set_editable(true);
			m_txtZip1.set_editable(true);
			m_txtZip2.set_editable(true);
			m_txtPhone.set_editable(true);
			m_txtCellphone.set_editable(true);
			m_txtReferer.set_editable(true);
			m_txtEmail.set_editable(true);
			m_btnCancel.set_label(Stock::CANCEL.id);
			m_btnCancel.set_use_stock(true);
			
			break;
		case PW_TYPE_VIEW:
			m_lblTitle.set_text("<b><big>Ficha clinica do paciente</big></b>");
			m_txtName.set_editable(false);
			m_txtHeight.set_editable(false);
			//m_cmbBlood.set_editable(false);
			//m_rbMale.set_editable(false);
			//m_rbFemale.set_editable(false);
			m_txtBirthday.set_editable(false);
			m_txtBirthplace.set_editable(false);
			m_txtNationality.set_editable(false);
			m_txtProfession.set_editable(false);
			m_txtTaxNumber.set_editable(false);
			//m_cmbMaritalStatus.set_editable(false);
			m_txtAddress.set_editable(false);
			m_txtLocation.set_editable(false);
			m_txtZip1.set_editable(false);
			m_txtZip2.set_editable(false);
			m_txtPhone.set_editable(false);
			m_txtCellphone.set_editable(false);
			m_txtReferer.set_editable(false);
			m_txtEmail.set_editable(false);
			m_btnAccept.set_visible(false);
			m_btnCancel.set_label(Stock::CLOSE.id);
			m_btnCancel.set_use_stock(true);

				break;
		} default: {
			//Image img(Stock::ADD, Gtk::ICON_SIZE_BUTTON);
			img = new Image(Stock::ADD, Gtk::ICON_SIZE_BUTTON);
			//img->set(Stock::ADD, Gtk::ICON_SIZE_BUTTON);
			m_btnAccept.set_image(*img);

			m_lblTitle.set_text("<big><b>Ficha de inscrição de novo paciente</b></big>");
			m_btnAccept.set_label("_Add");
			m_btnAccept.set_visible(true);
			m_txtName.set_editable(true);
			m_txtHeight.set_editable(true);
			//m_cmbBlood.set_editable(true);
			//m_rbMale.set_editable(true);
			//m_rbFemale.set_editable(true);
			m_txtBirthday.set_editable(true);
			m_txtBirthplace.set_editable(true);
			m_txtNationality.set_editable(true);
			m_txtProfession.set_editable(true);
			m_txtTaxNumber.set_editable(true);
			//m_cmbMaritalStatus.set_editable(true);
			m_txtAddress.set_editable(true);
			m_txtLocation.set_editable(true);
			m_txtZip1.set_editable(true);
			m_txtZip2.set_editable(true);
			m_txtPhone.set_editable(true);
			m_txtCellphone.set_editable(true);
			m_txtReferer.set_editable(true);
			m_txtEmail.set_editable(true);
			m_btnCancel.set_label(Stock::CANCEL.id);
			m_btnCancel.set_use_stock(true);
			
			break;
		}
	}
	m_lblTitle.set_use_markup();
	m_btnAccept.set_use_underline();
}

PatientWindow::PatientWindowType PatientWindow::get_window_type()
{
	return m_type;
}

bool PatientWindow::on_taxNumber_focus_out(GdkEventFocus *event)
{
	DBHandler db = DBHandler::get_instance();
	guint32 *id = (m_type == PW_TYPE_EDIT ? &m_id : NULL);

	if(m_txtTaxNumber.get_text_length() > 0) {
		db.open();
		if(db.exists_person_by_tax_number(m_txtTaxNumber.get_value(), id)) {
			m_txtTaxNumber.override_background_color(Gdk::RGBA("Red"), STATE_FLAG_NORMAL);
			//m_txtTaxNumber.override_color(Gdk::RGBA("White"), STATE_FLAG_NORMAL);
			m_txtTaxNumber.set_icon_from_stock(Stock::CAPS_LOCK_WARNING, ENTRY_ICON_SECONDARY);
			m_txtTaxNumber.set_icon_tooltip_text("O Número de Identificação Fiscal já existe!", ENTRY_ICON_SECONDARY);
			m_tnUnique = false;
		} else {
			m_txtTaxNumber.unset_background_color(STATE_FLAG_NORMAL);
			m_txtTaxNumber.unset_color(STATE_FLAG_NORMAL);
			m_txtTaxNumber.unset_icon(ENTRY_ICON_SECONDARY);
			m_tnUnique = true;
		}
		db.close();
	} else {
		//m_txtTaxNumber.unset_background_color(STATE_FLAG_NORMAL);
		//m_txtTaxNumber.unset_color(STATE_FLAG_NORMAL);
		//m_txtTaxNumber.unset_icon(ENTRY_ICON_SECONDARY);
		m_tnUnique = false;
	}
	return false;
}

bool PatientWindow::on_identificationCard_focus_out(GdkEventFocus *event)
{
	DBHandler db = DBHandler::get_instance();
	guint32 *id = (m_type == PW_TYPE_EDIT ? &m_id : NULL);

	if(m_txtIdentificationCard.get_text_length() > 0){
		db.open();
		if(db.exists_person_by_identification_card(m_txtIdentificationCard.get_value(), id)) {
			m_txtIdentificationCard.override_background_color(Gdk::RGBA("Red"), STATE_FLAG_NORMAL);
			//m_txtIdentificationCard.override_color(Gdk::RGBA("White"), STATE_FLAG_NORMAL);
			m_txtIdentificationCard.set_icon_from_stock(Stock::CAPS_LOCK_WARNING, ENTRY_ICON_SECONDARY);
			m_txtIdentificationCard.set_icon_tooltip_text("O bilhete de identidade já existe!", ENTRY_ICON_SECONDARY);
			m_icUnique = false;
		} else {
			m_txtIdentificationCard.unset_background_color(STATE_FLAG_NORMAL);
			m_txtIdentificationCard.unset_color(STATE_FLAG_NORMAL);
			m_txtIdentificationCard.unset_icon(ENTRY_ICON_SECONDARY);
			m_icUnique = true;
		}
		db.close();
	} else {
		//m_txtIdentificationCard.unset_background_color(STATE_FLAG_NORMAL);
		//m_txtIdentificationCard.unset_color(STATE_FLAG_NORMAL);
		//m_txtIdentificationCard.unset_icon(ENTRY_ICON_SECONDARY);
		m_icUnique = false;
	}
	return false;
}

inline void PatientWindow::validate_entry(Entry& entry, bool set_icon, Entry* other)
{
	if(entry.get_text_length() == 0 && (other == NULL || (other != NULL && other->get_text_length() == 0))) {
		entry.set_icon_from_stock(Stock::CAPS_LOCK_WARNING, ENTRY_ICON_SECONDARY);
		entry.set_icon_tooltip_text("O campo é de preenchimento obrigatório!", ENTRY_ICON_SECONDARY);
	} else {
		entry.unset_background_color(STATE_FLAG_NORMAL);
		entry.unset_color(STATE_FLAG_NORMAL);
		entry.unset_icon(ENTRY_ICON_SECONDARY);
	}
}

inline void PatientWindow::validate_entry(Entry& entry, guint32 val, bool set_icon)
{
	if(val == 0) {
		entry.override_background_color(Gdk::RGBA("Red"), STATE_FLAG_NORMAL);
		//entry.override_color(Gdk::RGBA("White"), STATE_FLAG_NORMAL);
		if(set_icon) {
			entry.set_icon_from_stock(Stock::CAPS_LOCK_WARNING, ENTRY_ICON_SECONDARY);
			entry.set_icon_tooltip_text("O campo é de preenchimento obrigatório!", ENTRY_ICON_SECONDARY);
		}
	} else {
		entry.unset_background_color(STATE_FLAG_NORMAL);
		entry.unset_color(STATE_FLAG_NORMAL);
		entry.unset_icon(ENTRY_ICON_SECONDARY);
	}
}