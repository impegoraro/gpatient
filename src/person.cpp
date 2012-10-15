/*
 * person.cpp
 *
 *  Created on: Aug 25, 2012
 *      Author: ilan
 */
#include <cstring>
#include <glibmm.h>
#include "person.h"

using namespace std;
using namespace Glib;

Person::Person() : m_id(0), m_height(1.0), m_bloodtype(BT_A), m_sex(true), m_taxNumber(0),
	m_phone(0), m_cellphone(0), m_maritalStatus(MS_OTHER)
{
	memset(m_zip, 0, 9);
}

Person::Person(const ustring& name, float height, Person::BloodType bloodtype, bool sex) :
	m_id(0), m_name(name), m_height(height), m_bloodtype(bloodtype), m_sex(sex), m_taxNumber(0),
	m_phone(0), m_cellphone(0), m_maritalStatus(MS_OTHER)
{
	memset(m_zip, 0, 9);
}

Person::Person(unsigned int id, const string& name, float height, BloodType bloodtype, bool sex) :
	m_id(id), m_name(name), m_height(height), m_bloodtype(bloodtype), m_sex(sex), m_taxNumber(0),
	m_phone(0), m_cellphone(0), m_maritalStatus(MS_OTHER)
{
	memset(m_zip, 0, 9);
}

const Glib::ustring& Person::get_address() const {
	return m_address;
}

void Person::set_address(const Glib::ustring& address) {
	m_address = address;
}

const Glib::Date& Person::get_birthday() const {
	return m_birthday;
}

void Person::set_birthday(const Glib::Date& birthday) {
	m_birthday = birthday;
}

const Glib::ustring& Person::get_birthplace() const {
	return m_birthplace;
}

void Person::set_birthplace(const Glib::ustring& birthplace) {
	m_birthplace = birthplace;
}

Person::BloodType Person::get_blood_type() const {
	return m_bloodtype;
}

void Person::set_blood_type(Person::BloodType bloodtype) {
	m_bloodtype = bloodtype;
}

void Person::set_blood_type(int val) {
	switch(val) {
	case 0:
		m_bloodtype = Person::BT_A;
		break;
	case 1:
		m_bloodtype = Person::BT_A_PLUS;
		break;
	case 2:
		m_bloodtype = Person::BT_A_MINUS;
		break;
	case 3:
		m_bloodtype = Person::BT_B;
		break;
	case 4:
		m_bloodtype = Person::BT_B_PLUS;
		break;
	case 5:
		m_bloodtype = Person::BT_B_MINUS;
		break;
	case 6:
		m_bloodtype = Person::BT_O;
		break;
	case 7:
		m_bloodtype = Person::BT_O_PLUS;
		break;
	case 8:
		m_bloodtype = Person::BT_O_MINUS;
		break;
	case 9:
		m_bloodtype = Person::BT_AB;
		break;
	case 10:
		m_bloodtype = Person::BT_AB_PLUS;
		break;
	case 11:
		m_bloodtype = Person::BT_AB_MINUS;
		break;
	}
}

guint32 Person::get_cellphone() const {
	return m_cellphone;
}

void Person::set_cellphone(guint32 cellphone) {
	m_cellphone = cellphone;
}

const Glib::ustring& Person::get_email() const {
	return m_email;
}

void Person::set_email(const Glib::ustring& email) {
	m_email = email;
}

float Person::get_height() const {
	return m_height;
}

void Person::set_height(float height) {
	m_height = height;
}

guint32 Person::get_id() const {
	return m_id;
}

void Person::set_id(guint32 id) {
	m_id = id;
}

const Glib::ustring& Person::get_locality() const {
	return m_locality;
}

void Person::set_locality(const Glib::ustring& locality) {
	m_locality = locality;
}

Person::MaritalStatus Person::get_marital_status() const {
	return m_maritalStatus;
}

void Person::set_marital_status(Person::MaritalStatus maritalStatus) {
	m_maritalStatus = maritalStatus;
}

void Person::set_marital_status(int val) {
	switch(val) {
	case 0:
		m_maritalStatus = Person::MS_SINGLE ; break;
	case 1:
		m_maritalStatus = Person::MS_DIVORCED ; break;
	case 2:
		m_maritalStatus = Person::MS_WIDOWED ; break;
	case 3:
		m_maritalStatus = Person::MS_MARRIED ; break;
	default:
		m_maritalStatus = Person::MS_OTHER; break;
	}
}

void Person::set_nationality(const Glib::ustring& nationality) {
	m_nationality = nationality;
}

const Glib::ustring& Person::get_nationality() const {
	return m_nationality;
}

const Glib::ustring& Person::get_name() const {
	return m_name;
}

void Person::set_name(const Glib::ustring& name) {
	m_name = name;
}

guint32 Person::get_phone() const {
	return m_phone;
}

void Person::set_phone(guint32 phone) {
	m_phone = phone;
}

const Glib::ustring& Person::get_profession() const {
	return m_profession;
}

void Person::set_profession(const Glib::ustring& profession) {
	m_profession = profession;
}

const Glib::ustring& Person::get_referer() const {
	return m_referer;
}

void Person::set_referer(const Glib::ustring& referer) {
	m_referer = referer;
}

bool Person::get_sex() const {
	return m_sex;
}

void Person::set_sex(bool sex) {
	m_sex = sex;
}

guint32 Person::get_tax_number() const {
	return m_taxNumber;
}

void Person::set_tax_number(guint32 taxNumber) {
	m_taxNumber = taxNumber;
}

void Person::set_zip(const char *zip1, const char* zip2)
{
	if(zip1 != NULL && strlen(zip1) == 4 && zip2 != NULL && strlen(zip2) == 3) {
		strncpy(m_zip, zip1,4);
		strcat(m_zip, "-");
		strncpy(m_zip + 5, zip2, 3);
	}
}

const char* Person::get_zip() const {
	return m_zip;
}

ustring Person::get_blood_type_string(int val)
{
	ustring bloodType[] = {"A", "A+", "A-", "B", "B+", "B-", "O", "O+", "O-", "AB", "AB+", "AB-"};
	return bloodType[val];
}

ustring Person::get_marital_status_string(int val)
{
	ustring status[] = {"Casado(a)", "Divorciado(a)", "Solteiro(a)", "Viúvo(a)", "Outro..."};
	return status[val];
}
