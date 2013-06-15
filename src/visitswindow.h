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

#include <gtkmm.h>

#include <gtkmm/assistant.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/treeview.h>
#include <gtkmm/textview.h>
#include <gtkmm/builder.h>
#include <gtkmm/liststore.h>

#include "ui/widgets/widgets.h"
#include "db/visit-interface.h"

class VisitsWindow : public VisitInterface
{
public:
	VisitsWindow(int personID);
	~VisitsWindow();

	const Gtk::Window* get_window();
	
	void setPersonID(int personID);

	void show();
private:
	class ListAlergies : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListAlergies()
		{
			add(m_col_id);
			add(m_col_name);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};
	class ListHereditary : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ListHereditary()
		{
			add(m_col_id);
			add(m_col_parent);
			add(m_col_name);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_parent;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};

	enum BasicInfoButtons
	{
		Hypertension,
		Cholesterol,
		Triglycerides,
		Diabetes
	};
	
	
	Gtk::Assistant *m_win;

	Gtk::Button *m_btnHyper;
	int m_hyper;
	Gtk::Button *m_btnChol;
	int m_chol;
	Gtk::Button *m_btnTrigl;
	int m_trigl;
	Gtk::Button *m_btnDiabetes;
	int m_diabetes;
	
	Gtk::ToolButton *m_tbAlergiesAdd;
	Gtk::ToolButton *m_tbAlergiesRemove;
	Gtk::TreeView *m_treeAlergies;

	Gtk::ToolButton *m_tbHereditaryAdd;
	Gtk::ToolButton *m_tbHereditaryRemove;
	Gtk::TreeView *m_treeHereditary;
	
	Gtk::Entry *m_txtComplaint;
	Gtk::Entry *m_txtDate;
	Gtk::TextView *m_txtAnamnesis;
	Gtk::Entry *m_txtSleepiness;
	Gtk::SpinButton *m_txtWeight;
	Gtk::Entry *m_txtTranspiration;
	Gtk::Entry *m_txtDehydration;

	Gtk::CheckButton *m_chkAnxiety;
	Gtk::CheckButton *m_chkIrrt;
	Gtk::CheckButton *m_chkFrustration;
	Gtk::CheckButton *m_chkCry;
	Gtk::CheckButton *m_chkVerm;
	Gtk::CheckButton *m_chkVed;
	Gtk::CheckButton *m_chkBra;
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
	Gtk::Entry *m_txtVoice;
	Gtk::Entry *m_txtSmell;
	Gtk::Entry *m_txtFatigue;
	Gtk::Entry *m_txtSexualActivity;
	Gtk::Entry *m_txtBody;
	Gtk::Entry *m_txtAbdomen;
	Gtk::Entry *m_txtHead;
	Gtk::Entry *m_txtCirculation;

	ListAlergies m_la;
	ListHereditary m_lhd;
	
	Widgets::CalendarWindow *m_wincal;	
	
	int m_personID;

	bool on_window_closing(GdkEventAny*);
	void on_btnChangeState(BasicInfoButtons);
	void hlpr_list_alergies_add();
	void hlpr_list_hereditary_add();
	void hlpr_list_remove(Gtk::TreeView *);
	void on_txtDate_iconPress(Gtk::EntryIconPosition, const GdkEventButton*);
	void on_apply();
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
};

#endif // _VISITSWINDOW_H_

