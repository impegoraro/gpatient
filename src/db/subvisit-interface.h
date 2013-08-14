#ifndef SUBVISIT_INTERFACE_H_
#define SUBVISIT_INTERFACE_H_

#include <glibmm/date.h>
#include <glibmm/ustring.h>

class SubVisitInterface
{
public:
	virtual guint32 getPersonID() = 0;
	virtual guint32 getPersonID() const = 0;
	virtual guint32 getSubVisitID() = 0;
	virtual guint32 getSubVisitID() const = 0;
	virtual guint32 getParentVisitID() = 0;
	virtual guint32 getParentVisitID() const = 0;
	virtual const Glib::Date getSubVisitDate() = 0;
	virtual const Glib::Date getSubVisitDate() const = 0;
	virtual const Glib::ustring getSubVisitSleepiness() = 0;
	virtual const Glib::ustring getSubVisitSleepiness() const = 0;
	virtual const Glib::ustring getSubVisitFatigue() = 0;
	virtual const Glib::ustring getSubVisitFatigue() const = 0;
	virtual const Glib::ustring getSubVisitHead() = 0;
	virtual const Glib::ustring getSubVisitHead() const = 0;
	virtual const Glib::ustring getSubVisitTongue() = 0;
	virtual const Glib::ustring getSubVisitTongue() const = 0;
	virtual const Glib::ustring getSubVisitUrine() = 0;
	virtual const Glib::ustring getSubVisitUrine() const = 0;
	virtual const Glib::ustring getSubVisitFaeces() = 0;
	virtual const Glib::ustring getSubVisitFaeces() const = 0;
	virtual const Glib::ustring getSubVisitMenstruation() = 0;
	virtual const Glib::ustring getSubVisitMenstruation() const = 0;
	virtual const Glib::ustring getSubVisitPulseD() = 0;
	virtual const Glib::ustring getSubVisitPulseD() const = 0;
	virtual const Glib::ustring getSubVisitPulseE() = 0;
	virtual const Glib::ustring getSubVisitPulseE() const = 0;
	virtual const Glib::ustring getSubVisitApal() = 0;
	virtual const Glib::ustring getSubVisitApal() const = 0;
	virtual const Glib::ustring getSubVisitObservations() = 0;
	virtual const Glib::ustring getSubVisitObservations() const = 0;
	virtual const void getSubVisitBloodPressure(guint16& max, guint16& min, guint16& bpm) = 0;
	virtual const void getSubVisitBloodPressure(guint16& max, guint16& min, guint16& bpm) const = 0;

	virtual void setPersonID(guint32 val) = 0;
	virtual void setParentVisitID(guint32 val) = 0;
	virtual void setSubVisitID(guint32 val) = 0;
	virtual void setSubVisitDate(const Glib::Date& val) = 0;
	virtual void setSubVisitSleepiness(const Glib::ustring& val) = 0;
	virtual void setSubVisitFatigue(const Glib::ustring& val) = 0;
	virtual void setSubVisitHead(const Glib::ustring& val) = 0;
	virtual void setSubVisitTongue(const Glib::ustring& val) = 0;
	virtual void setSubVisitUrine(const Glib::ustring& val) = 0;
	virtual void setSubVisitFaeces(const Glib::ustring& val) = 0;
	virtual void setSubVisitMenstruation(const Glib::ustring& val) = 0;
	virtual void setSubVisitPulseD(const Glib::ustring& val) = 0;
	virtual void setSubVisitPulseE(const Glib::ustring& val) = 0;
	virtual void setSubVisitApal(const Glib::ustring& val) = 0;
	virtual void setSubVisitObservations(const Glib::ustring& val) = 0;
	virtual void setSubVisitBloodPressure(guint16 max, guint16 min, guint16 bpm) = 0;

	virtual SubVisitInterface& operator=(const SubVisitInterface& rhs);
};
#endif