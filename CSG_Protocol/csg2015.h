#ifndef CSG2015_H
#define CSG2015_H
#include "../BaseType.h"
#include "../protocolbase.h"
#include <QFile>
#pragma pack(push)
#pragma pack(1)
#define             UPGRADE_FILE_TYPE                               0x00
typedef struct _terswVersion{
    WORD wCCCC;
    BYTE byTT;
    BYTE  byYYMMDD[3];
}TCSG_E0000B01;
typedef struct  _ipPort{
    WORD	wPort;								//端口在前
    DWORD	dwMasterIP;
    WORD	wPadding;
}TCSG_IPPort ;
typedef struct _die0000100{
    union{
        BYTE	byArrMasterNum[8];					//主站号码
        TCSG_IPPort theIPPort;                      //主站IP/端口
    }	MasterAddr;

    BYTE		byComType;                          //通信类型
}TCSG_E0000100;
typedef struct _fd1{
    WORD	wFileProperties;			//文件性质
    WORD	wFileSegSum;				//文件总段数
    DWORD	dwFileSize;					//文件大小
    char        szFileName[32];		  //文件名
    WORD	wCrc16;						//文件总校验
}	TCSG_E3010001;
typedef struct _fd2{
    WORD   wFileSegNum;					//文件段号
    WORD   wFileSegSize;				//文件段长度
    // BYTE   byFileCon;					//文件内容
    //缺个段校验
}TCSG_E3010002;
typedef struct _fd3{
    TCSG_E3010001 FD1;
    WORD		  wFileStartSegNo;
    WORD		  wFileEndSegNo;
}TCSG_E3010003;
class csg2015;
typedef void (csg2015::*CSGAFNSeries)(DWORD,DWORD *size);
#define AFNFUCMAX           16

class csg2015 : public ProtocolBase
{
public:
    csg2015(char* softVersion = NULL);
    void  CombinFrame(char* afnstr,BYTE*pContent,DWORD *size);
    void  ParseFrame(char*afnstr,BYTE*pContent,DWORD *size);
    void inline GetAAT(char* aat)
    {
        memcpy (aat,theAAT,7);
    }
    void SetUpFileInfo(DWORD dwFileSegSize,QFile &file,WORD _fileCrc16);
    DWORD  inline GetUpProgress(void)
    {
        return  (upProgress*fileSegSize*100)/upFile.size ();
    }
    void GetLastSoftwareVersion(char* pVersion)
    {
        if(NULL!=pVersion)
        {
            memcpy (pVersion,theLastSoftVersion,4);
        }
    }
    void inline SetIPPort(DWORD dwIP,WORD wPort)
    {
        SwitchIP = dwIP;
        SwitchPort = wPort;
    }
    ~csg2015();
private:
    CSGAFNSeries   afnFunc[AFNFUCMAX] ;
    TAFNDI  GetAFNDIbyStr (char *afnStr);

    /**************************************************************************/
    void AFN00( DWORD, DWORD *size);
    void AFN01( DWORD, DWORD *size);
    void AFN02( DWORD, DWORD *size);
    void AFN03( DWORD, DWORD *size);
    void AFN05( DWORD , DWORD *size);
    void AFN04( DWORD , DWORD *size);
    void AFN07( DWORD , DWORD *size);
    void AFN06( DWORD , DWORD *size);
    void AFN08( DWORD , DWORD *size);
    void AFN09( DWORD, DWORD *size);
    void AFN0a( DWORD, DWORD *size);
    void AFN0b( DWORD, DWORD *size);
    void AFN0c( DWORD, DWORD *size);
    void AFN0d( DWORD, DWORD *size);
    void AFN0e( DWORD, DWORD *size);
    void AFN0f( DWORD, DWORD *size);
    /**************************************************************************/
    enum CommType currentComType;
    enum TerAddrType currentAddrType;
    BYTE       theAAT[7];
    BYTE       theaAddrGrpFlag;
    BYTE       theSoftVersion[5];
    BYTE       theLastSoftVersion[5];
    char *             pDwnContent;
    char *              pUpContent;
    char*              pDwnStart;                                       //下行开始处
    char*              pUpStart;                                          //上行开始处
    char fileName[64];
    WORD fileCrc16;
    DWORD  fileSegSize;
    DWORD  fileSegCnt;
    QFile      upFile;
    DWORD  upProgress;
    DWORD  SwitchIP;
    WORD  SwitchPort;
    BYTE     byComType;                                        //通信类型
    /************************************************************************/

    void inline InitAFNFunc(void)
    {
        afnFunc[0] = &csg2015::AFN00 ;
        // afnFunc[1] =  &csg2015::AFN01;
        afnFunc[2] =  &csg2015::AFN02;
        // afnFunc[3] =  &csg2015::AFN03;
        afnFunc[4] =  &csg2015::AFN04;
        /*afnFunc[5] =  &csg2015::AFN05;
        afnFunc[6] =  &csg2015::AFN06;
        afnFunc[7] =  &csg2015::AFN07;
        afnFunc[8] =  &csg2015::AFN08;
       afnFunc[9] =  &csg2015::AFN09;*/
        afnFunc[0xa] =  &csg2015::AFN0a;
        /* afnFunc[0xb] =  &csg2015::AFN0b;
        afnFunc[0xc] =  &csg2015::AFN0c;
        afnFunc[0xd] =  &csg2015::AFN0d;
        afnFunc[0xe] =  &csg2015::AFN0e;*/
        afnFunc[0xf] =  &csg2015::AFN0f;
    }

    void  MakeHeader(int len)
    {
        WORD length = len - sizeof(TCSG_Header)- sizeof(TCSG_Tail);
        TCSG_Header * p = ( TCSG_Header*)pDwnContent;
        p->start68 =0x68;
        p->L1[0] = (char)length;
        p->L1[1] = (char)(length>>8);
        p->L2[0] = (char)length;
        p->L2[1] = (char)(length>>8);
        p->end68 = 0x68;
        pDwnContent += sizeof( TCSG_Header);
    }

    void MakeControlFiled( BYTE x)
    {
        TCSG_CF *p = (TCSG_CF*) pDwnContent;
        p->funCode = (x&0x0f);                          //功能码
        p->FCV = !!(x&0x10);                                //帧计数有效位
        p->FCB_ACD = !!(x&0x20);                      //帧计数位FCB请求访问位ACD
        p->PRM          = !!(x&0x40);                     //启动标识位        1启动 0从动
        p->DIR           = !!(x&0x80);                    //传输方向位         0down 1up
        pDwnContent += sizeof(TCSG_CF);
    }

    void  MakeAddr( BYTE AAT[],BYTE addrGrpFlag)
    {
        TCSG_AddrFiled *p = (TCSG_AddrFiled*) pDwnContent;
        p->AandTAddrFlag = addrGrpFlag;
        memcpy (p->bcdARC,AAT,6);
        pDwnContent += sizeof(TCSG_AddrFiled);
    }

    void MakeApp( char*attachData,DWORD atSize,TDA da,void* uIDI)
    {
        TCSG_App *p = (TCSG_App*) pDwnContent;

        p->DA1 = da.DA1;
        p->DA2 = da.DA2;
        p->ulDI =  *((DWORD*)uIDI);
        if(NULL != attachData)
            memcpy (p+1,attachData,atSize);
        pDwnContent += sizeof(TCSG_App)+atSize;
    }

    void MakeAFNInfo( BYTE afn,BYTE seq)
    {
        TCSG_AFNInfo *p = (TCSG_AFNInfo*) pDwnContent;

        p->AFN = afn;
        p->SEQ = seq;
        pDwnContent += sizeof(TCSG_AFNInfo);
    }

    void MakeTail( char* target,DWORD Size)
    {
        TCSG_Tail *p = (TCSG_Tail*) pDwnContent;

        p->cs =  ByteCheckSum ((BYTE*)target,Size);
        p->end = 0x16;
    }

    void checkHeadAGetLen(  WORD *len)
    {
        TCSG_Header * p = ( TCSG_Header*) pUpContent;
        if(p->end68!=0x68||
                p->start68!=0x68)
        {
            *len = 0;
            pUpContent += sizeof( TCSG_Header);;
        }
        memcpy (len,p->L1,2);
        pUpContent += sizeof( TCSG_Header);
    }

    void checkCtrl(void)
    {
        TCSG_CF *p = (TCSG_CF*) pUpContent;
        p +=1 ;
    }

    void checkAndGetAddr( BYTE  _ATT[ ],BYTE *addrGrpFlg)
    {
        TCSG_AddrFiled *p = (TCSG_AddrFiled*) pUpContent;
        *addrGrpFlg = p->AandTAddrFlag;
        memcpy (_ATT,p->bcdARC,6);
        pUpContent += sizeof(TCSG_AddrFiled);
    }

    void  checkAndGetAfnSeq( BYTE *afn,BYTE *seq)
    {
        TCSG_AFNInfo *p = (TCSG_AFNInfo*) pUpContent;
        *afn = p->AFN;
        *seq = p->SEQ;
        pUpContent += sizeof(TCSG_AFNInfo);
    }
    void checkAppAndGetAttachData( char*attachData,DWORD atSize,TDA *da,void *uDI)
    {
        TCSG_App *p = (TCSG_App*) pUpContent;
        da->DA1 = p->DA1;
        da->DA2 = p->DA2;
        *((DWORD*)uDI) = p->ulDI;
        if(attachData != NULL)
        {
            memcpy (attachData,p+1,atSize);
        }
        pUpContent += sizeof(TCSG_App)+atSize;
    }
    bool checkCs( int len)
    {
        char * pC ;
        if(currentComType == UpComm)
        {
            pC = pUpContent;
        }
        else
        {
            pC = pDwnContent;
        }
        TCSG_Tail *p = (TCSG_Tail*)(pC +len-2-sizeof(TCSG_Header));
        if( ByteCheckSum ((BYTE*)checkSumPoint (),checkSumLen (len)) != p->cs )
        {
            return false;
        }
        return true;
    }
    DWORD inline LengthWithAttach(DWORD len,int count)
    {
        int length = 0;
        int i;
        length += sizeof( TCSG_Header);
        length += sizeof(TCSG_CF);
        length += sizeof(TCSG_AddrFiled);
        length +=sizeof(TCSG_AFNInfo);
        for(i = 0;  i < count ; i++)
        {
            length += sizeof(TCSG_App);
            length += len;
        }
        length += sizeof(TCSG_Tail);
        return length;
    }
    char* checkSumPoint()
    {
        char*p;
        if(currentComType == UpComm)
        {
            p = pUpStart;
        }
        else
        {
            p = pDwnStart;
        }
        return (char*)(p+sizeof( TCSG_Header));
    }
    DWORD checkSumLen(DWORD len)
    {
        return (len - sizeof( TCSG_Header) - sizeof(TCSG_Tail));
    }
};

#endif // CSG2015_H
