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
#include <exception>

#include "exceptions/sql-connection.h"
#include "main-window.h"
#include "patient-window.h"
#include "db/dbhandler.h"
#include "person.h"


using namespace std;
using namespace Glib;
using namespace Gtk;


// File Constant
#define SEARCH_TIMEOUT 0.325

MainWindow::MainWindow(const ustring& title, RefPtr<Application>& app) : Window(WINDOW_TOPLEVEL),
	m_app(app), m_vp(NULL), m_vw(NULL),
	m_lblPatients("<b>_Pacientes</b>", true),
	m_mtbAdd("Novo Paciente"), m_mtbEdit(Stock::EDIT), m_mtbAddVisit("Nova Visita"),
	m_mtbRemove("Remover Paciente"), m_entryPatientStatus(true)
{
	DBHandler db = DBHandler::get_instance();
	Box *mbox = manage(new VBox()), *pbox2 = manage(new VBox(false, 0));
	Grid *pGridp2 = manage(new Grid());
	ScrolledWindow *swPatients = manage(new ScrolledWindow());
	m_modelPatients = ListStore::create(m_lpCols);
	Box *binfo, *pbox3, *pboxp2;
	Table *tbinfo;
	Widget * pwidget;
	ustring menu =  "<ui>" \
					 "<menubar name='MenuBar'>" \
					 "<menu name='File' action='MenuFile'>" \
					 "<menuitem name='Quit' action='MenuQuit' />" \
					 "</menu> " \
					 "<menu name='Help' action='MenuHelp'>" \
					 "<menuitem name='About' action='MenuAbout'/>" \
					 "</menu>" \
					 "</menubar>" \
					 "</ui>";

	m_pw = new PatientWindow(*this, "Dados do paciente", PatientWindow::PW_TYPE_ADD);
	get_visits_widgets();

	// Clear search timer...
	m_timerSearch.stop();
	m_timerSearch.reset();

	binfo = manage(new VBox(true, 2));
	tbinfo = manage(new Table(5, 2, false));
	pbox3 = manage(new VBox(false, 5));
	pboxp2 = manage(new VBox(false, 5));
	
	/*******************************
	 *      Creating The Menu      *
	 ******************************/
	m_uiman = UIManager::create();
	m_actionGroup = ActionGroup::create();

	m_actionGroup->add(Action::create("MenuFile", "_Ficheiro"));
	m_actionGroup->add(Action::create("MenuQuit", Gtk::Stock::QUIT), 
	                      sigc::ptr_fun(&Main::quit));
	m_actionGroup->add(Action::create("MenuHelp", "_Ajuda"));
	m_actionGroup->add(Action::create("MenuAbout", Stock::ABOUT), sigc::mem_fun(*this, &MainWindow::on_mhAbout_activate));

	m_uiman->insert_action_group(m_actionGroup);
	m_uiman->add_ui_from_string(menu);
	pwidget = m_uiman->get_widget("/MenuBar");

	add_accel_group(m_uiman->get_accel_group());
	
	m_mainToolbar.add(m_mtbAdd);
	m_mainToolbar.add(m_mtbEdit);
	m_mainToolbar.add(m_mtbRemove);
	m_mainToolbar.add(*manage(new SeparatorToolItem ())); 
	m_mainToolbar.add(m_mtbAddVisit);
	m_mainToolbar.add(m_mtbExpander);
	m_mainToolbar.add(m_mtbEntrySearch);
	
	swPatients->add(m_treePatients);
	
	m_mtbEntrySearch.add(m_entryPatients);

	/*******************************
	 *       Noteboook Page 1      *
	 ******************************/
	pbox2->pack_start(*swPatients, true, true, 0);

	m_nb.append_page(*pbox2);


	/*******************************
	*       Noteboook Page 2       *
	*******************************/

	m_nb.append_page(*m_gridVisits);

	mbox->pack_start(*pwidget, PACK_SHRINK);
	mbox->pack_start(m_mainToolbar, PACK_SHRINK);
	mbox->pack_start(m_nb, true, true, 0);
	

	/*pbox3->pack_start(m_frpinfo, false, true, 1);
	pbox3->pack_start(*swVisits, true, true, 2);

	/* Setting up columns in list patients */
	TreeViewColumn *col;
	m_modelVisits = ListStore::create(m_lvCols);

	col = m_treePatients.get_column(m_treePatients.append_column("id", m_lpCols.m_col_id) -1);
	col->set_visible(false);
	col = m_treePatients.get_column(m_treePatients.append_column("Nome", m_lpCols.m_col_name) -1);
	col->set_expand();
	col->set_resizable();
	col = m_treePatients.get_column(m_treePatients.append_column("Nº ID. Fiscal", m_lpCols.m_col_nif) -1);
	col->set_resizable();
	// set up the filter
	m_treeFilter = TreeModelFilter::create(m_modelPatients);
	
	m_treePatients.set_model (m_treeFilter);
	m_treeFilter->set_visible_func(sigc::mem_fun(*this, &MainWindow::filter_patient_by_name));


	/* Setup visits' treeview */
	col = m_treeVisits->get_column(m_treeVisits->append_column("id", m_lvCols.m_col_id) -1);
	col->set_visible(false);
	col = m_treeVisits->get_column(m_treeVisits->append_column("Queixa Principal", m_lvCols.m_col_complaint) -1);
	col->set_expand();
	col->set_clickable();
	col->set_resizable();
	m_treeVisits->get_column(m_treeVisits->append_column("Data", m_lvCols.m_col_date) -1);
	col->set_clickable();
	col->set_resizable();

	m_treeVisits->set_model(m_modelVisits);


	db.signal_person_added().connect(sigc::mem_fun(*this, &MainWindow::hlpr_append_patient));
	db.signal_visit_added().connect(sigc::mem_fun(*this, &MainWindow::hlpr_append_visit));
	db.signal_person_edited().connect(sigc::mem_fun(*this, &MainWindow::on_db_person_edited));
	m_mtbAdd.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolAdd_clicked));
	m_mtbAddVisit.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolAddVisit_clicked));
	m_mtbEdit.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolEdit_clicked));
	m_mtbRemove.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolRemove_clicked));
	m_entryPatients.signal_focus_in_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusIn));
	m_entryPatients.signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusOut));
	//m_treePatients.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_treePatients_selected));
	m_treePatients.signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_treePatients_activated));
	m_entryPatients.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_txtSearch_changed));
	signal_show().connect(sigc::mem_fun(*this, &MainWindow::on_window_show));
	signal_timeout().connect(sigc::mem_fun(*this, &MainWindow::handler_timeout_search), 1);
	m_pw->signal_add().connect(sigc::mem_fun(*this, &MainWindow::patient_window_add));
	m_btnViewPatient->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnShPatient_clicked));
	m_btnBack->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnBack_clicked));
	
	/************************************
	 *    Setting up some properties    *
	 ***********************************/
	m_mainToolbar.get_style_context()->add_class(GTK_STYLE_CLASS_PRIMARY_TOOLBAR);
	swPatients->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);

	m_entryPatients.set_placeholder_text ("Procurar paciente...");
	//m_entryPatients.set_icon_from_stock(Stock::FIND);
	m_entryPatients.set_icon_from_icon_name("preferences-system-search-symbolic");
	m_entryPatients.set_icon_sensitive(ENTRY_ICON_PRIMARY, true);
	m_entryPatients.set_icon_activatable(false);

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
	m_mtbExpander.set_expand();
	//m_mtbRemove.set_is_important();
	m_mtbRemove.set_use_underline();
	m_mtbAddVisit.set_stock_id(Stock::ADD);
	
	swPatients->set_shadow_type(SHADOW_ETCHED_OUT);

	set_title(title);
	set_default_size(720,500);
	set_icon_name(((ustring)PACKAGE_NAME).lowercase());
	
	m_entryPatients.set_width_chars(26);
	swPatients->set_margin_left(1);
	swPatients->set_margin_right(1);
	swPatients->set_margin_top(5);
	swPatients->set_margin_bottom(1);
	
	m_nb.set_show_tabs(false);
	m_nb.set_current_page(1);
	m_nb.set_current_page(0);
	m_app->add_window(*m_pw);

	add(*mbox);
	show_all_children();
}

MainWindow::~MainWindow()
{
	if(m_vp)
		delete m_vp;
	m_app->remove_window(*m_pw);
	if(m_vw) {
		m_app->remove_window((Window&)*m_vw->get_window());
		delete m_vw;
	}
	
	//delete m_pw;
}

/* Helpers */
void MainWindow::hlpr_append_patient(guint32 id, const ustring& name, guint32 nif)
{
	//m_treePatients.unset_model();
	TreeModel::Row row = *(m_modelPatients->append());

	row[m_lpCols.m_col_id] = id;
	row[m_lpCols.m_col_name] = name;
	row[m_lpCols.m_col_nif] = nif;
}

void MainWindow::hlpr_append_visit(guint32 id, const ustring& complaint, const ustring& date)
{
	TreeModel::Row row = *(m_modelVisits->append());

	row[m_lvCols.m_col_id] = id;
	row[m_lvCols.m_col_complaint] = complaint;
	row[m_lvCols.m_col_date] = date;
}



/* Signal Handlers */

bool MainWindow::on_delete_event(GdkEventAny * event)
{
	return false;
}

void MainWindow::on_window_show(void)
{
	DBHandler db = DBHandler::get_instance();

	if(db.open()) {
		db.get_patients(NULL);
		db.close();
	} else
		cout<< "Error while opening the database..."<< endl;
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


void MainWindow::on_btnToolAdd_clicked(void)
{
	m_pw->set_window_type(PatientWindow::PW_TYPE_ADD);
	m_pw->show();
}

void MainWindow::on_btnToolAddVisit_clicked(void)
{
	if(!m_vw) {
		m_vw = new VisitsWindow(10);
		m_app->add_window((Window&)*m_vw->get_window());
	}
	m_vw->show();
}

void MainWindow::on_btnShPatient_clicked(void)
{
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator iter = sel->get_selected();
	ListPatientsCols cols;
	DBHandler db = DBHandler::get_instance();

	if(*iter) {
		TreeModel::Row row = *iter;
		Person p(row[cols.m_col_id]);
		db.open();
		if(db.get_person(row[cols.m_col_id], p)) {
			db.close();

			if(m_vp == NULL) {
				MessageDialog msgbox("<b>Não é possível abrir a ficha do paciente</b>", true, MESSAGE_ERROR, BUTTONS_OK, true);
				msgbox.set_title("Erro na abertura da ficha do paciente");
				try {
					m_vp = new ViewPatientWindow(* ((Window*)this));

					m_vp->set_person(p);
					m_vp->show_all();
				} catch(FileError& ex) {
					delete m_vp;
					m_vp = NULL;
					msgbox.set_secondary_text(ex.what());
					msgbox.run();
				} catch(MarkupError& ex) {
					delete m_vp;
					m_vp = NULL;
					msgbox.set_secondary_text("Erro na estrutura do ficheiro de interface: " + ex.what());
					msgbox.run();
				} catch(BuilderError& ex) {
					delete m_vp;
					m_vp = NULL;
					msgbox.set_secondary_text("Erro na construção da interface: " + ex.what());
					msgbox.run();
				} catch(exception &ex) {
					delete m_vp;
					m_vp = NULL;
					msgbox.set_secondary_text("O ficheiro de interface está corrupto: um ou mais controles inexistentes." );
					msgbox.run();
				}
			} else { 

				m_vp->set_person(p);
				m_vp->show_all();
			}
		}
	}
}

void MainWindow::on_btnToolEdit_clicked(void)
{
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator iter = sel->get_selected();
	ListPatientsCols cols;
	DBHandler db = DBHandler::get_instance();

	if(*iter) {
		TreeModel::Row row = *iter;
		Person p(row[cols.m_col_id]);
		db.open();
		if(db.get_person(row[cols.m_col_id], p)) {
			db.close();
			m_pw->set_window_type(PatientWindow::PW_TYPE_EDIT);
			m_pw->set_person(p);
			m_pw->show();
		}
	}
}

void MainWindow::on_btnToolRemove_clicked()
{
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator iter = sel->get_selected();
	DBHandler db = DBHandler::get_instance();

	if(*iter) {
		db.open();
		if(db.person_remove((*iter)[m_lpCols.m_col_id]))
			m_modelPatients->erase(iter);
		else {
			MessageDialog dlg((string)"Não foi possível remover o paciente selecionado", false, MESSAGE_ERROR, BUTTONS_OK, true);
			dlg.run();
		}
		db.close();
	} else {
		MessageDialog dlg((string)"Deve selecionar um item para eliminar", false, MESSAGE_INFO, BUTTONS_OK, true);
		dlg.run();
	}
}


void MainWindow::on_treePatients_activated(const TreeModel::Path& path, TreeViewColumn* col)
{
	Person p;
	Date today;
	guint16 age;
	char tmp[6];
	bool close= true;
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator row = sel->get_selected();
	DBHandler db = DBHandler::get_instance();

	today.set_time_current();

	if(!row) {
		cout<< "Iterator is not valid"<< endl;
		return;
	}
	
	if(*row) {
		try{
			db.open();
		} catch(SqlConnectionOpenedException& ex) {
			close = false;
		}

		db.get_person((*row)[m_lpCols.m_col_id], p);
		if(close)
			db.close();
		m_lblPName->set_text(ustring("<b><span size=\"x-large\">" + p.get_name() + "</span></b>"));
		m_lblPName->set_use_markup();
		age = today.get_year() - p.get_birthday().get_year();

		if(today.get_month() < p.get_birthday().get_month() || (today.get_month() == p.get_birthday().get_month() && today.get_day() < p.get_birthday().get_day()))
			age--;

		sprintf(tmp, "%hu", age);
		m_lblPAge->set_text(tmp);
		m_lblPBloodtype->set_text(p.get_blood_type_string());
		sprintf(tmp, "%.2f", p.get_height());
		m_lblPHeight->set_text(tmp);
		m_lblPSex->set_text((p.get_sex()? "Masculino" : "Feminino"));

		m_nb.set_current_page(1);
	}
}

//void  MainWindow::on_treePatients_selected()
//{
//	Person p;
//	Date today;
//	guint16 age;
//	char tmp[6];
//	bool close= true;
//	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
//	TreeModel::iterator row = sel->get_selected();
//	DBHandler db = DBHandler::get_instance();
//
//	today.set_time_current();
//
//	if(!row) {
//		cout<< "Iterator is not valid"<< endl;
//		return;
//	}
//	
//	if(*row) {
//		try{
//			db.open();
//		} catch(SqlConnectionOpenedException& ex) {
//			close = false;
//		}
//
//		db.get_person((*row)[m_lpCols.m_col_id], p);
//		if(close)
//			db.close();
//		m_lblPName->set_text(ustring("<b>" + p.get_name() + "</b>"));
//		age = today.get_year() - p.get_birthday().get_year();
//
//		if(today.get_month() < p.get_birthday().get_month() || (today.get_month() == p.get_birthday().get_month() && today.get_day() < p.get_birthday().get_day()))
//			age--;
//
//		sprintf(tmp, "%hu", age);
//		m_lblPAge->set_text(tmp);
//		m_lblPBloodtype->set_text(p.get_blood_type_string());
//		sprintf(tmp, "%.2f", p.get_height());
//		m_lblPHeight->set_text(tmp);
//		m_lblPSex->set_text((p.get_sex()? "Masculino" : "Feminino"));
//
//		//m_lblpname.set_use_markup();
//		//m_lblpname.set_alignment(0.0f, 0.5f);
//		//m_lblpage.set_use_markup();
//		//m_lblpage.set_alignment(0.0f, 0.5f);
//		//m_lblpbloodtype.set_use_markup();
//		//m_lblpbloodtype.set_alignment(0.1f, 0.5f);
//		//m_lblpheight.set_use_markup();
//		//m_lblpheight.set_alignment(0.1f, 0.5f);
//		//m_lblpsex.set_use_markup();
//		//m_lblpsex.set_alignment(0.1f, 0.5f);
//
//		m_nb.set_current_page(1);
//	}
//}

void MainWindow::on_db_person_edited(const Person &p)
{
	Date today;
	int age;
	char tmp[10];

	today.set_time_current();

	m_lblPName->set_text(ustring("<b>" + p.get_name() + "</b>"));
	age = today.get_year() - p.get_birthday().get_year();

	if(today.get_month() < p.get_birthday().get_month() || (today.get_month() == p.get_birthday().get_month() && today.get_day() < p.get_birthday().get_day()))
		age--;

	sprintf(tmp, "%hu", age);
	m_lblPAge->set_text(tmp);
	m_lblPBloodtype->set_text(p.get_blood_type_string());
	sprintf(tmp, "%.2f", p.get_height());
	m_lblPHeight->set_text(tmp);
	m_lblPSex->set_text((p.get_sex()? "Masculino" : "Feminino"));

}

void MainWindow::patient_window_add(PatientWindow &pw)
{
	Person p;
	DBHandler db = DBHandler::get_instance();

	pw.get_person(p);
	db.open();
	try {
		if(pw.get_window_type() == PatientWindow::PW_TYPE_ADD)
			db.person_insert(p);
		else {
			db.person_update(p);
			RefPtr<TreeSelection> sel = m_treePatients.get_selection();
			TreeModel::iterator iter = sel->get_selected();
			ListPatientsCols cols;

			if(*iter) {
				TreeModel::Row row = *iter;
				row[cols.m_col_name] = p.get_name();
			}
		}
	} catch(std::invalid_argument& ex) {
		MessageDialog msg(*this, ex.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		msg.run();
	}
	db.close();
}

void MainWindow::on_txtSearch_changed()
{
	m_timerSearch.reset();
	m_timerSearch.start();
}

bool MainWindow::handler_timeout_search()
{
	if(m_timerSearch.elapsed() > SEARCH_TIMEOUT) {
		RefPtr<TreeModelFilter> filter = RefPtr<TreeModelFilter>::cast_dynamic (m_treePatients.get_model());

		filter->refilter();
		
		m_timerSearch.stop();
		m_timerSearch.reset();
	}

	return true;
}

bool MainWindow::on_entryPatient_focusIn(GdkEventFocus *focus)
{
	if(m_entryPatientStatus) {
		m_entryPatientStatus = false;
	//	m_entryPatients.override_color(Gdk::RGBA("Black"), STATE_FLAG_NORMAL);
	//	m_entryPatients.set_text("");
	}

	return true;
}

bool MainWindow::on_entryPatient_focusOut(GdkEventFocus *focus)
{

	if(!m_entryPatientStatus && m_entryPatients.get_text_length() == 0) {
		m_entryPatientStatus = true;
	//	m_entryPatients.override_color(Gdk::RGBA("Grey"), STATE_FLAG_NORMAL);	
	//	m_entryPatients.set_text("Procurar paciente...");
	}

	return true;
}

bool MainWindow::filter_patient_by_name(const TreeModel::const_iterator& iter)
{
	ustring item = (*iter)[m_lpCols.m_col_name];

	if(m_entryPatientStatus || Regex::match_simple(".*" + m_entryPatients.get_text() + ".*", item))
		return true;
	else
		return false;
}

void MainWindow::on_btnBack_clicked(void)
{
	m_nb.set_current_page(0);
}

void MainWindow::get_visits_widgets(void)
{
	RefPtr<Builder> builder = Builder::create_from_file(GLADE_VISITS);

	builder->get_widget("treeVisits", m_treeVisits);
	builder->get_widget("btnBack", m_btnBack);
	builder->get_widget("lblPName", m_lblPName);
	builder->get_widget("lblPBlood", m_lblPBloodtype);
	builder->get_widget("lblPHeight", m_lblPHeight);
	builder->get_widget("lblPSex", m_lblPSex);
	builder->get_widget("lblPAge", m_lblPAge);
	builder->get_widget("btnViewPatient", m_btnViewPatient);
	builder->get_widget("gridVisits", m_gridVisits);
}