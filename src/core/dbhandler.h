#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>

class DBHandler;

#include "mix.h"

class DBHandler : public QObject
{
Q_OBJECT
public:
	DBHandler();

    // returns a list of the mechanism names
	QStringList mech_list();
    // checks if mechanism exists
    bool mech_exists(QString mechname);

	bool isConnected();

    QString getHostname();
    QString getDB();
    QString getUsername();
    QString getPassword();
	int getPort();

signals:
    void connectionStatus(QString s);

    void hostnameChanged(QString h);
    void dbChanged(QString d);
    void usernameChanged(QString u);
    void passwordChanged(QString p);
	void portChanged(int p);

public slots:
	bool connectToServer();
	void disconnectFromServer();

    void setHostname(QString h);
    void setDB(QString d);
    void setUsername(QString u);
    void setPassword(QString p);
	void setPort(int p);

    // loads the mechanism from the database into Mix namespace
	bool load_mech(QString mechname);
    // saves the data in the Mix namespace to the database as <mechname>
    // if already exists, overwrites
	bool save_mech(QString mechname);

private:
    QSqlDatabase db;

};

#endif // DBHANDLER_H
