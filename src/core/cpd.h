#ifndef _CPD_H
#define _CPD_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVector>

class Cpd;

#include "step.h"

class Cpd : public QObject
{
	Q_OBJECT
public:
	friend class Step;
	
	//useful types
	/*struct StepPair{ Step* step; int stoi;
		StepPair(Step* s, int t) : step(s), stoi(t) {}
	};*/
	typedef enum {NONE=0, LINEAR, ATAN} Transition;
	typedef enum {HOMO=0, HETERO, AQ, S, L, G} State;

	// defined in source file	
	static const QString STATES[];

	Cpd();
	Cpd(QString, State);

	//void addAsReactant(Step* s); void removeAsReactant(Step* s);
	//void addAsProduct(Step* s);  void removeAsProduct(Step* s);

	QString toString();

	/*    Getters    */
	QString shortName();
	QString longName();
	State state();
	double threshold();
	double sharpness();
	Transition transition();
	double initialConc();

public slots:
	void setShortName(QString n);
	void setLongName(QString n);
	void setState(State s);
	void setState(int s);
	void setTransition(Transition t);
	void setTransition(int t);
	void setThreshold(double t);
	void setSharpness(double s);
	void setInitialConc(double c);
	void setInitialConc(QString c);

signals:
	void shortNameChanged(QString);
	void longNameChanged(QString);
	void stateChanged(int);
	void thresholdChanged(double);
	void sharpnessChanged(double);
	void transitionChanged(int);
	void initialConcChanged(int);

private:
	QString shortname, longname;
	State cpd_state;
	double thresh;
	double sharp;
	Transition trans;
	double partial_conc[6];
	double conc, saved_conc;
	double rate[6];



	//QList<StepPair*> reactantset;
	//QList<StepPair*> productset;
	
	QMap<Step*,int> stoiVals;
	
};

#endif

