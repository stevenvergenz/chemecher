#ifndef MIX_H
#define MIX_H

#include <QList>
#include <QString>
#include <QFile>

#include "step.h"
#include "cpd.h"

namespace Mix
{
	//general mix information
	extern QList<Step*> StepList;
	extern QList<Cpd*> CpdList;

	//current mix identifiers
	extern QString mechName;
	extern QFile fileName;

	//time data
	extern double timeStep, reportStep;
	extern int initialTime, maxTime;
	extern int debugStart, debugEnd;

	//accuracy data
	extern double precision;

	void initialize();

	void calculateRKF();
	void calculateLegacy();

}

#endif // MIX_H
