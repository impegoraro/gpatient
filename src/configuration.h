#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

class Configuration
{
public:
	virtual void get_window_size(gint& width, gint& height) = 0;
	//void get_size(gint& width, gint height) const = 0;

	virtual void get_window_position(gint& posx, gint& posy) = 0;
	//void get_position(gint& posx, &posy) const = 0;

	virtual bool get_window_maximized() = 0;
	//bool get_maximize() const = 0;

	virtual void set_window_maximized(bool maximized = true) = 0;

	virtual void set_window_resize(int width, int height) = 0;

	virtual void set_window_move(int posx, int posy) = 0;
};

#endif