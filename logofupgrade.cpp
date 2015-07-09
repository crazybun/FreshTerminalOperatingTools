#include "logofupgrade.h"
#include <QDebug>
LogOfUpgrade::LogOfUpgrade(BYTE *ter_fileName,QString str_fileName , enum TerAddrType terAddrType)
{
    QString s = logDir.currentPath ();
    if(!s.contains ("UpgradeLog"))
    {
        logDir.mkdir ("UpgradeLog");
    }
    QString ss;
    if(NULL != ter_fileName)
    {
        switch(terAddrType)
        {
        case TADDR:
            ss = QString("UpgradeLog/%1%2%3%4%5%6%7%8.log").arg (ter_fileName[0]/16,0,10).arg (ter_fileName[0]%16,0,10).
                    arg (ter_fileName[1]/16,0,10).arg (ter_fileName[1]%16,0,10).
                    arg (ter_fileName[2]/16,0,16).arg (ter_fileName[2]%16,0,16).
                    arg (ter_fileName[3]/16,0,16).arg (ter_fileName[3]%16,0,16);
            break;
        case FADDR:
        case CSGADDR:
            ss = QString("UpgradeLog/%1%2%3%4%5%6%7%8%9%10%11%12.log").arg (ter_fileName[0]/16,0,10).arg (ter_fileName[0]%16,0,10).
                    arg (ter_fileName[1]/16,0,10).arg (ter_fileName[1]%16,0,10).
                    arg (ter_fileName[2]/16,0,16).arg (ter_fileName[2]%16,0,16).
                    arg (ter_fileName[3]/16,0,16).arg (ter_fileName[3]%16,0,16).
                    arg (ter_fileName[4]/16,0,16).arg (ter_fileName[4]%16,0,16).
                    arg (ter_fileName[5]/16,0,16).arg (ter_fileName[5]%16,0,16);
            break;
        default:
            break;
        }
    }
    else
    {
        ss = QString("UpgradeLog/");
        ss+= str_fileName ;
        ss+=QString(".log");
    }
    this->logFile.setFileName (ss);
    if(!this->logFile.open(QIODevice::WriteOnly))
    {
        this->logFile.close ();
        return;
    }
    if(this->logFile.isOpen ())
    this->logFile.close ();
}

void LogOfUpgrade:: WriteToFile (const char *p, ProcFileType proft)
{
    if(!this->logFile.isOpen ())
    this->logFile.open (QIODevice::Append);
    QString timeS = QString(this->curTime.currentDateTime ().toString (" yyyy-MM-dd hh:mm:ss"));
    QString type;
    QString Last = QString("\r\n");
    if(_TX == proft)
    {
        type = QString("  Tx:");
    }
    if(_RX == proft)
    {
        type = QString("  Rx:");
    }
    if(_OT == proft)
    {
        type = QString(" TOT:");
    }
    QString Contents =timeS+type+QString((const char*)p)+Last;
    this->logFile.write (QByteArray(Contents.toLatin1 ()).data ());
    if(this->logFile.isOpen ())
    this->logFile.close ();
}

void LogOfUpgrade::ClosLogFile (void)
{
    if(this->logFile.isOpen ())
    this->logFile.close ();
}
LogOfUpgrade::~LogOfUpgrade(void)
{
    if(this->logFile.isOpen ())
    this->logFile.close ();
}
