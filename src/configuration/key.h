/*
 * key.h
 *
 *  Created on: Aug 21, 2012
 *      Author: ilan
 */

#ifndef KEY_H_
#define KEY_H_

#include <string>

class Key
{
protected:
	std::string m_name;
	std::string m_value;

public:
	Key(const std::string& key, const std::string& value);

	void set_name(const std::string& name);
	const std::string& get_name(void) const;
	void set_value(const std::string& value);
	const std::string& get_value(void) const;

};

#endif /* KEY_H_ */
