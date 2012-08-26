#ifndef LOGIN_WINDOW_H_
#define LOGIN_WINDOW_H_

#include <gtkmm.h>
#include "configuration/configuration.h"

using namespace std;
using namespace Gtk;

class LoginWindow : public Gtk::Window
{
public:
	LoginWindow(string title, Window *mwin = NULL);

protected:
	Label lblTitle;

	Label lblAddress;
	Entry entryAddress;
	SpinButton *entryPort;

	Label lblUser;
	Label lblPass;
	Entry entryUser;
	Entry entryPass;

	CheckButton chkPersistent;
	Button btnConnect;
	Button btnClose;

	InfoBar msgInfo;
	Label lblMsgInfo;

	Configuration cfg;

	void on_btnClose_clicked(void);
	void on_btnConnect_clicked(void);

	bool on_window_deleteEvent(GdkEventAny *ev);
};

#endif
