#ifndef NETPROTOCOL_H
#define NETPROTOCOL_H
#include "gsoap/soapH.h"
#include <QObject>
#include <QHostInfo>
#include <QMessageBox>
#include <QTcpServer>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QFileDialog>
#include <QLinkedList>
#include <QThread>
#include <QMutex>
#include <QList>
#include <QtNetwork>
#include "BaseType.h"
class UdpProc:public QUdpSocket             // UDP服务客户端
{
    Q_OBJECT
public:

    UdpProc(QObject *parent)
    {
        this->setParent (parent);
        portDiffer = false;
        connect (this,SIGNAL(readyRead()),this,SLOT(GetReadReady()));
        connect (this,SIGNAL(NewReadReady()),this->parent (),SLOT(NetLinkedProc()));
        connect(this,SIGNAL(TheSameTerLink(int)),this->parent (),SLOT(SameLinkProcess(int)));
    }
    void inline setPortDiffer(bool yes)
    {
         portDiffer = yes;
    }
private:
    bool   portDiffer;
    QList<QHostAddress>         ipList;
    QList<quint16>              portList;
public slots:
    void GetReadReady(void)
    {
        if(portDiffer)
        {
            for(int i = 0 ; i < portList.size();i++)
            {
                if(this->peerPort () == portList.at (i))
                {
                    emit TheSameTerLink(i);
                    return;
                }
            }
        }
        else
        {
            for(int i = 0 ; i < ipList.size();i++)
            {
                if(this->peerAddress () == ipList.at (i))
                {
                    emit TheSameTerLink(i);
                    return;
                }
            }
        }
        ipList.append (this->peerAddress ());
        portList.append (this->peerPort ());
        emit NewReadReady ();
    }

signals:
    void NewReadReady(void);
    void TheSameTerLink(int i);
};
class CommProcess;

class TerTcpSocket: public QTcpSocket{
    Q_OBJECT
private:
    qint32 sortNum;
                                //甘肃兰州主站的端口区分
public:
    TerTcpSocket(void)
    {
        sortNum = -1;
        connect (this,SIGNAL(readyRead()),this,SLOT(GetReadReady()));
        connect (this,SIGNAL(connected()),this,SLOT(GetConnected()));
        connect (this,SIGNAL(disconnected()),this,SLOT(Disconnected()));
        connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ErrorProc(QAbstractSocket::SocketError)));
    }
    TerTcpSocket(qint32 thesort)
    {
        sortNum = thesort;
        connect (this,SIGNAL(readyRead()),this,SLOT(GetReadReady()));
        connect (this,SIGNAL(connected()),this,SLOT(GetConnected()));
        connect (this,SIGNAL(disconnected()),this,SLOT(Disconnected()));
        connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ErrorProc(QAbstractSocket::SocketError)));
    }
    qint32 GetTcpSocketSort(void)
    {
        return sortNum;
    }
public slots:
    void GetReadReady(void)
    {
        emit NewReadReady (this);
    }
    void GetConnected(void)
    {
        emit NewConnected (this);
    }
    void Disconnected (void)
    {
        emit NewDisConnected(this);
    }
    void ErrorProc(QAbstractSocket::SocketError error)
    {
        emit NewError(error,this);
    }

signals:
    void NewReadReady(TerTcpSocket *);
    void NewConnected(TerTcpSocket *);
    void NewDisConnected(TerTcpSocket *);
    void NewError(QAbstractSocket::SocketError,TerTcpSocket*);

};
class TcpServerProc: public QTcpServer          //TCP服务端
{
    Q_OBJECT
public:
    explicit TcpServerProc(QObject *parent = 0);
    void setPortDiffer(bool yes)
    {
         portDiffer = yes;
    }
public slots:
    void NetBrokeProcess(TerTcpSocket* tcpSocket);
private:
    bool   portDiffer;
    QList<qintptr>              sockfdList;
    QList<QHostAddress>         ipList;
    QList<quint16>              portList;
protected:
    void incomingConnection (qintptr sockDescriptor);
signals:
    void TheSameTerLink(int i);
};


#endif // NETPROTOCOL_H
