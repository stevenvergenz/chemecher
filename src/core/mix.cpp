#include "mix.h"

using namespace Mix;

namespace Mix {

	//general mix information
	QList<Step*> StepList;
	QList<Cpd*> CpdList;
	QStringList cpdIdList() {
		QStringList ret;
		QListIterator<Cpd*> i(CpdList);
		for( ; i.hasNext(); i.next() )
			ret.append(i.peekNext()->toString());
		return ret;
	}

	//current mix identifiers
	QString mechName = "";
	QString mechDesc = "";
	QFile fileName;

	//time data
	double timeStep=.01, reportStep=.1;
	double startTime=0, endTime=100;
	int debugStart=-1, debugEnd=-1;

	//accuracy data
	double precision=.00001;

	DBHandler db;

	void initialize()
	{

	}

	void calculateRKF()
	{

	}

	void calculateLegacy()
	{

	}

}
