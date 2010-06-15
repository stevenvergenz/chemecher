#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class DBHandler : public QObject
{
Q_OBJECT
public:
	DBHandler();

    // returns a list of the mechanism names
    QList<QString> mech_list();
    // checks if mechanism exists
    bool mech_exists(QString mechname);

    QString getHostname();
    QString getDB();
    QString getUsername();
    QString getPassword();

signals:
    void connectionStatus(QString s);

    void hostnameChanged(QString h);
    void dbChanged(QString d);
    void usernameChanged(QString u);
    void passwordChanged(QString p);

public slots:
    void connectToServer();

    void setHostname(QString h);
    void setDB(QString d);
    void setUsername(QString u);
    void setPassword(QString p);

    // loads the mechanism from the database into Mix namespace
    void load_mech(QString mechname);
    // saves the data in the Mix namespace to the database as <mechname>
    // if already exists, overwrites
    void save_mech(QString mechname);

private:
    QSqlDatabase db;

};

#endif // DBHANDLER_H
