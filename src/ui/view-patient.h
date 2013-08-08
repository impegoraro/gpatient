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

#ifndef _VIEW_PATIENT_H_
#define _VIEW_PATIENT_H_

#include <gtkmm/dialog.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>

#include "person.h"

class ViewPatientWindow : public Gtk::Dialog 
{
public:
	ViewPatientWindow(Gtk::Window&);

	void set_person(const Person& p);
private:
	Gtk::Box* m_mainBox;
	
	Gtk::Image* m_imgPersonal; 
	Gtk::Label* m_lblName;
	Gtk::Label* m_lblHeight;
	Gtk::Label* m_lblSex;
	Gtk::Label* m_lblNationality;
	Gtk::Label* m_lblBirthday;
	Gtk::Label* m_lblBirthplace;
	Gtk::Label* m_lblProfession;
	Gtk::Label* m_lblTaxNumber;
	Gtk::Label* m_lblIdentificationCard;
	Gtk::Label* m_lblMaritalStatus;
	Gtk::Label* m_lblBloodType;

	Gtk::Image* m_imgContact;
	Gtk::Label* m_lblAddress;
	Gtk::Label* m_lblLocation;
	Gtk::Label* m_lblZip;
	Gtk::Label* m_lblTelephone;
	Gtk::Label* m_lblCellphone;
	Gtk::Label* m_lblEmail;
	Gtk::Label* m_lblReferer;

	bool close_on_esc(GdkEventKey* event);

};

#endif // _VIEW_PATIENT_H_

