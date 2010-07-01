#ifndef MIX_H
#define MIX_H

#include <QList>
#include <QString>
#include <QFile>

class Mix;

#include "step.h"
#include "cpd.h"
#include "dbhandler.h"

class Mix : public QObject
{
Q_OBJECT
public:
	friend class Step;
	
	// members
	QList<Cpd*> CpdList;
	QStringList cpdIdList();
	Cpd* addCpd(Cpd *cpd);
	Cpd* removeCpd(Cpd *cpd);
	void swapCpds(int first, int second);
	Cpd* getCpdById(QString id);
	
	QList<Step*> StepList;
	QStringList stepNameList();
	Step* addStep(Step *step);
	Step* removeStep(Step *step);
	void swapSteps(int first, int second);
	Step* getStepByString(QString string);
	Step* getStepByName(QString name);
	
	void initialize();
	void calculateRKF();
	void calculateLegacy();
	

	//current mix identifiers
	QString mechName;
	QString mechDesc;
	QFile fileName;

	//time data
	double timeStep, reportStep;
	double startTime, endTime;
	int debugStart, debugEnd;

	// sql data
	DBHandler *db;

	//accuracy data
	double precision;
	
signals:
	void stepListChanged();
	void cpdListChanged();
	
public slots:
	
};

extern Mix *mix;

#endif // MIX_H
