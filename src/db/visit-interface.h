#include <glibmm/date.h>
#include <glibmm/ustring.h>
#include <gtkmm/treeview.h>

#include "allergy.h"

#ifndef _VISIT_INTERFACE_H
#define _VISIT_INTERFACE_H

class VisitInterface 
{
public:
	virtual guint32 getPersonID() const = 0;
	virtual guint32 getVisitID() const = 0;
	virtual Glib::ustring getComplaint() const = 0;
	virtual Glib::ustring getAnamnesis() const = 0;
	virtual Glib::ustring getDate() const = 0;
	virtual float getWeight() const = 0;
	virtual Glib::ustring getAppearance() const = 0;
	virtual Glib::ustring getMovement() const = 0;
	virtual Glib::ustring getVoice() const = 0;
	virtual Glib::ustring getSmell() const = 0;
	virtual int getHypertension() const = 0;
	virtual int getCholesterol() const = 0;
	virtual int getTriglyceride() const = 0;
	virtual int getDiabetes() const = 0;
	virtual Glib::ustring getSleepiness() const = 0;
	virtual Glib::ustring getTranspiration() const = 0;
	virtual Glib::ustring getDehydration() const = 0;
	virtual int isAnxiety() const = 0;
	virtual int isIrrt() const = 0;
	virtual int isFrustration() const = 0;
	virtual int isCry() const = 0;
	virtual int isVerm() const = 0;
	virtual int isVed() const = 0;
	virtual int isBrad() const = 0;
	virtual int isPrt() const = 0;
	virtual int isAml() const = 0;
	virtual int isAlg() const = 0;
	virtual int isIrritable() const = 0;
	virtual int isSad() const = 0;
	virtual int isMed() const = 0;
	virtual int isMelan() const = 0;
	virtual Glib::ustring getHearing() const = 0;
	virtual Glib::ustring getThroat() const = 0;
	virtual Glib::ustring getScent() const = 0;
	virtual Glib::ustring getVision() const = 0;
	virtual Glib::ustring getFatigue() const = 0;
	virtual Glib::ustring getSexualActivity() const = 0;
	virtual Glib::ustring getBody() const = 0;
	virtual Glib::ustring getAbdomen() const = 0;
	virtual Glib::ustring getHead() const = 0;
	virtual Glib::ustring getCirculation() const = 0;
	virtual Glib::ustring getEatingHabits() const = 0;
	virtual Glib::ustring getMenstruation() const = 0;
	virtual Glib::ustring getPregnancy() const = 0;
	virtual Glib::ustring getPain() const = 0;
	virtual Glib::ustring getPainSince() const = 0;
	virtual Glib::ustring getPainObs() const = 0;
	virtual Glib::ustring getSurgery() const = 0;
	virtual Glib::ustring getPreviousTreatment() const = 0;
	virtual bool getProstheses() const = 0;
	virtual bool getWeightBool() const = 0;
	virtual Glib::ustring getUrine() const = 0;
	virtual Glib::ustring getFaeces() const = 0;
	virtual Glib::ustring getTongue() const = 0;
	virtual Glib::ustring getPulseD() const = 0;
	virtual Glib::ustring getPulseE() const = 0;
	virtual gint16 getBPMax() const = 0;
	virtual gint16 getBPMin() const = 0;
	virtual gint16 getBPM() const = 0;
	virtual Glib::ustring getApal() const = 0;
	virtual Glib::ustring getExams() const = 0;
	virtual Glib::ustring getClinicalAnalysis() const = 0;
	virtual Glib::ustring getColor() const = 0;
	virtual Glib::ustring getEscle() const = 0;
	virtual Glib::ustring getObservations() const = 0;
	virtual Glib::ustring getMed() const = 0;
	virtual Glib::ustring getMedication() const = 0;
	virtual Glib::ustring getTreatment() const = 0;
	virtual Gtk::TreeModel::Children getAllergies() const = 0;


	/* Setters */
	virtual void setVisitID(const guint32 id) = 0;
	virtual void setComplaint(const Glib::ustring& val) = 0;
	virtual void setAnamnesis(const Glib::ustring& val) = 0;
	virtual void setDate(const Glib::ustring& val) = 0;
	virtual void setWeight(float val) = 0;
	virtual void setAppearance(const Glib::ustring& val) = 0;
	virtual void setMovement(const Glib::ustring& val) = 0;
	virtual void setVoice(const Glib::ustring& val) = 0;
	virtual void setSmell(const Glib::ustring& val) = 0;
	virtual void setHypertension(int val) = 0;
	virtual void setCholesterol(int val) = 0;
	virtual void setTriglyceride(int val) = 0;
	virtual void setDiabetes(int val) = 0;
	virtual void setSleepiness(const Glib::ustring& val) = 0;
	virtual void setTranspiration(const Glib::ustring& val) = 0;
	virtual void setDehydration(const Glib::ustring& val) = 0;
	virtual void setAnxiety(int val) = 0;
	virtual void setIrrt(int val) = 0;
	virtual void setFrustration(int val) = 0;
	virtual void setCry(int val) = 0;
	virtual void setVerm(int val) = 0;
	virtual void setVed(int val) = 0;
	virtual void setBrad(int val) = 0;
	virtual void setPrt(int val) = 0;
	virtual void setAml(int val) = 0;
	virtual void setAlg(int val) = 0;
	virtual void setIrritable(int val) = 0;
	virtual void setSad(int val) = 0;
	virtual void setMed(int val) = 0;
	virtual void setMelan(int val) = 0;
	virtual void setHearing(const Glib::ustring& val) = 0;
	virtual void setThroat(const Glib::ustring& val) = 0;
	virtual void setScent(const Glib::ustring& val) = 0;
	virtual void setVision(const Glib::ustring& val) = 0;
	virtual void setFatigue(const Glib::ustring& val) = 0;
	virtual void setSexualActivity(const Glib::ustring& val) = 0;
	virtual void setBody(const Glib::ustring& val) = 0;
	virtual void setAbdomen(const Glib::ustring& val) = 0;
	virtual void setHead(const Glib::ustring& val) = 0;
	virtual void setCirculation(const Glib::ustring& val) = 0;
	virtual void setEatingHabits(const Glib::ustring& val) = 0;

	virtual void setMenstruation(const Glib::ustring& val) = 0;
	virtual void setPregnancy(const Glib::ustring& val) = 0;
	virtual void setPain(const Glib::ustring& val) = 0;
	virtual void setPainSince(const Glib::ustring& val) = 0;
	virtual void setPainObs(const Glib::ustring& val) = 0;
	virtual void setSurgery(const Glib::ustring& val) = 0;
	virtual void setPreviousTreatment(const Glib::ustring& val) = 0;
	virtual void setProstheses(bool val) = 0;
	virtual void setWeightBool(bool val) = 0;
	virtual void setUrine(const Glib::ustring& val) = 0;
	virtual void setFaeces(const Glib::ustring& val) = 0;
	virtual void setTongue(const Glib::ustring& val) = 0;
	virtual void setPulseD(const Glib::ustring& val) = 0;
	virtual void setPulseE(const Glib::ustring& val) = 0;
	virtual void setBloodPressure(gint16 high, gint16 low, gint16 bpm) = 0;
	virtual void setApal(const Glib::ustring& val) = 0;
	virtual void setExams(const Glib::ustring& val) = 0;
	virtual void setClinicalAnalysis(const Glib::ustring& val) = 0;
	virtual void setColor(const Glib::ustring& val) = 0;
	virtual void setEscle(const Glib::ustring& val) = 0;
	virtual void setObservations(const Glib::ustring& val) = 0;
	virtual void setMed(const Glib::ustring& val) = 0;
	virtual void setMedication(const Glib::ustring& val) = 0;
	virtual void setTreatment(const Glib::ustring& val) = 0;

	virtual VisitInterface& operator=(const VisitInterface &rhs);
};
#endif