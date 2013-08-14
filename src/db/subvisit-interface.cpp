#include "subvisit-interface.h"


SubVisitInterface& SubVisitInterface::operator=(const SubVisitInterface& rhs)
{
	guint16 max, min, bpm;

	setPersonID(rhs.getPersonID());
	setParentVisitID(rhs.getParentVisitID());
	setSubVisitID(rhs.getSubVisitID());
	setSubVisitDate(rhs.getSubVisitDate());
	setSubVisitSleepiness(rhs.getSubVisitSleepiness());
	setSubVisitFatigue(rhs.getSubVisitFatigue());
	setSubVisitHead(rhs.getSubVisitHead());
	setSubVisitTongue(rhs.getSubVisitTongue());
	setSubVisitUrine(rhs.getSubVisitUrine());
	setSubVisitFaeces(rhs.getSubVisitFaeces());
	setSubVisitMenstruation(rhs.getSubVisitMenstruation());
	setSubVisitPulseD(rhs.getSubVisitPulseD());
	setSubVisitPulseE(rhs.getSubVisitPulseE());
	setSubVisitApal(rhs.getSubVisitApal());
	setSubVisitObservations(rhs.getSubVisitObservations());
	rhs.getSubVisitBloodPressure(max, min, bpm);
	setSubVisitBloodPressure(max, min, bpm);

	return *this;
}