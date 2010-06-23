#ifndef _STEP_H
#define _STEP_H

#include <QObject>
#include <QString>

class Cpd;

class Step : public QObject
{
	Q_OBJECT;
public:
	Step();
	QString name();
	double kPlus();
	double kMinus();

	Cpd* getReagent(int n);
	
	QString toString();

public slots:
	void setName(QString n);
	void setKPlus(double k);
	void setKPlus(QString k);
	void setKMinus(double k);
	void setKMinus(QString k);
	bool setReagent(int n, Cpd* cpd);

signals:
	void nameChanged(QString);
	void kPlusChanged(double);
	void kMinusChanged(double);

private:
	QString stepname;
	double kplus, kminus;
	Cpd* reagents[6];
	
};

#endif

