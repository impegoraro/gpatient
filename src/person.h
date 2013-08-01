/*
 * person.h
 *
 *  Created on: Aug 25, 2012
 *      Author: ilan
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <glibmm.h>

class Person
{
public:
	enum BloodType
	{
		BT_A = 1,
		BT_A_PLUS,
		BT_A_MINUS,
		BT_B,
		BT_B_PLUS,
		BT_B_MINUS,
		BT_O,
		BT_O_PLUS,
		BT_O_MINUS,
		BT_AB,
		BT_AB_PLUS,
		BT_AB_MINUS
	};

	enum MaritalStatus
	{
		MS_MARRIED = 1,
		MS_DIVORCED,
		MS_SINGLE,
		MS_WIDOWED,
		MS_OTHER
	};

protected:
	guint32 m_id;
	guint32 m_taxNumber;
	guint32 m_identificationCard;
	guint32 m_phone;
	guint32 m_cellphone;
	guint16 m_zip1;
	guint16 m_zip2;
	float m_height;
	bool m_sex;
	Glib::ustring m_name;
	Glib::ustring m_birthplace;
	Glib::ustring m_nationality;
	Glib::ustring m_profession;
	Glib::ustring m_address;
	Glib::ustring m_locality;
	Glib::ustring m_email;
	Glib::ustring m_referer;
	Glib::Date m_birthday;
	BloodType m_bloodtype;
	MaritalStatus m_maritalStatus;


public:
	Person();
	Person(guint32 id);

	guint32 get_id() const;
	void set_id(guint32 id);
	const Glib::ustring& get_address() const;
	void set_address(const Glib::ustring& address);
	const Glib::Date& get_birthday() const;
	void set_birthday(const Glib::Date& birthday);
	const Glib::ustring& get_birthplace() const;
	void set_birthplace(const Glib::ustring& birthplace);
	BloodType get_blood_type() const;
	Glib::ustring get_blood_type_string() const;
	void set_blood_type(BloodType bloodtype);
	void set_blood_type(unsigned int val);
	guint32 get_cellphone() const;
	void set_cellphone(guint32 cellphone);
	const Glib::ustring& get_email() const;
	void set_email(const Glib::ustring& email);
	float get_height() const;
	void set_height(float height);
	const Glib::ustring& get_locality() const;
	void set_locality(const Glib::ustring& locality);
	MaritalStatus get_marital_status() const;
	Glib::ustring get_marital_status_text() const; 
	void set_marital_status(MaritalStatus maritalStatus);
	void set_marital_status(unsigned int val);
	const Glib::ustring& get_nationality() const;
	void set_nationality(const Glib::ustring& nationality);
	const Glib::ustring& get_name() const;
	void set_name(const Glib::ustring& name);
	guint32 get_phone() const;
	void set_phone(guint32 phone);
	const Glib::ustring& get_profession() const;
	void set_profession(const Glib::ustring& profession);
	const Glib::ustring& get_referer() const;
	void set_referer(const Glib::ustring& referer);
	bool get_sex() const;
	void set_sex(bool sex);
	guint32 get_tax_number() const;
	void set_tax_number(guint32 taxNumber);
	void set_zip(guint16 zip1, guint16 zip2);
	void set_zip(Glib::ustring& zip);
	void get_zip(guint16 &zip1, guint16 &zip2) const;
	Glib::ustring get_zip() const;
	void set_identification_card(guint32 id);
	guint32 get_identification_card(void) const;
	
	bool validate(void) const;

	static Glib::ustring get_blood_type_string(int val);
	static Glib::ustring get_marital_status_string(int val);
};


#endif /* PERSON_H_ */
