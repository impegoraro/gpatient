#include "main-window.h"
#include "patient-window.h"
#include "db/dbhandler.h"

using namespace Glib;
using namespace Gtk;


MainWindow::MainWindow(string title) : Window(),
		m_mFile("File"), m_mfQuit(Stock::QUIT),
		m_mHelp("Help"), m_mhAbout(Stock::ABOUT),
		m_lblPatients("<b>_Pacientes</b>", true),
		m_mtbAdd("_Novo Paciente"), m_mtbRemove("_Remover Paciente"), m_entryPatientStatus(true)
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

	m_mtbAdd.set_stock_id(Stock::ADD);
	m_mtbAdd.set_use_underline();
	m_mtbAdd.set_is_important();
	m_mtbRemove.set_stock_id(Stock::DELETE);
	m_mtbRemove.set_is_important();
	m_mtbRemove.set_use_underline();

	m_lblPatients.set_mnemonic_widget(m_entryPatients);

	swPatients->set_shadow_type(SHADOW_ETCHED_OUT);
	swVisits->set_shadow_type(SHADOW_ETCHED_OUT);

	set_title(title);
	set_default_size(680,500);

	/* Setting up columns in list patients */
	TreeViewColumn *col;

	m_treePatients.set_model(m_modelPatients);
	col = m_treePatients.get_column(m_treePatients.append_column("id", m_lpCols.m_col_id)-1);
	col->set_visible(false);
	col = m_treePatients.get_column(m_treePatients.append_column("Nome", m_lpCols.m_col_name)-1);
	col->set_expand();
	col->set_resizable();

	m_mtbAdd.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolAdd_clicked));
	m_entryPatients.signal_focus_in_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusIn));
	m_entryPatients.signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusOut));
	m_mtbRemove.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolRemove_clicked));
	signal_show().connect(sigc::mem_fun(*this, &MainWindow::on_window_show));

	add(*mbox);
	show_all_children();
}

/* Helpers */
void MainWindow::hlpr_append_patient(unsigned int id, const Glib::ustring& name)
{
	TreeModel::Row row = *(m_modelPatients->append());

	row[m_lpCols.m_col_id] = id;
	row[m_lpCols.m_col_name] = name;
}

/* Signal Handlers */

void MainWindow::on_btnToolAdd_clicked(void)
{
	PatientWindow p;

	if(p.run() == RESPONSE_ACCEPT) {
		if(p.get_patient_name().length() > 0) {
			hlpr_append_patient(1, p.get_patient_name());
		}
	}
}

void MainWindow::on_btnToolRemove_clicked()
{

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

void MainWindow::on_window_show(void)
{
	DBHandler db(string("/home/ilan/Escritorio/herbanaria/herbanaria.sqlite"));
	if(db.open()) {
		vector<string> *tmp = db.get_patients();
		for(string& t: *tmp)
			hlpr_append_patient(1, ustring(t));
		delete tmp;
		db.close();
	} else
		cout<< "Error while opening the database..."<< endl;
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
