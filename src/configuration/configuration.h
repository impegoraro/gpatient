/*
 * configuration.h
 *
 *  Created on: Aug 21, 2012
 *      Author: Ilan
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>
#include <vector>

#include "section.h"

class Configuration
{
protected:
	std::string m_path;
	std::vector<Section> m_sections;

public:
	Configuration(const std::string& path);

	void set_path(const std::string& path);
	const std::string& get_path(void) const;
	const Section& get_section(const std::string& byName) const;

	void add(const Section& sec);

	void print(void) const;

	void parse(void);
	void save(void) const;
};

#endif /* CONFIGURATION_H_ */
