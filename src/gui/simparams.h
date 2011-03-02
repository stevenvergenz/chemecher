#ifndef SIMPARAMS_H
#define SIMPARAMS_H

#include <QDialog>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QSignalMapper>
#include <QMap>
#include <QLineEdit>
#include <QFileDialog>

#include "mix.h"
#include <iostream>

namespace Ui {
    class SimParams;
}

typedef struct {
	QLineEdit *field;
	union{
		int *intval;
		double *doubleval;
	};
} ParamInfo;

class SimParams : public QDialog {
    Q_OBJECT
public:
    SimParams(QWidget *parent = 0);
    ~SimParams();
	
public slots:
	void setOrder( int index );
	void setMethod( QString value );
	void setTransition( QString value );
	void setAutostep( bool state );
	void updateAutostep();
	void browseFile(QString field);
	
	void setParameter( QString name );
	ParamInfo* makeParam( double* dval, QLineEdit* field );
	ParamInfo* makeParam( int* ival, QLineEdit* field );
	
protected:
    void changeEvent(QEvent *e);

private:
    Ui::SimParams *ui;
	QSignalMapper *paramMapper;
	QSignalMapper *browseMapper;
	
	QMap<QString,ParamInfo*> intparams;
	QMap<QString,ParamInfo*> doubleparams;
};

#endif // SIMPARAMS_H
