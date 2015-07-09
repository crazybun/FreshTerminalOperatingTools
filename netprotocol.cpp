#include "netprotocol.h"
/////////////////////////////////////////////////////////
/// \brief TcpServerProc::incomingConnection
/// \param sockDescriptor
/// \abstract When  a connection is Coming, it will
///                   triggle a SLOT named MessageRecv Through
///                   SIGNAL named NewReadReady.
///                   当一个TCP链接到来的时候，它将通过发送信号
///                   NewReadReady来触发槽函数NetLinkedProc()
/////////////////////////////////////////////////////////
TcpServerProc::TcpServerProc(QObject *parent)
{//parent should not be set,cause it will result in Thread switch error
    this->setParent (parent);
    parent = parent;
}
void TcpServerProc::incomingConnection(qintptr sockDescriptor)
{
    for(int i = 0 ; i < sockfdList.size();i++)
    {
        if(sockDescriptor == sockfdList.at (i))
        {
            emit TheSameTerLink(i);
            sockfdList.removeAt (i);

        }
    }
    TerTcpSocket *pSocket = new TerTcpSocket();
    pSocket->setSocketDescriptor (sockDescriptor);
    if(portDiffer)
    {
        for(int i = 0 ; i < portList.size();i++)
        {
            if(pSocket->peerPort () == portList.at (i))
            {
                emit TheSameTerLink(i);
                portList.removeAt (i);
            }
        }
    }
    else
    {
        for(int i = 0 ; i < ipList.size();i++)
        {
            if(pSocket->peerAddress () == ipList.at (i))
            {
                emit TheSameTerLink(i);
                 ipList.removeAt (i);
            }
        }
    }
    ipList.append (pSocket->peerAddress ());
    portList.append (pSocket->peerPort ());
    sockfdList.append (sockDescriptor);
    connect (pSocket,SIGNAL(NewReadReady(TerTcpSocket*)),this->parent (),SLOT(NetLinkedProc(TerTcpSocket*)));
    connect(pSocket,SIGNAL(NewDisConnected(TerTcpSocket*)),this,SLOT(NetBrokeProcess(TerTcpSocket*)));
    connect(this,SIGNAL(TheSameTerLink(int)),this->parent (),SLOT(SameLinkProcess(int)));
}
void TcpServerProc::NetBrokeProcess(TerTcpSocket* tcpSocket)
{
    for(int i =0 ; i < sockfdList.size();i++)
    {
        if(tcpSocket->socketDescriptor ()== sockfdList.at (i))
        {
            sockfdList.removeAt (i);
            return;
        }
    }
}
