#include "visit-interface.h"

VisitInterface& VisitInterface::operator=(const VisitInterface &rhs)
{
	this->setVisitID(rhs.getVisitID());
	this->setComplaint(rhs.getComplaint());
	this->setAnamnesis(rhs.getAnamnesis());
	this->setDate(rhs.getDate());
	this->setWeight(rhs.getWeight());
	this->setAppearance(rhs.getAppearance());
	this->setMovement(rhs.getMovement());
	this->setVoice(rhs.getVoice());
	this->setSmell(rhs.getSmell());
	this->setHypertension(rhs.getHypertension());
	this->setCholesterol(rhs.getCholesterol());
	this->setTriglyceride(rhs.getTriglyceride());
	this->setDiabetes(rhs.getDiabetes());
	this->setSleepiness(rhs.getSleepiness());
	this->setTranspiration(rhs.getTranspiration());
	this->setDehydration(rhs.getDehydration());
	this->setAnxiety(rhs.isAnxiety());
	this->setIrrt(rhs.isIrrt());
	this->setFrustration(rhs.isFrustration());
	this->setCry(rhs.isCry());
	this->setVerm(rhs.isVerm());
	this->setVed(rhs.isVed());
	this->setBrad(rhs.isBrad());
	this->setPrt(rhs.isPrt());
	this->setAml(rhs.isAml());
	this->setAlg(rhs.isAlg());
	this->setIrritable(rhs.isIrritable());
	this->setSad(rhs.isSad());
	this->setMed(rhs.isMed());
	this->setMelan(rhs.isMelan());
	this->setHearing(rhs.getHearing());
	this->setThroat(rhs.getThroat());
	this->setScent(rhs.getScent());
	this->setVision(rhs.getVision());
	this->setFatigue(rhs.getFatigue());
	this->setSexualActivity(rhs.getSexualActivity());
	this->setBody(rhs.getBody());
	this->setAbdomen(rhs.getAbdomen());
	this->setHead(rhs.getHead());
	this->setCirculation(rhs.getCirculation());
	this->setEatingHabits(rhs.getEatingHabits());

	this->setMenstruation(rhs.getMenstruation());
	this->setPregnancy(rhs.getPregnancy());
	this->setPain(rhs.getPain());
	this->setPainSince(rhs.getPainSince());
	this->setPainObs(rhs.getPainObs());
	this->setSurgery(rhs.getSurgery());
	this->setPreviousTreatment(rhs.getPreviousTreatment());
	this->setProstheses(rhs.getProstheses());
	this->setWeightBool(rhs.getWeightBool());
	this->setUrine(rhs.getUrine());
	this->setFaeces(rhs.getFaeces());
	this->setTongue(rhs.getTongue());
	this->setPulseD(rhs.getPulseD());
	this->setPulseE(rhs.getPulseE());
	this->setBloodPressure(rhs.getBPMax(), rhs.getBPMin(), rhs.getBPM());
	this->setApal(rhs.getApal());
	this->setExams(rhs.getExams());
	this->setClinicalAnalysis(rhs.getClinicalAnalysis());
	this->setColor(rhs.getColor());
	this->setEscle(rhs.getEscle());
	this->setObservations(rhs.getObservations());
	this->setMed(rhs.getMed());
	this->setMedication(rhs.getMedication());
	this->setTreatment(rhs.getTreatment());

	return *this;
}