/*
 * main-window.h
 *
 *  Created on: Aug 24, 2012
 *      Author: ilan
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treeselection.h>
#include <gtkmm/treeiter.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/toolitem.h>
#include <gtkmm/window.h>
#include <gtkmm/notebook.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/table.h>
#include <gtkmm/scrolledwindow.h> 
#include <gtkmm/aboutdialog.h>
#include <gtkmm/main.h>
#include <gtkmm/separatortoolitem.h>
#include <gtkmm/image.h>


#include "../person.h"
#include "../configuration.h"
#include "../db/dbhandler.h"
#include "patient-window.h"
#include "view-patient.h"
#include "subvisit-window.h"
#include "visitswindow.h"

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
	Gtk::Button *m_btnNewSubvisit;
	Gtk::Grid *m_gridVisits;

	Glib::RefPtr<Gtk::TreeModelFilter> m_treeFilter;
	Glib::RefPtr<Gtk::ListStore> m_modelPatients;
	Glib::RefPtr<Gtk::ListStore> m_modelVisits;

	Gtk::Button *m_btnVisitEdit;
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

	Gtk::TreeView *m_treeAllergies;
	
	Gtk::TreeModel::iterator m_visitSelected;

	//Glib::SignalTimeout m_searchTimeout;
	sigc::connection m_connSearch;
	Glib::Timer m_timerSearch;

	guint32 m_personID;
	guint32 m_visitID;
	bool m_entryPatientStatus;
	bool m_fireSearch;
	Glib::RefPtr<Gtk::Application> m_app;

	ViewPatientWindow *m_vp;
	PatientWindow *m_pw;
	VisitsWindow *m_vw;
	SubVisitWindow *m_svw;
	bool m_maximized;
public:

	MainWindow(const Glib::ustring& title, Glib::RefPtr<Gtk::Application>& app);
	virtual ~MainWindow();
	/* Helper Functions */
	void hlpr_append_patient(guint32 id, const Glib::ustring& name, guint32 nif);
	void hlpr_append_visit(guint32 id, const Glib::ustring& complaint, const Glib::ustring& date);

	void hlpr_append_allergy(const Allergy& allergy, const Glib::Date& date);
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
	void on_visitEdit_clicked(void);
	void on_btnRemoveVisit(void);

	bool on_entryPatient_focusIn(GdkEventFocus *focus); 
	bool on_entryPatient_focusOut(GdkEventFocus *focus);

	void on_treePatients_activated(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
	//void on_treePatients_selected();
	void on_txtSearch_changed();
	void on_db_person_edited(const Person& p);
	void on_mhAbout_activate(void);
	bool handler_timeout_search();

	void on_visitEdited(const VisitInterface&);
	void patient_window_add(PatientWindow &);

	bool filter_patient_by_name(const Gtk::TreeModel::const_iterator&);

	void on_visits_selection_changed(void);
	bool on_maximized_change(GdkEventWindowState *state);

	void on_newSubVisit_clicked(void);
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
	virtual guint32 getPersonID() const;
	virtual guint32 getVisitID() const;
	virtual Glib::ustring getComplaint() const;
	virtual Glib::ustring getAnamnesis() const;
	virtual Glib::ustring getDate() const;
	virtual float getWeight() const;
	virtual Glib::ustring getAppearance() const;
	virtual Glib::ustring getMovement() const;
	virtual Glib::ustring getVoice() const;
	virtual Glib::ustring getSmell() const;
	virtual int getHypertension() const;
	virtual int getCholesterol() const;
	virtual int getTriglyceride() const;
	virtual int getDiabetes() const;
	virtual Glib::ustring getSleepiness() const;
	virtual Glib::ustring getTranspiration() const;
	virtual Glib::ustring getDehydration() const;
	virtual int isAnxiety() const;
	virtual int isIrrt() const;
	virtual int isFrustration() const;
	virtual int isCry() const;
	virtual int isVerm() const;
	virtual int isVed() const;
	virtual int isBrad() const;
	virtual int isPrt() const;
	virtual int isAml() const;
	virtual int isAlg() const;
	virtual int isIrritable() const;
	virtual int isSad() const;
	virtual int isMed() const;
	virtual int isMelan() const;
	virtual Glib::ustring getHearing() const;
	virtual Glib::ustring getThroat() const;
	virtual Glib::ustring getScent() const;
	virtual Glib::ustring getVision() const;
	virtual Glib::ustring getFatigue() const;
	virtual Glib::ustring getSexualActivity() const;
	virtual Glib::ustring getBody() const;
	virtual Glib::ustring getAbdomen() const;
	virtual Glib::ustring getHead() const;
	virtual Glib::ustring getCirculation() const;
	virtual Glib::ustring getEatingHabits() const;
	virtual Glib::ustring getMenstruation() const;
	virtual Glib::ustring getPregnancy() const;
	virtual Glib::ustring getPain() const;
	virtual Glib::ustring getPainSince() const;
	virtual Glib::ustring getPainObs() const;
	virtual Glib::ustring getSurgery() const;
	virtual Glib::ustring getPreviousTreatment() const;
	virtual bool getProstheses() const;
	virtual bool getWeightBool() const;
	virtual Glib::ustring getUrine() const;
	virtual Glib::ustring getFaeces() const;
	virtual Glib::ustring getTongue() const;
	virtual Glib::ustring getPulseD() const;
	virtual Glib::ustring getPulseE() const;
	virtual gint16 getBPMax() const;
	virtual gint16 getBPMin() const;
	virtual gint16 getBPM() const;
	virtual Glib::ustring getApal() const;
	virtual Glib::ustring getExams() const;
	virtual Glib::ustring getClinicalAnalysis() const;
	virtual Glib::ustring getColor() const;
	virtual Glib::ustring getEscle() const;
	virtual Glib::ustring getObservations() const;
	virtual Glib::ustring getMed() const;
	virtual Glib::ustring getMedication() const;
	virtual Glib::ustring getTreatment() const;
	virtual Gtk::TreeModel::Children getAllergies() const;
	
	virtual void setPersonID(guint32 val);
	virtual void setVisitID(const guint32 val);
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
