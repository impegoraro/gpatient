#include "visit.h"

using namespace Glib;
using namespace DB;

Visit::Visit(const ustring& anamnesis, const Date& visitDate) : 
	_anamnesis(anamnesis), _date(visitDate)
{

}

const ustring& Visit::getAbdomen() const {
	return _abdomen;
}

void Visit::setAbdomen(const ustring& abdomen) {
	_abdomen = abdomen;
}

bool Visit::isAlg() const {
	return _alg;
}

void Visit::setAlg(bool alg) {
	_alg = alg;
}

bool Visit::isAml() const {
	return _aml;
}

void Visit::setAml(bool aml) {
	_aml = aml;
}

const ustring& Visit::getAnamnesis() const {
	return _anamnesis;
}

void Visit::setAnamnesis(const ustring& anamnesis) {
	_anamnesis = anamnesis;
}

bool Visit::isAnxiety() const {
	return _anxiety;
}

void Visit::setAnxiety(bool anxiety) {
	_anxiety = anxiety;
}

const ustring& Visit::getBody() const {
	return _body;
}

void Visit::setBody(const ustring& body) {
	_body = body;
}

bool Visit::isBrad() const {
	return _brad;
}

void Visit::setBrad(bool brad) {
	_brad = brad;
}

int Visit::getCholesterol() const {
	return _cholesterol;
}

void Visit::setCholesterol(int cholesterol) {
	_cholesterol = cholesterol;
}

const ustring& Visit::getCirculation() const {
	return _circulation;
}

void Visit::setCirculation(const ustring& circulation) {
	_circulation = circulation;
}

bool Visit::isCry() const {
	return _cry;
}

void Visit::setCry(bool cry) {
	_cry = cry;
}

const Date& Visit::getDate() const {
	return _date;
}

void Visit::setDate(const Date& date) {
	_date = date;
}

const ustring& Visit::getDehydration() const {
	return _dehydration;
}

void Visit::setDehydration(const ustring& dehydration) {
	_dehydration = dehydration;
}

int Visit::getDiabetes() const {
	return _diabetes;
}

void Visit::setDiabetes(int diabetes) {
	_diabetes = diabetes;
}

const ustring& Visit::getFatigue() const {
	return _fatigue;
}

void Visit::setFatigue(const ustring& fatigue) {
	_fatigue = fatigue;
}

bool Visit::isFrustration() const {
	return _frustration;
}

void Visit::setFrustration(bool frustration) {
	_frustration = frustration;
}

const ustring& Visit::getHead() const {
	return _head;
}

void Visit::setHead(const ustring& head) {
	_head = head;
}

const ustring& Visit::getHearing() const {
	return _hearing;
}

void Visit::setHearing(const ustring& hearing) {
	_hearing = hearing;
}

int Visit::getHypertension() const {
	return _hypertension;
}

void Visit::setHypertension(int hypertension) {
	_hypertension = hypertension;
}

int Visit::getId() const {
	return _id;
}

void Visit::setId(int id) {
	_id = id;
}

bool Visit::isIrritable() const {
	return _irritable;
}

void Visit::setIrritable(bool irritable) {
	_irritable = irritable;
}

bool Visit::isIrrt() const {
	return _irrt;
}

void Visit::setIrrt(bool irrt) {
	_irrt = irrt;
}

bool Visit::isMed() const {
	return _med;
}

void Visit::setMed(bool med) {
	_med = med;
}

bool Visit::isMelan() const {
	return _melan;
}

void Visit::setMelan(bool melan) {
	_melan = melan;
}

bool Visit::isPrt() const {
	return _prt;
}

void Visit::setPrt(bool prt) {
	_prt = prt;
}

bool Visit::isSad() const {
	return _sad;
}

void Visit::setSad(bool sad) {
	_sad = sad;
}

const ustring& Visit::getScent() const {
	return _scent;
}

void Visit::setScent(const ustring& scent) {
	_scent = scent;
}

const ustring& Visit::getSexualActivity() const {
	return _sexualActivity;
}

void Visit::setSexualActivity(const ustring& sexualActivity) {
	_sexualActivity = sexualActivity;
}

const ustring& Visit::getSleepiness() const {
	return _sleepiness;
}

void Visit::setSleepiness(const ustring& sleepiness) {
	_sleepiness = sleepiness;
}

const ustring& Visit::getThroat() const {
	return _throat;
}

void Visit::setThroat(const ustring& throat) {
	_throat = throat;
}

const ustring& Visit::getTranspiration() const {
	return _transpiration;
}

void Visit::setTranspiration(const ustring& transpiration) {
	_transpiration = transpiration;
}

int Visit::getTriglyceride() const {
	return _triglyceride;
}

void Visit::setTriglyceride(int triglyceride) {
	_triglyceride = triglyceride;
}

bool Visit::isVed() const {
	return _ved;
}

void Visit::setVed(bool ved) {
	_ved = ved;
}

bool Visit::isVerm() const {
	return _verm;
}

void Visit::setVerm(bool verm) {
	_verm = verm;
}

const ustring& Visit::getVision() const {
	return _vision;
}

void Visit::setVision(const ustring& vision) {
	_vision = vision;
}

float Visit::getWeight() const {
	return _weight;
}

void Visit::setWeight(float weight) {
	_weight = weight;
}

const ustring& Visit::getMovement() const {
	return _movement;
}

void Visit::setMovement(const ustring& movement) {
	_movement = movement;
}

const ustring& Visit::getVoice() const {
	return _voice;
}

void Visit::setVoice(const ustring& voice) {
	_voice = voice;
}

const ustring& Visit::getSmell() const {
	return _smell;
}

void Visit::setSmell(const ustring& smell) {
	_smell = smell;
}

const Glib::ustring& Visit::getComplaint() const
{
	return _complaint;
}

void Visit::setComplaint(const Glib::ustring& complaint)
{
	_complaint = complaint;
}

bool Visit::validate() const
{
	return true;
}

int Visit::getPersonID() const
{
	return _personID;
}

const ustring& Visit::getAppearance() const
{
	return _appearance;
}

void Visit::setAppearance(const ustring& appearance)
{
	_appearance = appearance;
}