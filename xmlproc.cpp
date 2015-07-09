#include "xmlproc.h"
#include <QDebug>
XMLProc::XMLProc(char* ter_addr ,char*or_DB_Time,QObject *parent ) :
    QObject(parent)
{
    pDom = new QDomDocument("xml");
    memset(teraddr,0,16);
    memset (orDBTime,0,32);
    if(NULL != ter_addr)
    memcpy (teraddr,ter_addr,strlen (ter_addr));
    if(NULL!= or_DB_Time)
    memcpy (orDBTime,or_DB_Time,strlen(or_DB_Time));
    QFile                   xmlFile;
    xmlFile.setFileName ("sendSourceFrameByRtuAddress.xml");
    if(!xmlFile.open (QIODevice::ReadOnly))
    {
        return;
    }
    theDatagram = theFileData = xmlFile.readAll ();
    setNedRes (true);
}
void XMLProc::ParaSettings (char *terAddr, char *content, char *orDebugTime, bool needResp)
{
    pDom->setContent (theFileData);
    QDomNodeList nodeList = pDom->childNodes ();
    int target = 0; 
    for(int i  = 0;  i < nodeList.length ();i++)
    {
        QDomNode node = nodeList.at (i);
         if(0 == node.nodeName ().compare (QString("para")))
         {
             target = i;
             break;
         }
    }
     nodeList =  pDom->childNodes ().at (target).childNodes ();
      for(int i  = 0;  i < nodeList.length ();i++)
      {
        QDomNode node = nodeList.at (i);
        if(0 == node.nodeName ().compare (QString("rtuAddress")))
        {
            NodeProc (terAddr,node);
        }
        if(0 == node.nodeName ().compare (QString("frameContent")))
        {
            NodeProc (content,node);
        }
        if(0 == node.nodeName ().compare (QString("timeOut")))
        {
           NodeProc (orDebugTime,node);
        }
        if(0 == node.nodeName ().compare (QString("needResponse")))
        {
            QString ss;
            if(needResp)
            {
                ss = QString("1");
            }
            else
            {
                ss = QString("0");
            }
            NodeProc (ss.toLatin1 ().data(),node);
        }
    }
    theDatagram = pDom->toByteArray ();
    qDebug() << theDatagram;
}
bool XMLProc::ParaGetting (QByteArray ba, char *content, char *terAddr)
{
    pDom->setContent (ba);
    QDomNodeList nodeList = pDom->childNodes ();
    int target = -1;
    for(int i  = 0;  i < nodeList.length ();i++)
    {
        QDomNode node = nodeList.at (i);
         if(0 == node.nodeName ().compare (QString("result")) ||
              0 == node.nodeName ().compare (QString("exception")) )
         {
             target = i;
             break;
         }
    }
     nodeList =  pDom->childNodes ().at (target).childNodes ();
    for(int i  = 0;  i < nodeList.length ();i++)
    {
        QDomNode node = nodeList.at (i);
        if(0 == node.nodeName ().compare (QString("terAddress")))
        {
           memcpy (terAddr,node.firstChild ().nodeValue().toLatin1 ().data (),node.firstChild ().nodeValue().length ());
        }
        if(0 == node.nodeName ().compare (QString("frameContent")))
        {
           memcpy (content,node.firstChild ().nodeValue().toLatin1 ().data (),node.firstChild ().nodeValue().length ());
        }
        if(0 == node.nodeName ().compare (QString("exceptionContent")))
        {
          memcpy (content,node.firstChild ().nodeValue().toLatin1 ().data (),node.firstChild ().nodeValue().length ());
          return false;
        }
    }
    return true;
}

void XMLProc::NodeProc (char *value,QDomNode &node)
{
    QDomNode oldNode = node.firstChild ();
    node.firstChild ().setNodeValue (value);
     QDomNode newNode = node.firstChild ();
     node.replaceChild (newNode,oldNode);
}
QByteArray XMLProc::GetXMLData(void)
{
    return theDatagram;
}

void XMLProc::CombinFrame (char *afnstr, BYTE *content,DWORD *size)
{
    afnstr = afnstr;
    QByteArray orgData;
    QString ss;
    orgData.resize (*size);
    memcpy (orgData.data (),content,*size);
    SPECData2String (orgData,ss);
    //put the data into xml type.
    ParaSettings (teraddr,ss.toLatin1 ().data (),orDBTime,needRes);
    memcpy (content,theDatagram.data (),theDatagram.size ());
    *size = theDatagram.size ();
}
void XMLProc::ParseFrame (char *afnstr, BYTE *content,DWORD *size)
{
     afnstr = afnstr;
    QByteArray orgData;
    orgData.resize (*size);
    memcpy (orgData.data (),content,*size);
    if(!ParaGetting (orgData,orgData.data (),teraddr))
    {//Get the Recved xml content or exception;
        memcpy (content,orgData.data (),orgData.size ());
        return;
    }
    QString ss(orgData);
    StrString2SPECData (ss.toLatin1 ().data (),orgData);
    memcpy (content,orgData.data (),orgData.size ());
}
