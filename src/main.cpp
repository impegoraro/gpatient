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
#include "db/dbhandler.h"

using namespace Gtk;
using namespace std;
using namespace Glib;

static RefPtr<Application> app;

void add_window(Window& win)
{
	cout<< "adding a new window ("<< win.get_title()<<")"<<endl;
	app->add_window(win);
}

void remove_window(Window& win)
{
	cout<< "removing window ("<< win.get_title()<<")"<<endl;
	app->remove_window(win);
}

int main(int argc, char** argv)
{
	Main kit(argc, argv);
	app = Application::create("org.pacientes.base");
	ustring dbpath;
	RefPtr<Gio::File> fdb;
	
	app->register_application();

	dbpath = get_home_dir();
	dbpath += "/." + (ustring)PACKAGE_NAME + (ustring)"/" + ((ustring)PACKAGE_NAME) + ".sqlite";
	dbpath = dbpath.lowercase();

	fdb = Gio::File::create_for_path(dbpath);

	if(!fdb->query_exists()) {
		cerr<<"FATAL: A base de dados '"<< fdb->get_parse_name()<< "' não existe";
		cerr<< ". O programa vai terminar..."<< endl;
		MessageDialog dlg("Não foi possível carregar a base de dados!", true, MessageType::MESSAGE_ERROR, ButtonsType::BUTTONS_OK, false);
		dlg.set_title((ustring)PACKAGE_NAME + (ustring)" - Erro inesperado");
		dlg.set_secondary_text("A base de dados é inexistente. Deve proceder com a reinstalação da aplicação.");

		dlg.run();
		exit(1);
	}
	DBHandler::get_instance((string)dbpath);
	MainWindow mwin("Gestão de Pacientes", app);
	app->add_window(mwin);
	mwin.show();
	Main::run(mwin);

	return 0;
}

