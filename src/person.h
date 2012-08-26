/*
 * person.h
 *
 *  Created on: Aug 25, 2012
 *      Author: ilan
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <string>

class Person
{
protected:
	enum BloodType
	{
		A,
		B,
		O,
		AB
	};

	std::string m_name;
	float m_height;
	BloodType m_bloodtype;
	bool m_sex;

public:
	Person(const std::string& name, float height, BloodType bloodtype, bool sex);

	void set_name(const std::string& name);
	const std::string& get_name(void) const;
	bool set_height(float height);
	float get_height(void) const;
	void set_blood_type(BloodType bt);
	BloodType get_blood_type(void) const;
	void set_sex(bool sex);
	bool get_sex(void);
};


#endif /* PERSON_H_ */
