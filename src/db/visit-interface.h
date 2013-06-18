#include <glibmm/date.h>
#include <glibmm/ustring.h>

#ifndef _VISIT_INTERFACE_H
#define _VISIT_INTERFACE_H

class VisitInterface 
{
public:
	virtual int getPersonID() = 0;
	virtual Glib::ustring getComplaint() = 0;
	virtual Glib::ustring getAnamnesis() = 0;
	virtual Glib::ustring getDate() = 0;
	virtual float getWeight() = 0;
	virtual Glib::ustring getAppearance() = 0;
	virtual Glib::ustring getMovement() = 0;
	virtual Glib::ustring getVoice() = 0;
	virtual Glib::ustring getSmell() = 0;
	virtual int getHypertension() = 0;
	virtual int getCholesterol() = 0;
	virtual int getTriglyceride() = 0;
	virtual int getDiabetes() = 0;
	virtual Glib::ustring  getSleepiness() = 0;
	virtual Glib::ustring  getTranspiration() = 0;
	virtual Glib::ustring  getDehydration() = 0;
	virtual int isAnxiety() = 0;
	virtual int isIrrt() = 0;
	virtual int isFrustration() = 0;
	virtual int isCry() = 0;
	virtual int isVerm() = 0;
	virtual int isVed() = 0;
	virtual int isBrad() = 0;
	virtual int isPrt() = 0;
	virtual int isAml() = 0;
	virtual int isAlg() = 0;
	virtual int isIrritable() = 0;
	virtual int isSad() = 0;
	virtual int isMed() = 0;
	virtual int isMelan() = 0;
	virtual Glib::ustring getHearing() = 0;
	virtual Glib::ustring getThroat() = 0;
	virtual Glib::ustring getScent() = 0;
	virtual Glib::ustring getVision() = 0;
	virtual Glib::ustring getFatigue() = 0;
	virtual Glib::ustring getSexualActivity() = 0;
	virtual Glib::ustring getBody() = 0;
	virtual Glib::ustring getAbdomen() = 0;
	virtual Glib::ustring getHead() = 0;
	virtual Glib::ustring getCirculation() = 0;
	virtual Glib::ustring getEatingHabits() = 0;


	/* Setters */
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
};
#endif