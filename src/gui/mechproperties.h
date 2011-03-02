#ifndef MECHPROPERTIES_H
#define MECHPROPERTIES_H

#include <QDialog>

namespace Ui {
    class MechProperties;
}

class MechProperties : public QDialog
{
    Q_OBJECT

public:
    explicit MechProperties(QWidget *parent = 0);
    ~MechProperties();
    QString name();
    QString description();
    void setName(QString n);
    void setDescription(QString d);
    
private:
    Ui::MechProperties *ui;
};

#endif // MECHPROPERTIES_H
