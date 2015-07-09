#ifndef XMLPROC_H
#define XMLPROC_H

#include <QObject>
#include <QFile>
#include <QDomDocument>
#include "protocolbase.h"
class XMLProc : public QObject , public ProtocolBase
{
    Q_OBJECT
public:
    explicit XMLProc(char* ter_addr ,char*or_DB_Time,QObject *parent = 0);
    void CombinFrame(char* afnstr,BYTE*content,DWORD *size) ;
    void  ParseFrame(char*afnstr,BYTE*content,DWORD *size) ;
    // enum ProtocolException ExceptionProc (void);
    void inline setNedRes(bool bN)
    {
        needRes = bN;
    }
signals:
    
public slots:

private:
    void ParaSettings(char* terAddr,char* content,char* orDebugTime,bool needResp);
    bool ParaGetting( QByteArray ba,char* content,char*terAddr);
    void NodeProc(char*value,QDomNode &node);
    QByteArray GetXMLData(void);
    QDomDocument *pDom;
    QByteArray  theDatagram;
    QByteArray theFileData;
    char teraddr[16];
    char orDBTime[32];
    bool needRes;

};

#endif // XMLPROC_H
