/*
 * main.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <gtkmm.h>
#include <iostream>

#include "ui/main-window.h"

using namespace std;
using namespace Glib;
using namespace Gtk;

int main(int argc, char** argv)
{
	RefPtr<Application> app = Application::create(argc, argv, "org.herbanaria.base");

	ustring dbpath;
	dbpath = get_home_dir();
	dbpath += "/." + (ustring)PACKAGE_NAME + (ustring)"/" + ((ustring)PACKAGE_NAME) + ".sqlite";
	dbpath = dbpath.lowercase();
	RefPtr<Gio::File> fdb = Gio::File::create_for_path(dbpath);

	if(!fdb->query_exists()) {
		cerr<<"FATAL: A base de dados '"<< fdb->get_parse_name()<< "' não existe";
		cerr<< ". O programa vai terminar..."<< endl;
		MessageDialog dlg("Não foi possível carregar a base de dados!", true, MessageType::MESSAGE_ERROR, ButtonsType::BUTTONS_OK, false);
		dlg.set_title((ustring)PACKAGE_NAME + (ustring)" - Erro inesperado");
		dlg.set_secondary_text("A base de dados é inexistente. Deve proceder com a reinstalação da aplicação.");

		dlg.run();
		exit(1);
	}
	MainWindow mwin("Hebanária - Gestão", dbpath, app);

	mwin.show();

	app->run(mwin);

	return 0;
}

