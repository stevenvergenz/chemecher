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
	emit cpdListChanged();
}
void Mix::removeCpd(Cpd *cpd)
{
	CpdList.removeOne(cpd);
	emit cpdListChanged();
}
void Mix::swapCpds(int first, int second)
{
	CpdList.swap(first,second);
	emit cpdListChanged();
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
	emit stepListChanged();
}
void Mix::removeStep(Step *step)
{
	StepList.removeOne(step);
	emit stepListChanged();
}
void Mix::swapSteps(int first, int second)
{
	StepList.swap(first,second);
	emit stepListChanged();
}
Step* Mix::getStepByString(QString string)
{
	Step *ret = 0;
	for( int i=0; i<StepList.size(); i++ ) {
		if( StepList[i]->toString() == string )
			ret = StepList[i];
	}
	return ret;
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
