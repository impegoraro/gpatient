#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <glibmm/ustring.h>
#include <string>

class Configuration
{
public:
	Configuration();
	Configuration(const std::string& fpath);


	virtual void get_window_size(gint& width, gint& height) = 0;
	//void get_size(gint& width, gint height) const = 0;

	virtual void get_window_position(gint& posx, gint& posy) = 0;
	//void get_position(gint& posx, &posy) const = 0;

	virtual bool get_window_maximized() = 0;
	//bool get_maximize() const = 0;

	virtual void set_window_maximized(bool maximized = true) = 0;

	virtual void set_window_resize(int width, int height) = 0;

	virtual void set_window_move(int posx, int posy) = 0;

	virtual void store();

	virtual bool parse();

protected:
	Glib::ustring m_fpath;

	std::string read_input_file();
};

#endif