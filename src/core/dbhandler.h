#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

class DBHandler;

#include "mix.h"

class DBHandler : public QObject
{
Q_OBJECT
public:
	DBHandler();
	
    QSqlDatabase sqldb;

    // returns a list of the mechanism names
	QStringList mechList();
    // checks if mechanism exists
    bool mechExists(QString mechname);

    QString getHostName();
    QString getDatabase();
    QString getUserName();
    QString getPassword();
	int getPort();

signals:
    void connectionStatus(QString s);

    void hostNameChanged(QString h);
    void databaseChanged(QString d);
    void userNameChanged(QString u);
    void passwordChanged(QString p);
	void portChanged(int p);

public slots:

    void setHostName(QString h);
    void setDatabase(QString d);
    void setUserName(QString u);
    void setPassword(QString p);
	void setPort(int p);

    // loads the mechanism from the database into Mix namespace
	bool loadMech(QString mechname);
    // saves the data in the Mix namespace to the database as <mechname>
    // if already exists, overwrites
	bool saveMech(QString mechname);

};

#endif // DBHANDLER_H
