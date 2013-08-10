#include "subvisit-interface.h"


SubVisitInterface& SubVisitInterface::operator=(const SubVisitInterface& rhs)
{
	guint16 max, min, bpm;

	setPersonID(getPersonID());
	setParentVisitID(getParentVisitID());
	setVisitID(getVisitID());
	setDate(rhs.getDate());
	setSleepiness(rhs.getSleepiness());
	setFatigue(rhs.getFatigue());
	setHead(rhs.getHead());
	setTongue(rhs.getTongue());
	setUrine(rhs.getUrine());
	setFaeces(rhs.getFaeces());
	setMenstruation(rhs.getMenstruation());
	setPulseD(rhs.getPulseD());
	setPulseE(rhs.getPulseE());
	setApal(rhs.getApal());
	setObservations(rhs.getObservations());
	getBloodPressure(max, min, bpm);
	setBloodPressure(max, min, bpm);

	return *this;
}