#ifndef MIX_H
#define MIX_H

#include <QList>
#include <QString>
#include <QFile>

#include "step.h"
#include "cpd.h"
#include "dbhandler.h"

namespace Mix
{
	//general mix information
	extern QList<Step*> StepList;
	extern QList<Cpd*> CpdList;

	//current mix identifiers
	extern QString mechName;
	extern QString mechDesc;
	extern QFile fileName;

	//time data
	extern double timeStep, reportStep;
	extern double startTime, endTime;
	extern int debugStart, debugEnd;

	// sql data
	extern DBHandler db;

	//accuracy data
	extern double precision;

	void initialize();

	void calculateRKF();
	void calculateLegacy();

}

#endif // MIX_H
