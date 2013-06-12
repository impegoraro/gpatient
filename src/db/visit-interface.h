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
	virtual Glib::Date getDate() = 0;
	virtual float getWeight() = 0;
	
	virtual Glib::ustring getAppearance() = 0;
	virtual Glib::ustring getMovement() = 0;
	virtual Glib::ustring getVoice() = 0;
	virtual Glib::ustring getSmell() = 0;
	
	virtual int getHypertension() = 0;
	virtual int getCholesterol() = 0;
	virtual int getTriglyceride() = 0;
	virtual int getDiabetes() = 0;
	
	virtual Glib::ustring getSleepiness() = 0;
	virtual Glib::ustring getTranspiration() = 0;
	virtual Glib::ustring getDehydration() = 0;
	
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
};
#endif