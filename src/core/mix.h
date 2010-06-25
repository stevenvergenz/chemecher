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
	
	// members
	QList<Cpd*> CpdList;
	QStringList cpdIdList();
	void addCpd(Cpd *cpd);
	void removeCpd(Cpd *cpd);
	Cpd* getCpdById(QString id);
	
	QList<Step*> StepList;
	QStringList stepNameList();
	void addStep(Step *step);
	void removeStep(Step *step);
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
	void addedStep(Step *step);
	void removedStep(Step *step);
	void addedCpd(Cpd *cpd);
	void removedCpd(Cpd *cpd);
	
public slots:
	
};

extern Mix *mix;

#endif // MIX_H
