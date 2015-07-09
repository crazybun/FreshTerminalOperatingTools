#ifndef THREADPROC_H
#define THREADPROC_H

#include <QThread>
#include <QtNetwork>
#include <QTimer>
#include "BaseType.h"
#include "statusMachine/someprotocoltask.h"
#include "protocolbase.h"
#include "xmlproc.h"
#include "gsoap/httpsoap.h"
#include "logofupgrade.h"
/***********************the protocol***********************/
#include "GB376_Protocol/gb376_1.h"
#include "Maintain_Protocol/wsdmaintain.h"
#include "CSG_Protocol/csg2015.h"
/*************************************************************/
class CommProcess;
class ThreadProc : public QThread
{
    Q_OBJECT
public:
    explicit ThreadProc(TParaSettings *p,int index,QFile &Upfile,WORD ,QObject *parent = 0);
    void GoGoStatusMachineSend(void);
    void GoGoStatusMachineRecv(void);
    void CombineFrameByProtocol(enum ProtocolStack pStack,char*afnStr,DWORD* size);
    void ParseFrameByProtocol(enum ProtocolStack pStack,char*afnStr,DWORD* size);
    void SendOrRecvToByProtocol (enum InProtocolType InProT);
    bool StatusGoingEnd(void);
    void SetWorkinPermit(bool yes);
    void SocketfdTransmit(qintptr socketfd);
    void SocketReBuild();

    void GetPeerInfo(TPeerInfo peerInfo)
    {
        thePeerInfo = peerInfo;
    }
signals:
    void UIInfoSend(TUpgradeUIStruct   theUUIStruct);
    void HttpReadReay(void);
    void Need2Back(void);                                                       //StatusMachine is going end.
public slots:
    void ProcRecv(void);
    void Back2TheLast(void);
    void TcpDisconnect(void);
    void GameOver(void)
    {
        gameOver = true;
    }
protected:
    void run(void);
private:
    qintptr         CurSocketfd;
    TParaSettings   *pParaSettings;                                               //para settings
    QNetworkAccessManager *theManager;                                          //for http
    QNetworkRequest *theRequest;                                                // for http
    QNetworkReply *theReply;                                                    //for http
    QList<TTaskStruct> theTaskList;                                            //the taskList for status machine running
    quint8                      theTaskListIndex;                              // the taskList index for status machine running
    quint8                      theTaskListLastSendIndex;                      // the Last TaskSend Index
    TUpgradeUIStruct   *theUUIStruct;                                       // the UI data

    ProtocolBase           *pProtocolBase;                                   //the Protocol Base class
    BYTE*                       sendBuffer;                                         // send buffer
    DWORD                        sendSize;
    BYTE*                       recvBuffer;                                         //recv buffer
    DWORD                        recvSize;
    WORD                        TerminalCount;
    TerminalConfig              *pTConfig;                                          //TerminalConfig;
    LogOfUpgrade                *theloginFile;                                      //the Login File
    XMLProc                     *pXMLProc;
    HttpSoap                    *pHttpSoap;
    QTcpSocket               *pTcpSocket;
    QTimer                        *theTimer;
    int                          theThreadIndex;
    /*****************************the protocol*************************************/
    GB376_1                   *gb376_protocol;                                            //国网
    WSDMaintain               *WSDMaintain_protocol;                                       //维护
    csg2015                   *pCsg2015;                                                   //南网
    /****************************the ui**********************************************/
    void UIFreshFoo(int);
    QString theStartUpTime;
    QString theFinishTime;
    char     theSwVerson[5];
    char     LastDisTime[32];
    char     LastDisDate[32];
    char    AAT[16];
    DWORD  upProgress;
    TerTcpSocket *theTcpSocket;
    UdpProc      *theUdpSocket;
    TPeerInfo    thePeerInfo;

    void Write2FileRecv(void);
    void Write2FileSend(void);
    /****************************misc***********************************************/
    DWORD tMsCount;
    bool workinPermit;
    bool gameOver;
    char errCode[256];
    void StatusEndProc(void);

    void inline OverTimeProc(void);

    void inline GetStartTime(void);

    void inline GetEndTime(void);

    bool inline IsWorkinPermit(void);

    bool TheThreadIndexIsValid(void);

    void inline DelaySRTime(void);

    bool IsRecvInfo(void);

    void TerComParaInit(TParaSettings *p);

    void StatusMachineInit(void);

    void UIDisplayInit(void);

    void SendRecvBufferInit(void);

    void ComMethodInit(void);

    void LogFileAddrAssignInit(void);

    void LogFileNoAddrAssignInit(ProtocolStack pStack);

    void MiscParaInit(int index);

    void ProtocolInit(QFile &Upfile,WORD filecrc);

    void MakeAddrByProType(BYTE *);

    void ThreadMachineJuger(void);

    void GetIndexThroughAAT(ProtocolStack pStack);

    bool BinaryTerSearch (BYTE _AAT[],WORD * theIndex);

    bool inline THreadIndexGetFaild(void)
    {
        return (theThreadIndex >= 0)?false:true;
    }


};

#endif // THREADPROC_H
