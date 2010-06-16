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
	
	QString getHostname();
	QString getUsername();
	QString getPassword();
	
protected:
    void changeEvent(QEvent *e);

private:
    Ui::SqlConfig *ui;
};

#endif // SQLCONFIG_H
