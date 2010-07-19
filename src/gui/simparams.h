#ifndef SIMPARAMS_H
#define SIMPARAMS_H

#include <QDialog>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QSignalMapper>
#include <QMap>

#include "mix.h"

namespace Ui {
    class SimParams;
}

class SimParams : public QDialog {
    Q_OBJECT
public:
    SimParams(QWidget *parent = 0);
    ~SimParams();
	
public slots:
	void setParameter( QString name );
	
protected:
    void changeEvent(QEvent *e);

private:
    Ui::SimParams *ui;
	QSignalMapper *paramMapper;
	
	QMap<QString,int*>    intparams;
	QMap<QString,double*> doubleparams;
};

#endif // SIMPARAMS_H
