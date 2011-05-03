#ifndef MIX_H
#define MIX_H

#include <QList>
#include <QString>
#include <QFile>
#include <iostream>
using namespace std;

class Mix;

#include "step.h"
#include "cpd.h"
#include "dbhandler.h"
#include "iomanager.h"

namespace Trans {
	enum{NONE=0, LINEAR=1, ATAN=2};
}

class Mix : public QThread
{
Q_OBJECT
public:
	friend class Step;
	
	Mix();
	void run();
	
	bool isActive;	// stores whether mix is in use
	bool calcSuccess; // stores retval of calculation
	
	// compound functions
	QList<Cpd*> CpdList;
	QStringList cpdIdList();
	Cpd* addCpd(Cpd *cpd);
	Cpd* removeCpd(Cpd *cpd);
	void swapCpds(int first, int second);
	Cpd* getCpdById(QString id);
	
	// step functions
	QList<Step*> StepList;
	QStringList stepNameList();
	QStringList stepStringList();
	Step* addStep(Step *step);
	Step* removeStep(Step *step);
	void swapSteps(int first, int second);
	Step* getStepByString(QString string);
	Step* getStepByName(QString name);
	
	int maxCpdNameLen();
	int maxCpdIdLen();
	int maxStateLen();
	int maxStepArrowPos();
	int maxStepLen();
	
	void clone(Mix* newmix);
	void initialize();

	// calculation functions and error flags
	void calculateRKF();
	void calculateLegacy();
	int error;
	QStringList errorMsg;
	QString errorString;
	
	/*************************
	 * Simulation parameters *
	 *************************/
	
	//current mix identifiers
	QString mechFile;
	QString mechName;
	QString mechDesc;

	//time data
	double timeStep, reportStep;
	double startTime, endTime;
	double debugStart, debugEnd;
	double time;
	bool cancel;
	
	//accuracy data
	double precision;

	//legacy parameters
	int order;
	QString method;
	int transition;
	bool autostep;
	double gateband;
	int shifttest;
	int maxreduce;
	double stepfactor;
	
	// sql data
	DBHandler *db;
	
	QList<QStringList> availableMethods();
	
public slots:
	void setAutostep( bool val );
	void setName( QString val );
	void setDesc( QString val );
	
signals:
	void stepListChanged();
	void cpdListChanged();
	
private:
	double hBal(Cpd* acpd);
	bool setCalcConstants();
	double a[5][4];
	double b[5];
	
	
};

extern Mix *mix;

#endif // MIX_H
