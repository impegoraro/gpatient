/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gestao-herb
 * Copyright (C) 2013 Ilan Pegoraro <impegoraro@gmail.com>
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

#include <exception>

#include <gtkmm/stock.h>

#include "view-patient.h"

#define GLADE_FILE "src/ui/view-patient.glade"

using namespace Gtk;
using namespace Glib;
using namespace std;

ViewPatientWindow::ViewPatientWindow(Window &parent) : Dialog("Ficha do paciente", parent, false)
{
	RefPtr<Builder> builder = Builder::create_from_file(GLADE_FILE);
	Button *btnClose;
	
	builder->get_widget("mainBox", m_mainBox);
	builder->get_widget("lblName", m_lblName);
	builder->get_widget("lblHeight", m_lblHeight);
	builder->get_widget("lblSex", m_lblSex);
	builder->get_widget("lblNationality", m_lblNationality);
	builder->get_widget("lblBirthday", m_lblBirthday);
	builder->get_widget("lblBirthplace", m_lblBirthplace);
	builder->get_widget("lblProfession", m_lblProfession);
	builder->get_widget("lblTaxNumber", m_lblTaxNumber);
	builder->get_widget("lblMaritalStatus", m_lblMaritalStatus);
	builder->get_widget("lblBloodType", m_lblBloodType);

	builder->get_widget("lblAddress", m_lblAddress);
	builder->get_widget("lblLocation", m_lblLocation);
	builder->get_widget("lblZip", m_lblZip);
	builder->get_widget("lblTelephone", m_lblTelephone); 
	builder->get_widget("lblCellphone", m_lblCellphone);
	builder->get_widget("lblEmail", m_lblEmail);
	builder->get_widget("lblReferer", m_lblReferer);

	set_resizable(false);
	set_border_width(10);
	get_vbox()->pack_start(*m_mainBox, true, true, 0);

	btnClose = add_button(Stock::CLOSE, RESPONSE_CLOSE);

	//btnClose->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &ViewPatientWindow::response), RESPONSE_CLOSE));
	btnClose->signal_clicked().connect(sigc::mem_fun(*this, &ViewPatientWindow::hide));
	if(!(m_mainBox && m_lblName && m_lblHeight && m_lblSex && m_lblNationality && m_lblBirthday && m_lblBirthplace &&
	   m_lblProfession && m_lblTaxNumber && m_lblMaritalStatus && m_lblBloodType && m_lblAddress && m_lblLocation &&
	   m_lblZip && m_lblTelephone && m_lblCellphone && m_lblEmail && m_lblReferer))

		throw std::exception();
}

void ViewPatientWindow::set_person(const Person& p)
{
	m_lblName->set_text(p.get_name());
	m_lblHeight->set_text(ustring::compose("%1", p.get_height()));
	m_lblSex->set_text(p.get_sex() ? "Masculino" : "Feminino");
	m_lblZip->set_text(p.get_zip());	
	m_lblNationality->set_text(p.get_nationality());
	m_lblBirthday->set_text(p.get_birthday().format_string("%d/%m/%Y"));
	m_lblBirthplace->set_text(p.get_birthplace());
	m_lblProfession->set_text(p.get_profession());
	m_lblBloodType->set_text(p.get_blood_type_string());
	m_lblAddress->set_text(p.get_address());
	m_lblMaritalStatus->set_text(p.get_marital_status_text());
	m_lblLocation->set_text(p.get_locality());
	m_lblTelephone->set_text(ustring::compose("%1", p.get_phone()));
	m_lblCellphone->set_text(ustring::compose("%1", p.get_cellphone()));
	m_lblEmail->set_text(p.get_email());
	m_lblReferer->set_text(p.get_referer());
	m_lblTaxNumber->set_text(ustring::compose("%1", p.get_tax_number()));
}


