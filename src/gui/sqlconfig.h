#ifndef SQLCONFIG_H
#define SQLCONFIG_H

#include <QDialog>
#include <QString>

#include "mechdb.h"
#include "mix.h"
#include "dbhandler.h"

using namespace Mix;

namespace Ui {
    class SqlConfig;
}

class SqlConfig : public QDialog {
    Q_OBJECT
public:
    SqlConfig(QWidget *parent = 0);
    ~SqlConfig();
	
	QString getHostName();
	QString getUserName();
	QString getPassword();
	
protected:
    void changeEvent(QEvent *e);

public slots:
	void apply();
	void changed();
	
private:
    Ui::SqlConfig *ui;
};

#endif // SQLCONFIG_H
