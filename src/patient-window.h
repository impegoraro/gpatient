/*
 * patient-window.h
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifndef PATIENT_WINDOW_H_
#define PATIENT_WINDOW_H_

#include <gtkmm.h>

enum PatientWindowType
{
	PW_TYPE_ADD,
	PW_TYPE_EDIT
};

class PatientWindow : public Gtk::Dialog
{
protected:
	Gtk::Label m_lblTitle;

	Gtk::Label m_lblName;
	Gtk::Entry m_entryName;
	Gtk::Label m_lblHeight;
	Gtk::SpinButton m_entryHeight;
	Gtk::Label m_lblBlood;
	Gtk::ComboBoxText m_cmbBlood;
	Gtk::Label m_lblSex;
	Gtk::RadioButtonGroup m_grpSex;
	Gtk::RadioButton m_rbMale;
	Gtk::RadioButton m_rbFemale;

	Gtk::Button m_btnAccept;
	Gtk::Button m_btnCancel;

	PatientWindowType m_type;
public:

	PatientWindow(const std::string& title = "Dados do Paciente", PatientWindowType type = PW_TYPE_ADD);
	const Glib::ustring get_patient_name(void) const;
};


#endif /* PATIENT_WINDOW_H_ */
