/*
 * key.cpp
 *
 *  Created on: Aug 21, 2012
 *      Author: ilan
 */

#include "key.h"

Key::Key(const std::string& name, const std::string& value) : m_name(name), m_value(value)
{
}

/*
Key::~Key()
{
}
*/

void Key::set_name(const std::string& name)
{
	if(name.length() > 0)
		this->m_name = name;
}

const std::string& Key::get_name(void) const
{
	return m_name;
}

void Key::set_value(const std::string& value)
{
	this->m_value = value;
}

const std::string& Key::get_value(void) const
{
	return m_value;
}

