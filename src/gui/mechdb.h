#ifndef MECHDB_H
#define MECHDB_H

#include <QDialog>

namespace Ui {
    class mechdb;
}

class mechdb : public QDialog {
    Q_OBJECT
public:
    mechdb(QWidget *parent = 0);
    ~mechdb();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::mechdb *ui;
};

#endif // MECHDB_H
