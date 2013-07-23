/*
 * main-window.h
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <gtkmm.h>

#include "patient-window.h"
#include "view-patient.h"
#include "../person.h"
#include "../db/dbhandler.h"
#include "visitswindow.h"
#include "../configuration.h"

#define GLADE_VISITS "src/ui/main-visit.glade"

class MainWindow : public Gtk::Window, public VisitInterface, public Configuration
{
protected:
	class ListPatientsCols : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListPatientsCols()
		{
			add(m_col_id);
			add(m_col_name);
			add(m_col_nif);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<int> m_col_nif;
	};
	
	class ListVisitsCols : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListVisitsCols()
		{
			add(m_col_id);
			add(m_col_complaint);
			add(m_col_date);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_complaint;
		Gtk::TreeModelColumn<Glib::ustring> m_col_date;
	};

	Glib::RefPtr<Gtk::UIManager> m_uiman;
	Glib::RefPtr<Gtk::ActionGroup> m_actionGroup;

	Gtk::Box *m_boxVisitInfo;
	Gtk::Label *m_lblSuggestions;

	Gtk::Toolbar m_mainToolbar;
	Gtk::ToolButton m_mtbAdd;
	Gtk::ToolButton m_mtbEdit;
	Gtk::ToolButton m_mtbRemove;
	Gtk::ToolItem m_mtbExpander;
	Gtk::ToolItem m_mtbEntrySearch;
	
	Gtk::Notebook m_nb;
	
	Gtk::Label m_lblPatients;
	Gtk::Entry m_entryPatients;
	Gtk::TreeView m_treePatients;

	Gtk::Toolbar m_tbVisits;

	ListPatientsCols m_lpCols;
	ListVisitsCols m_lvCols;

	// Visit widgets
	Gtk::TreeView *m_treeVisits;
	Gtk::Button *m_btnBack;

	// Selected patient information
	Gtk::Label *m_lblPName;
	Gtk::Label *m_lblPBloodtype;
	Gtk::Label *m_lblPHeight;
	Gtk::Label *m_lblPSex;
	Gtk::Label *m_lblPAge;

	Gtk::Button *m_btnViewPatient;
	Gtk::Button *m_btnRemoveVisit;
	Gtk::Button *m_btnNewVisit;
	Gtk::Grid *m_gridVisits;

	Glib::RefPtr<Gtk::TreeModelFilter> m_treeFilter;
	Glib::RefPtr<Gtk::ListStore> m_modelPatients;
	Glib::RefPtr<Gtk::ListStore> m_modelVisits;


	Gtk::Label *m_lblComplaint;
	Gtk::Label *m_lblAnamnesis;
	Gtk::Label *m_lblDate;
	Gtk::Label *m_lblWeight;
	Gtk::Label *m_lblAppearance;
	Gtk::Label *m_lblMovement;
	Gtk::Label *m_lblVoice;
	Gtk::Label *m_lblSmell;
	Gtk::Image *m_imgHypertension;
	Gtk::Image *m_imgCholesterol;
	Gtk::Image *m_imgTriglycerides;
	Gtk::Image *m_imgDiabetes;
	Gtk::Label *m_lblSleepiness;
	Gtk::Label *m_lblTranspiration;
	Gtk::Label *m_lblDehydration;
	Gtk::Image *m_imgAnxiety;
	Gtk::Image *m_imgIrrt;
	Gtk::Image *m_imgFrustration;
	Gtk::Image *m_imgCry;
	Gtk::Image *m_imgVerm;
	Gtk::Image *m_imgVed;
	Gtk::Image *m_imgBra;
	Gtk::Image *m_imgPrt;
	Gtk::Image *m_imgAml;
	Gtk::Image *m_imgAlg;
	Gtk::Image *m_imgIrritable;
	Gtk::Image *m_imgSad;
	Gtk::Image *m_imgMed;
	Gtk::Image *m_imgMelan;
	Gtk::Label *m_lblHearing;
	Gtk::Label *m_lblThroat;
	Gtk::Label *m_lblScent;
	Gtk::Label *m_lblVision;
	Gtk::Label *m_lblFatigue;
	Gtk::Label *m_lblSexualActivity;
	Gtk::Label *m_lblBody;
	Gtk::Label *m_lblAbdomen;
	Gtk::Label *m_lblHead;
	Gtk::Label *m_lblCirculation;
	Gtk::Label *m_lblEatingHabits;
	
	Gtk::Label *m_lblPain;
	Gtk::Label *m_lblPainSince;
	Gtk::Label *m_lblPainObs;
	Gtk::Label *m_lblSurgery;
	Gtk::Label *m_lblProstheses;
	Gtk::Label *m_lblWeight2;
	Gtk::Label *m_lblPreviousTreatment;

	Gtk::Label *m_lblMenstruationStr;
	Gtk::Label *m_lblMenstruation;
	Gtk::Label *m_lblPregnancyStr;
	Gtk::Label *m_lblPregnancy;

	Gtk::Label *m_lblUrine;
	Gtk::Label *m_lblFaeces;
	Gtk::Label *m_lblTongue;
	Gtk::Label *m_lblPulseD;
	Gtk::Label *m_lblPulseE;
	Gtk::Label *m_lblBloodPressure;
	Gtk::Label *m_lblApal;
	Gtk::Label *m_lblExams;
	Gtk::Label *m_lblClinicalAnalysis;
	Gtk::Label *m_lblColor;
	Gtk::Label *m_lblEscle;
	Gtk::Label *m_lblObservations;
	Gtk::Label *m_lblMed;
	Gtk::Label *m_lblMedication;
	Gtk::Label *m_lblTreatment;

	//Glib::SignalTimeout m_searchTimeout;
	sigc::connection m_connSearch;
	Glib::Timer m_timerSearch;

	bool m_entryPatientStatus;
	bool m_fireSearch;
	Glib::RefPtr<Gtk::Application> m_app;

	ViewPatientWindow *m_vp;
	PatientWindow *m_pw;
	VisitsWindow *m_vw;
	
	bool m_maximized;
public:

	MainWindow(const Glib::ustring& title, Glib::RefPtr<Gtk::Application>& app);
	virtual ~MainWindow();
	/* Helper Functions */
	void hlpr_append_patient(guint32 id, const Glib::ustring& name, guint32 nif);
	void hlpr_append_visit(guint32 id, const Glib::ustring& complaint, const Glib::ustring& date);

protected:
	void get_visits_widgets(void);

	/* Signal Handlers */
	virtual bool on_delete_event(GdkEventAny *);

	void on_btnBack_clicked(void);
	void on_btnShPatient_clicked(void);
	void on_btnToolAdd_clicked(void);
	void on_btnToolAddVisit_clicked(void);
	void on_btnToolEdit_clicked(void);
	void on_btnToolRemove_clicked(void);
	void on_window_show(void);
	void on_treeVisit_activated(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);

	void on_btnRemoveVisit(void);

	bool on_entryPatient_focusIn(GdkEventFocus *focus); 
	bool on_entryPatient_focusOut(GdkEventFocus *focus);

	void on_treePatients_activated(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
	//void on_treePatients_selected();
	void on_txtSearch_changed();
	void on_db_person_edited(const Person& p);
	void on_mhAbout_activate(void);
	bool handler_timeout_search();

	void patient_window_add(PatientWindow &);

	bool filter_patient_by_name(const Gtk::TreeModel::const_iterator&);

	void on_visits_selection_changed(void);
	bool on_maximized_change(GdkEventWindowState *state);
public:

/***********************************
 *          Configuration          *
***********************************/
 	virtual void get_window_size(gint& width, gint& height);
	virtual void get_window_position(gint& posx, gint& posy);
	virtual bool get_window_maximized();

	virtual void set_window_maximized(bool maximized = true);
	virtual void set_window_resize(int width, int height);
	virtual void set_window_move(int posx, int posy);

/*****************************************
 *           Interface methods           *
 ****************************************/
	virtual int getPersonID();
	virtual Glib::ustring getComplaint();
	virtual Glib::ustring getAnamnesis();
	virtual Glib::ustring getDate();
	virtual float getWeight();
	virtual Glib::ustring getAppearance();
	virtual Glib::ustring getMovement();
	virtual Glib::ustring getVoice();
	virtual Glib::ustring getSmell();
	virtual int getHypertension();
	virtual int getCholesterol();
	virtual int getTriglyceride();
	virtual int getDiabetes();
	virtual Glib::ustring getSleepiness();
	virtual Glib::ustring getTranspiration();
	virtual Glib::ustring getDehydration();
	virtual int isAnxiety();
	virtual int isIrrt();
	virtual int isFrustration();
	virtual int isCry();
	virtual int isVerm();
	virtual int isVed();
	virtual int isBrad();
	virtual int isPrt();
	virtual int isAml();
	virtual int isAlg();
	virtual int isIrritable();
	virtual int isSad();
	virtual int isMed();
	virtual int isMelan();
	virtual Glib::ustring getHearing();
	virtual Glib::ustring getThroat();
	virtual Glib::ustring getScent();
	virtual Glib::ustring getVision();
	virtual Glib::ustring getFatigue();
	virtual Glib::ustring getSexualActivity();
	virtual Glib::ustring getBody();
	virtual Glib::ustring getAbdomen();
	virtual Glib::ustring getHead();
	virtual Glib::ustring getCirculation();
	virtual Glib::ustring getEatingHabits();

	virtual Glib::ustring getMenstruation();
	virtual Glib::ustring getPregnancy();
	virtual Glib::ustring getPain();
	virtual Glib::ustring getPainSince();
	virtual Glib::ustring getPainObs();
	virtual Glib::ustring getSurgery();
	virtual Glib::ustring getPreviousTreatment();
	virtual bool getProstheses();
	virtual bool getWeightBool();
	virtual Glib::ustring getUrine();
	virtual Glib::ustring getFaeces();
	virtual Glib::ustring getTongue();
	virtual Glib::ustring getPulseD();
	virtual Glib::ustring getPulseE();
	virtual gint16 getBPMax();
	virtual gint16 getBPMin();
	virtual gint16 getBPM();
	virtual Glib::ustring getApal();
	virtual Glib::ustring getExams();
	virtual Glib::ustring getClinicalAnalysis();
	virtual Glib::ustring getColor();
	virtual Glib::ustring getEscle();
	virtual Glib::ustring getObservations();
	virtual Glib::ustring getMed();
	virtual Glib::ustring getMedication();
	virtual Glib::ustring getTreatment();

	virtual void setPersonID(int val);
	virtual void setComplaint(const Glib::ustring& val);
	virtual void setAnamnesis(const Glib::ustring& val);
	virtual void setDate(const Glib::ustring& val);
	virtual void setWeight(float val);
	virtual void setAppearance(const Glib::ustring& val);
	virtual void setMovement(const Glib::ustring& val);
	virtual void setVoice(const Glib::ustring& val);
	virtual void setSmell(const Glib::ustring& val);
	virtual void setHypertension(int val);
	virtual void setCholesterol(int val);
	virtual void setTriglyceride(int val);
	virtual void setDiabetes(int val);
	virtual void setSleepiness(const Glib::ustring& val);
	virtual void setTranspiration(const Glib::ustring& val);
	virtual void setDehydration(const Glib::ustring& val);
	virtual void setAnxiety(int val);
	virtual void setIrrt(int val);
	virtual void setFrustration(int val);
	virtual void setCry(int val);
	virtual void setVerm(int val);
	virtual void setVed(int val);
	virtual void setBrad(int val);
	virtual void setPrt(int val);
	virtual void setAml(int val);
	virtual void setAlg(int val);
	virtual void setIrritable(int val);
	virtual void setSad(int val);
	virtual void setMed(int val);
	virtual void setMelan(int val);
	virtual void setHearing(const Glib::ustring& val);
	virtual void setThroat(const Glib::ustring& val);
	virtual void setScent(const Glib::ustring& val);
	virtual void setVision(const Glib::ustring& val);
	virtual void setFatigue(const Glib::ustring& val);
	virtual void setSexualActivity(const Glib::ustring& val);
	virtual void setBody(const Glib::ustring& val);
	virtual void setAbdomen(const Glib::ustring& val);
	virtual void setHead(const Glib::ustring& val);
	virtual void setCirculation(const Glib::ustring& val);
	virtual void setEatingHabits(const Glib::ustring& val);

	virtual void setMenstruation(const Glib::ustring& val);
	virtual void setPregnancy(const Glib::ustring& val);
	virtual void setPain(const Glib::ustring& val);
	virtual void setPainSince(const Glib::ustring& val);
	virtual void setPainObs(const Glib::ustring& val);
	virtual void setSurgery(const Glib::ustring& val);
	virtual void setPreviousTreatment(const Glib::ustring& val);
	virtual void setProstheses(bool val);
	virtual void setWeightBool(bool val);
	virtual void setUrine(const Glib::ustring& val);
	virtual void setFaeces(const Glib::ustring& val);
	virtual void setTongue(const Glib::ustring& val);
	virtual void setPulseD(const Glib::ustring& val);
	virtual void setPulseE(const Glib::ustring& val);
	virtual void setBloodPressure(gint16 high, gint16 low, gint16 bpm);
	virtual void setApal(const Glib::ustring& val);
	virtual void setExams(const Glib::ustring& val);
	virtual void setClinicalAnalysis(const Glib::ustring& val);
	virtual void setColor(const Glib::ustring& val);
	virtual void setEscle(const Glib::ustring& val);
	virtual void setObservations(const Glib::ustring& val);
	virtual void setMed(const Glib::ustring& val);
	virtual void setMedication(const Glib::ustring& val);
	virtual void setTreatment(const Glib::ustring& val);
};

#endif
