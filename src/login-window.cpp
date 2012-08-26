#include <iostream>
#include <sstream>

#include "configuration/configuration.h"
#include "login-window.h"

using namespace std;
using namespace Glib;
using namespace Gtk;

LoginWindow::LoginWindow(string title, Window *mwin) : Window(),
	lblTitle("<b><span size=\"large\">Herbanaria</span></b>\n<span size=\"small\">Ligação ao servidor de base de dados</span>"),
	lblAddress("<b>Endereço</b>", ALIGN_START, ALIGN_CENTER, true),
	lblUser("<b>Utilizador</b>", ALIGN_END,ALIGN_CENTER, true),
	lblPass("<b>Palavra-passe</b>", ALIGN_END, ALIGN_CENTER, true),
	chkPersistent("_Entrar automáticamente", true),
	btnConnect(Stock::CONNECT),
	btnClose(Stock::CLOSE),
	cfg(Glib::get_home_dir() + "/." PACKAGE_NAME ".conf")
{
	HBox *mbox = manage(new HBox(false, 10)), *boxBtns = manage(new HBox(false, 2));
	VBox *box1 = manage(new VBox(false, 0));
	Table *tbServer = manage(new Table(2, 2, false));
	Table *tbCred  = manage(new Table(2, 2, false));
	Frame *fCred = manage(new Frame("Credenciais"));
	Adjustment *adj = manage(new Adjustment(3306, 1, 65535));

	entryPort = manage(new SpinButton(*adj, 0));

	tbServer->attach(lblAddress, 0, 2, 0, 1, FILL | EXPAND, FILL, 0, 4);
	tbServer->attach(entryAddress, 0, 1, 1, 2, FILL | EXPAND, FILL | EXPAND, 0, 0);
	tbServer->attach(*entryPort, 1, 2, 1, 2, FILL | SHRINK, FILL | EXPAND, 0, 0);

	box1->pack_start(lblTitle, false, true, 20);
	box1->pack_start(*manage(new HSeparator()), false, true, 0);
	box1->pack_start(msgInfo, false, true, 4);
	box1->pack_start(*tbServer, false, true, 0);


	boxBtns->pack_start(chkPersistent, true, true, 2);
	boxBtns->pack_start(btnConnect, false, true, 2);
	boxBtns->pack_start(btnClose, false, true, 2);
	box1->pack_end(*boxBtns, false, true, 0);

	tbCred->attach(lblUser, 0, 1, 0, 1, FILL, FILL, 0, 5);
	tbCred->attach(entryUser, 1, 2, 0, 1, FILL, FILL, 10, 5);
	tbCred->attach(lblPass, 0, 1, 1, 2, FILL, FILL, 0, 5);
	tbCred->attach(entryPass, 1, 2, 1, 2, FILL, FILL, 10, 5);

	fCred->add(*tbCred);

	box1->pack_start(*fCred, true, true, 4);
	mbox->pack_start(*box1, true, true, 10);
	mbox->show_all();
	add(*mbox);

	set_title("Aceder ao servidor");
	entryUser.set_size_request(185, -1);
	entryPass.set_size_request(185, -1);

	lblTitle.set_use_markup();
	lblAddress.set_use_markup();
	lblUser.set_use_markup();
	lblPass.set_use_markup();

	tbCred->set_size_request(100, -1);
	entryPort->set_size_request(80, -1);
	tbCred->set_size_request(150, -1);
	//entryPort.set_max_length(5);
	entryPass.set_visibility(false);
	if(mwin != NULL) {
		set_transient_for(*mwin);
		set_modal();
	}
	Box *t_box = (Box*) msgInfo.get_content_area();
	t_box->pack_start(lblMsgInfo, true, true, 0);

	set_position(WIN_POS_CENTER);

	// Setting up signal handlers
	signal_delete_event().connect(sigc::mem_fun(*this, &LoginWindow::on_window_deleteEvent));
	btnClose.signal_clicked().connect(sigc::mem_fun(*this, &LoginWindow::on_btnClose_clicked));
	btnConnect.signal_clicked().connect(sigc::mem_fun(*this, &LoginWindow::on_btnConnect_clicked));

	try {
		string hostname("hostname"), port_str("port"), user("username"), pass("password"), autologin("auto-login");
		double port;

		cfg.parse();

		const Section &tmp = cfg.get_section("Global");

		entryAddress.set_text(ustring(tmp.get_value(hostname) != NULL ? *tmp.get_value(hostname) : ""));
		entryUser.set_text(ustring(tmp.get_value(user) != NULL ? *tmp.get_value(user) : ""));
		entryPass.set_text(ustring(tmp.get_value(pass) != NULL ? *tmp.get_value(pass) : ""));

		if(tmp.get_value(autologin) != NULL)
			chkPersistent.set_active(*tmp.get_value(autologin) == "true"? true : false);

		istringstream ss(tmp.get_value(port_str) != NULL ? *tmp.get_value(port_str) : "3306");
		ss.width(5);
		ss >> port;
		entryPort->set_value(port);
	} catch (exception&e) { std::cout<< "Error..."<< e.what() << std::endl; }

	lblMsgInfo.show();
	msgInfo.hide();
}

bool LoginWindow::on_window_deleteEvent(GdkEventAny *ev)
{
	hide();
	return true;
}

void LoginWindow::on_btnClose_clicked(void)
{
	hide();
}

void LoginWindow::on_btnConnect_clicked(void)
{
	if(entryAddress.get_text().size() > 0 && entryUser.get_text().size() && entryPass.get_text().size() ) {
		msgInfo.hide();
		Section sec_global("Global");
		stringstream ss;

		ss << entryPort->get_value();

		sec_global.add("hostname", entryAddress.get_text());
		sec_global.add("port", ss.str());
		sec_global.add("username", entryUser.get_text());
		sec_global.add("password", entryPass.get_text());
		sec_global.add("auto-login", chkPersistent.get_active() ? "true" : "false");

		cfg.add(sec_global);
		cfg.save();

		lblMsgInfo.set_text("this is the path " + cfg.get_path());
		msgInfo.show();
	} else {
		lblMsgInfo.set_text("É necessário preencher todos os campos");
		msgInfo.set_message_type(MESSAGE_ERROR);
		msgInfo.show();
	}
}
