#include "dbhandler.h"

using namespace Mix;

DBHandler::DBHandler() :QObject()
{
	sqldb = QSqlDatabase::addDatabase("QMYSQL");
	sqldb.setHostName("localhost");
	sqldb.setDatabaseName("chemecher");
	sqldb.setUserName("root");
	sqldb.setPassword("vergenz");
}

DBHandler::~DBHandler()
{
	sqldb.close();
}

// loads the mechanism from the database into Mix namespace
bool DBHandler::loadMech(QString mechname)
{
	sqldb.open();
	QSqlQuery query;
	
	// retrieve and load the mechanism info
	if( !query.exec("SELECT * FROM mechs WHERE MechName='" + mechname + "';") )
		return false;
	if( !query.next() )
		return false;
	mechName   = mechname;
	mechDesc   = query.value(1).toString();
	timeStep   = query.value(2).toDouble();
	reportStep = query.value(3).toDouble();
	startTime  = query.value(4).toDouble();
	endTime    = query.value(5).toDouble();
	debugStart = query.value(6).toInt();
	debugEnd   = query.value(7).toInt();
	precision  = query.value(8).toDouble();
	
	// retrieve and add the compounds
	if( !query.exec("SELECT * FROM cpds WHERE MechName='" + mechname + "';") )
		return false;
	while( query.next() ) {
		Cpd *newcpd = new Cpd();
		newcpd->setShortName(   query.value(2).toString() );
		newcpd->setLongName(    query.value(3).toString() );
		newcpd->setState(       query.value(4).toInt()    );
		newcpd->setTransition(  query.value(5).toInt()    );
		newcpd->setInitialConc( query.value(6).toDouble() );
		CpdList.append(newcpd);
	}
	
	// retrieve and add the steps
	if( !query.exec("SELECT * FROM steps WHERE MechName='" + mechname + "';") )
		return false;
	
	/***************************/
	
	sqldb.close();
	return true;
}

// saves the data in the Mix namespace to the database as <mechname>
// if already exists, overwrites
bool DBHandler::saveMech(QString mechname)
{
	sqldb.open();
	
	// start transaction if possible
	if( sqldb.driver()->hasFeature(QSqlDriver::Transactions) )
		sqldb.transaction();
	
	// drop the mechanism (so overwrites if exists)
	dropMech(mechname, false);
	
	// write the mech data
	QSqlQuery query;
	query.prepare("INSERT INTO mechs"
			"(MechName,MechDesc,TimeStep,ReportStep,"
			"StartTime,EndTime,DebugStart,DebugEnd,Precis) "
			"VALUES (?,?,?,?,?,?,?,?,?);");
	query.addBindValue( mechname   );
	query.addBindValue( mechDesc   );
	query.addBindValue( timeStep   );
	query.addBindValue( reportStep );
	query.addBindValue( startTime  );
	query.addBindValue( endTime    );
	query.addBindValue( debugStart );
	query.addBindValue( debugEnd   );
	query.addBindValue( precision  );
	if( !query.exec() )
		return false;
	
	// add the cpds
	/*query.prepare("INSERT INTO cpds"
				  "(MechName,CpdID,ShortName,LongName,State,Transition,InitConc) "
				  "VALUES (?,?,?,?,?,?,?);");
	// loop through the cpds
	QList<Cpd*>::iterator i;
	for( i = CpdList.begin(); i!=CpdList.end(); ++i ) {
		
		query.addBindValue(mechname);
		query.addBindValue();
	}*/
	
	// add the steps
	
	
	// commit transaction if possible
	if( sqldb.driver()->hasFeature(QSqlDriver::Transactions) )
		if( !sqldb.commit() )
			return false;
	
	// change the name, since it acts like `save as`
	mechName = mechname;
	
	return true;
}

// returns a list of the mechanism names
QStringList DBHandler::mechList()
{
	QStringList ret;
	if( !sqldb.open() )
		return ret;
	QSqlQuery query;
	if( !query.exec("SELECT MechName FROM mechs;" ) )
		return ret;
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

// deletes mechanism from the database
bool DBHandler::dropMech(QString mechname, bool trans)
{
	sqldb.open();
	bool existed = mechExists(mechname);
	
	// begin transaction if possible
	if( trans && sqldb.driver()->hasFeature(QSqlDriver::Transactions) )
		sqldb.transaction();
	
	// run the delete queries
	QSqlQuery query;
	if( !(
			query.exec("DELETE FROM mechs WHERE MechName='" + mechname + "';") &&
			query.exec("DELETE FROM cpds  WHERE MechName='" + mechname + "';") &&
			query.exec("DELETE FROM steps WHERE MechName='" + mechname + "';")
		) )
		return false;
	
	// commit transaction if possible
	if( trans && sqldb.driver()->hasFeature(QSqlDriver::Transactions) )
		if( !sqldb.commit() )
			return false;
	
	// return true only if queries succeeded AND mech existed at the beginning
	return existed;
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
