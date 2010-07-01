#ifndef _STEP_H
#define _STEP_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class Step;

#include "cpd.h"

class Step : public QObject
{
	Q_OBJECT;
public:
	friend class Cpd;
	
	Step();
	
	QString name();
	double kPlus();
	double kMinus();
	int stoi( Cpd* cpd );

	bool hasCpd( Cpd* cpd );
	bool isReactant( Cpd* cpd );
	bool isProduct ( Cpd* cpd );
	
	QList<Cpd*> getReactantList();
	QList<Cpd*> getProductList();
	
	QString toString();

public slots:
	void setName(QString n);
	void setKPlus(double k);
	void setKMinus(double k);
	
	bool addReactant( Cpd* cpd );
	bool addProduct ( Cpd* cpd );
	
	bool removeReagent( Cpd* cpd );
	bool removeReagent( int cpd );
	bool swapReagents( Cpd* cpd1, Cpd* cpd2 );
	bool swapReagents( int cpd1, int cpd2 );
	
	/*bool setStoi( Cpd* cpd, int stoi );
	bool addStoi( Cpd* cpd, int diff );*/

signals:
	void nameChanged(QString);
	void kPlusChanged(double);
	void kMinusChanged(double);
	void reagentsChanged();

private:
	QString stepname;
	double kplus, kminus;
	QList<Cpd*> reactants, products;
	
};

#endif

