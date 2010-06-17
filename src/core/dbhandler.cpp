#include "dbhandler.h"

DBHandler::DBHandler() : QObject() {
	QSqlDatabase::QSqlDatabase();
	QSqlDatabase::addDatabase("QMYSQL" );
	QSqlDatabase::setHostName("localhost");
	qDebug() << "hostname: " + QSqlDatabase::hostName();
	setDatabaseName("chemecher");
	setUserName("root");
	setPassword("vergenz");
}

bool DBHandler::open() {
	bool ok = QSqlDatabase::open();
	if(ok) emit connectionStatus("Connection succeeded.");
	else emit connectionStatus("Connection failed.");
	return ok;
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
bool DBHandler::load_mech(QString mechname)
{
	//QSqlQuery query;
	//query.exec("SELECT * FROM mechs WHERE MechName=" + mechname + ";");
	//if
	return false;
}

// saves the data in the Mix namespace to the database as <mechname>
// if already exists, overwrites
bool DBHandler::save_mech(QString mechname)
{
	return false;
}

// returns a list of the mechanism names
QStringList DBHandler::mech_list()
{
	QStringList ret;
	ret += "Abcdlkfjsdaf";
	ret += "Badfjjf  jasdjfk";
	ret += "Csdfjjkbje";
	return ret;
}

// checks if mechanism exists
bool DBHandler::mech_exists(QString mechname)
{
	return false;
}

// getters/setters
QString DBHandler::getHostname() {return QSqlDatabase::hostName();}
void DBHandler::setHostname(QString h) {QSqlDatabase::setHostName(h); emit hostnameChanged(h);}
QString DBHandler::getDB() {return QSqlDatabase::databaseName();}
void DBHandler::setDB(QString d) {QSqlDatabase::setDatabaseName(d); emit dbChanged(d);}
QString DBHandler::getUsername() {return QSqlDatabase::userName();}
void DBHandler::setUsername(QString u) {QSqlDatabase::setUserName(u); emit usernameChanged(u);}
QString DBHandler::getPassword() {return QSqlDatabase::password();}
void DBHandler::setPassword(QString p) {QSqlDatabase::setPassword(p); emit passwordChanged(p);}
int DBHandler::getPort() {return QSqlDatabase::port();}
void DBHandler::setPort(int p) {QSqlDatabase::setPort(p); emit portChanged(p);}
