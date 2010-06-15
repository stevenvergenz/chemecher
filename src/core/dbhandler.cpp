#include "dbhandler.h"

DBHandler::DBHandler() :QObject() {
        db = QSqlDatabase::addDatabase("QMYSQL");
}

void DBHandler::connectToServer() {
        bool ok = db.open();
        if(ok) emit connectionStatus("Connection succeeded.");
        else emit connectionStatus("Connection failed.");
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

