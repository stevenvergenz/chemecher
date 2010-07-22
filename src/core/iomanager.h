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
public:
	enum Status{ NOT_LOADED=0, LOADED_CM3, LOADED_CM4, PARSE_ERROR, FS_ERROR, ERROR };

	IOManager();
	
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
	
	QString mechfile, simfile;
	
private:
	QString getLine( QTextStream& txt, int &linecounter, bool stripcomments = true );
	bool setError( Status stat, QString errmsg, int linenum=-1, QString filename="" );
	
	Status status;
	QString message;
	
};

extern IOManager *iomgr;

#endif // IOMANAGER_H
