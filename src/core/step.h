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
	QString desc();
	double kPlus();
	double kMinus();
	int stoi( Cpd* cpd );

	// returns whether or not step is valid
	//  i.e. has both products and reactants
	bool isValid();
	
	bool hasCpd( Cpd* cpd );
	bool hasReactant( Cpd* cpd );
	bool hasProduct ( Cpd* cpd );
	
	QList<Cpd*> reactantList();
	QList<Cpd*> productList();
	
	QString toString();

public slots:
	void setName(QString n);
	void setDesc(QString d);
	void setKPlus(double k);
	void setKMinus(double k);
	
	bool addReactant( Cpd* cpd );
	bool addProduct ( Cpd* cpd );
	bool removeReactant( int cpd );
	bool removeProduct ( int cpd );
	
	/*bool setStoi( Cpd* cpd, int stoi );
	bool addStoi( Cpd* cpd, int diff );*/

signals:
	void nameChanged(QString);
	void descChanged(QString);
	void kPlusChanged(double);
	void kMinusChanged(double);
	void reagentsChanged();

private:
	QString stepname, stepdesc;
	double kplus, kminus;
	QList<Cpd*> reactants, products;
	
};

#endif

