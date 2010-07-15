#ifndef SIMPARAMS_H
#define SIMPARAMS_H

#include <QDialog>
#include <QDoubleValidator>

namespace Ui {
    class SimParams;
}

class SimParams : public QDialog {
    Q_OBJECT
public:
    SimParams(QWidget *parent = 0);
    ~SimParams();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SimParams *ui;
};

#endif // SIMPARAMS_H
