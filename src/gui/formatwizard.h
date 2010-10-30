#ifndef FORMATWIZARD_H
#define FORMATWIZARD_H

#include <QFrame>

/*
double arrow = 0x21c4 ⇄
single arrow = 0x2192 →
*/

namespace Ui {
    class FormatWizard;
}

class FormatWizard : public QFrame {
    Q_OBJECT
public:
    FormatWizard(QWidget *parent = 0);
    ~FormatWizard();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FormatWizard *ui;
};

#endif // FORMATWIZARD_H
