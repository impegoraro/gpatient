/*
 * main-window.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif
#include <iostream>
#include <cstdio>

#include "exceptions/sql-connection.h"
#include "main-window.h"
#include "patient-window.h"
#include "db/dbhandler.h"
#include "person.h"

using namespace std;
using namespace Glib;
using namespace Gtk;


// File Constant
#define SEARCH_TIMEOUT 1.7568

MainWindow::MainWindow(const ustring& title, const ustring& dbpath) : Window(), m_db(dbpath),
	m_mFile("_Ficheiro",true ), m_mfQuit(Stock::QUIT),
	m_mHelp("_Ajuda", true), m_mhAbout(Stock::ABOUT),
	m_lblPatients("<b>_Pacientes</b>", true),
	m_mtbAdd("Novo Paciente"), m_mtbEdit(Stock::EDIT),
	m_mtbRemove("Remover Paciente"), m_entryPatientStatus(true),
	m_frpinfo("<b>Identificação</b>"),
	m_lblsugestions("<span size=\"xx-large\">Para começar selecione um paciente da lista</span>")
{
	Box *mbox = manage(new VBox()), *pbox1 = manage(new VBox()), *pbox2 = manage(new HBox());
	ScrolledWindow *swPatients = manage(new ScrolledWindow()), *swVisits = manage(new ScrolledWindow());
	m_modelPatients = ListStore::create(m_lpCols);
	Box *binfo, *pbox3;
	Table *tbinfo;

	// Clear search timer...
	m_timerSearch.stop();
	m_timerSearch.reset();

	binfo = manage(new VBox(true, 2));
	tbinfo = manage(new Table(5, 2, false));
	pbox3 = manage(new VBox(false, 5));

	m_mFile.set_submenu(m_filemenu);
	m_filemenu.append(*manage(new SeparatorMenuItem()));
	m_filemenu.append(m_mfQuit);

	m_mHelp.set_submenu(m_helpmenu);
	m_helpmenu.append(m_mhAbout);

	m_mainMenu.append(m_mFile);
	m_mainMenu.append(m_mHelp);

	m_mainToolbar.add(m_mtbAdd);
	m_mainToolbar.add(m_mtbEdit);
	m_mainToolbar.add(m_mtbRemove);

	m_frpinfo.add(*binfo);
	binfo->pack_start(*tbinfo, true, true, 10);
	tbinfo->attach(m_lblpname, 0, 4, 0, 1, FILL | SHRINK | EXPAND, FILL, 10, 0);
	tbinfo->attach(m_lblpage, 0, 1, 1, 2, FILL | SHRINK | EXPAND, FILL, 10, 0);
	tbinfo->attach(m_lblpbloodtype, 1, 2, 1, 2, FILL | SHRINK | EXPAND, FILL, 0, 0);
	tbinfo->attach(m_lblpheight, 2, 3, 1, 2, FILL | SHRINK | EXPAND, FILL, 0, 0);
	tbinfo->attach(m_lblpsex, 3, 4, 1, 2, FILL | SHRINK | EXPAND, FILL, 0, 0);
	tbinfo->set_row_spacings(6);

	swPatients->add(m_treePatients);
	pbox2->pack_start(m_lblPatients, false, true, 4);
	pbox2->pack_start(m_entryPatients, true, true, 0);
	pbox1->pack_start(*pbox2, false, true, 4);
	pbox1->pack_start(*swPatients, true, true, 2);

	swVisits->add(m_treeVisits);

	pbox3->pack_start(m_frpinfo, false, true, 1);
	pbox3->pack_start(*swVisits, true, true, 2);
	pbox3->pack_start(m_lblsugestions, true, true, 50);

	mbox->pack_start(m_mainMenu, false, true, 0);
	mbox->pack_start(m_mainToolbar, false, true, 0);
	mbox->pack_start(m_paned1);

	swPatients->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);
	swVisits->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);


	m_paned1.pack1(*pbox1, false, true);
	m_paned1.pack2(*pbox3);

	((Label*)m_frpinfo.get_label_widget())->set_use_markup();
	m_lblPatients.set_use_markup();
	m_lblPatients.set_alignment(0.00f, 0.5f);

	m_paned1.get_child1()->set_size_request(220, -1);

	m_entryPatients.modify_text(STATE_NORMAL, Gdk::Color(ustring("Grey")));
	m_entryPatients.set_text("Procurar...");
	m_entryPatients.set_icon_from_stock(Stock::FIND);

	m_mtbAdd.set_stock_id(Stock::ADD);
	m_mtbAdd.set_use_underline();
	m_mtbAdd.set_is_important();
	m_mtbAdd.set_has_tooltip();
	m_mtbAdd.set_tooltip_text("Inserir um novo paciente");
	m_mtbEdit.set_has_tooltip();
	m_mtbEdit.set_tooltip_text("Editar o paciente selecionado");
	m_mtbRemove.set_has_tooltip();
	m_mtbRemove.set_tooltip_text("Eliminar o paciente selecionado");
	m_mtbRemove.set_stock_id(Stock::DELETE);
	//m_mtbRemove.set_is_important();
	m_mtbRemove.set_use_underline();

	m_lblPatients.set_mnemonic_widget(m_entryPatients);

	swPatients->set_shadow_type(SHADOW_ETCHED_OUT);
	swVisits->set_shadow_type(SHADOW_ETCHED_OUT);

	m_lblsugestions.set_use_markup();
	//m_lblsugestions.set_line_wrap();

	set_title(title);
	set_default_size(720,500);
	set_icon_name(((ustring)PACKAGE_NAME).lowercase());

	/* Setting up columns in list patients */
	TreeViewColumn *col;

	m_treePatients.set_model(m_modelPatients);
	col = m_treePatients.get_column(m_treePatients.append_column("id", m_lpCols.m_col_id)-1);
	col->set_visible(false);
	col = m_treePatients.get_column(m_treePatients.append_column("Nome", m_lpCols.m_col_name)-1);
	col->set_expand();
	col->set_resizable();

	m_db.signal_person_added().connect(sigc::mem_fun(*this, &MainWindow::hlpr_append_patient));
	m_mtbAdd.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolAdd_clicked));
	m_mtbEdit.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolEdit_clicked));
	m_mtbRemove.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolRemove_clicked));
	m_entryPatients.signal_focus_in_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusIn));
	m_entryPatients.signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusOut));
	m_mhAbout.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_mhAbout_activate));
	m_treePatients.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_treePatients_selected));
	m_entryPatients.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_txtSearch_changed));
	signal_show().connect(sigc::mem_fun(*this, &MainWindow::on_window_show));
	signal_timeout().connect(sigc::mem_fun(*this, &MainWindow::handler_timeout_search), 1);

	add(*mbox);
	binfo->show_all();
	show_all_children();
	m_frpinfo.hide();
	swVisits->hide();
}

/* Helpers */
void MainWindow::hlpr_append_patient(guint32 id, const ustring& name)
{
	TreeModel::Row row = *(m_modelPatients->append());

	row[m_lpCols.m_col_id] = id;
	row[m_lpCols.m_col_name] = name;
}

/* Signal Handlers */

void MainWindow::on_btnToolAdd_clicked(void)
{
	PatientWindow p(*this);

	if(p.run() == RESPONSE_ACCEPT) {
		int res(0);
		Person tmp;
		p.get_person(tmp);
		m_db.open();
		try {
			m_db.person_insert(tmp);
		} catch(std::invalid_argument& ex) {
			MessageDialog msg(*this, ex.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
			msg.run();
		}
		m_db.close();
	}
}

void MainWindow::on_btnToolEdit_clicked(void)
{
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator iter = sel->get_selected();
	ListPatientsCols cols;
	PatientWindow pw(*this, "Dados do Paciente", PatientWindow::PW_TYPE_EDIT);

	if(*iter) {
		TreeModel::Row row = *iter;
		Person p(row[cols.m_col_id]);
		m_db.open();
		if(m_db.get_person(row[cols.m_col_id], p)) {
			pw.set_person(p);
			if(pw.run() == RESPONSE_ACCEPT) {
				pw.get_person(p);
				row[cols.m_col_name] = p.get_name();
				try {
					m_db.person_update(p);
				} catch (std::invalid_argument& ex) {
					MessageDialog msg(*this, ex.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
					msg.run();
				}
			}
		}
		m_db.close();
	}
}

void MainWindow::on_btnToolRemove_clicked()
{
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator iter = sel->get_selected();

	if(*iter) {
		m_db.open();
		if(m_db.person_remove((*iter)[m_lpCols.m_col_id]))
			m_modelPatients->erase(iter);
		else {
			MessageDialog dlg((string)"Não foi possível remover o paciente selecionado", false, MESSAGE_ERROR, BUTTONS_OK, true);
			dlg.run();
		}
		m_db.close();
	} else {
		MessageDialog dlg((string)"Deve selecionar um item para eliminar", false, MESSAGE_INFO, BUTTONS_OK, true);
		dlg.run();
	}
}

void MainWindow::on_window_show(void)
{
	if(m_db.open()) {
		m_db.get_patients(NULL);
		m_db.close();
	} else
		cout<< "Error while opening the database..."<< endl;
}

bool MainWindow::on_entryPatient_focusIn(GdkEventFocus *focus)
{
	if(m_entryPatientStatus) {
		m_entryPatientStatus = false;
		m_entryPatients.unset_text(STATE_NORMAL);
		m_entryPatients.set_text("");
	}

	return true;
}

bool MainWindow::on_entryPatient_focusOut(GdkEventFocus *focus)
{
	if(!m_entryPatientStatus && m_entryPatients.get_text_length() == 0) {
		m_entryPatientStatus = true;
		m_entryPatients.modify_text(STATE_NORMAL, Gdk::Color(ustring("Grey")));
		m_entryPatients.set_text("Procurar...");
	}

	return true;
}

void MainWindow::on_mhAbout_activate(void)
{
	AboutDialog about;
	vector<ustring> authors;
	authors.push_back("Ilan Pegoraro <iemoreirap@gmail.com>");

	//about.set_default_icon(get_icon());
	//about.set_modal();
	//about.set_parent(*this);

	about.set_program_name(ustring(PACKAGE_NAME));
	about.set_version(ustring(PACKAGE_VERSION));
	about.set_comments("Permite gestão de pacientes, com o respectivo histórico de cada visita");
	about.set_copyright("(C) 2012 Ilan Pegoraro");
	about.set_authors(authors);

	about.run();
}

void  MainWindow::on_treePatients_selected()
{
	Person p;
	Date today;
	guint16 age;
	char tmp[6];
	bool close= true;
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator row = sel->get_selected();

	today.set_time_current();

	if(*row) {
		try{
			m_db.open();
		} catch(SqlConnectionOpenedException& ex) {
			close = false;
		}

		m_db.get_person((*row)[m_lpCols.m_col_id], p);
		if(close)
			m_db.close();
		m_lblpname.set_text(ustring("<b>Paciente:</b> <i>") + p.get_name() + "</i>");
		age = today.subtract_years(p.get_birthday().get_year()).get_year();

		if(today.get_month() < p.get_birthday().get_month() || (today.get_month() == p.get_birthday().get_month() && today.get_day() < p.get_birthday().get_day()))
			age--;

		sprintf(tmp, "%hu", age);
		m_lblpage.set_text(ustring("<b>Idade:</b> <i>") + tmp + (ustring)"</i>");
		m_lblpbloodtype.set_text("<b>Tipo de Sangue:</b> <i>" + p.get_blood_type_string() + "</i>");
		sprintf(tmp, "%.2f", p.get_height());
		m_lblpheight.set_text(ustring("<b>Altura:</b> <i>") + tmp + (ustring)"</i>");
		m_lblpsex.set_text(ustring("<b>Sexo:</b> ") + (p.get_sex()? "Masculino" : "Feminino") + (ustring)"<i></i>");

		m_lblpname.set_use_markup();
		m_lblpname.set_alignment(0.0f, 0.5f);
		m_lblpage.set_use_markup();
		m_lblpage.set_alignment(0.0f, 0.5f);
		m_lblpbloodtype.set_use_markup();
		m_lblpbloodtype.set_alignment(0.1f, 0.5f);
		m_lblpheight.set_use_markup();
		m_lblpheight.set_alignment(0.1f, 0.5f);
		m_lblpsex.set_use_markup();
		m_lblpsex.set_alignment(0.1f, 0.5f);

		m_lblsugestions.hide();
		m_frpinfo.show_all();
		m_treeVisits.get_parent()->show();
	} else {
		m_lblsugestions.show();
		m_frpinfo.hide();
		m_treeVisits.get_parent()->hide();
	}
}

void MainWindow::on_txtSearch_changed()
{
	m_timerSearch.reset();
	m_timerSearch.start();
}

bool MainWindow::handler_timeout_search()
{
	if(m_timerSearch.elapsed() > SEARCH_TIMEOUT) {
		try {
			ustring tmp;
			m_db.open();
			m_modelPatients->clear();
			if(m_entryPatientStatus)
				m_db.get_patients(NULL);
			else {
				tmp = m_entryPatients.get_text();
				m_db.get_patients(&tmp);
			}
			m_db.close();

			m_timerSearch.stop();
			m_timerSearch.reset();

		} catch(SqlConnectionException& ex) {

		}
	}

	return true;
}
