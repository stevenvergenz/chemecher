#ifndef RUNWINDOW_H
#define RUNWINDOW_H

#include <QFrame>
#include <QTimer>
#include <QTime>

#include "mix.h"

namespace Ui {
    class RunWindow;
}

class RunWindow : public QFrame
{
    Q_OBJECT

public:
    explicit RunWindow(QWidget *parent = 0);
    ~RunWindow();

public slots:
	void updateIndicators();
	void startCalculation();
	void abortCalculation();
	void finishCalculation();
	
private:
    Ui::RunWindow *ui;
    QTimer* timer;
    QTime startTime, endTime;
};

#endif // RUNWINDOW_H
