#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>

class IOManager;

#include "mix.h"

class IOManager : public QObject
{
public:
	enum Status{ NOT_LOADED=0, LOADED_CM3, LOADED_CM4, PARSE_ERROR, FS_ERROR, ERROR };

	IOManager();

	bool saveToCM3(QString mech, QString sim);
	bool loadFromCM3(QString mech, QString sim);

	bool saveToCM4(QString filename);
	bool loadFromCM4(QString filename);

	Status getStatus() {return status;}
	QString getMessage() {return message;}

private:
	Status status;
	QString message;

	QString getLineWithoutComments(QTextStream& txt);
};

extern IOManager *iomgr;

#endif // IOMANAGER_H
