#ifndef LOGOFUPGRADE_H
#define LOGOFUPGRADE_H
#include "BaseType.h"
#include <QTime>
#include <QDir>
#include <QFile>
class LogOfUpgrade
{
private:
    QFile  logFile;
    QDir  logDir;
    QDateTime curTime;
public:

    LogOfUpgrade(BYTE *ter_fileName,QString str_fileName , enum TerAddrType);
    ~LogOfUpgrade();
   void WriteToFile(const  char *p,enum ProcFileType proft);
   void ClosLogFile(void);
};

#endif // LOGOFUPGRADE_H
