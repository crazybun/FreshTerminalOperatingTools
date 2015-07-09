#include "commprocess.h"
#include <QDebug>
CommProcess::CommProcess(QObject *parent) :
    QObject(parent)
{
    pParaSettings = NULL;
    if(!GetParaSettings (&pParaSettings))
    {//Get the Para u have setted;
        taskInitStatus = PARAERROR;
        return;
    }
    if(pParaSettings->terPara.terAddrOperateType == ADDRASSIGN)
    {//IF ASSIGN The RTU addr
        pTConfig = new TerminalConfig [MAXCONFIG];
        if(!GetTerConfig(pTConfig))
        {
            taskInitStatus = TERMINALADDRERROR;
            return;
        }
    }
    else
    {
        pTConfig = NULL;
    }
    theUpFile = new QFile(pParaSettings->terPara.filePathName);
    if(!theUpFile->open (QIODevice::ReadOnly))
    {
        taskInitStatus = UPFILENOTEXIST;
        return;
    }
     theUpFileCrc16 = fileCrc16Calc(theUpFile->size (),(BYTE*)theUpFile->readAll ().data ());
     theUpFile->close ();
    if(!CommInterfaceInitByPara ())
    {//Init the communitcation Interface
        return;
    }
    taskInitStatus = INITSUCCESS;
}

bool CommProcess::CommInterfaceInitByPara ()
{
    switch(pParaSettings->comPara.comType)
    {
    case SERIAL:
    {
        theSerialPort = new QSerialPort(QString(pParaSettings->comPara.serialName));
        theSerialPort->setBaudRate ((QSerialPort::BaudRate)pParaSettings->comPara.baudRate);
        theSerialPort->setParity ((QSerialPort::Parity)(pParaSettings->comPara.checkBit));
        theSerialPort->setDataBits ((QSerialPort::DataBits)(pParaSettings->comPara.DataBit));
        theSerialPort->setStopBits ((QSerialPort::StopBits)(pParaSettings->comPara.stopBit));
        if(!theSerialPort->open (QIODevice::ReadWrite))
        {
            taskInitStatus = SERIALINITERROR;
            return false;
        }
        connect (theSerialPort,SIGNAL(readyRead()),this,SLOT(SerialDataRecv()));
    }
        break;
    case SERVER:
    {
        switch(pParaSettings->comPara.InPType)
        {
        case TCP:
        {
            if(pParaSettings->terPara.terProtocol == CSG_2015)
            {
                InitTask_UpgradeFile_CSG ();
            }
            else
            {
                InitTask_UpgradeFile_Com((int)_GB13);
            }
            theTcpServer = new TcpServerProc((QObject*)this);
            if(! theTcpServer->listen (QHostAddress::Any,pParaSettings->comPara.sPort))
            {
                taskInitStatus = TCPLISTENERROR;
                return false;
            }
        }
            break;
        case UDP:
        {
            theUdpSocket = new UdpProc(this);
            theUdpSocket->setPortDiffer (true);      //甘肃兰州用端口区分不同的链接
            if(!theUdpSocket->bind(QHostAddress::Any,pParaSettings->comPara.sPort))
            {
                taskInitStatus = BINDERROR;
                return false;
            }
        }
            break;
        case HTTP:
        {

        }
            break;
        }


    }
        break;
    case CLIENT:
    {
        int nTerCount = 0; 
        for(int i = 0;  pTConfig[i].IsUsed;i++)
        {
            nTerCount++;
        }
        switch(pParaSettings->comPara.InPType)
        {
        case TCP:
        {
            InitTask_UpgradeFile_Com_client_HB();
            for(int i = 0;  i < nTerCount; i++)
            {
                QHostAddress host(QString(pParaSettings->comPara.mIP));
                quint16 port = (quint16)pParaSettings->comPara.mPort;
                this->theTcpSocket = new TerTcpSocket(i);
                theTcpSocket->connectToHost (host,port);
                connect(this->theTcpSocket,SIGNAL(NewConnected(TerTcpSocket*)),this,SLOT(NetLinkedProc(TerTcpSocket*)));
                connect(this->theTcpSocket,SIGNAL(NewError(QAbstractSocket::SocketError,TerTcpSocket*)),this,SLOT(LinkErrorProc(QAbstractSocket::SocketError,TerTcpSocket * )));
            }
        }
            break;
        case UDP:
        {
            taskInitStatus = UDPNOTSEVER;
            return false;
        }
            break;
        case HTTP:
        {
            InitTask_UpgradeFile_Http();
            for(int i = 0;  i < nTerCount; i++)
            {
                ThreadProc *newProc = new ThreadProc(pParaSettings,i,*theUpFile,theUpFileCrc16,this);
                newProc->start ();
                threadList.append (newProc);
            }
        }
            break;
        }
    }
        break;
    }
    return true;
}
void CommProcess::NetLinkedProc (TerTcpSocket * terTcpSocket )
{

    int index = -1;
    ThreadProc *theThread = NULL;
    qintptr sockfd = -1;
    if( pParaSettings->comPara.comType == CLIENT )
    {
        index = terTcpSocket->GetTcpSocketSort ();
    }
    theThread = new ThreadProc(pParaSettings,index,*theUpFile,theUpFileCrc16,this);
    switch(pParaSettings->comPara.InPType)
    {
    case TCP:
    {
        sockfd = terTcpSocket->socketDescriptor ();
        theThread->SocketfdTransmit (sockfd);
        terTcpSocket->moveToThread (theThread);
        disconnect (terTcpSocket,SIGNAL(NewReadReady(TerTcpSocket*)),0,0);
    }
        break;
    case UDP:
    {
        sockfd = theUdpSocket->socketDescriptor ();
        theThread->SocketfdTransmit (sockfd);
        theUdpSocket->moveToThread (theThread);
    }
        break;
    }
    threadList.append (theThread);
    theThread->start ();
    
}
void CommProcess::SameLinkProcess (int i)
{
    if(threadList.size () > i)
    {
        threadList.at (i)->GameOver ();
        threadList.removeAt (i);
    }

}
void CommProcess::SerialDataRecv (void)
{

}
void CommProcess::HttpRecv (void)
{

}
void CommProcess::ThreadPause ()
{
    for(int i = 0;  i < threadList.size (); i++)
    {
        threadList.at (i)->SetWorkinPermit (false);
    }
}
void CommProcess::ThreadWork ()
{
    for(int i = 0;  i < threadList.size (); i++)
    {
        threadList.at (i)->SetWorkinPermit (true);
    }
}
void CommProcess::RecLinkProc ()
{

}
void CommProcess::LinkErrorProc (QAbstractSocket::SocketError error,TerTcpSocket * terTcpSocket)
{
    error = error;

    QHostAddress host(QString(pParaSettings->comPara.mIP));
    quint16 port = (quint16)pParaSettings->comPara.mPort;
    terTcpSocket->connectToHost (host,port);
}
