/*
 * section.h
 *
 *  Created on: Aug 21, 2012
 *      Author: ilan
 */

#ifndef SECTION_H_
#define SECTION_H_

#include <string>
#include <vector>

#include "key.h"

class Section
{
protected:
	std::string m_name;
	std::vector<Key> m_options;


public:
	Section(const std::string& name);
	//virtual ~Section();

	void set_name(const std::string& name);
	const std::string& get_name(void) const;
	const std::string* get_value(std::string& key) const;
	const std::vector<Key>& get_options() const;

	Section& operator=(const Section& rhs);

	bool add(const std::string& key, const std::string& value);
	bool remove(const std::string& key);


	void print(void) const;
};

#endif /* SECTION_H_ */
