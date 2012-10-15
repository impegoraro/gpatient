/*
 * Util.h
 *
 *  Created on: Oct 14, 2012
 *      Author: ilan
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <glibmm.h>

class Util
{
public:
	Util();
	virtual ~Util();

	static Glib::Date parse_date(const std::string& strdate);
};

#endif /* UTIL_H_ */
