/*
 * patient-window.h
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifndef PATIENT_WINDOW_H_
#define PATIENT_WINDOW_H_

#include <gtkmm/button.h>
#include <gtkmm/radiobutton.h> 
#include <gtkmm/radiobuttongroup.h> 
#include <gtkmm/spinbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>


#include "widgets/widgets.h"
#include "../person.h"

class PatientWindow : public Gtk::Window
{
public:
	enum PatientWindowType
	{
		PW_TYPE_ADD,
		PW_TYPE_EDIT,
		PW_TYPE_VIEW
	};

protected:
	guint32 m_id;

	bool m_phoneStatus;
	bool m_cellphoneStatus;
	bool m_dateStatus;
	Gtk::Label m_lblTitle;
	bool m_tnUnique;
	bool m_icUnique;

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
	Gtk::Entry m_txtNationality;
	Gtk::Label m_lblProfession;
	Gtk::Entry m_txtProfession;
	Gtk::Label m_lblTaxNumber;
	Widgets::NumericEntry m_txtTaxNumber;
	Gtk::Label m_lblIdentificationCard;
	Widgets::NumericEntry m_txtIdentificationCard;
	Gtk::Label m_lblMaritalStatus;
	Gtk::ComboBoxText m_cmbMaritalStatus;

	Gtk::Label m_lblAddress;
	Gtk::Entry m_txtAddress;
	Gtk::Label m_lblLocation;
	Gtk::Entry m_txtLocation;
	Gtk::Label m_lblZip;
	Widgets::NumericEntry m_txtZip1;
	Widgets::NumericEntry m_txtZip2;
	Gtk::Label m_lblContact;
	Widgets::NumericEntry m_txtPhone;
	Widgets::NumericEntry m_txtCellphone;
	Gtk::Label m_lblEmail;
	Gtk::Entry m_txtEmail;
	Gtk::Label m_lblReferer;
	Gtk::Entry m_txtReferer;

	Gtk::Button m_btnAccept;
	Gtk::Button m_btnCancel;

	PatientWindowType m_type;
	Widgets::CalendarWindow m_wincal;

	sigc::signal<void, PatientWindow &> m_signal_add;
public:

	PatientWindow(Gtk::Window& parent, const std::string& title = "Dados do Paciente",PatientWindowType type = PW_TYPE_ADD);
	~PatientWindow();
	void set_person(const Person& p);
	void get_person(Person&) const;
	guint32 get_id(void) const;


	void set_window_type(PatientWindow::PatientWindowType);
	PatientWindow::PatientWindowType get_window_type();

	sigc::signal<void, PatientWindow &>& signal_add();

	virtual void show();
	virtual void show_all();
	void clean();
	int run();

protected:
	bool on_PhoneFocusIn(GdkEventFocus *focus);
	bool on_PhoneFocusOut(GdkEventFocus *focus);
	bool on_CellphoneFocusIn(GdkEventFocus *focus);
	bool on_CellphoneFocusOut(GdkEventFocus *focus);

	void on_txtBirthday_iconPress(Gtk::EntryIconPosition, const GdkEventButton*);

	bool close_on_esc(GdkEventKey* event);

	bool on_focusOut_trim(GdkEventFocus*, Gtk::Entry*);
	bool on_taxNumber_focus_out(GdkEventFocus *event);
	bool on_identificationCard_focus_out(GdkEventFocus *event);
	void activate_close(bool);

	inline void validate_entry(Gtk::Entry& entry, bool set_icon = true, Gtk::Entry* other = NULL);
	inline void validate_entry(Gtk::Entry& entry, guint32 val, bool set_icon = true);
	virtual bool on_delete_event(GdkEventAny *);
};


#endif /* PATIENT_WINDOW_H_ */
