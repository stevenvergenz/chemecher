#ifndef NEWCPDDIALOG_H
#define NEWCPDDIALOG_H

#include <QDialog>
#include "ui_newcpddialog.h"
#include "mix.h"

class NewCpdDialog : public QDialog {
    Q_OBJECT
public:
    NewCpdDialog(QWidget *parent = 0);
	QString getName();
	int getState();

protected:
    void changeEvent(QEvent *e);

public slots:
    void accept();

private:
    Ui::NewCpdDialog ui;
};

#endif // NEWCPDDIALOG_H
