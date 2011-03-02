#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>
#include <QtXml>
#include <QStringList>
#include <QString>

class IOManager;

#include "mix.h"

class IOManager : public QObject
{
Q_OBJECT
public:
	enum Status{ NOT_LOADED=0, LOADED_CM3, LOADED_CM4, PARSE_ERROR, FS_ERROR, ERROR };

	IOManager();
	~IOManager();
	
	// store information about entries into simulation file
	enum SimEntry_types{ v_bool, v_double, v_int, v_order, v_method, v_trans };
	typedef struct {
		SimEntry_types vtype;
		union {
			int *intval;
			double *doubleval;
			bool *boolval;
			QString *stringval;
		};
		bool entered;
	} SimEntry;
	
	bool saveToCM3(QString mech, QString sim);
	bool loadFromCM3(QString mech, QString sim);

	bool saveToCM4(QString filename);
	bool loadFromCM4(QString filename);

	Status getStatus() {return status;}
	QString getMessage() {return message;}
	
	void lineUpWhitespace( QList<QString> &lines, int numcols );

	// functions to manage data output in the midst of calculation

	bool openRunOutputFile();
	bool openDebugOutputFile();
	bool openLogFile();

	void printMechSummary(QTextStream& fout);
	void printData( double curTime );

	// public file pointers (just in case)
	QString outputFile, logFile, debugFile;
	QTextStream data, log, debug;
	void setTimeFormat(QTextStream& fp);
	void setDataFormat(QTextStream& fp);
	
public slots:
	void setOutputFile(QString s){ outputFile = s; }
	void setLogFile(QString s)   { logFile = s;    }
	void setDebugFile(QString s) { debugFile = s;  }

private:
	QString getLine( QTextStream& txt, int &linecounter, bool stripcomments = true );
	bool setError( Status stat, QString errmsg, int linenum=-1, QString filename="" );
	
	int outputPrecision, timePrecision;

	Status status;
	QString message;
	
};

extern IOManager *iomgr;

#endif // IOMANAGER_H
