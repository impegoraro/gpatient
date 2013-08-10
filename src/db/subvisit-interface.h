#ifndef SUBVISIT_INTERFACE_H_
#define SUBVISIT_INTERFACE_H_

#include <glibmm/date.h>
#include <glibmm/ustring.h>

class SubVisitInterface
{
public:
	virtual guint32 getPersonID() = 0;
	virtual guint32 getPersonID() const = 0;
	virtual guint32 getVisitID() = 0;
	virtual guint32 getVisitID() const = 0;
	virtual guint32 getParentVisitID() = 0;
	virtual guint32 getParentVisitID() const = 0;
	virtual const Glib::Date getDate() = 0;
	virtual const Glib::Date getDate() const = 0;
	virtual const Glib::ustring getSleepiness() = 0;
	virtual const Glib::ustring getSleepiness() const = 0;
	virtual const Glib::ustring getFatigue() = 0;
	virtual const Glib::ustring getFatigue() const = 0;
	virtual const Glib::ustring getHead() = 0;
	virtual const Glib::ustring getHead() const = 0;
	virtual const Glib::ustring getTongue() = 0;
	virtual const Glib::ustring getTongue() const = 0;
	virtual const Glib::ustring getUrine() = 0;
	virtual const Glib::ustring getUrine() const = 0;
	virtual const Glib::ustring getFaeces() = 0;
	virtual const Glib::ustring getFaeces() const = 0;
	virtual const Glib::ustring getMenstruation() = 0;
	virtual const Glib::ustring getMenstruation() const = 0;
	virtual const Glib::ustring getPulseD() = 0;
	virtual const Glib::ustring getPulseD() const = 0;
	virtual const Glib::ustring getPulseE() = 0;
	virtual const Glib::ustring getPulseE() const = 0;
	virtual const Glib::ustring getApal() = 0;
	virtual const Glib::ustring getApal() const = 0;
	virtual const Glib::ustring getObservations() = 0;
	virtual const Glib::ustring getObservations() const = 0;
	virtual const void getBloodPressure(guint16& max, guint16& min, guint16& bpm) = 0;
	virtual const void getBloodPressure(guint16& max, guint16& min, guint16& bpm) const = 0;

	virtual void setPersonID(guint32 val) = 0;
	virtual void setParentVisitID(guint32 val) = 0;
	virtual void setVisitID(guint32 val) = 0;
	virtual void setDate(const Glib::Date& val) = 0;
	virtual void setSleepiness(const Glib::ustring& val) = 0;
	virtual void setFatigue(const Glib::ustring& val) = 0;
	virtual void setHead(const Glib::ustring& val) = 0;
	virtual void setTongue(const Glib::ustring& val) = 0;
	virtual void setUrine(const Glib::ustring& val) = 0;
	virtual void setFaeces(const Glib::ustring& val) = 0;
	virtual void setMenstruation(const Glib::ustring& val) = 0;
	virtual void setPulseD(const Glib::ustring& val) = 0;
	virtual void setPulseE(const Glib::ustring& val) = 0;
	virtual void setApal(const Glib::ustring& val) = 0;
	virtual void setObservations(const Glib::ustring& val) = 0;
	virtual void setBloodPressure(guint16 max, guint16 min, guint16 bpm) = 0;

	virtual SubVisitInterface& operator=(const SubVisitInterface& rhs);
};
#endif