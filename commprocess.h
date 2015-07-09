#ifndef COMMPROCESS_H
#define COMMPROCESS_H

#include <QObject>
#include <QSerialPort>
#include <QLinkedList>
#include <QList>
#include "netprotocol.h"
#include "threadproc.h"
#include "statusMachine/someprotocoltask.h"
class CommProcess : public QObject
{
    Q_OBJECT
public:
    explicit CommProcess(QObject *parent = 0);
    enum TaskInitStatus GetTaskInitStatus(void)
    {
        return taskInitStatus;
    }
    bool CommInterfaceInitByPara(void);
    void ThreadPause(void);
    void ThreadWork(void);
signals:

public slots:
    void NetLinkedProc(TerTcpSocket*terTcpSocket = NULL);
    void SerialDataRecv(void);
    void HttpRecv(void);
    void RecLinkProc(void);
    void LinkErrorProc(QAbstractSocket::SocketError,TerTcpSocket * );
    void SameLinkProcess(int i);
private:
    QTimer                          *pTimer;
    TParaSettings                   *pParaSettings;
    enum TaskInitStatus              taskInitStatus;
    TcpServerProc                   *theTcpServer;
    TerTcpSocket                    *theTcpSocket;
    UdpProc                         *theUdpSocket;
    QSerialPort                     *theSerialPort;

    QList<ThreadProc *>             threadList;
    quint32                         theTerminalCount;
    TerminalConfig                  *pTConfig;
    QFile                           *theUpFile;
    WORD                            theUpFileCrc16;
    qint64                          theThreadIndex;
};

#endif // COMMPROCESS_H
