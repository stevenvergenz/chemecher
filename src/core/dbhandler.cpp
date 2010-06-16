#include "dbhandler.h"

DBHandler::DBHandler() :QObject() {
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("chemecher");
	db.setUserName("root");
	db.setPassword("vergenz");
}

bool DBHandler::connectToServer() {
	bool ok = db.open();
	if(ok) emit connectionStatus("Connection succeeded.");
	else emit connectionStatus("Connection failed.");
	return ok;
}
bool DBHandler::isConnected() {
	return db.isOpen();
}

void DBHandler::disconnectFromServer()
{
	db.close();
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
	QSqlQuery query;
	query.exec("SELECT * FROM mechs WHERE MechName=" + mechname + ";");
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
	ret += "A";
	ret += "B";
	return ret;
}

// checks if mechanism exists
bool DBHandler::mech_exists(QString mechname)
{
	return false;
}

// getters/setters
QString DBHandler::getHostname() {return db.hostName();}
void DBHandler::setHostname(QString h) {db.setHostName(h); emit hostnameChanged(h);}
QString DBHandler::getDB() {return db.databaseName();}
void DBHandler::setDB(QString d) {db.setDatabaseName(d); emit dbChanged(d);}
QString DBHandler::getUsername() {return db.userName();}
void DBHandler::setUsername(QString u) {db.setUserName(u); emit usernameChanged(u);}
QString DBHandler::getPassword() {return db.password();}
void DBHandler::setPassword(QString p) {db.setPassword(p); emit passwordChanged(p);}
int DBHandler::getPort() {return db.port();}
void DBHandler::setPort(int p) {db.setPort(p); emit portChanged(p);}
