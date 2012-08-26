/*
 * person.cpp
 *
 *  Created on: Aug 25, 2012
 *      Author: ilan
 */

#include "person.h"

using namespace std;

Person::Person(const string& name, float height, BloodType bloodtype, bool sex) :
		m_name(name), m_height(height), m_bloodtype(bloodtype), m_sex(sex)
{
}

void Person::set_name(const string& name)
{
	if(name.length() > 0)
		m_name = name;
}

const string& Person::get_name(void) const
{
	return m_name;
}

bool Person::set_height(float height)
{
	if(height >= 0.0 && height < 3.0) {
		m_height = height;
		return true;
	} else
		return false;
}

float Person::get_height(void) const
{
	return m_height;
}

void Person::set_blood_type(Person::BloodType bt)
{
	m_bloodtype = bt;
}

Person::BloodType Person::get_blood_type(void) const
{
	return m_bloodtype;
}

void Person::set_sex(bool sex)
{
	m_sex = sex;
}

bool Person::get_sex(void)
{
	return m_sex;
}



