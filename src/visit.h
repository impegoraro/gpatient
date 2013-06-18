#include <glibmm/ustring.h>
#include <glibmm/date.h>

#include "db/state.h"

#ifndef _VISIT_H
#define _VISIT_H

class Visit
{
public:

	Visit(const Glib::ustring& anamnesis, const Glib::Date& visitDate);

	const Glib::ustring& getAbdomen() const;
	void setAbdomen(const Glib::ustring& abdomen);
	bool isAlg() const;
	void setAlg(bool alg);
	bool isAml() const;
	void setAml(bool aml);
	const Glib::ustring& getAnamnesis() const;
	void setAnamnesis(const Glib::ustring& anamnese);
	bool isAnxiety() const;
	void setAnxiety(bool anxiety);
	const Glib::ustring& getBody() const;
	void setBody(const Glib::ustring& body);
	bool isBrad() const;
	void setBrad(bool brad);
	int getCholesterol() const;
	void setCholesterol(int cholesterol);
	const Glib::ustring& getCirculation() const;
	void setCirculation(const Glib::ustring& circulation);
	bool isCry() const;
	void setCry(bool cry);
	const Glib::Date& getDate() const;
	void setDate(const Glib::Date& date);
	const Glib::ustring& getDehydration() const;
	void setDehydration(const Glib::ustring& dehydration);
	int getDiabetes() const;
	void setDiabetes(int diabetes);
	const Glib::ustring& getFatigue() const;
	void setFatigue(const Glib::ustring& fatigue);
	bool isFrustration() const;
	void setFrustration(bool frustration);
	const Glib::ustring& getHead() const;
	void setHead(const Glib::ustring& head);
	const Glib::ustring& getHearing() const;
	void setHearing(const Glib::ustring& hearing);
	int getHypertension() const;
	void setHypertension(int hypertension);
	int getId() const;
	void setId(int id);
	bool isIrritable() const;
	void setIrritable(bool irritable);
	bool isIrrt() const;
	void setIrrt(bool irrt);
	bool isMed() const;
	void setMed(bool med);
	bool isMelan() const;
	void setMelan(bool melan);
	bool isPrt() const;
	void setPrt(bool prt);
	bool isSad() const;
	void setSad(bool sad);
	const Glib::ustring& getScent() const;
	void setScent(const Glib::ustring& scent);
	const Glib::ustring& getSexualActivity() const;
	void setSexualActivity(const Glib::ustring& sexualActivity);
	const Glib::ustring& getSleepiness() const;
	void setSleepiness(const Glib::ustring& sleepiness);
	const Glib::ustring& getThroat() const;
	void setThroat(const Glib::ustring& throat);
	const Glib::ustring& getTranspiration() const;
	void setTranspiration(const Glib::ustring& transpiration);
	int getTriglyceride() const;
	void setTriglyceride(int triglyceride);
	bool isVed() const;
	void setVed(bool ved);
	bool isVerm() const;
	void setVerm(bool verm);
	const Glib::ustring& getVision() const;
	void setVision(const Glib::ustring& vision);
	float getWeight() const;
	void setWeight(float weight);
	const Glib::ustring& getMovement() const;
	void setMovement(const Glib::ustring& movement);
	const Glib::ustring& getVoice() const;
	void setVoice(const Glib::ustring& voice);
	const Glib::ustring& getSmell() const;
	void setSmell(const Glib::ustring& smell);
	const Glib::ustring& getAppearance() const;
	void setAppearance(const Glib::ustring& appearance);
	const Glib::ustring& getComplaint() const;
	void setComplaint(const Glib::ustring& complaint);
	int getPersonID() const;
	void setEatingHabits(const Glib::ustring & eatingHabits);
	const Glib::ustring& getEatingHabits();

	bool validate() const;
private:
	int _id;
	int _personID;
	Glib::ustring _complaint;
	Glib::ustring _anamnesis;
	Glib::Date _date;
	float _weight;

	Glib::ustring _eatingHabits;
	Glib::ustring _appearance;
	Glib::ustring _movement;
	Glib::ustring _voice;
	Glib::ustring _smell;
	Glib::ustring _hearing;
	Glib::ustring _throat; 
	Glib::ustring _scent; 
	Glib::ustring _vision;
	Glib::ustring _fatigue;
	Glib::ustring _sexualActivity;
	Glib::ustring _body;
	Glib::ustring _abdomen;
	Glib::ustring _head;
	Glib::ustring _circulation;
	Glib::ustring _sleepiness;
	Glib::ustring _transpiration;
	Glib::ustring _dehydration;

	int _hypertension;
	int _cholesterol;
	int _triglyceride;
	int _diabetes;
	
	bool _anxiety;
	bool _irrt;
	bool _frustration;
	bool _cry;
	bool _verm;
	bool _ved;
	bool _brad;
	bool _prt;
	bool _aml;
	bool _alg;
	bool _irritable;
	bool _sad;
	bool _med;
	bool _melan;

	

};


#endif