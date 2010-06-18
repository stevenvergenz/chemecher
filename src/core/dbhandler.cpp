#include "dbhandler.h"

using namespace Mix;

DBHandler::DBHandler() :QObject() {
	sqldb = QSqlDatabase::addDatabase("QMYSQL");
	sqldb.setHostName("localhost");
	sqldb.setDatabaseName("chemecher");
	sqldb.setUserName("root");
	sqldb.setPassword("vergenz");
}

/*
	//general mix information
	extern QList<Step*> StepList;
	extern QList<Cpd*> CpdList;

	//current mix identifiers
	extern QString mechName;
	extern QFile fileName;

	//time data
	extern double timeStep, reportStep;
	extern int initialTime, maxTime;
	extern int debugStart, debugEnd;

	// sql data
	extern DBHandler db;

	//accuracy data
	extern double precision;
*/

// loads the mechanism from the database into Mix namespace
bool DBHandler::loadMech(QString mechname)
{
	sqldb.open();
	QSqlQuery query("SELECT * FROM mechs WHERE MechName=" + mechname + ";", sqldb);
	
	return false;
}

// saves the data in the Mix namespace to the database as <mechname>
// if already exists, overwrites
bool DBHandler::saveMech(QString mechname)
{
	return false;
}

// returns a list of the mechanism names
QStringList DBHandler::mechList()
{
	sqldb.open();
	QStringList ret;
	QSqlQuery query("SELECT MechName FROM mechs;", sqldb);
	while(query.next()) {
		QString name = query.value(0).toString();
		qDebug() << name;
		ret.append(name);
	}
	return ret;
}

// checks if mechanism exists
bool DBHandler::mechExists(QString mechname)
{
	return mechList().contains(mechname);
}

// getters/setters
QString DBHandler::getHostName() {return sqldb.hostName();}
void DBHandler::setHostName(QString h) {sqldb.setHostName(h); emit hostNameChanged(h);}
QString DBHandler::getDatabase() {return sqldb.databaseName();}
void DBHandler::setDatabase(QString d) {sqldb.setDatabaseName(d); emit databaseChanged(d);}
QString DBHandler::getUserName() {return sqldb.userName();}
void DBHandler::setUserName(QString u) {sqldb.setUserName(u); emit userNameChanged(u);}
QString DBHandler::getPassword() {return sqldb.password();}
void DBHandler::setPassword(QString p) {sqldb.setPassword(p); emit passwordChanged(p);}
int DBHandler::getPort() {return sqldb.port();}
void DBHandler::setPort(int p) {sqldb.setPort(p); emit portChanged(p);}
