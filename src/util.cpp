/*
 * Util.cpp
 *
 *  Created on: Oct 14, 2012
 *      Author: ilan
 */

#include "util.h"
#include <cstring>
#include <stdlib.h>
#include <string>

#include <glibmm.h>

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
	guint16 month=12, year=9999;

	if(strdate.length() == 10) {
		char date[11];
		char *tmp, *tmp2;

		strncpy(date, strdate.c_str(), 10);
		tmp = strchr(date, '/');
		if(tmp != NULL) {
			*tmp++ = '\0';
			day = atoi(date);
		}
		tmp2 = tmp;
		tmp = strchr(tmp2, '/');
		if(tmp != NULL) {
			*tmp++ = '\0';
			month = atoi(tmp2);
			year = atoi(tmp);
		}
	}
	return Date(day, helper_get_month(month), year);
}

static Date::Month helper_get_month(const guint16& month)
{
	Date::Month m[] = {Date::BAD_MONTH, Date::JANUARY, Date::FEBRUARY, Date::MARCH,
				Date::APRIL, Date::MAY, Date::JUNE, Date::JULY, Date::AUGUST,
				Date::SEPTEMBER, Date::OCTOBER, Date::NOVEMBER, Date::DECEMBER};

	return m[month % 13];
}
