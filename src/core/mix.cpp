#include "mix.h"

Mix *mix = new Mix();

QStringList Mix::cpdIdList() {
	QStringList ret;
	for( int i=0; i<CpdList.size(); i++ )
		ret.append(CpdList[i]->toString());
	return ret;
}
void Mix::addCpd(Cpd *cpd) {
	CpdList.append(cpd);
	emit addedCpd(cpd);
}
void Mix::removeCpd(Cpd *cpd)
{
	CpdList.removeOne(cpd);
	emit removedCpd(cpd);
}
Cpd* Mix::getCpdById(QString id)
{
	Cpd *ret = 0;
	for( int i=0; i<CpdList.size(); i++ ) {
		if( CpdList[i]->toString() == id )
			ret = CpdList[i];
	}
	return ret;
}

QStringList Mix::stepNameList() {
	QStringList ret;
	for( int i=0; i<StepList.size(); i++ )
		ret.append(StepList[i]->toString());
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
