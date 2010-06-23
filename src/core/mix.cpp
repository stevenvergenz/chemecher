#include "mix.h"

Mix *mix = new Mix();

QStringList Mix::cpdIdList() {
	QStringList ret;
	QListIterator<Cpd*> i(CpdList);
	for( ; i.hasNext(); i.next() )
		ret.append(i.peekNext()->toString());
	return ret;
}
void Mix::addCpd(Cpd *cpd) {
	CpdList.append(cpd);
	emit addedCpd(cpd);
}
void Mix::removeCpd(Cpd *cpd)
{
	
}

QStringList Mix::stepNameList() {
	QStringList ret;
	QListIterator<Step*> i(StepList);
	for( ; i.hasNext(); i.next() )
		ret.append(i.peekNext()->name());
	return ret;
	
}
void Mix::addStep(Step *step) {
	StepList.append(step);
	emit addedStep(step);
}
void Mix::removeStep(Step *step)
{
	
}

void Mix::initialize()
{

}

void Mix::calculateRKF()
{

}

void Mix::calculateLegacy()
{

}
