#include "dbhandler.h"

using namespace Mix;

DBHandler::DBHandler() :QObject() {
	sqldb = QSqlDatabase::addDatabase("QMYSQL");
	sqldb.setHostName("localhost");
	sqldb.setDatabaseName("chemecher");
	sqldb.setUserName("root");
	sqldb.setPassword("vergenz");
}

// loads the mechanism from the database into Mix namespace
bool DBHandler::loadMech(QString mechname)
{
	sqldb.open();
	
	QSqlQuery query;
	if( !query.exec("SELECT * FROM mechs WHERE MechName=" + mechname + ";") )
		return false;
	if( !query.next() )
		return false;
	
	// load the main mech info
	mechName   = mechname;
	mechDesc   = query.value(1).toString();
	timeStep   = query.value(3).toDouble();
	reportStep = query.value(2).toDouble();
	startTime  = query.value(4).toDouble();
	endTime    = query.value(5).toDouble();
	debugStart = query.value(6).toInt();
	debugEnd   = query.value(7).toInt();
	precision  = query.value(8).toDouble();
	
	/*
+------------+----------------------------------------+------+-----+---------+-------+
| Field      | Type                                   | Null | Key | Default | Extra |
+------------+----------------------------------------+------+-----+---------+-------+
| MechName   | char(20)                               | NO   | PRI | NULL    |       |
| CpdID      | int(11)                                | NO   | PRI | NULL    |       |
| ShortName  | char(2)                                | NO   |     | NULL    |       |
| LongName   | char(30)                               | NO   |     | NULL    |       |
| State      | enum('homo','hetero','aq','s','l','g') | NO   |     | NULL    |       |
| Transition | enum('linear','atan')                  | YES  |     | NULL    |       |
| InitConc   | double                                 | NO   |     | NULL    |       |
+------------+----------------------------------------+------+-----+---------+-------+
	  */
	
	if( !query.exec("SELECT * FROM compounds WHERE MechName=" + mechname + ";") )
		return false;
	while( query.next() ) {
		Cpd *newcpd = new Cpd();
		newcpd->setShortName(   query.value(2).toString() );
		newcpd->setLongName(    query.value(3).toString() );
		newcpd->setState(       query.value(4).toInt()    );
		newcpd->setTransition(  query.value(5).toInt()    );
		newcpd->setInitialConc( query.value(6).toDouble() );
		
	}
	
	sqldb.close();
	return true;
}

// saves the data in the Mix namespace to the database as <mechname>
// if already exists, overwrites
bool DBHandler::saveMech(QString mechname)
{
	//if( sqldb.driver()->hasFeature(QSqlDriver::Transactions) )
	//	qDebug() << "Has transactions! :D";
	
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
