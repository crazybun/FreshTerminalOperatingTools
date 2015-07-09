#include "threadproc.h"

ThreadProc::ThreadProc(TParaSettings *p,int index,QFile &Upfile,WORD filecrc,QObject *parent) :
    QThread(parent)
{
    TerComParaInit(p);

    StatusMachineInit();

    UIDisplayInit();

    SendRecvBufferInit();

    ComMethodInit();

    MiscParaInit(index);

    LogFileAddrAssignInit();

    ProtocolInit(Upfile, filecrc);

    connect (this,SIGNAL(Need2Back()),this,SLOT(Back2TheLast()));
    connect (this,SIGNAL(UIInfoSend(TUpgradeUIStruct)),
             this->parent ()->parent (),
             SLOT(UiFresh (TUpgradeUIStruct)));
}

void ThreadProc::run (void)
{

    GetStartTime ();
    SocketReBuild();
    for(;!gameOver;)
    {
        if(!IsWorkinPermit())
        {
            msleep(10);
            continue;
        }
        GoGoStatusMachineRecv();
        DelaySRTime();
        GoGoStatusMachineSend ();
        OverTimeProc ();
    }
}

void ThreadProc::GoGoStatusMachineSend (void)
{
    if(gameOver)
    {
        return;
    }
    TTaskStruct ts = theTaskList.at (theTaskListIndex);

    switch(ts.commType)
    {
    case UpComm:
        break;
    case DownComm:
    {
        DWORD size = 0;
        tMsCount = 0;
        theTaskListLastSendIndex = theTaskListIndex;
        UIFreshFoo(theTaskListIndex);
        for(int i = 0;  i < ts.PStack.length ();i++)
        {
            enum ProtocolStack pStack = ts.PStack.at (i);
            CombineFrameByProtocol(pStack,ts.aFN.at (i).toLatin1 ().data (),&size);
        }
        sendSize = size;
        Write2FileSend ();
        SendOrRecvToByProtocol(pParaSettings->comPara.InPType);
        theTaskListIndex++;                                                                                                         //point to next status
        break;
    }
    default:
        break;
    }
}

void ThreadProc::GoGoStatusMachineRecv (void)
{
    if(!IsRecvInfo ()
            ||gameOver)
    {
        return;
    }
    TTaskStruct ts = theTaskList.at (theTaskListIndex);
    switch(ts.commType)
    {
    case UpComm :
    {
        DWORD size = recvSize;
        tMsCount = 0;
        int i  = ( 0 == ts.PStack.length ())?0:(ts.PStack.length ()-1);
        for(;i >= 0;i--)
        {                                                                                                                     //point to previous status
            enum ProtocolStack pStack = ts.PStack.at (i);
            ParseFrameByProtocol (pStack,ts.aFN.at (i).toLatin1 ().data (),&size);
        }
        if(!THreadIndexGetFaild ())
        {
            if(recvSize != size)
            {
                return;
            }
            Write2FileRecv ();
            UIFreshFoo(theTaskListIndex);
            ThreadMachineJuger();
            if(StatusGoingEnd ())
            {
               StatusEndProc();
            }
        }

    }
        break;
    case DownComm:
        break;
    default:
        break;
    }
}

void ThreadProc::CombineFrameByProtocol(enum ProtocolStack pStack,char*afnStr,DWORD *size)
{
    switch(pStack)
    {
    case _GB09:
        pProtocolBase = gb376_protocol;
        break;
    case _WH:
        pProtocolBase = WSDMaintain_protocol;
        break;
    case _GB13:
        pProtocolBase = gb376_protocol;
        break;
    case _XL:
        break;
    case _SDMASTER:
        pProtocolBase = pXMLProc;
        break;
    case _DTL645_09:
        break;
    case _CSG_2015:
        pProtocolBase = pCsg2015;
        break;
    default:
        break;
    }
    pProtocolBase->CombinFrame (afnStr,sendBuffer,size);
}
void ThreadProc::ParseFrameByProtocol (ProtocolStack pStack, char *afnStr,DWORD *size)
{

    switch(pStack)
    {
    case _GB09:
        pProtocolBase = gb376_protocol;
        gb376_protocol->GetLastSoftwareVersion (theSwVerson);
        upProgress = gb376_protocol->GetUpProgress ();
        break;
    case _WH:
        pProtocolBase = WSDMaintain_protocol;
        upProgress = WSDMaintain_protocol->GetUpProgress();

        break;
    case _GB13:
        pProtocolBase = gb376_protocol;
        gb376_protocol->GetLastSoftwareVersion (theSwVerson);
        upProgress = gb376_protocol->GetUpProgress ();
        break;
    case _XL:
        break;
    case _SDMASTER:
        pProtocolBase = pXMLProc;
        break;
    case _DTL645_09:
        break;
    case _CSG_2015:
        pProtocolBase = pCsg2015;
        upProgress = pCsg2015->GetUpProgress ();
        pCsg2015->GetLastSoftwareVersion (theSwVerson);
        break;
    default:
        break;
    }
    pProtocolBase->GetSwDisDate (LastDisDate);
    pProtocolBase->GetSwDisTime (LastDisTime);
    pProtocolBase->ParseFrame  (afnStr,recvBuffer,size);

    if(NULL == theloginFile)
    {
        pProtocolBase->GetAAT (AAT);
        if (NOADDRASSIGN == pParaSettings->terPara.terAddrOperateType)
        {
            LogFileNoAddrAssignInit(pStack);
        }
        else
        {
            GetIndexThroughAAT(pStack);
        }
    }

}
void ThreadProc::SendOrRecvToByProtocol (enum InProtocolType InProT)
{

    QByteArray ba;
    if(0 == sendSize)
    {
        return;
    }
    ba.resize (sendSize);
    memcpy (ba.data (),sendBuffer,sendSize);
    switch (InProT) {
    case UDP:
    {
        theUdpSocket->writeDatagram (ba,thePeerInfo.IP,thePeerInfo.port);
    }
        break;
    case TCP:
    {
        theTcpSocket->write (ba);
        theTcpSocket->flush ();
    }
        break;
    case HTTP:
        if(! pHttpSoap->onceRequest ((char*)sendBuffer,(char*)recvBuffer))
        {
            theloginFile->WriteToFile ("Http Error!",_OT);
            return;
        }
        recvSize = strlen ((char*)recvBuffer);            //we can use the strlen to calc the xml  size
        if(recvSize == 0)
        {
            return;
        }
        emit HttpReadReay ();                       //send the signal
        break;
    default:
        break;
    }
}

bool ThreadProc::StatusGoingEnd(void)
{
    if(theTaskListIndex+1 >= theTaskList.length ())
    {
        return true;
    }
    return false;
}
void ThreadProc::ProcRecv (void)
{
    GoGoStatusMachineRecv ();
}
void ThreadProc::StatusEndProc ()
{
    GetEndTime ();
    UIFreshFoo(theTaskList.length ()-1);
    GameOver ();
    return;
}
void ThreadProc::Back2TheLast ()
{
    theTaskListIndex = theTaskListLastSendIndex ;
}
void ThreadProc::UIFreshFoo (int taskIndex)
{
    TUpgradeUIStruct TUs;
    TTaskStruct ts = theTaskList.at (taskIndex);
    memset(TUs.ATT,0,16);
    if(TheThreadIndexIsValid ())
    {
        memcpy (TUs.ATT,pTConfig[theThreadIndex].ARCandTAddr,16);
    }
    else
    {
        MakeAddrByProType(TUs.ATT);
    }
    if(taskIndex >= theTaskList.length ()-1)
    {
        TUs.upProgress = 100;
    }
    else
    {
        TUs.upProgress = upProgress;
    }
    TUs.UpStatus = ts.tName;
    TUs.terStartTime = theStartUpTime;
    TUs.finishTime = theFinishTime;
    TUs.VersionAfterUpgrade =QString(pParaSettings->terPara.swVersion);
    TUs.VersionAfterUpgrade +=QString(" | ");
    TUs.VersionAfterUpgrade += QString(pParaSettings->terPara.swReleaseDate);
    TUs.VersionAfterUpgrade +=QString(" | ");
    TUs.VersionAfterUpgrade += QString(pParaSettings->terPara.swReleaseTime);
    TUs.VersionBeforUpgrade = QString(theSwVerson);
    TUs.VersionBeforUpgrade +=QString(" | ");
    TUs.VersionBeforUpgrade += QString(LastDisDate);
    TUs.VersionBeforUpgrade +=QString(" | ");
    TUs.VersionBeforUpgrade += QString(LastDisTime);

    UIInfoSend(TUs);
}
void ThreadProc::OverTimeProc ()
{
    if(pParaSettings->terPara.overTimechecked)
    {
        if(tMsCount > pParaSettings->terPara.overTimeGap)
        {
            emit Need2Back ();
        }
    }
    tMsCount += pParaSettings->terPara.SRDelayTime;
}
void ThreadProc::GetStartTime ()
{
    theStartUpTime = QDateTime::currentDateTime ().toString ( "yyyy-MM-dd hh:mm:ss");
}
void ThreadProc::GetEndTime ()
{
    theFinishTime = QDateTime::currentDateTime ().toString ( "yyyy-MM-dd hh:mm:ss");
}

void ThreadProc::TcpDisconnect()
{
    disconnect (this->theTcpSocket,SIGNAL(NewReadReady(TerTcpSocket*)),0,0);
}

bool ThreadProc::IsRecvInfo(void)
{//RUN RUN ! gadget！
    switch (pParaSettings->comPara.InPType) {
    case UDP:
    {
        TPeerInfo peerInfo;
        QByteArray datagram;
        while(this->theUdpSocket->hasPendingDatagrams())                                                              //数据到来
        {
            datagram.resize (this->theUdpSocket->pendingDatagramSize());
            recvSize =  theUdpSocket->readDatagram (datagram.data (),datagram.size (),&peerInfo.IP,&peerInfo.port);
            if(recvSize <= 0)
            {
                return false;
            }
            if((peerInfo.IP == thePeerInfo.IP)
                    &&(peerInfo.port == thePeerInfo.port))
            {//is current Thread;
                memcpy (recvBuffer,datagram.data (),recvSize);
                return true;
            }

        }
    }
        break;
    case TCP:
    {
        if(0 == (int)theTcpSocket->state ())
        {
            GameOver ();
        }
        theTcpSocket->waitForReadyRead (20);
        this->recvSize = theTcpSocket->read ((char*)this->recvBuffer,MAXBUFFERSIZE);
        if(recvSize > 0)
        {
            return true;
        }
    }
        break;
    case HTTP:
        break;
    default:
        break;
    }
    return false;
}
void ThreadProc::SocketfdTransmit (qintptr socketfd)
{
    CurSocketfd = socketfd;
}
void ThreadProc::SocketReBuild(void)
{
    if(CurSocketfd !=-1)
    {
        theTcpSocket = new TerTcpSocket();
        theTcpSocket->setSocketDescriptor (CurSocketfd);
    }
}
void ThreadProc::Write2FileRecv ()
{
    if(( recvSize > 0 )
            &&(recvSize < 2048))
    {
        QString ss;
        QByteArray datagram;
        datagram.resize (recvSize);

        memcpy (datagram.data(),recvBuffer,recvSize);

        if(HTTP != pParaSettings->comPara.InPType)
        {
            SPECData2String (datagram,ss);
            theloginFile->WriteToFile (ss.toLatin1 ().data (),_RX);
        }
        else
        {
            theloginFile->WriteToFile (datagram.data (),_RX);
        }
    }

}
void ThreadProc::Write2FileSend ()
{
    QString ss;
    QByteArray datagram;
    datagram.resize (sendSize);
    memcpy (datagram.data(),sendBuffer,sendSize);
    if(HTTP != pParaSettings->comPara.InPType)
    {
        SPECData2String (datagram,ss);
        theloginFile->WriteToFile (ss.toLatin1 ().data (),_TX);
    }
    else
    {
        theloginFile->WriteToFile (datagram.data (),_TX);
    }
}

bool inline ThreadProc::IsWorkinPermit(void)
{
    return workinPermit;
}

void inline ThreadProc::DelaySRTime(void)
{
    if(pParaSettings->terPara.SRDelayTime < 10)
    {
        pParaSettings->terPara.SRDelayTime = 10;
    }

    msleep (pParaSettings->terPara.SRDelayTime);
}

void ThreadProc::SetWorkinPermit(bool yes)
{
    workinPermit = yes;
}

void ThreadProc::TerComParaInit(TParaSettings *p)
{
    pParaSettings = new TParaSettings;                          //TerComParaInit(TParaSettings *p)
    memcpy (pParaSettings,p,sizeof(TParaSettings));
}

void ThreadProc::StatusMachineInit(void)
{
    theTaskList = GetFileUpgradeTaskList ();                    //StatusMachineInit(void)
    theTaskListLastSendIndex =  theTaskListIndex = 0;
}

void ThreadProc::UIDisplayInit(void)
{
    upProgress = 0;                                              //UIDisplayInit(void)
    theUUIStruct  = new TUpgradeUIStruct;
    memset(theUUIStruct,0,sizeof(TUpgradeUIStruct));
    memset (theSwVerson,0,5);
    memset (LastDisDate,0,32);
    memset (LastDisTime,0,32);
}

void ThreadProc::SendRecvBufferInit(void)
{
    sendBuffer  = new BYTE[MAXBUFFERSIZE];                      //SendRecvBufferInit(void)
    sendSize = 0;
    recvBuffer = new BYTE[MAXBUFFERSIZE];
    recvSize = 0;
}

void ThreadProc::ComMethodInit(void)
{
    CurSocketfd = -1;                                           //ComMethodInit(void)
}

void ThreadProc::LogFileAddrAssignInit(void)
{
    pTConfig = new TerminalConfig[MAXCONFIG];
    if(GetTerConfig(pTConfig))
    {
        int i = 0;
        for(; pTConfig[i].IsUsed;i++);
        TerminalCount = i;
    }
    else
    {
        delete pTConfig;
        pTConfig = NULL;
    }
    if(TheThreadIndexIsValid ()) //LogFileAddrAssignInit(LogOfUpgrade *pLogFile)
    {//assignment of the address.

        theloginFile = new LogOfUpgrade(NULL,QString(pTConfig[theThreadIndex].ARCandTAddr),pParaSettings->terPara.terAddrType);
    }
    else
    {
        theloginFile = NULL;
    }
}

bool ThreadProc::TheThreadIndexIsValid(void)
{
    if((pParaSettings->terPara.terAddrOperateType == ADDRASSIGN)
            &&(theThreadIndex >= 0))
    {
        return true;
    }
    return false;
}

void ThreadProc::MiscParaInit(int index)
{
    memset (AAT,0,16);                                          //MiscParaInit(void)
    workinPermit = true;
    tMsCount = 0;
    gameOver = false;
    theThreadIndex = index;
}

void ThreadProc::ProtocolInit(QFile &Upfile,WORD filecrc)
{
    switch(pParaSettings->comPara.InPType)                      //ProtocolInit(int index,QFile &Upfile,WORD filecrc)
    {
    case HTTP:
    {//init the xmlProc which named _SDMASTER protocol.
        QString TerAddr(((theThreadIndex >= 0)?(char*)pTConfig[theThreadIndex].ARCandTAddr:(char*)"0xFFFF"));
        pXMLProc = new XMLProc(TerAddr.toLatin1 ().data (),
                               QString::number (pParaSettings->comPara.uLOrFrameDebugTime).toLatin1 ().data ());
        pHttpSoap = new HttpSoap(pParaSettings->comPara.urlLink);
        WSDMaintain_protocol = new WSDMaintain(pParaSettings->terPara.dwFileSegSize,Upfile,filecrc);
        connect (this,SIGNAL(HttpReadReay()),this,SLOT(ProcRecv()));
    }
        break;
    default:
    {
        if((GB13 == pParaSettings->terPara.terProtocol)
                ||(GB09 == pParaSettings->terPara.terProtocol))
        {
            MasterIPPort AFN04F3;
            AFN04F3.mIp = QHostAddress(QString(pParaSettings->terPara.mIp)).toIPv4Address ();
            ReverseMem((BYTE*)&AFN04F3.mIp,4);
            AFN04F3.bakUpIp = QHostAddress(QString(pParaSettings->terPara.bIp)).toIPv4Address ();
            ReverseMem((BYTE*)&AFN04F3.bakUpIp,4);
            AFN04F3.mPort = pParaSettings->terPara.mPort;
            AFN04F3.bakUpPort = pParaSettings->terPara.bPort;
            memcpy (AFN04F3.APN,pParaSettings->terPara.apn,16);
            gb376_protocol = new GB376_1(pParaSettings->terPara.terAddrType, pParaSettings->terPara.terProtocol,
                                         ((theThreadIndex >= 0)?(char*)pTConfig[theThreadIndex].ARCandTAddr:(char*)"0xFFFF"),
                                         pParaSettings->terPara.swVersion);
            gb376_protocol->SetUpFileInfo (pParaSettings->terPara.dwFileSegSize,Upfile,filecrc);
            gb376_protocol->GetAFN04F3 (&AFN04F3);
            gb376_protocol->StrDate_ProtocolDateSwitch((BYTE*)pParaSettings->terPara.swReleaseDate);
        }
        if(CSG_2015 == pParaSettings->terPara.terProtocol)
        {
            pCsg2015 = new csg2015(pParaSettings->terPara.swVersion);
            pCsg2015->SetUpFileInfo (pParaSettings->terPara.dwFileSegSize,Upfile,filecrc);
            QHostAddress BackAddr(QString(pParaSettings->terPara.mIp));
            pCsg2015->SetIPPort ((DWORD)BackAddr.toIPv4Address (),pParaSettings->terPara.mPort);
        }
    }
        break;
    }
}

void ThreadProc::MakeAddrByProType(BYTE * dst)
{
    quint32 a = 0,b= 0;
    QString ss;
    int first = 3,second =3;
    if((GB13 == pParaSettings->terPara.terProtocol)
            ||(GB09 == pParaSettings->terPara.terProtocol))
    {
        if(FADDR == pParaSettings->terPara.terAddrType)
        {
            first = 2;
            second = 4;
        }
        else
        {
            first = 2;
            second = 2;
        }
    }
    else if(CSG_2015 == pParaSettings->terPara.terProtocol)
    {
        first = 3;
        second = 3;
    }
    memcpy(&a,AAT,first);
    memcpy(&b,AAT+first,second);
    if((GB13 == pParaSettings->terPara.terProtocol)
            ||(GB09 == pParaSettings->terPara.terProtocol))
    {
        if(FADDR == pParaSettings->terPara.terAddrType)
        {
            ss.sprintf ("%04x%08x",a,b);
        }
        else
        {
            ss.sprintf ("%04x%04x",a,b);
        }
    }
    else if(CSG_2015 == pParaSettings->terPara.terProtocol)
    {
        ss.sprintf ("%03x%03x",a,b);
    }
    memcpy (dst,ss.toLatin1 ().data (),ss.size ());
}

void ThreadProc::LogFileNoAddrAssignInit(ProtocolStack pStack)
{
    if((pStack == _CSG_2015)
            ||(pStack == _GB13)
            ||(pStack == _GB09))
    {

        BYTE _AAT[16] = {0};
        MakeAddrByProType (_AAT);
        QString ss((char*)_AAT);
        if(NULL == theloginFile)
        {
            theloginFile = new LogOfUpgrade(NULL,ss,pParaSettings->terPara.terAddrType);
        }

    }
}

void ThreadProc::GetIndexThroughAAT(ProtocolStack pStack)
{
    if((pStack == _CSG_2015)
            ||(pStack == _GB13)
            ||(pStack == _GB09))
    {
        WORD Index = 0;
        BYTE _AAT[16] = {0};
        MakeAddrByProType (_AAT);
        if(!BinaryTerSearch (_AAT,(WORD*)&Index))
        {
            GameOver ();
            return ;
        }
        LogFileNoAddrAssignInit(pStack);
        theThreadIndex = (int)Index;
    }
}

void ThreadProc::ThreadMachineJuger(void)
{
    TTaskStruct ts = theTaskList.at (theTaskListIndex);

    switch( pProtocolBase->ExceptionProc ())
    {
    case EXCEPTIONREPEAT:
        theTaskListIndex = theTaskListLastSendIndex;
        break;
    case EXCEPTIONJUMP:
        theTaskListIndex = ts.exceptionJumpIndex;
        break;
    case EXCEPTIONWAIT:
        break;
    case NOEXCEPTION:
        theTaskListIndex++;
        break;
    default:
        break;
    }
}

bool ThreadProc::BinaryTerSearch (BYTE _AAT[], WORD * theIndex)
{
    int uLow = 0, uHigh = this->TerminalCount  -1;
    int nAddrNum = 0;
    if((FADDR == pParaSettings->terPara.terAddrType)
            ||(CSGADDR == pParaSettings->terPara.terAddrType))
    {
        nAddrNum = 12;
    }
    else if(TADDR== pParaSettings->terPara.terAddrType)
    {
        nAddrNum = 8;
    }
    while(uLow <= uHigh)
    {
        int res = memcmp(pTConfig[(uLow+uHigh)/2].ARCandTAddr,_AAT,nAddrNum);
        if(res > 0)
        {
            uHigh = (uLow+uHigh)/2-1;
        }
        if(res < 0)
        {
            uLow = (uLow+uHigh)/2+1;
        }
        if(0 == res )
        {
            if(NULL != theIndex)
            {
                *theIndex =  (uLow+uHigh)/2;
            }
            return true;
        }
    }
    return false;
}

