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

#include "main-window.h"
#include "patient-window.h"
#include "db/dbhandler.h"
#include "person.h"

using namespace std;
using namespace Glib;
using namespace Gtk;

MainWindow::MainWindow(const ustring& title, const ustring& dbpath) : Window(), m_db(dbpath),
		m_mFile("_Ficheiro",true ), m_mfQuit(Stock::QUIT),
		m_mHelp("_Ajuda", true), m_mhAbout(Stock::ABOUT),
		m_lblPatients("<b>_Pacientes</b>", true),
		m_mtbAdd("Novo Paciente"), m_mtbEdit(Stock::EDIT),
		m_mtbRemove("Remover Paciente"), m_entryPatientStatus(true)
{
	Box *mbox = manage(new VBox()), *pbox1 = manage(new VBox()), *pbox2 = manage(new HBox());
	ScrolledWindow *swPatients = manage(new ScrolledWindow()), *swVisits = manage(new ScrolledWindow());
	m_modelPatients = ListStore::create(m_lpCols);

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

	swPatients->add(m_treePatients);
	pbox2->pack_start(m_lblPatients, false, true, 4);
	pbox2->pack_start(m_entryPatients, true, true, 0);
	pbox1->pack_start(*pbox2, false, true, 0);
	pbox1->pack_start(*swPatients, true, true, 2);

	swVisits->add(m_treeVisits);

	m_paned1.pack1(*pbox1, false, true);
	m_paned1.pack2(*swVisits);

	mbox->pack_start(m_mainMenu, false, true, 0);
	mbox->pack_start(m_mainToolbar, false, true, 0);
	mbox->pack_start(m_paned1);

	swPatients->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);
	swVisits->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);

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


	set_title(title);
	set_default_size(680,500);
	set_icon_name(((ustring)PACKAGE_NAME).lowercase());

	/* Setting up columns in list patients */
	TreeViewColumn *col;

	m_treePatients.set_model(m_modelPatients);
	col = m_treePatients.get_column(m_treePatients.append_column("id", m_lpCols.m_col_id)-1);
	col->set_visible(false);
	col = m_treePatients.get_column(m_treePatients.append_column("Nome", m_lpCols.m_col_name)-1);
	col->set_expand();
	col->set_resizable();

	m_db.signal_person_added.connect(sigc::mem_fun(*this, &MainWindow::hlpr_append_patient));
	m_mtbAdd.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolAdd_clicked));
	m_mtbEdit.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolEdit_clicked));
	m_mtbRemove.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolRemove_clicked));
	m_entryPatients.signal_focus_in_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusIn));
	m_entryPatients.signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusOut));
	m_mhAbout.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_mhAbout_activate));
	signal_show().connect(sigc::mem_fun(*this, &MainWindow::on_window_show));


#ifdef WIN32
	cout<< "appending theme... "<< ((ustring)PACKAGE_NAME).lowercase()<< endl;
	RefPtr<IconTheme> it =  IconTheme::get_default();
	it->append_search_path("/home/ilan/Desktop/herbanaria/data/icons/hicolor");
#endif



	add(*mbox);
	show_all_children();
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
		if((res = m_db.person_insert(tmp)) > 0) {
			tmp.set_id(res);
			hlpr_append_patient(tmp.get_id(), tmp.get_name());
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

			}
		}
		m_db.close();
	}
}

void MainWindow::on_btnToolRemove_clicked()
{
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator iter = sel->get_selected();
	ListPatientsCols cols;

	if(*iter) {
		m_db.open();
		if(m_db.person_remove((*iter)[cols.m_col_id]))
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
		m_db.get_patients();
		m_db.close();
	} else
		cout<< "Error while opening the database..."<< endl;
}

bool MainWindow::on_entryPatient_focusIn(GdkEventFocus *focus)
{
	if(m_entryPatientStatus) {
		m_entryPatients.unset_text(STATE_NORMAL);
		m_entryPatients.set_text("");
		m_entryPatientStatus = false;
	}

	return true;
}

bool MainWindow::on_entryPatient_focusOut(GdkEventFocus *focus)
{
	if(!m_entryPatientStatus && m_entryPatients.get_text_length() == 0) {
		m_entryPatients.modify_text(STATE_NORMAL, Gdk::Color(ustring("Grey")));
		m_entryPatients.set_text("Procurar...");
		m_entryPatientStatus = true;
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
