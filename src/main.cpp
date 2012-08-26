#include <gtkmm.h>
#include <iostream>

#include "main-window.h"


using namespace Glib;
using namespace Gtk;

int main(int argc, char** argv)
{
	Gtk::Main kit(argc, argv);
	MainWindow mwin("Hebanária - Gestão");

	mwin.show();

	kit.run(mwin);

	return 0;
}

