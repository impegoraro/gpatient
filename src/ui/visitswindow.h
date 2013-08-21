/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gestao-herb
 * Copyright (C) 2013 Ilan Pegoraro <iemoreirap@gmail.com>
 * 
gestao-herb is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gestao-herb is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _VISITSWINDOW_H_
#define _VISITSWINDOW_H_


#include <gtkmm/window.h>
#include <gtkmm/assistant.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/treeview.h>
#include <gtkmm/textview.h>
#include <gtkmm/builder.h>
#include <gtkmm/liststore.h>
#include <gtkmm/combobox.h>
#include <gtkmm/comboboxtext.h>


#include "ui/widgets/widgets.h"
#include "db/visit-interface.h"
#include "db/list-status.h"

class VisitsWindow : public VisitInterface
{
public:
	enum BasicInfoButtons
	{
		Hypertension,
		Cholesterol,
		Triglycerides,
		Diabetes
	};

	enum WindowType
	{
		WINDOW_TYPE_ADD,
		WINDOW_TYPE_EDIT
	};

	VisitsWindow(Gtk::Window& win, int personID);
	~VisitsWindow();

	const Gtk::Window* get_window();

	//TODO: Remove garbage
	void set_to_garbage();
	virtual void setPersonID(int personID);
	virtual void set_sex_widgets(bool sex);
	virtual int get_window_type();
	virtual void set_window_type(WindowType type = WINDOW_TYPE_ADD, guint32 visitID = 0);
	void show();	

	class ListAllergies : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListAllergies()
		{
			add(m_col_id);
			add(m_col_name);
			add(m_col_obs);
			add(m_col_status);
		}

		Gtk::TreeModelColumn<guint32> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<Glib::ustring> m_col_obs;
		Gtk::TreeModelColumn<int> m_col_status;
	};

	class ListHereditary : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListHereditary()
		{
			add(m_col_id);
			add(m_col_parent);
			add(m_col_name);
			add(m_col_obs);
			add(m_col_status);
		}

		Gtk::TreeModelColumn<guint32> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_parent;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<Glib::ustring> m_col_obs;
		Gtk::TreeModelColumn<guint32> m_col_status;
	};
	
private:
	
	guint32 m_visitID;
	int m_type;
	Gtk::Assistant *m_win;

	Gtk::Button *m_btnHyper;
	int m_hyper;
	Gtk::Button *m_btnChol;
	int m_chol;
	Gtk::Button *m_btnTrigl;
	int m_trigl;
	Gtk::Button *m_btnDiabetes;
	int m_diabetes;
	
	Gtk::ToolButton *m_tbAllergiesAdd;
	Gtk::ToolButton *m_tbAllergiesRemove;
	Gtk::TreeView *m_treeAllergies;

	Gtk::ToolButton *m_tbHereditaryAdd;
	Gtk::ToolButton *m_tbHereditaryRemove;
	Gtk::TreeView *m_treeHereditary;
	
	Gtk::Entry *m_txtComplaint;
	Gtk::Entry *m_txtDate;
	Gtk::TextView *m_txtAnamnesis;
	Gtk::SpinButton *m_txtWeight;
	Gtk::Entry *m_txtSleepiness;
	Gtk::Entry *m_txtEatingHabits;

	Gtk::CheckButton *m_chkAnxiety;
	Gtk::CheckButton *m_chkIrrt;
	Gtk::CheckButton *m_chkFrustration;
	Gtk::CheckButton *m_chkCry;
	Gtk::CheckButton *m_chkVerm;
	Gtk::CheckButton *m_chkVed;
	Gtk::CheckButton *m_chkBrad;
	Gtk::CheckButton *m_chkPrt;
	Gtk::CheckButton *m_chkAml;
	Gtk::CheckButton *m_chkAlg;
	Gtk::CheckButton *m_chkIrritable;
	Gtk::CheckButton *m_chkSad;
	Gtk::CheckButton *m_chkMed;
	Gtk::CheckButton *m_chkMelan;

	Gtk::ComboBoxText *m_cmbAppearance;
	Gtk::ComboBoxText *m_cmbMovement;
	Gtk::ComboBoxText *m_cmbHearing;
	Gtk::ComboBoxText *m_cmbThroat; 
	Gtk::ComboBoxText *m_cmbScent; 
	Gtk::ComboBoxText *m_cmbVision;
	Gtk::Entry *m_txtTranspiration;
	Gtk::Entry *m_txtDehydration;
	Gtk::Entry *m_txtVoice;
	Gtk::Entry *m_txtSmell;
	Gtk::Entry *m_txtFatigue;
	Gtk::Entry *m_txtSexualActivity;
	Gtk::Entry *m_txtBody;
	Gtk::Entry *m_txtAbdomen;
	Gtk::Entry *m_txtHead;
	Gtk::Entry *m_txtCirculation;

	Gtk::Entry *m_txtPainSince;
	Gtk::Entry *m_txtPainObs;
	Gtk::Entry *m_txtSurgery;
	Gtk::Entry *m_txtPreviousTreatment;
	Gtk::CheckButton *m_chkProstheses;
	Gtk::CheckButton *m_chkWeight;

	Gtk::Label *m_lblTitle;
	Gtk::Label *m_lblMenstruation;
	Gtk::Label *m_lblPregnancy;
	Gtk::ComboBoxText *m_cmbPain;
	Gtk::Entry *m_txtMenstruation;
	Gtk::Entry *m_txtPregnancy;
	Gtk::Entry *m_txtUrine;
	Gtk::Entry *m_txtFaeces;
	Gtk::Entry *m_txtTongue;
	Gtk::Entry *m_txtPulseD;
	Gtk::Entry *m_txtPulseE;
	Gtk::Entry *m_txtBPMax;
	Gtk::Entry *m_txtBPMin;
	Gtk::Entry *m_txtBPM;
	Gtk::Entry *m_txtApal;
	Gtk::Entry *m_txtExams;
	Gtk::Entry *m_txtClinicalAnalysis;
	Gtk::Entry *m_txtColor;
	Gtk::Entry *m_txtEscle;
	Gtk::Entry *m_txtObservations;
	Gtk::Entry *m_txtMed;
	Gtk::TextView *m_txtMedication;
	Gtk::TextView *m_txtTreatment;

	ListHereditary m_lhd;
	
	Widgets::CalendarWindow *m_wincal;	
	
	guint32 m_personID;

	void set_button_state(BasicInfoButtons type);
	bool on_window_closing(GdkEventAny*);
	void on_btnChangeState(BasicInfoButtons);
	void hlpr_list_allergies_add();
	void hlpr_list_hereditary_add();
	void hlpr_list_remove(int);
	void hlpr_append_allergy(const Allergy& allergy, const Glib::Date& date);

	void on_txtDate_iconPress(Gtk::EntryIconPosition, const GdkEventButton*);
	void on_treeVisit_activate(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
	void on_apply();

	void on_listAllergies_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter);
	bool close_on_esc(GdkEventKey* event);
	void next_page(Gtk::Widget* page);
	void on_widget_check();
	void inline clear();

	void get_db_data();

public:
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

	virtual void setVisitID(const guint32 visitID);
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

