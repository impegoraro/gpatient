/*
 * Util.cpp
 *
 *  Created on: Oct 14, 2012
 *      Author: ilan
 */

#include <string>
#include <stdexcept>

#include <glibmm.h>

// C standard headers
#include <cstring>
#include <cstdlib>
#include <cstdio>

// Application  dependant
#include "util.h"

using namespace Glib;

static Glib::Date::Month helper_get_month(const guint16& month);

Util::Util()
{
}

Util::~Util()
{
}

Date Util::parse_date(const std::string& strdate)
{
	guint8 day=31;
	guint16 month=12, year=1990;

	if(strdate.length() > 4 && strdate.length() <= 10) {
		std::string str = Glib::locale_from_utf8(strdate);
		if(sscanf(str.c_str(), "%hhu/%hu/%hu", &day, &month, &year)==0)
			printf("Warning Util::parse_date...\n");
	}
	return Date(day, helper_get_month(month), year);
}

ustring& Util::string_trim(ustring& str)
{
	unsigned int i = 0;

	if(!str.length())
		return str;

	try {
		while(i < str.length()) {
			if(isspace(str[i])) i++;
			else break;
		}
	} catch(std::out_of_range& ex) {}

	if(i > 0 && i <= str.length())
		str.erase(0, i);

	i = str.length() - 1;
	try {
		while(i > 0) {
			if(isspace(str[i])) i--;
			else break;
		}
	} catch(std::out_of_range& ex) {}

	i++;
	if(i > 0 && i < str.length())
		str.erase(i, str.length());

	return str;
}


static Date::Month helper_get_month(const guint16& month)
{
	Date::Month m[] = {Date::BAD_MONTH, Date::JANUARY, Date::FEBRUARY, Date::MARCH,
				Date::APRIL, Date::MAY, Date::JUNE, Date::JULY, Date::AUGUST,
				Date::SEPTEMBER, Date::OCTOBER, Date::NOVEMBER, Date::DECEMBER};

	return m[month % 13];
}
