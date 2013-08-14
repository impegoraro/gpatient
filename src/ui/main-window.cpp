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
#include "visitswindow.h"
#include "db/dbhandler.h"
#include "person.h"
#include "util.h"


using namespace std;
using namespace Glib;
using namespace Gtk;


// File Constant
#define SEARCH_TIMEOUT 0.325

MainWindow::MainWindow(const ustring& title, RefPtr<Application>& app) : Window(WINDOW_TOPLEVEL),
	m_app(app), m_vp(NULL), m_vw(NULL), m_svw(NULL), m_visitID(0),
	m_lblPatients("<b>_Pacientes</b>", true),
	m_mtbAdd("Novo Paciente"), m_mtbEdit(Stock::EDIT),
	m_mtbRemove("Remover Paciente"), m_entryPatientStatus(true), m_maximized(false)
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
	mbox->pack_end(m_statusbar, false, true, 0);

	/*pbox3->pack_start(m_frpinfo, false, true, 1);
	pbox3->pack_start(*swVisits, true, true, 2);

	/* Setting up columns in list patients */
	TreeViewColumn *col;
	m_modelVisits = TreeStore::create(m_lvCols);

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
	db.signal_subvisit_added().connect(sigc::mem_fun(*this, &MainWindow::hlpr_append_subvisit));
	db.signal_person_edited().connect(sigc::mem_fun(*this, &MainWindow::on_db_person_edited));
	db.signal_visit_edited().connect(sigc::mem_fun(*this, &MainWindow::on_visitEdited));
	db.signal_subvisit_edited().connect(sigc::mem_fun(*this, &MainWindow::on_subvisitEdited));
	m_mtbAdd.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolAdd_clicked));
	m_btnNewVisit->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolAddVisit_clicked));
	m_btnNewVisit2->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolAddVisit_clicked));
	m_mtbEdit.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolEdit_clicked));
	m_mtbRemove.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnToolRemove_clicked));
	m_entryPatients.signal_focus_in_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusIn));
	m_entryPatients.signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::on_entryPatient_focusOut));
	//m_treePatients.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_treePatients_selected));
	m_treePatients.signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_treePatients_activated));
	m_entryPatients.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_txtSearch_changed));
	signal_show().connect(sigc::mem_fun(*this, &MainWindow::on_window_show));
	signal_timeout().connect(sigc::mem_fun(*this, &MainWindow::handler_timeout_search), 1);
	signal_window_state_event().connect(sigc::mem_fun(*this, &MainWindow::on_maximized_change));
	m_pw->signal_add().connect(sigc::mem_fun(*this, &MainWindow::patient_window_add));
	m_btnViewPatient->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnShPatient_clicked));
	m_btnBack->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnBack_clicked));
	m_btnRemoveVisit->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_btnRemoveVisit));
	m_treeVisits->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_treeVisit_activated));
	m_treeVisits->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_visits_selection_changed));
	m_btnVisitEdit->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_visitEdit_clicked));
	m_btnNewSubvisit->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_newSubVisit_clicked));
	m_btnSubVisitEdit->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_subvisitEdit_clicked));


	/************************************
	 *    Setting up some properties    *
	 ***********************************/
	
	m_mainToolbar.get_style_context()->add_class(GTK_STYLE_CLASS_PRIMARY_TOOLBAR);
	swPatients->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);

	m_entryPatients.set_placeholder_text ("Procurar paciente...");
	m_entryPatients.set_icon_from_stock(Stock::FIND);
	//preferences-system-search-symbolic
	//m_entryPatients.set_icon_from_icon_name("preferences-system-search-symbolic");
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
	
	m_treeVisits->set_grid_lines(TREE_VIEW_GRID_LINES_HORIZONTAL);
	swPatients->set_shadow_type(SHADOW_ETCHED_OUT);

	m_panedVisits->set_position(420);

	set_title(title);
	set_default_size(860, 640);
	set_icon_name(((ustring)PACKAGE_NAME).lowercase());

	m_entryPatients.set_width_chars(34);
	swPatients->set_margin_left(1);
	swPatients->set_margin_right(1);
	swPatients->set_margin_top(5);
	swPatients->set_margin_bottom(1);
	
	m_nb.set_show_tabs(false);
	m_app->add_window(*m_pw);

	add(*mbox);
	show_all_children();
	m_nb.set_current_page(0);
	parse();
}

MainWindow::~MainWindow()
{
	if(m_vp)
		delete m_vp;
	
	if(m_vw) {
		m_app->remove_window((Window&)*m_vw->get_window());
		delete m_vw;
	}
	
	if(m_pw) {
		m_app->remove_window(*m_pw);
		delete m_pw;
	}
	if(m_svw) {
		m_app->remove_window((Window&)*m_svw->get_window());
		delete m_svw;
	}
		
}

/* Helpers */
void MainWindow::hlpr_append_patient(guint32 id, const ustring& name, guint32 nif)
{
	//m_treePatients.unset_model();
	TreeModel::Row row =  *m_modelPatients->append();
	ustring msg;

	row[m_lpCols.m_col_id] = id;
	row[m_lpCols.m_col_name] = name;
	row[m_lpCols.m_col_nif] = nif;

	if(m_nb.get_current_page() == 0)
		msg = ustring::compose("Pacientes registados: %1", m_modelPatients->children().size());
	else 
		msg = ustring::compose("Pacientes registados: %1, Visitas registadas: %2", m_modelPatients->children().size(), m_modelVisits->children().size());
	m_statusbar.push(msg);
}

void MainWindow::hlpr_append_subvisit(guint32 id, const Glib::ustring& complaint, const Glib::ustring& date)
{
	ustring msg;
	TreeModel::Row row = *(m_modelVisits->append((*m_visitSelected).children()));

	row[m_lvCols.m_col_id] = id;
	row[m_lvCols.m_col_complaint] = complaint;
	row[m_lvCols.m_col_date] = date;
	msg = ustring::compose("Pacientes registados: %1, Visitas registadas: %2", m_modelPatients->children().size(), m_modelVisits->children().size());
	m_statusbar.push(msg);
}

void MainWindow::hlpr_append_visit(guint32 id, const ustring& complaint, const ustring& date)
{
	ustring msg;

	m_visitSelected = m_modelVisits->append();
	TreeModel::Row row = *m_visitSelected;

	row[m_lvCols.m_col_id] = id;
	row[m_lvCols.m_col_complaint] = complaint;
	row[m_lvCols.m_col_date] = date;
	msg = ustring::compose("Pacientes registados: %1, Visitas registadas: %2", m_modelPatients->children().size(), m_modelVisits->children().size());
	m_statusbar.push(msg);
}

void MainWindow::hlpr_append_allergy(const Allergy& allergy, const Glib::Date& date)
{
	RefPtr<ListStore> model = RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model());
	TreeIter iter = model->append();
	VisitsWindow::ListAllergies la;
	if(iter) {
		TreeModel::Row row = *iter;

		row[la.m_col_id] = allergy.m_id;
		row[la.m_col_name] = allergy.m_name;
		row[la.m_col_obs] = allergy.m_obs;
		//row[la.m_col_status] = LIST_STATUS_NORMAL;
	}
}



/* Signal Handlers */

bool MainWindow::on_delete_event(GdkEventAny * event)
{
	this->hide();
	store();
	return false;
}

void MainWindow::on_window_show(void)
{
	DBHandler db = DBHandler::get_instance();
	if(db.open()) {
		ustring msg;

		db.get_patients(NULL);
		msg = ustring::compose("Pacientes registados: %1", m_modelPatients->children().size());
		m_statusbar.push(msg);
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
	if(m_vw == NULL) {
		m_vw = new VisitsWindow(*this, m_personID);
		m_app->add_window((Window&)*m_vw->get_window());
	}

	// TODO: remove gargage
	//m_vw->clear();
	m_vw->set_window_type();
	m_vw->set_to_garbage();
	m_vw->setPersonID(m_personID);
	m_vw->set_sex_widgets(!(m_lblPSex->get_text().substr(0,1) == (ustring)"M"));
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
				MessageDialog msgbox(*this, "<b>Não é possível abrir a ficha do paciente</b>", false, MESSAGE_ERROR, BUTTONS_OK, true);
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
		iter = m_treeFilter->convert_iter_to_child_iter(iter);
		TreeModel::Row row = *iter;
		Person p(row[cols.m_col_id]);
		db.open();
		if(db.get_person(row[cols.m_col_id], p)) {
			db.close();
			m_pw->set_window_type(PatientWindow::PW_TYPE_EDIT);
			m_pw->set_person(p);
			m_pw->show();
		}
	} else {
		MessageDialog dlg(*this, "Não é possivel editar paciente.", false, MESSAGE_INFO, BUTTONS_OK, true);
		dlg.set_secondary_text("Deve selecionar um paciente antes de editar.");
		dlg.set_title("Editar paciente");
		dlg.run();
	}
}

void MainWindow::on_btnToolRemove_clicked()
{
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator iter = sel->get_selected();
	DBHandler db = DBHandler::get_instance();
	MessageDialog msg(*this, "Tem a certeza que pretende remover o paciente", false, MESSAGE_QUESTION, BUTTONS_YES_NO, true);

	msg.set_title("Remover paciente");
	msg.set_secondary_text("Se continuar com esta operação não poderá ver mais o histórico do paciente.");
	
	if(*iter) {
		if(msg.run() == RESPONSE_YES) {
			iter = m_treeFilter->convert_iter_to_child_iter(iter);

			db.open();
			if(db.person_remove((*iter)[m_lpCols.m_col_id])) {
				ustring msg;

				m_modelPatients->erase(iter);
				if(m_nb.get_current_page() == 0)
					msg = ustring::compose("Pacientes registados: %1", m_modelPatients->children().size());
				else 
					msg = ustring::compose("Pacientes registados: %1, Visitas registadas: %2", m_modelPatients->children().size(), m_modelVisits->children().size());
				m_statusbar.push(msg);
			}
			else {
				MessageDialog dlg(*this, "Não foi possível remover o paciente selecionado.", false, MESSAGE_ERROR, BUTTONS_OK, true);
				dlg.set_title("Remover paciente");
				dlg.run();
			}
			db.close();
		}
	} else {
		MessageDialog dlg(*this, "Não é possível eliminar um paciente.", false, MESSAGE_INFO, BUTTONS_OK, true);
		dlg.set_secondary_text("Deve selecionar um paciente antes de eliminar.");
		dlg.set_title("Remover paciente");
		dlg.run();
	}
}


void MainWindow::on_treePatients_activated(const TreeModel::Path& path, TreeViewColumn* col)
{
	Person p;
	Date today;
	guint16 age;
	char tmp[10];
	bool close= true;
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator row = sel->get_selected();
	DBHandler db = DBHandler::get_instance();
	gint n_rows;
	ustring msg;

	today.set_time_current();

	row = m_treeFilter->convert_iter_to_child_iter(row);
	if(*row) {
		try{
			db.open();
		} catch(SqlConnectionOpenedException& ex) {
			close = false;
		}

		m_modelVisits->clear();
		db.get_person((*row)[m_lpCols.m_col_id], p);
		db.get_visits((*row)[m_lvCols.m_col_id]);
		if(close)
			db.close();
		m_personID = p.get_id();
		m_lblPName->set_text(ustring("<b><i><span size=\"x-large\">" + p.get_name() + "</span></i></b>"));
		m_lblPName->set_use_markup();
		age = today.get_year() - p.get_birthday().get_year();

		if(today.get_month() < p.get_birthday().get_month() || (today.get_month() == p.get_birthday().get_month() && today.get_day() < p.get_birthday().get_day()))
			age--;

		sprintf(tmp, "%hu", age);
		m_lblPAge->set_text(tmp);
		m_lblPBloodtype->set_text(p.get_blood_type_string());
		sprintf(tmp, "%.2f cm", p.get_height());
		m_lblPHeight->set_text(tmp);
		
		if(p.get_sex()) {
			m_lblPSex->set_text("Masculino");
			m_lblMenstruationStr->hide();
			m_lblPregnancyStr->hide();
		} else {
			m_lblPSex->set_text("Feminino");
			m_lblMenstruationStr->show();
			m_lblPregnancyStr->show();
		}
		m_entryPatients.hide();
	
		m_boxSuggestions->show();
		m_boxVisitInfo->hide();
		m_boxSubVisits->hide();
		
		m_nb.set_current_page(1);
		msg = ustring::compose("Pacientes registados: %1, Visitas registadas: %2", m_modelPatients->children().size(), m_modelVisits->children().size());
		m_statusbar.push(msg);
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
	char tmp[12];

	today.set_time_current();

	m_lblPName->set_text(ustring("<b><span size=\"x-large\">" + p.get_name() + "</span></b>"));
	age = today.get_year() - p.get_birthday().get_year();

	if(today.get_month() < p.get_birthday().get_month() || (today.get_month() == p.get_birthday().get_month() && today.get_day() < p.get_birthday().get_day()))
		age--;

	sprintf(tmp, "%hu", age);
	m_lblPAge->set_text(tmp);
	m_lblPBloodtype->set_text(p.get_blood_type_string());
	sprintf(tmp, "%.2f cm", p.get_height());
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
	ustring nameLower = m_entryPatients.get_text().lowercase();
	if(m_entryPatientStatus || Regex::match_simple(".*" + nameLower + ".*", item.lowercase()))
		return true;
	else
		return false;
}

void MainWindow::on_btnBack_clicked(void)
{
	ustring msg;
	m_entryPatients.show();
	m_nb.set_current_page(0);
	msg = ustring::compose("Pacientes registados: %1", m_modelPatients->children().size());
	m_statusbar.push(msg);
}

void MainWindow::on_visitEdit_clicked(void)
{
	DBHandler db = DBHandler::get_instance();

	if(m_vw == NULL) {
		m_vw = new VisitsWindow(*this, m_personID);
		m_app->add_window((Window&)*m_vw->get_window());
	}
	m_vw->set_window_type(VisitsWindow::WindowType::WINDOW_TYPE_EDIT, m_visitID);
	m_vw->setPersonID(m_personID);
	m_vw->set_sex_widgets(!(m_lblPSex->get_text().substr(0,1) == (ustring)"M"));
	
	db.open();
	db.get_visit(m_visitID, *m_vw);
	db.close();

	m_vw->show();
}

void MainWindow::on_subvisitEdit_clicked(void)
{
	if(m_svw == NULL) {
		m_svw = new SubVisitWindow(*this);
		m_app->add_window((Window&)*m_svw->get_window());
	}
	m_svw->clean();
	m_svw->set_window_type(SubVisitWindow::WindowType::WINDOW_TYPE_EDIT, getSubVisitID());
	m_svw->setParentVisitID(getParentVisitID());
	m_svw->setPersonID(m_personID);

	DBHandler::get_instance().open();
	DBHandler::get_instance().get_subvisit(getSubVisitID(), *m_svw);
	DBHandler::get_instance().close();
	m_svw->set_sex_widgets(!(m_lblPSex->get_text().substr(0,1) == (ustring)"M"));
	m_svw->show();

}
void MainWindow::on_newSubVisit_clicked(void)
{
	if(m_svw == NULL) {
		m_svw = new SubVisitWindow(*this);
		m_app->add_window((Window&)*m_svw->get_window());
	}
	m_svw->set_window_type(SubVisitWindow::WindowType::WINDOW_TYPE_ADD);
	m_svw->clean();
	m_svw->setParentVisitID(m_visitID);
	m_svw->setPersonID(m_personID);
	m_svw->set_sex_widgets(!(m_lblPSex->get_text().substr(0, 1) == (ustring)"M"));
	m_svw->show();
}


void MainWindow::on_visitEdited(const VisitInterface& v)
{
	//RefPtr<TreeSelection> sel = m_treeVisits->get_selection();
	//TreeModel::iterator m_visitSelected = sel->get_selected();
	RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model())->clear();

	if(m_visitSelected) {
		TreeRow row =*m_visitSelected;

		row[m_lvCols.m_col_complaint] = v.getComplaint();
		row[m_lvCols.m_col_date] = v.getDate();

		*((VisitInterface*) this) = v;
	}
}

void MainWindow::on_subvisitEdited(const SubVisitInterface& v)
{
	RefPtr<TreeSelection> sel = m_treeVisits->get_selection();
	TreeModel::iterator iter = sel->get_selected();
	
	if(iter) {
		TreeRow row =*iter;

		row[m_lvCols.m_col_complaint] = v.getSubVisitFatigue();
		row[m_lvCols.m_col_date] = v.getSubVisitDate().format_string("%Y-%m-%d");

		*((SubVisitInterface*) this) = v;
	} else cout<<"HERE BITCHES"<<endl;
}

void MainWindow::get_visits_widgets(void)
{
	RefPtr<Builder> builder = Builder::create_from_file(GLADE_VISITS);
	TreeViewColumn *col;
	VisitsWindow::ListAllergies m_la;

	builder->get_widget("treeVisits", m_treeVisits);
	builder->get_widget("btnBack", m_btnBack);
	builder->get_widget("lblPName", m_lblPName);
	builder->get_widget("lblPBlood", m_lblPBloodtype);
	builder->get_widget("lblPHeight", m_lblPHeight);
	builder->get_widget("lblPSex", m_lblPSex);
	builder->get_widget("lblPAge", m_lblPAge);
	builder->get_widget("btnViewPatient", m_btnViewPatient);
	builder->get_widget("gridVisits", m_gridVisits);
	builder->get_widget("btnNewVisit", m_btnNewVisit);
	builder->get_widget("btnRemoveVisit", m_btnRemoveVisit);

	builder->get_widget("btnVisitEdit", m_btnVisitEdit);
	builder->get_widget("lblComplaint", m_lblComplaint);
	builder->get_widget("lblAnamnesis", m_lblAnamnesis);
	builder->get_widget("lblDate", m_lblDate);
	builder->get_widget("lblWeight", m_lblWeight);
	builder->get_widget("lblAppearance", m_lblAppearance);
	builder->get_widget("lblMovement", m_lblMovement);
	builder->get_widget("lblVoice", m_lblVoice);
	builder->get_widget("lblSmell", m_lblSmell);
	builder->get_widget("imgHypertension", m_imgHypertension);
	builder->get_widget("imgCholesterol", m_imgCholesterol);
	builder->get_widget("imgTriglycerides", m_imgTriglycerides);
	builder->get_widget("imgDiabetes", m_imgDiabetes);
	builder->get_widget("lblSleepiness", m_lblSleepiness);
	builder->get_widget("lblTranspiration", m_lblTranspiration);
	builder->get_widget("lblDehydration", m_lblDehydration);
	builder->get_widget("imgAnxiety", m_imgAnxiety);
	builder->get_widget("imgIrrt", m_imgIrrt);
	builder->get_widget("imgFrustration", m_imgFrustration);
	builder->get_widget("imgCry", m_imgCry);
	builder->get_widget("imgVerm", m_imgVerm);
	builder->get_widget("imgVed", m_imgVed);
	builder->get_widget("imgBra", m_imgBra);
	builder->get_widget("imgPrt", m_imgPrt);
	builder->get_widget("imgAml", m_imgAml);
	builder->get_widget("imgAlg", m_imgAlg);
	builder->get_widget("imgIrritable", m_imgIrritable);
	builder->get_widget("imgSad", m_imgSad);
	builder->get_widget("imgMed", m_imgMed);
	builder->get_widget("imgMelan", m_imgMelan);
	builder->get_widget("lblHearing", m_lblHearing);
	builder->get_widget("lblThroat", m_lblThroat);
	builder->get_widget("lblScent", m_lblScent);
	builder->get_widget("lblVision", m_lblVision);
	builder->get_widget("lblFatigue", m_lblFatigue);
	builder->get_widget("lblSexualActivity", m_lblSexualActivity);
	builder->get_widget("lblBody", m_lblBody);
	builder->get_widget("lblAbdomen", m_lblAbdomen);
	builder->get_widget("lblHead", m_lblHead);
	builder->get_widget("lblCirculation", m_lblCirculation);
	builder->get_widget("lblEatingHabits", m_lblEatingHabits);
	builder->get_widget("lblSuggestions", m_lblSuggestions);
	builder->get_widget("boxVisitInfo", m_boxVisitInfo);
	builder->get_widget("lblPain", m_lblPain);
	builder->get_widget("lblPainSince", m_lblPainSince);
	builder->get_widget("lblPainObs", m_lblPainObs);
	builder->get_widget("lblSurgery", m_lblSurgery);
	builder->get_widget("lblWeight2", m_lblWeight2);
	builder->get_widget("lblPreviousTreatment", m_lblPreviousTreatment);
	builder->get_widget("lblProstheses", m_lblProstheses);
	builder->get_widget("lblUrine", m_lblUrine);
	builder->get_widget("lblFaeces", m_lblFaeces);
	builder->get_widget("lblTongue", m_lblTongue);
	builder->get_widget("lblPulseD", m_lblPulseD);
	builder->get_widget("lblPulseE", m_lblPulseE);
	builder->get_widget("lblBloodPressure", m_lblBloodPressure);
	builder->get_widget("lblApal", m_lblApal);
	builder->get_widget("lblExams", m_lblExams);
	builder->get_widget("lblClinicalAnalysis", m_lblClinicalAnalysis);
	builder->get_widget("lblColor", m_lblColor);
	builder->get_widget("lblEscle", m_lblEscle);
	builder->get_widget("lblObservations", m_lblObservations);
	builder->get_widget("lblMed", m_lblMed);
	builder->get_widget("lblMedication", m_lblMedication);
	builder->get_widget("lblTreatment", m_lblTreatment);
	builder->get_widget("lblMenstruationStr", m_lblMenstruationStr);
	builder->get_widget("lblMenstruation", m_lblMenstruation);
	builder->get_widget("lblPregnancyStr", m_lblPregnancyStr);
	builder->get_widget("lblPregnancy", m_lblPregnancy);
	builder->get_widget("treeAllergies", m_treeAllergies);
	builder->get_widget("btnNewSubvisit", m_btnNewSubvisit);
	builder->get_widget("lblSubVisitDate", m_lblSubVisitDate);
	builder->get_widget("lblSubVisitSleepiness", m_lblSubVisitSleepiness);
	builder->get_widget("lblSubVisitFatigue", m_lblSubVisitFatigue);
	builder->get_widget("lblSubVisitHead", m_lblSubVisitHead);
	builder->get_widget("lblSubVisitTongue", m_lblSubVisitTongue);
	builder->get_widget("lblSubVisitUrine", m_lblSubVisitUrine);
	builder->get_widget("lblSubVisitFaeces", m_lblSubVisitFaeces);
	builder->get_widget("lblSubVisitMenstruationInfo", m_lblSubVisitMenstruationInfo);
	builder->get_widget("lblSubVisitMenstruation", m_lblSubVisitMenstruation);
	builder->get_widget("lblSubVisitPulseD", m_lblSubVisitPulseD);
	builder->get_widget("lblSubVisitPulseE", m_lblSubVisitPulseE);
	builder->get_widget("lblSubVisitApal", m_lblSubVisitApal);
	builder->get_widget("lblSubVisitBloodPressure", m_lblSubVisitBloodPressure);
	builder->get_widget("lblSubVisitObservations", m_lblSubVisitObservations);
	builder->get_widget("panedVisits", m_panedVisits);
	builder->get_widget("boxSubVisits", m_boxSubVisits);
	builder->get_widget("boxSuggestions", m_boxSuggestions);
	builder->get_widget("btnNewVisit2", m_btnNewVisit2);
	builder->get_widget("btnSubVisitEdit", m_btnSubVisitEdit);

	/* Tree allergies configuration */
	m_treeAllergies->set_model(ListStore::create(m_la));
	col = m_treeAllergies->get_column(m_treeAllergies->append_column_numeric("ID", m_la.m_col_id, "%u")-1);
	col->set_visible(false);
	col = m_treeAllergies->get_column(m_treeAllergies->append_column_editable("Nome", m_la.m_col_name)-1);
	col->set_resizable();
	col = m_treeAllergies->get_column(m_treeAllergies->append_column_editable("Observações", m_la.m_col_obs)-1);
	col->set_resizable();
	//col = m_treeAllergies->get_column(m_treeAllergies->append_column_numeric_editable("Status", m_la.m_col_status,"%u")-1);
	
}

void MainWindow::on_btnRemoveVisit(void)
{
	TreeModel::iterator row = m_treeVisits->get_selection()->get_selected();
	DBHandler db = DBHandler::get_instance();
	bool closed = true;
	MessageDialog msg(*this, "Tem a certeza que pretende remover a visita", false, MESSAGE_QUESTION, BUTTONS_YES_NO, true);

	msg.set_title("Remover visita");
	msg.set_secondary_text("Se continuar com esta operação perderá a informação relativa a esta visita.");
	if(msg.run() == RESPONSE_YES) {
		if(*row) {
			try {
				db.open();
				if(db.visit_remove((*row)[m_lvCols.m_col_id])) {
					gint n_rows;
					ustring msg;

					m_modelVisits->erase(row);
					n_rows = m_modelVisits->children().size();
					if(n_rows <= 0) {
						m_boxSuggestions->show();
						m_boxVisitInfo->hide();
					} else {
						m_boxSuggestions->hide();
						m_boxVisitInfo->show();
					}
					msg = ustring::compose("Pacientes registados: %1, Visitas registadas: %2", m_modelPatients->children().size(), n_rows);
					m_statusbar.push(msg);
				} else {
					MessageDialog dlg(*this, "Não foi possível remover a visita!", false, MESSAGE_ERROR, BUTTONS_OK, true);
					dlg.set_title("Remover visita");
					dlg.run();
				}
			} catch(SqlConnectionOpenedException& ex) {
				closed = false;
			}

			if(closed)
				db.close();
		} else {
			MessageDialog dlg(*this, "Deve selecionar um item para eliminar", false, MESSAGE_INFO, BUTTONS_OK, true);
			dlg.set_title("Remover visita");
			dlg.run();
		}
	}
}

void MainWindow::on_treeVisit_activated(const TreeModel::Path& path, TreeViewColumn* col)
{
	RefPtr<TreeSelection> sel = m_treeVisits->get_selection();
	//TreeModel::iterator m_visitSelected = sel->get_selected();
	m_visitSelected = sel->get_selected();
	DBHandler db = DBHandler::get_instance();
	bool close = true;
	RefPtr<ListStore>::cast_dynamic(m_treeAllergies->get_model())->clear();
	TreeIter iter;

	if(*m_visitSelected) {
		db.open();
		
		try{
			
			if(*(m_visitSelected->parent())) {
				iter = m_visitSelected;
				m_visitSelected = m_visitSelected->parent();
				m_visitID = (*m_visitSelected)[m_lvCols.m_col_id];
				db.get_visit(m_visitID, *this);
				db.get_subvisit((*iter)[m_lvCols.m_col_id], *this);
				m_boxSuggestions->hide();
				m_boxVisitInfo->show();
				m_boxSubVisits->show();
			} else {
				m_visitID = (*m_visitSelected)[m_lvCols.m_col_id];
				db.get_visit((*m_visitSelected)[m_lvCols.m_col_id], *this);
				db.get_person_allergies(m_personID, Util::parse_date(m_lblDate->get_text()), sigc::mem_fun(*this, &MainWindow::hlpr_append_allergy));	
				m_boxSuggestions->hide();
				m_boxVisitInfo->show();
				m_boxSubVisits->hide();
			}
		}
		catch(SqlConnectionOpenedException& ex) { close = false;}
		
		if(close)
			db.close();
	}
}

void MainWindow::on_visits_selection_changed(void)
{
	RefPtr<TreeSelection> sel = m_treePatients.get_selection();
	TreeModel::iterator row = sel->get_selected();

	if(*row) {
		m_boxSuggestions->show();
		m_boxVisitInfo->hide();
		m_boxSubVisits->hide();
	}
}

bool MainWindow::on_maximized_change(GdkEventWindowState *state)
{
	if(state->type == GDK_WINDOW_STATE && state->changed_mask == GDK_WINDOW_STATE_MAXIMIZED)
		m_maximized = !m_maximized;


	return false;
}


/***********************************
 *          Configuration          *
***********************************/


void MainWindow::get_window_size(gint& width, gint& height)
{
	get_size(width, height);
}
void MainWindow::get_window_position(gint& posx, gint& posy)
{
	get_position(posx, posy);
}
bool MainWindow::get_window_maximized()
{
	return m_maximized;
}
int MainWindow::get_visit_paned_position()
{
	return m_panedVisits->get_position();
}
void MainWindow::set_window_maximized(bool maximized)
{
	if(maximized)
		this->maximize();
	else
		this->unmaximize();
}
void MainWindow::set_window_resize(int width, int height) 
{
	resize(width, height);
}
void MainWindow::set_window_move(int posx, int posy) 
{
	move(posx, posy);
}
void MainWindow::set_visit_paned_position(int &val)
{
	m_panedVisits->set_position(val);
}


/***********************************
 *             Getters             *
***********************************/

guint32 MainWindow::getPersonID()
{
	return m_personID;
}
guint32 MainWindow::getPersonID() const
{
	return m_personID;
}
guint32 MainWindow::getVisitID() const
{
	return m_visitID;
}
ustring MainWindow::getComplaint() const
{
	return m_lblComplaint->get_text();
}
ustring MainWindow::getAnamnesis() const
{
	return m_lblAnamnesis->get_text();
}
ustring MainWindow::getDate() const
{
	return m_lblDate->get_text();
}
float MainWindow::getWeight() const
{
	float w;
	auto i = m_lblWeight->get_text().find_first_of(' ');
	stringstream ss;
	ss<<m_lblWeight->get_text().substr(0, i).raw();
	ss>>w;
	return w;
}

ustring MainWindow::getAppearance() const
{
	return m_lblAppearance->get_text();
}
ustring MainWindow::getMovement() const
{
	return m_lblMovement->get_text();
}
ustring MainWindow::getVoice() const
{
	return m_lblVoice->get_text();
}
ustring MainWindow::getSmell() const
{
	return m_lblSmell->get_text();
}

int MainWindow::getHypertension() const
{
}
int MainWindow::getCholesterol() const
{
}
int MainWindow::getTriglyceride() const
{
}
int MainWindow::getDiabetes() const
{
}

ustring MainWindow::getSleepiness() const
{
	return m_lblSleepiness->get_text();
}
ustring MainWindow::getTranspiration() const
{
	return m_lblTranspiration->get_text();
}
ustring MainWindow::getDehydration() const
{
	return m_lblDehydration->get_text();
}

int MainWindow::isAnxiety() const
{
}
int MainWindow::isIrrt() const
{
}
int MainWindow::isFrustration() const
{
}
int MainWindow::isCry() const
{
}
int MainWindow::isVerm() const
{
}
int MainWindow::isVed() const
{
}
int MainWindow::isBrad() const
{
}
int MainWindow::isPrt() const
{
}
int MainWindow::isAml() const
{
}
int MainWindow::isAlg() const
{
}
int MainWindow::isIrritable() const
{
}
int MainWindow::isSad() const
{
}
int MainWindow::isMed() const
{
}
int MainWindow::isMelan() const
{
}
ustring MainWindow::getHearing() const
{
	m_lblHearing->get_text();
}
ustring MainWindow::getThroat() const
{
	m_lblThroat->get_text();
}
ustring MainWindow::getScent() const
{
	m_lblScent->get_text();
}
ustring MainWindow::getVision() const
{
	m_lblVision->get_text();
}
ustring MainWindow::getFatigue() const
{
	m_lblFatigue->get_text();
}
ustring MainWindow::getSexualActivity() const
{
	m_lblSexualActivity->get_text();
}
ustring MainWindow::getBody() const
{
	m_lblBody->get_text();
}
ustring MainWindow::getAbdomen() const
{
	m_lblAbdomen->get_text();
}
ustring MainWindow::getHead() const
{
	m_lblHead->get_text();
}
ustring MainWindow::getCirculation() const
{
	m_lblCirculation->get_text();
}
ustring  MainWindow::getEatingHabits() const
{
	m_lblEatingHabits->get_text();
}

ustring MainWindow::getMenstruation() const
{
	m_lblMenstruation->get_text();
}
ustring MainWindow::getPregnancy() const
{
	m_lblPregnancy->get_text();
}
ustring MainWindow::getPain() const
{
	m_lblPain->get_text();
}
ustring MainWindow::getPainSince() const
{
	m_lblPainSince->get_text();
}
ustring MainWindow::getPainObs() const
{
	m_lblPainObs->get_text();
}
ustring MainWindow::getSurgery() const
{
	m_lblSurgery->get_text();
}
ustring MainWindow::getPreviousTreatment() const
{
	m_lblPreviousTreatment->get_text();
}
bool MainWindow::getProstheses() const
{
}
bool MainWindow::getWeightBool() const
{
}
ustring MainWindow::getUrine() const
{
	m_lblUrine->get_text();
}
ustring MainWindow::getFaeces() const
{
	m_lblFaeces->get_text();
}
ustring MainWindow::getTongue() const
{
	m_lblTongue->get_text();
}
ustring MainWindow::getPulseD() const
{
	m_lblPulseD->get_text();
}
ustring MainWindow::getPulseE() const
{
	m_lblPulseE->get_text();
}
gint16 MainWindow::getBPMax() const
{
	guint16 val;
	string tmp = m_lblBloodPressure->get_text();
	stringstream ss;
	auto i = tmp.find_first_of('/');
	ss<<tmp.substr(0, i-1);
	ss>>val;
	return val;
}
gint16 MainWindow::getBPMin() const
{
	guint16 val;
	string tmp = m_lblBloodPressure->get_text();
	stringstream ss;
	auto i = tmp.find_first_of('/');
	auto i2 = tmp.find_first_of("PPM");
	ss<<tmp.substr(i+1, i2-1);
	ss>>val;
	return val;
}
gint16 MainWindow::getBPM() const
{
	guint16 val;
	string tmp = m_lblBloodPressure->get_text();
	stringstream ss;
	auto i = tmp.find_first_of("PPM");
	ss<<tmp.substr(i+6);
	ss>>val;
	return val;
}
ustring MainWindow::getApal() const
{
	m_lblApal->get_text();
}
ustring MainWindow::getExams() const
{
	m_lblExams->get_text();
}
ustring MainWindow::getClinicalAnalysis() const
{
	m_lblClinicalAnalysis->get_text();
}
ustring MainWindow::getColor() const
{
	m_lblColor->get_text();
}
ustring MainWindow::getEscle() const
{
	m_lblEscle->get_text();
}
ustring MainWindow::getObservations() const
{
	m_lblObservations->get_text();
}
ustring MainWindow::getMed() const
{
	m_lblMed->get_text();
}
ustring MainWindow::getMedication() const
{
	m_lblMedication->get_text();
}
ustring MainWindow::getTreatment() const
{
	m_lblTreatment->get_text();
}
TreeModel::Children MainWindow::getAllergies() const
{
}


/***********************************
 *            SubVisits            *
***********************************/
guint32 MainWindow::getSubVisitID()
{
	TreeIter iter = m_treeVisits->get_selection()->get_selected();
	if(iter){
		return (*iter)[m_lvCols.m_col_id];
	}
}
guint32 MainWindow::getSubVisitID() const
{
	TreeIter iter = m_treeVisits->get_selection()->get_selected();
	if(iter) {
		return (*iter)[m_lvCols.m_col_id];
	}
}
guint32 MainWindow::getParentVisitID()
{
	TreeIter iter = m_treeVisits->get_selection()->get_selected();
	if(iter && m_modelVisits->iter_is_valid(iter->parent())){
		return (*iter->parent())[m_lvCols.m_col_id];
	}
}
guint32 MainWindow::getParentVisitID() const
{
	TreeIter iter = m_treeVisits->get_selection()->get_selected();
	if(iter && m_modelVisits->iter_is_valid(iter->parent())){
		return (*iter->parent())[m_lvCols.m_col_id];
	}
}
const Glib::Date MainWindow::getSubVisitDate()
{
	return Util::parse_date(m_lblSubVisitDate->get_text());
}
const Glib::Date MainWindow::getSubVisitDate() const
{
	return Util::parse_date(m_lblSubVisitDate->get_text());
}
const Glib::ustring MainWindow::getSubVisitSleepiness()
{
	return m_lblSubVisitSleepiness->get_text();
}
const Glib::ustring MainWindow::getSubVisitSleepiness() const
{
	return m_lblSubVisitSleepiness->get_text();
}
const Glib::ustring MainWindow::getSubVisitFatigue()
{
	return m_lblSubVisitFatigue->get_text();
}
const Glib::ustring MainWindow::getSubVisitFatigue() const
{
	return m_lblSubVisitFatigue->get_text();
}
const Glib::ustring MainWindow::getSubVisitHead()
{
	return m_lblSubVisitHead->get_text();
}
const Glib::ustring MainWindow::getSubVisitHead() const
{
	return m_lblSubVisitHead->get_text();
}
const Glib::ustring MainWindow::getSubVisitTongue()
{
	return m_lblSubVisitTongue->get_text();
}
const Glib::ustring MainWindow::getSubVisitTongue() const
{
	return m_lblSubVisitTongue->get_text();
}
const Glib::ustring MainWindow::getSubVisitUrine()
{
	return m_lblSubVisitUrine->get_text();
}
const Glib::ustring MainWindow::getSubVisitUrine() const
{
	return m_lblSubVisitUrine->get_text();
}
const Glib::ustring MainWindow::getSubVisitFaeces()
{
	return m_lblSubVisitFaeces->get_text();
}
const Glib::ustring MainWindow::getSubVisitFaeces() const
{
	return m_lblSubVisitFaeces->get_text();
}
const Glib::ustring MainWindow::getSubVisitMenstruation()
{
	return m_lblSubVisitMenstruation->get_text();
}
const Glib::ustring MainWindow::getSubVisitMenstruation() const
{
	return m_lblSubVisitMenstruation->get_text();
}
const Glib::ustring MainWindow::getSubVisitPulseD()
{
	return m_lblSubVisitPulseD->get_text();
}
const Glib::ustring MainWindow::getSubVisitPulseD() const
{
	return m_lblSubVisitPulseD->get_text();
}
const Glib::ustring MainWindow::getSubVisitPulseE()
{
	return m_lblSubVisitPulseE->get_text();
}
const Glib::ustring MainWindow::getSubVisitPulseE() const
{
	return m_lblSubVisitPulseE->get_text();
}
const Glib::ustring MainWindow::getSubVisitApal()
{
	return m_lblSubVisitApal->get_text();
}
const Glib::ustring MainWindow::getSubVisitApal() const
{
	return m_lblSubVisitApal->get_text();
}
const Glib::ustring MainWindow::getSubVisitObservations()
{
	return m_lblSubVisitObservations->get_text();
}
const Glib::ustring MainWindow::getSubVisitObservations() const
{
	return m_lblSubVisitObservations->get_text();
}
const void MainWindow::getSubVisitBloodPressure(guint16& max, guint16& min, guint16& bpm)
{
	guint16 val;
	string tmp = m_lblSubVisitBloodPressure->get_text();
	stringstream ss;
	auto i = tmp.find_first_of('/');
	auto i2 = tmp.find_first_of("PPM");
	
	ss<<tmp.substr(0, i-1);
	ss>>max;
	i = tmp.find_first_of('/');
	ss<<tmp.substr(i + 1, i2 - 1);
	ss>>min;
	i = tmp.find_first_of("PPM");
	ss<<tmp.substr(i+6);
	ss>>bpm;
}
const void MainWindow::getSubVisitBloodPressure(guint16& max, guint16& min, guint16& bpm) const
{
	guint16 val;
	string tmp = m_lblSubVisitBloodPressure->get_text();
	stringstream ss;
	auto i = tmp.find_first_of('/');
	auto i2 = tmp.find_first_of("PPM");
	
	ss<<tmp.substr(0, i-1);
	ss>>max;
	i = tmp.find_first_of('/');
	ss<<tmp.substr(i + 1, i2 - 1);
	ss>>min;
	i = tmp.find_first_of("PPM");
	ss<<tmp.substr(i+6);
	ss>>bpm;
}

/***********************************
 *             Setters             *
***********************************/
void MainWindow::setPersonID(guint32 val)
{
}
void MainWindow::setVisitID(const guint32 val)
{
}
void MainWindow::setComplaint(const Glib::ustring& val)
{
	m_lblComplaint->set_text(val);
}
void MainWindow::setAnamnesis(const Glib::ustring& val)
{
	m_lblAnamnesis->set_text(val);
}
void MainWindow::setDate(const Glib::ustring& val)
{
	m_lblDate->set_text(val);
}
void MainWindow::setWeight(float val)
{
	m_lblWeight->set_text(ustring::compose("%1 kg", val));
}
void MainWindow::setAppearance(const Glib::ustring& val)
{
	m_lblAppearance->set_text(val);
}
void MainWindow::setMovement(const Glib::ustring& val)
{
	m_lblMovement->set_text(val);
}
void MainWindow::setVoice(const Glib::ustring& val)
{
	m_lblVoice->set_text(val);
}
void MainWindow::setSmell(const Glib::ustring& val)
{
	m_lblSmell->set_text(val);
}
void MainWindow::setHypertension(int val)
{
	ustring iconStr;

	switch(val) {
		case 0:
			iconStr = "list-remove";
			break;
		case 1:
			iconStr = "go-up";
			break;
		default:
			iconStr = "go-down";
			break;
	}
	m_imgHypertension->clear();
	m_imgHypertension->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setCholesterol(int val)
{
	ustring iconStr;

	switch(val) {
		case 0:
			iconStr = "list-remove";
			break;
		case 1:
			iconStr = "go-up";
			break;
		default:
			iconStr = "go-down";
			break;
	}
	m_imgCholesterol->clear();
	m_imgCholesterol->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setTriglyceride(int val)
{
	ustring iconStr;

	switch(val) {
		case 0:
			iconStr = "list-remove";
			break;
		case 1:
			iconStr = "go-up";
			break;
		default:
			iconStr = "go-down";
			break;
	}
	m_imgTriglycerides->clear();
	m_imgTriglycerides->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setDiabetes(int val)
{
	ustring iconStr;

	switch(val) {
		case 0:
			iconStr = "list-remove";
			break;
		case 1:
			iconStr = "go-up";
			break;
		default:
			iconStr = "go-down";
			break;
	}
	m_imgDiabetes->clear();
	m_imgDiabetes->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setSleepiness(const Glib::ustring& val)
{
	m_lblSleepiness->set_text(val);
}
void MainWindow::setTranspiration(const Glib::ustring& val)
{
	m_lblTranspiration->set_text(val);
}
void MainWindow::setDehydration(const Glib::ustring& val)
{
	m_lblDehydration->set_text(val);
}
void MainWindow::setAnxiety(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgAnxiety->clear();
	m_imgAnxiety->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setIrrt(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgIrrt->clear();
	m_imgIrrt->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setFrustration(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgFrustration->clear();
	m_imgFrustration->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setCry(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgCry->clear();
	m_imgCry->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setVerm(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgVerm->clear();
	m_imgVerm->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setVed(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgVed->clear();
	m_imgVed->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setBrad(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgBra->clear();
	m_imgBra->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setPrt(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgPrt->clear();
	m_imgPrt->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setAml(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgAml->clear();
	m_imgAml->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setAlg(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgAlg->clear();
	m_imgAlg->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setIrritable(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgIrritable->clear();
	m_imgIrritable->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setSad(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgSad->clear();
	m_imgSad->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setMed(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgMed->clear();
	m_imgMed->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setMelan(int val)
{
	ustring iconStr;

	switch(val)
	{
		case 1:
			iconStr = "emblem-default";
			break;
		default:
			iconStr = "list-remove";
			break;
	}
	m_imgMelan->clear();
	m_imgMelan->set_from_icon_name(iconStr, ICON_SIZE_BUTTON);
}
void MainWindow::setHearing(const Glib::ustring& val)
{
	m_lblHearing->set_text(val);
}
void MainWindow::setThroat(const Glib::ustring& val)
{
	m_lblThroat->set_text(val);
}
void MainWindow::setScent(const Glib::ustring& val)
{
	m_lblScent->set_text(val);
}
void MainWindow::setVision(const Glib::ustring& val)
{
	m_lblVision->set_text(val);
}
void MainWindow::setFatigue(const Glib::ustring& val)
{
	m_lblFatigue->set_text(val);
}
void MainWindow::setSexualActivity(const Glib::ustring& val)
{
	m_lblSexualActivity->set_text(val);
}
void MainWindow::setBody(const Glib::ustring& val)
{
	m_lblBody->set_text(val);
}
void MainWindow::setAbdomen(const Glib::ustring& val)
{
	m_lblAbdomen->set_text(val);
}
void MainWindow::setHead(const Glib::ustring& val)
{
	m_lblHead->set_text(val);
}
void MainWindow::setCirculation(const Glib::ustring& val)
{
	m_lblCirculation->set_text(val);
}
void MainWindow::setEatingHabits(const Glib::ustring& val)
{
	m_lblEatingHabits->set_text(val);
}
void MainWindow::setMenstruation(const Glib::ustring& val)
{
	m_lblMenstruation->set_text(val);
}
void MainWindow::setPregnancy(const Glib::ustring& val)
{
	m_lblPregnancy->set_text(val);
}
void MainWindow::setPain(const Glib::ustring& val)
{
	m_lblPain->set_text(val);
}
void MainWindow::setPainSince(const Glib::ustring& val)
{
	m_lblPainSince->set_text(val);
}
void MainWindow::setPainObs(const Glib::ustring& val)
{
	m_lblPainObs->set_text(val);
}
void MainWindow::setSurgery(const Glib::ustring& val)
{
	m_lblSurgery->set_text(val);
}
void MainWindow::setPreviousTreatment(const Glib::ustring& val)
{
	m_lblPreviousTreatment->set_text(val);
}
void MainWindow::setProstheses(bool val)
{
	if(val)
		m_lblProstheses->set_text("Sim");
	else
		m_lblProstheses->set_text("Não");
}
void MainWindow::setWeightBool(bool val)
{
	if(val)
		m_lblWeight2->set_text("Sim");
	else
		m_lblWeight2->set_text("Não");
}
void MainWindow::setUrine(const Glib::ustring& val)
{
	m_lblUrine->set_text(val);
}
void MainWindow::setFaeces(const Glib::ustring& val)
{
	m_lblFaeces->set_text(val);
}
void MainWindow::setTongue(const Glib::ustring& val)
{
	m_lblTongue->set_text(val);
}
void MainWindow::setPulseD(const Glib::ustring& val)
{
	m_lblPulseD->set_text(val);
}
void MainWindow::setPulseE(const Glib::ustring& val)
{
	m_lblPulseE->set_text(val);
}
void MainWindow::setBloodPressure(gint16 high, gint16 low, gint16 bpm)
{
	m_lblBloodPressure->set_text(ustring::compose(ustring("%1 <b>/</b> %2    <b>PPM: </b>%3"),high, low, bpm));
	m_lblBloodPressure->set_use_markup();
}
void MainWindow::setApal(const Glib::ustring& val)
{
	m_lblApal->set_text(val);
}
void MainWindow::setExams(const Glib::ustring& val)
{
	m_lblExams->set_text(val);
}
void MainWindow::setClinicalAnalysis(const Glib::ustring& val)
{
	m_lblClinicalAnalysis->set_text(val);
}
void MainWindow::setColor(const Glib::ustring& val)
{
	m_lblColor->set_text(val);
}
void MainWindow::setEscle(const Glib::ustring& val)
{
	m_lblEscle->set_text(val);
}
void MainWindow::setObservations(const Glib::ustring& val)
{
	m_lblObservations->set_text(val);
}
void MainWindow::setMed(const Glib::ustring& val)
{
	m_lblMed->set_text(val);
}
void MainWindow::setMedication(const Glib::ustring& val)
{
	m_lblMedication->set_text(val);
}
void MainWindow::setTreatment(const Glib::ustring& val)
{
	m_lblTreatment->set_text(val);
}

/*void setSubVisitPersonID(guint32 val)
{
}*/
void MainWindow::setParentVisitID(guint32 val)
{
}
void MainWindow::setSubVisitID(guint32 val)
{
}
void MainWindow::setSubVisitDate(const Glib::Date& val)
{
	m_lblSubVisitDate->set_text(val.format_string("%Y-%m-%d"));
}
void MainWindow::setSubVisitSleepiness(const Glib::ustring& val)
{
	m_lblSubVisitSleepiness->set_text(val);
}
void MainWindow::setSubVisitFatigue(const Glib::ustring& val)
{
	m_lblSubVisitFatigue->set_text(val);
}
void MainWindow::setSubVisitHead(const Glib::ustring& val)
{
	m_lblSubVisitHead->set_text(val);
}
void MainWindow::setSubVisitTongue(const Glib::ustring& val)
{
	m_lblSubVisitTongue->set_text(val);
}
void MainWindow::setSubVisitUrine(const Glib::ustring& val)
{
	m_lblSubVisitUrine->set_text(val);
}
void MainWindow::setSubVisitFaeces(const Glib::ustring& val)
{
	m_lblSubVisitFaeces->set_text(val);
}
void MainWindow::setSubVisitMenstruation(const Glib::ustring& val)
{
	m_lblSubVisitMenstruation->set_text(val);
}
void MainWindow::setSubVisitPulseD(const Glib::ustring& val)
{
	m_lblSubVisitPulseD->set_text(val);
}
void MainWindow::setSubVisitPulseE(const Glib::ustring& val)
{
	m_lblSubVisitPulseE->set_text(val);
}
void MainWindow::setSubVisitApal(const Glib::ustring& val)
{
	m_lblSubVisitApal->set_text(val);
}
void MainWindow::setSubVisitObservations(const Glib::ustring& val)
{
	m_lblSubVisitObservations->set_text(val);
}
void MainWindow::setSubVisitBloodPressure(guint16 max, guint16 min, guint16 bpm)
{
	m_lblSubVisitBloodPressure->set_text(ustring::compose(ustring("%1 <b>/</b> %2    <b>PPM: </b>%3"), max, min, bpm));
	m_lblSubVisitBloodPressure->set_use_markup();
}