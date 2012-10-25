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
		MS_SINGLE = 0,
		MS_DIVORCED,
		MS_WIDOWED,
		MS_MARRIED,
		MS_OTHER
	};

protected:
	guint32 m_id;
	guint32 m_taxNumber;
	guint32 m_phone;
	guint32 m_cellphone;
	float m_height;
	bool m_sex;
	char m_zip[9];
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
	void set_blood_type(BloodType bloodtype);
	void set_blood_type(int val);
	guint32 get_cellphone() const;
	void set_cellphone(guint32 cellphone);
	const Glib::ustring& get_email() const;
	void set_email(const Glib::ustring& email);
	float get_height() const;
	void set_height(float height);
	const Glib::ustring& get_locality() const;
	void set_locality(const Glib::ustring& locality);
	MaritalStatus get_marital_status() const;
	void set_marital_status(MaritalStatus maritalStatus);
	void set_marital_status(int val);
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
	void set_zip(const char *zip1, const char* zip2);
	const char* get_zip() const;

	static Glib::ustring get_blood_type_string(int val);
	static Glib::ustring get_marital_status_string(int val);
};


#endif /* PERSON_H_ */
