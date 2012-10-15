/*
 * patient-window.h
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifndef PATIENT_WINDOW_H_
#define PATIENT_WINDOW_H_

#include <gtkmm.h>

#include "person.h"

class NumericEntry : public Gtk::Entry
{
public:
		void set_allow_alphanumeric(bool allow=true);
protected:
	bool m_allow_alphanumeric = false;
	virtual void on_insert_text(const Glib::ustring& text, int *position);
};

class PatientWindow : public Gtk::Dialog
{
public:
	enum PatientWindowType
	{
		PW_TYPE_ADD,
		PW_TYPE_EDIT
	};

protected:
	guint32 m_id;
	Gtk::Label m_lblTitle;

	Gtk::Label m_lblName;
	Gtk::Entry m_txtName;
	Gtk::Label m_lblHeight;
	Gtk::SpinButton m_txtHeight;
	Gtk::Label m_lblBlood;
	Gtk::ComboBoxText m_cmbBlood;
	Gtk::Label m_lblSex;
	Gtk::RadioButtonGroup m_grpSex;
	Gtk::RadioButton m_rbMale;
	Gtk::RadioButton m_rbFemale;
	Gtk::Label m_lblBirthday;
	Gtk::Entry m_txtBirthday;
	Gtk::Label m_lblBirthplace;
	Gtk::Entry m_txtBirthplace;
	Gtk::Label m_lblNacionality;
	Gtk::Entry m_txtNacionality;
	Gtk::Label m_lblProfession;
	Gtk::Entry m_txtProfession;
	Gtk::Label m_lblTaxNumber;
	NumericEntry m_txtTaxNumber;
	Gtk::Label m_lblMaritalStatus;
	Gtk::ComboBoxText m_cmbMaritalStatus;

	Gtk::Label m_lblAddress;
	Gtk::Entry m_txtAddress;
	Gtk::Label m_lblLocation;
	Gtk::Entry m_txtLocation;
	Gtk::Label m_lblZip;
	NumericEntry m_txtZip1;
	NumericEntry m_txtZip2;
	Gtk::Label m_lblContact;
	NumericEntry m_txtPhone;
	NumericEntry m_txtCellphone;
	Gtk::Label m_lblEmail;
	Gtk::Entry m_txtEmail;
	Gtk::Label m_lblReferer;
	Gtk::Entry m_txtReferer;

	Gtk::Button m_btnAccept;
	Gtk::Button m_btnCancel;

	PatientWindowType m_type;

	bool m_phoneStatus;
	bool m_cellphoneStatus;
public:

	PatientWindow(Gtk::Window& parent, const std::string& title = "Dados do Paciente",PatientWindowType type = PW_TYPE_ADD);
	void set_person(const Person& p);
	void get_person(Person&) const;
	guint32 get_id(void) const;

	void on_btnAdd_clicked(void);

	bool on_PhoneFocusIn(GdkEventFocus *focus);
	bool on_PhoneFocusOut(GdkEventFocus *focus);
	bool on_CellphoneFocusIn(GdkEventFocus *focus);
	bool on_CellphoneFocusOut(GdkEventFocus *focus);

};


#endif /* PATIENT_WINDOW_H_ */
