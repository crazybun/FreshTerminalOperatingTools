#ifndef BASETYPE_H
#define BASETYPE_H

#include <string.h>
#include <math.h>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QtAlgorithms>
#include <QHostAddress>
typedef unsigned char BYTE ;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
#pragma pack(push)
#pragma pack(1)
typedef enum OperateTyep{
    UPGRADE_TYPE,
    PARASET_TYPE,
    SLAVE_MODE_TYPE
}OperateType;
typedef enum AFN0FYTPE{
   FILE_HEAD,
   FILE_CONTENT,
   FILE_TAIL
}T0FTYPE;
typedef struct AFNFNUPINFO{
    BYTE afn;
    WORD fn;
    T0FTYPE ft;
}TAFNFNUpInfo;
typedef struct AFNDI{
    BYTE afn;
    DWORD uDI;
}TAFNDI;
typedef struct AFNDA{
    BYTE DA1;
    BYTE DA2;
}TDA;
typedef  struct  TypicalTime{
    BYTE byHYear;
    BYTE byLYear;
    BYTE Mon;
    BYTE Day;
    BYTE Hour;
    BYTE Min;
    BYTE Sec;
}TypicalTime;
enum TaskInitStatus{
    PARAERROR,
    UPFILENOTEXIST,
    SERIALINITERROR,
    TCPLISTENERROR,
    BINDERROR,
    TERMINALADDRERROR,
    INITSUCCESS,
    UDPNOTSEVER
};
enum ProtocolException{
  NOEXCEPTION,                                                                      //No exception
  EXCEPTIONJUMP,                                                                  //exception of jump
  EXCEPTIONREPEAT,                                                              //need to repeat
  EXCEPTIONWAIT     ,                                                            //need to wait
  EXCEPTIONENSURE  ,                                                             //need to ensure directly.
  EXCEPTIONOVERPROC                                                              //need to overtime Process
};
enum InProtocolType{                                                             //协议类型 Protocol Type
   TCP,
    UDP,
    HTTP
};
enum TerProtocol{
    GB09 ,
    GB13 ,
    WH,
    CSG_2015
};
enum MasterProtocol{
    XL,
    NONE
};
enum MeterProtocol{
    DTL645_09
};
enum ProtocolStack{
    _GB09,
    _WH,
    _GB13,
    _XL,
    _SDMASTER,                                                                   //ShanDong master mode
    _DTL645_09,
    _CSG_2015
};
enum TerAddrType{
    TADDR,
    FADDR,
    CSGADDR
};
enum ComType{                                                                    //CS模式      the Client-Server Mode
    SERIAL,
    SERVER ,
    CLIENT
};
enum CheckType{
    NONEP = 0,
    EVEN = 2,
    ODD = 3
};
enum DataBit{
    DATA5 = 5,
    DATA6,
    DATA7,
    DATA8
};
enum ConfigType{
  MULTI = 10,
  SINGLE
};
enum AddrType{
   HEX_ADDR = 10,
   DEC_ADDR
};
enum status{
    LINK_ERR,
    LINK_OK
};
enum WillsendCommd{
    SURE,
    DENY,
    OTHER
};
enum ProcFileType{
    _TX ,                                                               //发送信息
    _RX,                                                                    //接收信息
    _OT                                                                 //其它信息
};
enum terminalOnlineStatus{
    ONL = 0,
    NONL
};
enum TerminalAddrOperateType{
    NOADDRASSIGN,                                                                              //不指定终端地址
    ADDRASSIGN                                                                                    //指定终端地址
};
enum TerminalType{
    SPETERMINAL,                                                                                    //专变终端
    CONCENTRATOR_I,                                                                             //集中器I型
    CONCENTRATOR_II
};
enum SpeTerType{
    TPAFWS200V,
    TPATWS100V,
    TPAFWS57V
};
typedef struct _XLMasterPara
{
    char userName[50];
    char passwd[50];
    char channelNum[16];
    char pVersion[32];
    char heartBeatTime[32];
    char tranWms[32];

}TXLMasterPara;
struct _ParaSettings                                     //需要设置的参数
{
    //enum SCType scType[1];                                     //客户端还是服务器模式 TRUE客户端，FALSE服务器
    char softwareVersion[32];                           //软件版本
    char protocolType[32];                              //协议类型
    char MIP[32];                                       //客户端时链接的服务器IP
    char MPort[32];                                     //客户端时链接的服务器端口
    char MPort_s[32];                                   //做服务器时绑定的端口
    char MIP_C[32];                                     //现场主站IP
    char MIP_CBAK[32];                                     //现场备IP
    char MPort_C[32];                                   //现场主站端口
    char MPort_CBAK[32];                                   //现场主站端口
    char APN_C[32];                                     //现场主站APN
    char WaittingDelay[32];                             //发送延时
    char ReadOverTime[32];                              //接收最大延时
    char FILENAME[256];                                      //升级文件名             
    DWORD FILESEGMENT;                      //每段的文件大小
    enum TerAddrType theTerAddrType;                //规范不同所对应的地址类型
    DWORD TimeTickDelay;
   TXLMasterPara XLPara;                                        //新联主站参数

    bool bModified;
};

typedef struct _TerminalConfig                  //终端配置
{
    char ARCandTAddr[16];
    bool IsUsed;
    enum AddrType AddrType;
} TerminalConfig;
enum CommType{
    UpComm = 10,
    DownComm,
    NoComm
};
typedef struct _376_header
{
    BYTE start68 ;
    BYTE L1[2];                       //D0-D1：0 and 1 this Protocol defined.
    BYTE L2[2];
    BYTE  end68;
}TGB376_header;

typedef  struct _376Controlfied
{
    BYTE funCode:4;
    BYTE FCV:1;                            //下行
    BYTE FCB_ACD:1;                //上行下行
    BYTE PRM:1;
    BYTE DIR:1;
}TGB376_CF;
typedef TGB376_header TCSG_Header;
typedef TGB376_CF        TCSG_CF;
typedef struct _376addr
{
    BYTE bcdARC[2];              //BCD行政区划码
    BYTE binTAddr[2];           //BINTerminalADDr
    BYTE AandTAddrFlag;    //FLAG

}TGB376_ARCandTaddr;
typedef struct _csg_addr{
    BYTE bcdARC[3];
    BYTE binTAddr[3];
    BYTE AandTAddrFlag;
}TCSG_AddrFiled;

typedef struct _376addrjl
{
    BYTE bcdARC[2];              //BCD行政区划码
    BYTE binTAddr[4];           //BINTerminalADDr
    BYTE AandTAddrFlag;    //FLAG

}T4ADDRTGB376_ARCandTaddr;
typedef struct _376app
{
    BYTE DA1;              //pn
    BYTE DA2;
    BYTE DT1;              //fn
    BYTE DT2;
}TGB376_App;
typedef struct _csg_app
{
    BYTE DA1;              //pn
    BYTE DA2;
    DWORD  ulDI;
}TCSG_App;
typedef struct  _376afnInfo
{
    BYTE AFN;
    BYTE SEQ;
}TGB376_AFNInfo;
typedef TGB376_AFNInfo TCSG_AFNInfo;
typedef  struct _376tail
{
    BYTE  cs;
    BYTE  end;
}TGB376_Tail;
typedef TGB376_Tail TCSG_Tail;
typedef struct _376_full_frame_lack_tail
{
    TGB376_header header;
    TGB376_CF    controField;
    TGB376_ARCandTaddr AandT;
     TGB376_App app;
}TGB376_FULL_FRAME_LACK_TAIL ;
typedef struct _jl376_full_frame_lack_tail
{
    TGB376_header header;
    TGB376_CF    controField;
     T4ADDRTGB376_ARCandTaddr AandT;
     TGB376_App app;
}JLTGB376_FULL_FRAME_LACK_TAIL ;
typedef struct  __04f299timestamp{
    BYTE yearh;
    BYTE yearl;
    BYTE   mon;
    BYTE   day;
    BYTE   hour;
    BYTE   min;
    BYTE   sec;
    BYTE   mmsh;
    BYTE   mmsl;
}_04f299timestamp;

typedef struct _MasterIPPort
{
    DWORD mIp;                                  //主IP
    WORD mPort;                                  //主端口
    DWORD bakUpIp;                                //备IP
    WORD bakUpPort;                              //备端口
    BYTE APN[16]   ;                                 //APN

}MasterIPPort;
#pragma pack(pop)
#define  MAXCONFIG  65535                             //最多终端个数
#define CONFIGNAME  "TerminalConfig"
#define MAXCRCSIZE      1024
#define COMPARASFILENMAE          "CommunicatPara"                           //通信参数文件
#define TERPARASFILENAME           "TerminalPara"                                  //终端参数文件
#define BACKGROUNDIMAGIN          "./new/prefix1/Tres/flfd.png"             //背景图片
#define MAXBUFFERSIZE                       2048
typedef struct _com_para_settings{
    enum ComType comType;                       //通信参数名
    //Serial
    char                    serialName[32];
    DWORD            baudRate;
    enum CheckType          checkBit;
    BYTE             stopBit;
    enum DataBit     DataBit;
    //InternetProtocol
    enum InProtocolType InPType;
    //Client
    char                    mIP[32];
    WORD           mPort;
    //Server
    WORD          sPort;
    char                    urlLink[256];
    DWORD            uLOrFrameDebugTime;
}TComParaSettings;
typedef struct _Ter_para_settings{
    bool isChecked[3];                                                  //0:升级,1:规约,2:杂项
    //Upgrade
    char mIp[32];
    WORD mPort;
    char bIp[32];
    WORD bPort;
    char apn[32];
    char swVersion[32];
    char swReleaseDate[32];
    char filePathName[256];
    //Protocol
    enum TerProtocol terProtocol;
    enum MasterProtocol masterProtocol;
    enum MeterProtocol meterProtocol;
   //Misc
    enum TerAddrType terAddrType;
    //XL
    TXLMasterPara  xlMasterPara;
    enum TerminalAddrOperateType terAddrOperateType;
    enum TerminalType terType;
    enum SpeTerType speTerType;
    bool overTimechecked;
    DWORD overTimeGap;
    char swReleaseTime[32];
    DWORD SRDelayTime;
    DWORD dwFileSegSize;
}TTerParaSettings;
typedef struct _para_settings{
    TComParaSettings comPara;
    TTerParaSettings terPara;
}TParaSettings;
//BaseFunc.cpp
typedef struct UIStruct{
    BYTE ATT[16];
    QString UpStatus;
    DWORD upProgress;
    QString terStartTime;
    QString finishTime;
    QString VersionBeforUpgrade;
    QString VersionAfterUpgrade;
    QString TerType;
    QString TerStatus;
}TUpgradeUIStruct;
typedef struct peerInfo
{
    QHostAddress IP;
    quint16      port;
}TPeerInfo;
void MakeCrc16Tab(WORD *Crc16Table);
WORD Crc16(WORD crc,WORD *Crc16Table,void *pvBuf,DWORD count);
WORD _Crc16(BYTE *fileBuffer,DWORD fileLen);
WORD fileCrc16Calc (DWORD fileLen,BYTE *fileBuffer);
void InsertConfig(TerminalConfig configArr[],TerminalConfig newConfigenum ,TerAddrType type);
void TerCAddr2BAddr(BYTE CAAT[],BYTE BAddr[]);
DWORD Dec2Bcd(int x);
DWORD Bcd2Dec(DWORD x);
char *Sfilter( char *str1,const char *str2);
int TerAddrCmp(BYTE _AAT[],BYTE __AAT[],enum TerAddrType type);
bool GetParaSettings (  TParaSettings **pParaSettings_);
bool TerConfigCmp(TerminalConfig t1,TerminalConfig t2);
bool GetTerConfig (TerminalConfig* pTConfig);
void SPECData2String (QByteArray datagram,QString & ss);
void addrTrans (QByteArray datagram,QString & ss);
void String2SPECData(QString ss,QByteArray & datagram);
void StrString2SPECData(const char* strString,QByteArray & datagram);
BYTE ByteCheckSum(const BYTE* buf,int index);
void ReverseMem(BYTE *str,DWORD strlen);

void CommonPlaceMessageBoxWork(const char * Titil,const char * msg);
#endif // BASETYPE_H
