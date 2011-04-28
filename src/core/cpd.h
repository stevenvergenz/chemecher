#ifndef _CPD_H
#define _CPD_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVector>

class Cpd;

#include "step.h"
#include "mix.h"

class Cpd : public QObject
{
	Q_OBJECT
public:
	friend class Step;
	
	//useful types
	/*struct StepPair{ Step* step; int stoi;
		StepPair(Step* s, int t) : step(s), stoi(t) {}
	};*/
	typedef enum {HOMO=0, HETERO, AQ, S, L, G} State;

	// defined in source file	
	static const QString STATES[];
	static const QString TRANS[];
	
	Cpd();
	Cpd(QString, State);

	//void addAsReactant(Step* s); void removeAsReactant(Step* s);
	//void addAsProduct(Step* s);  void removeAsProduct(Step* s);

	const QString toString();
	const QString tov3String();

	bool isHomo();
	bool isHetero();

	/*    Getters    */
	QString shortName();
	QString longName();
	State state();
	double threshold();
	double sharpness();
	int transition();
	double initialConc();

	// calculation intermediates
	double partialConc[5];
	double prevConc;
	double finalConc;
	double rate[5];
	QMap<Step*,int> stoiVals;

public slots:
	void setShortName(QString n);
	void setLongName(QString n);
	void setState(State s);
	void setState(int s);
	bool setState(QString s, bool *ok = 0 );
	void setTransition(int t);
	bool setTransition(QString t, bool *ok = 0 );
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
	int trans;
	double init_conc;

	//QList<StepPair*> reactantset;
	//QList<StepPair*> productset;
	
	
};

#endif

