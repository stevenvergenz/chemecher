#ifndef MECHDB_H
#define MECHDB_H

#include <QDialog>
#include <QString>
#include <QDebug>

class MechDB;

#include "ui_mechdb.h"
#include "dbhandler.h"
#include "mix.h"
#include "sqlconfig.h"

class MechDB : public QDialog {
    Q_OBJECT
public:
	typedef enum{save,load} dlgtype_t;
	
	MechDB(dlgtype_t _dlgtype, QWidget *parent = 0);
    ~MechDB();
	
public slots:
	void accept();
	
	void updateInfo();
	void refreshList();
	void updateList();
	void sqlConfig();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::mechdb *ui;
	dlgtype_t dlgtype;
};

#endif // MECHDB_H
