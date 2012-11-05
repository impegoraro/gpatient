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
	m_phone(0), m_cellphone(0), m_maritalStatus(MS_OTHER), m_zip1(0), m_zip2(0)
{
}

Person::Person(guint32 id) :
	m_id(id), m_height(1.0), m_bloodtype(BT_A), m_sex(true), m_taxNumber(0),
	m_phone(0), m_cellphone(0), m_maritalStatus(MS_OTHER), m_zip1(0), m_zip2(0)
{
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

ustring Person::get_blood_type_string() const {
	return Person::get_blood_type_string(m_bloodtype);
}

void Person::set_blood_type(Person::BloodType bloodtype) {
	m_bloodtype = bloodtype;
}

void Person::set_blood_type(unsigned int val) {
	BloodType array[] = {BT_A, BT_A_PLUS, BT_A_MINUS, BT_B, BT_B_PLUS, BT_B_MINUS, BT_O, BT_O_PLUS, BT_O_MINUS, BT_AB, BT_AB_PLUS, BT_AB_MINUS};

	m_bloodtype = array[(val - 1) % 12];
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

void Person::set_marital_status(unsigned int val) {
	MaritalStatus array[] = {MS_MARRIED, MS_SINGLE, MS_DIVORCED, MS_WIDOWED, MS_OTHER};

	m_maritalStatus = array[(val-1) % 5];
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

void Person::set_zip(guint16 zip1, guint16 zip2)
{
	m_zip1 = zip1;
	m_zip2 = zip2;
}

string Person::get_zip() const {
	stringstream ss(stringstream::in);

	ss<< m_zip1<< "-"<< m_zip2;
	return ss.str();
}

void Person::get_zip(guint16 &zip1, guint16 &zip2) const
{
	zip1 = m_zip1;
	zip2 = m_zip2;
}

bool Person::validate(void) const
{
	return(m_name.length() > 0 && (m_height >= 0.0 && m_height <= 3.0) && m_birthday.valid() && m_birthplace.length() > 0 && \
			m_nationality.length() > 0 && m_profession.length() > 0 && m_address.length() > 0 && m_locality.length() > 0 && \
			m_email.length() && m_taxNumber > 0 && m_phone > 0 && m_cellphone > 0);
}

ustring Person::get_blood_type_string(int val)
{
	ustring bloodType[] = {"A", "A+", "A-", "B", "B+", "B-", "O", "O+", "O-", "AB", "AB+", "AB-"};
	return bloodType[val - 1];
}

ustring Person::get_marital_status_string(int val)
{
	ustring status[] = {"Casado(a)", "Divorciado(a)", "Solteiro(a)", "Viúvo(a)", "Outro..."};
	return status[val - 1];
}
