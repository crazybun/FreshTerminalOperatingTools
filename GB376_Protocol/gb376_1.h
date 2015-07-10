#ifndef TGB376_1_H
#define TGB376_1_H
#include "../BaseType.h"
#include "../protocolbase.h"
#pragma pack(push)
#pragma pack(1)
typedef struct _TerminalVersion
{
    BYTE indasCode[4];                         //厂家代号
    BYTE deviceCode[8];                        //设备编号
    BYTE terSWVersoin[4];                  //终端软件版本号
    BYTE sWDisDate[3];                            //软件发布日期
    BYTE  TerVConfigCode[11];                  //终端容量配置信息码
    BYTE TerProtocolV[4];                          //终端通信协议版本号
    BYTE TerHWVersion[4];                      //终端硬件版本;
    BYTE terHWdisDate[3];                      //终端硬件版本号；

}TTerminalVersion;
typedef struct pw{
        BYTE pw[16];
}TPW;
typedef struct _FileTransfer
{
    BYTE fileTags;                                               //文件标识
    BYTE fillePro;                                                //文件属性
    BYTE fileCmd;                                                //文件指令
    WORD sumSegment;                                       //总段数n
    DWORD fileOffset;                                               //偏移量   第i段数据
    WORD dataLOfn;                                            //第i段数据长度
}  FileTransfer;
typedef struct _fileNameDomain
{
    BYTE filename[16];
    BYTE fTime[6];                    //秒分时日月年
}fileNameDomain ;

typedef struct functionN{
    BYTE DT1;
    BYTE DT2;
} TFN;

#pragma pack(pop)
class GB376_1;
typedef void (GB376_1::*AFNSeries)(TFN,DWORD *size);
const int AFNFUCMAX     = 16;
class GB376_1 : public ProtocolBase
{
public:
    GB376_1(enum TerAddrType tType,enum TerProtocol pType,const char* at_string = NULL,char* softVersion = NULL);
    void  CombinFrame(char* afnstr,BYTE*content,DWORD *size);
    void  ParseFrame(char*afnstr,BYTE*content,DWORD *size);
     void StrDate_ProtocolDateSwitch(const BYTE*);
    void GetLastSoftwareVersion(char* pVersion)
    {
        if(NULL!=pVersion)
        {
            memcpy (pVersion,theLastSoftVersion,4);
        }
    }
    void inline GetAAT(char* aat)
    {
        memcpy (aat,theAAT,6);
    }
    DWORD  inline GetUpProgress(void)
    {
        return  (upProgress*fileSegSize*100)/upFile.size ();
    }
    void inline GetAFN04F3(const MasterIPPort *afn04f3)
    {
        memcpy (&AFN04F3,afn04f3,sizeof(MasterIPPort));
    }
    void SetUpFileInfo(DWORD dwFileSegSize,QFile &file,WORD _fileCrc16);
  //  enum ProtocolException   ExceptionProc(void);
private:
    WORD GetFN( const BYTE dt1, const BYTE dt2 );
    TFN MakeFN ( WORD thefn );
    TAFNFNUpInfo  GeTAFNFNUpInfobyStr( char* afnStr );

    /**************************************************************************/
    void AFN00( TFN, DWORD *size );
    void AFN01( TFN, DWORD *size );
    void AFN02( TFN, DWORD *size );
    void AFN03( TFN, DWORD *size );
    void AFN05( TFN, DWORD *size );
    void AFN04( TFN, DWORD *size );
    void AFN07( TFN, DWORD *size );
    void AFN06( TFN, DWORD *size );
    void AFN08( TFN, DWORD *size );
    void AFN09( TFN, DWORD *size );
    void AFN0a( TFN, DWORD *size );
    void AFN0b( TFN, DWORD *size );
    void AFN0c( TFN, DWORD *size );
    void AFN0d( TFN, DWORD *size );
    void AFN0e( TFN, DWORD *size );
    void AFN0f( TFN, DWORD *size,T0FTYPE  fileType);
    /**************************************************************************/
    AFNSeries   afnFunc[AFNFUCMAX] ;
    enum CommType currentComType;
    enum TerAddrType currentAddrType;
    BYTE       theAAT[6];
    BYTE       theCurTAAT[6];
    BYTE       theaAddrGrpFlag;
    TGB376_CF       byLastCtrl;
    BYTE       theSoftVersion[5];
    BYTE       theLastSoftVersion[5];
    char fileName[64];
    BYTE sWDisDate_[3];
    WORD fileCrc16;
    DWORD  fileSegSize;
    DWORD  fileSegCnt;
    QFile      upFile;
    DWORD  upProgress;
    MasterIPPort AFN04F3;
    BYTE    LastSendAfn;
   enum TerProtocol proType;
    /************************************************************************/
    void inline InitAFNFunc(void)
    {
        afnFunc[0] = &GB376_1::AFN00 ;
        afnFunc[1] =  &GB376_1::AFN01;
        afnFunc[2] =  &GB376_1::AFN02;
       //afnFunc[3] =  &GB 376_1::AFN03;
        afnFunc[4] =  &GB376_1::AFN04;
       /* afnFunc[5] =  &GB376_1::AFN05;
        afnFunc[6] =  &GB376_1::AFN06;
        afnFunc[7] =  &GB376_1::AFN07;
        afnFunc[8] =  &GB376_1::AFN08;*/
        afnFunc[9] =  &GB376_1::AFN09;
        /*afnFunc[0xa] =  &GB376_1::AFN0a;
        afnFunc[0xb] =  &GB376_1::AFN0b;
        afnFunc[0xc] =  &GB376_1::AFN0c;
        afnFunc[0xd] =  &GB376_1::AFN0d;
        afnFunc[0xe] =  &GB376_1::AFN0e;
        afnFunc[0xf] =  &GB376_1::AFN0f;*/
    }
    bool  inline  AATIsNotEixst(void);

    void  MakeHeader( int );
    
    void   MakeControlFiled( BYTE );
    
    void   MakeAddr( BYTE*, BYTE );
    
    void   MakeApp(char*,DWORD ,TDA ,void* );
    
    void   MakeAFNInfo(  BYTE , BYTE );
    
    void   MakeTail(char* ,DWORD );
    
    
    void   checkHeadAGetLen( WORD *);
    
    void   checkCtrl(void);
    
    void   checkAndGetAddr(BYTE* ,BYTE *);

    void   checkAndGetAfnSeq(BYTE *,BYTE *);
    
    void   checkAppAndGetAttachData( char*, DWORD ,TDA *,void*);
    
    bool   checkCs(int );
    
    DWORD    LengthWithAttach(DWORD ,int );
    
    char   *   checkSumPoint(void);
    
    DWORD   checkSumLen(DWORD );
   
};

#endif // TGB376_1_H
