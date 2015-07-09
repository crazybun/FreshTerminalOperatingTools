#include "csg2015.h"

csg2015::csg2015(char* softVersion )
{
    InitAFNFunc();
    memset(theAAT,0,7);
    memset(theSoftVersion,0,5);
    memcpy (theSoftVersion,softVersion,4);
    memset(theLastSoftVersion,0,5);
    upProgress = 0;
}
csg2015::~csg2015 ()
{

}
void csg2015::SetUpFileInfo (DWORD dwFileSegSize, QFile &file, WORD _fileCrc16)
{
    memset(fileName,0,64);
    memcpy(fileName,Sfilter (file.fileName ().toLatin1 ().data (),"/"),64);
    fileSegSize = dwFileSegSize;
    upFile.setFileName (file.fileName ());
    upFile.open (QIODevice::ReadOnly);
    fileSegCnt = (upFile.size()/fileSegSize)+(((upFile.size ()/fileSegSize)==0)?0:1);
    fileCrc16 = _fileCrc16;
}
void csg2015::CombinFrame(char* afnstr,BYTE*pContent,DWORD *size)
{
    pDwnStart =  pDwnContent =(char*) pContent;
    currentComType = DownComm;
    TAFNDI  afnDI = GetAFNDIbyStr (afnstr);
    (this->*afnFunc[afnDI.afn])(afnDI.uDI,size);
}
void csg2015::ParseFrame(char*afnstr,BYTE*pContent,DWORD *size)
{
    pUpStart =  pUpContent =(char*) pContent;
    QByteArray ba = QByteArray((char*)pContent,*size);
    QString ss;
    SPECData2String (ba,ss);
    currentComType = UpComm;
    TAFNDI  afnDI = GetAFNDIbyStr (afnstr);
    (this->*afnFunc[afnDI.afn])(afnDI.uDI,size);
}
TAFNDI csg2015::GetAFNDIbyStr (char *afnStr)
{
    TAFNDI afndi;
    bool ok;
    char buf[9] = {0},*p;
    p = strstr(afnStr,"AFN");
    p+=3;
    memcpy (buf,p,2);
    afndi.afn = QString(buf).toUShort (&ok,16);
    p+=2;
    memcpy (buf,p,8);
    afndi.uDI = QString (buf).toULong (&ok,16);
    return afndi;
}
