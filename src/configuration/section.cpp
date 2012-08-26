/*
 * section.cpp
 *
 *  Created on: Aug 21, 2012
 *      Author: ilan
 */

#include <exception>
#include <iostream>

#include "section.h"
#include "key.h"

Section::Section(const std::string& name)
{
	this->m_name = name;
}

void Section::set_name(const std::string& name)
{
	if(name.length() > 0)
		this->m_name = name;
}

const std::string& Section::get_name(void) const
{
	return this->m_name;
}


bool Section::add(const std::string& key, const std::string& value)
{
	bool exists = false;

	if(key.length() == 0 || value.length() == 0)
		return false;

	for(int i = 0; i< m_options.size(); i++) {
		if(m_options[i].get_name() == key) {
			exists = true;
			break;
		}
	}

	if(!exists)
		m_options.push_back(Key(key, value));

	return true;
}

const std::vector<Key>& Section::get_options() const
{
	return m_options;
}

bool Section::remove(const std::string& key)
{
	std::vector<Key>::iterator it;

	for(it = m_options.begin(); it < m_options.end(); it++) {
		if(it->get_name() == key)
			m_options.erase(it);
	}

	return true;
}

const std::string* Section::get_value(std::string& key) const
{
	//std::vector<Key>::iterator it;

	//for(it = m_options.begin(); it < m_options.end(); it++) {
	for(int i = 0; i < m_options.size(); i++) {
		if(m_options[i].get_name() == key)
			return &(m_options[i].get_value());
	}

	return NULL;
}

Section& Section::operator=(const Section& rhs)
{
	m_options.clear();
	this->m_name = rhs.m_name;

	for(const Key& key : rhs.m_options)
		this->m_options.push_back(key);

	return *this;
}

void Section::print(void) const
{
	std::cout<< "[" << m_name<< "]"<<std::endl;

	for(int i = 0; i < m_options.size(); i++) {
		std::cout<< m_options[i].get_name()<< "="<< m_options[i].get_value()<< std::endl;
	}
}
