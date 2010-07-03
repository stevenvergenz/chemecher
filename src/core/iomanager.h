#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include "mix.h"

class IOManager : public QObject
{
public:
	enum Status{ NOT_LOADED=0, LOADED_CM3, LOADED_CM4, PARSE_ERROR, FS_ERROR, ERROR };

	IOManager();

	bool saveToCM3(QString sim, QString mech);
	bool loadFromCM3(QString sim, QString mech);

	bool saveToCM4(QString filename);
	bool loadFromCM4(QString filename);

	Status getStatus() {return status;}
	QString getMessage() {return message;}

private:
	Status status;
	QString message;
};

extern IOManager *iomgr;

#endif // IOMANAGER_H
