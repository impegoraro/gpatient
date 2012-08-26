/*
 * configuration.cpp
 *
 *  Created on: Aug 21, 2012
 *      Author: ilan
 */

#include <exception>
#include <iostream>

#include <cstdio>
#include <cstring>
#include <cctype>

#include "configuration.h"
#include "section.h"


static char* strtrim(char *str);

Configuration::Configuration(const std::string& path) : m_path(path)
{
}

void Configuration::set_path(const std::string& path)
{
	if(path.length() > 0)
		this->m_path = path;
}

const std::string& Configuration::get_path(void) const
{
	return m_path;
}

void Configuration::parse(void)
{
	m_sections.clear();

	/* TODO: Replace C code with a better solution in C++ */

	char str[2050];
	char *end;
	FILE *fd = fopen(m_path.c_str(), "r");
	Section *sec = NULL;

	str[2050] = '\0';

	if(fd != NULL) {
		while(fgets(str, 2049, fd) != NULL) {

			//Removes comments
			if((end = strchr(str, ';')) != NULL)
				*end = '\0';

			// TODO: remove blank lines
			strtrim(str);

			if(str[0] == '[' && str[strlen(str) - 1] == ']') {
				str[strlen(str + 1)] = '\0';

				if(sec != NULL) {
					m_sections.push_back(*sec);
					delete sec;
				}

				sec = new Section(std::string(strtrim(str + 1)));
			} else {
				end = strchr(str, '=');
				if(end == NULL)
					continue; // invalid line

				*end = '\0';
				end++;

				if(sec != NULL)
					sec->add(std::string(strtrim(str)), std::string(strtrim(end)));
			}
		}

		// Adding last section
		if(sec != NULL) {
			m_sections.push_back(*sec);
			delete sec;
		}

		fclose(fd);
	}
}

void Configuration::add(const Section& sec)
{
	for(Section& s : m_sections)
		if(sec.get_name() == s.get_name())
			s = sec;

	m_sections.push_back(sec);
}

const Section& Configuration::get_section(const std::string& byName) const
{
	for(int i = 0; i < m_sections.size(); i++) {
		if(m_sections[i].get_name() == byName)
			return m_sections[i];
	}

	throw std::exception();
}

void Configuration::save(void) const
{
	FILE *fd = fopen(m_path.c_str(), "w");

	if(fd != NULL) {
		for(int i = 0; i < m_sections.size(); i++) {
			fprintf(fd, "[%s]\n", m_sections[i].get_name().c_str());

			for(int j = 0; j < m_sections[i].get_options().size(); j++)
				fprintf(fd, "%s=%s\n", m_sections[i].get_options()[j].get_name().c_str(), m_sections[i].get_options()[j].get_value().c_str());
		}

		fclose(fd);
	}
}

void Configuration::print(void) const
{
	for(int i = 0; i < m_sections.size(); i++)
		m_sections[i].print();
}

/* Helper function */
static char* strtrim(char *str)
{
	char *tmp = str;
	char *end;

	if(str != NULL && strlen(str) > 0) {
		// Trim leading non-letters
		while(isspace(*tmp)) tmp++;

		// Trim trailing non-letters
		end = tmp + strlen(tmp) - 1;
		while(end > str && isspace(*end)) end--;

		memcpy(str, tmp, (end + 1) - tmp);
		str[(end + 1 - tmp)] = '\0';
	}
	return str;
}
