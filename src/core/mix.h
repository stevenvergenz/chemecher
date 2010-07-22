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
	
	Mix();
	
	// compound functions
	QList<Cpd*> CpdList;
	QStringList cpdIdList();
	Cpd* addCpd(Cpd *cpd);
	void removeCpd(Cpd *cpd);
	void swapCpds(int first, int second);
	Cpd* getCpdById(QString id);
	
	// step functions
	QList<Step*> StepList;
	QStringList stepNameList();
	QStringList stepStringList();
	Step* addStep(Step *step);
	void removeStep(Step *step);
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
	void calculateRKF();
	void calculateLegacy();
	
	/*************************
	 * Simulation parameters *
	 *************************/
	
	//current mix identifiers
	QString mechName;
	QString mechDesc;

	//time data
	double timeStep, reportStep;
	double startTime, endTime;
	int debugStart, debugEnd;

	//accuracy data
	double precision;

	//legacy parameters
	int order;
	QString method;
	QString transition;
	bool autostep;
	int gateband;
	int shifttest;
	int maxreduce;
	int stepfactor;
	
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
	
	void isActiveChanged( bool val );
	
private:
	// stores whether mix is in use
	bool active;
public slots:
	void setActive( bool val );
public:
	bool isActive();
	
};

extern Mix *mix;

#endif // MIX_H
