#include "GB376_1.h"
#include <QString>
#include <QDebug>
GB376_1::GB376_1(enum TerAddrType tType,enum TerProtocol pType,const char* at_string,char* softVersion)
{
    InitAFNFunc ();
    currentAddrType = tType;
    theSeq = 0x60;
    if((NULL != at_string)
            &&(0!=memcmp(at_string,"0xFFFF",4)))
    {
        QByteArray ba;
        StrString2SPECData (at_string,ba);
        memcpy (theAAT,ba.data (),ba.size ());
        ReverseMem (theAAT,2);
        if(currentAddrType == TADDR)                //Terminal addr big little -endian switch.
            ReverseMem (theAAT+2,2);
        else
            ReverseMem (theAAT+2,4);
         upProgress = 0;
    }
    else
    {
        memset(theAAT,0,6);
    }
    memset(theCurTAAT,0,6);
    memset(theSoftVersion,0,5);
    if(NULL != softVersion)
    { 
        memcpy (theSoftVersion,softVersion,4);
    }
    memset(theLastSoftVersion,0,5);
    proType = pType;
}
void GB376_1::CombinFrame (char *afnstr, BYTE *content, DWORD *size)
{

    currentComType = DownComm;
    pDwnAppPtr = pDwnStart = (char*)content;
    TAFNFNUpInfo  afnfn = GeTAFNFNUpInfobyStr (afnstr);
    LastSendAfn = afnfn.afn;
    if( ( afnfn.afn  ==  0xff )
            &&( afnfn.fn == 0xfff ) )
    {//用来表示结束状态而已。
        SetExceptionJump ();
        return;
    }
    if(0x0f == afnfn.afn)
    {
       AFN0f (MakeFN (afnfn.fn),size,afnfn.ft);
    }
    else
    {
       (this->*afnFunc[afnfn.afn])(MakeFN (afnfn.fn),size);
    }
    MakeHeader (*size);
    MakeControlFiled (0x40);
    MakeAddr (theAAT,0x08);
    MakeAFNInfo (afnfn.afn,theSeq++);
    MakeTail (checkSumPoint (),checkSumLen (*size));
}
void GB376_1::ParseFrame (char *afnstr, BYTE *content, DWORD *size)
{


    WORD len;
    BYTE afn;
    TAFNFNUpInfo  afnfn = GeTAFNFNUpInfobyStr (afnstr);
    currentComType = UpComm;
    pUpAppPtr = pUpStart = (char*)content;
    checkHeadAGetLen (&len);
    if(0 == len)
    {
        SetExceptionRepeat ();
        return;
    }
    if(!checkCs (len))
    {
        SetExceptionRepeat ();
        return;
    }
    checkCtrl ();
    checkAndGetAddr (theCurTAAT,&theaAddrGrpFlag);
    if(0!= memcmp (theCurTAAT,theAAT,((currentAddrType == TADDR)?4:6)))
    {
       if(!AATIsNotEixst())
       {
           SetExceptionRepeat ();
           return;
       }
       else
       {//copy the addr to the variable
           memcpy (theAAT,theCurTAAT,6);
       }
    }
    checkAndGetAfnSeq (&afn,&theSeq);
    if(afn != afnfn.afn)
    {
        SetExceptionWait ();
        return;
    }
    if(0x0f == afnfn.afn)
    {
       AFN0f (MakeFN (afnfn.fn),size,afnfn.ft);
       return;
    }

    (this->*afnFunc[afnfn.afn])(MakeFN (afnfn.fn),size);
}
WORD GB376_1::GetFN(const BYTE dt1, const BYTE dt2)
{
    int x ;
    for(BYTE i  = 0 ; i< 8 ; i++)
    {
        if((1<<i) == dt1)
        {
            x = i;
        }
    }
    x+=1;
    return dt2*8+x;
}
void GB376_1::SetUpFileInfo (DWORD dwFileSegSize, QFile &file, WORD _fileCrc16)
{
    memset(fileName,0,64);
    memcpy(fileName,Sfilter (file.fileName ().toLatin1 ().data (),"/"),64);
    fileSegSize = dwFileSegSize;
    upFile.setFileName (file.fileName ());
    upFile.open (QIODevice::ReadOnly);
    fileSegCnt = (upFile.size()/fileSegSize)+(((upFile.size ()/fileSegSize)==0)?0:1);
    fileCrc16 = _fileCrc16;
}
TFN GB376_1::MakeFN (WORD thefn)
{
    TFN   fn;
    fn.DT2 = (thefn>>3)&0xff;
    fn.DT1 = 1<<((thefn&0x7)-1);
    return fn;
}
TAFNFNUpInfo GB376_1::GeTAFNFNUpInfobyStr (char *afnStr)
{
    TAFNFNUpInfo afnfn;
    bool ok;
    char buf[8] = {'\0'},*p;
    p = strstr(afnStr,"AFN");
    p+=3;
    memcpy (buf,p,2);
    afnfn.afn = QString(buf).toUShort (&ok,16);
    p+=2;
    p = strstr(p,"F");
    p += 1;
    memcpy (buf,p,3);
    afnfn.fn = QString (buf).toUShort (&ok,16);
    p += 3;
    if(0 == memcmp (p,"content",sizeof("content")))
    {
        afnfn.ft = FILE_CONTENT;
    }
    if(0 == memcmp (p,"head",sizeof("head")))
    {
        afnfn.ft = FILE_HEAD;
    }
    if(0 == memcmp (p,"tail",sizeof("tail")))
    {
        afnfn.ft = FILE_TAIL;
    }
    return afnfn;
}
void GB376_1::StrDate_ProtocolDateSwitch(const BYTE* in  )
{
    char buf[5];
    short year,mon,day;
    if(NULL == in)
    {
        return;
    }
    memset(buf,0,5);
    memcpy (buf,in,4);
    year = QString(buf).toShort () - 2000 ;
    in += 5;
    memset(buf,0,5);
    memcpy (buf,in,2);
    mon = QString(buf).toShort () ;
    in += 3;
    memset(buf,0,5);
    memcpy (buf,in,2);
    day = QString(buf).toShort () ;

    sWDisDate_[0] = (BYTE)Dec2Bcd((int)day);
    sWDisDate_[1] = (BYTE)Dec2Bcd((int)mon);
    sWDisDate_[2] = (BYTE)Dec2Bcd((int)year);
}

void GB376_1::MakeHeader( int len)
{
    WORD length = ((len-8) <<2)+2;
    TGB376_header * p = (TGB376_header*)pDwnStart;
    p->start68 =0x68;
    p->L1[0] = (char)length;
    p->L1[1] = (char)(length>>8);
    p->L2[0] = (char)length;
    p->L2[1] = (char)(length>>8);
    p->end68 = 0x68;
}

void GB376_1::MakeControlFiled( BYTE x)
{
    TGB376_CF *p = (TGB376_CF*)(pDwnStart+sizeof ( TGB376_header ) );
    p->funCode     = (x&0x0f);                                            //功能码
    p->FCV         = !!(x&0x10);                                              //帧计数有效位
    p->FCB_ACD     = !!(x&0x20);                                          //帧计数位FCB请求访问位ACD
    p->PRM         = !!(x&0x40);                                     //启动标识位        1启动 0从动
    p->DIR         = !!(x&0x80);                                    //传输方向位         0down 1up
}

void GB376_1::MakeAddr( BYTE *AAT, BYTE addrGrpFlag )
{
    if(TADDR == currentAddrType)
    {
        TGB376_ARCandTaddr *p = (TGB376_ARCandTaddr*)(pDwnStart+sizeof ( TGB376_header ) 
                                                      +sizeof(TGB376_CF));
        p->AandTAddrFlag = addrGrpFlag;
        memcpy (p->bcdARC,AAT,4);
    }
    else
    {
        T4ADDRTGB376_ARCandTaddr *p = (T4ADDRTGB376_ARCandTaddr*)(pDwnStart+sizeof ( TGB376_header ) 
                                                                  +sizeof(TGB376_CF));
        p->AandTAddrFlag = addrGrpFlag;
        memcpy (p->bcdARC,AAT,6);
    }
}

void GB376_1::MakeApp(char* attachData,DWORD atSize,TDA da,void* fn )
{
    TGB376_App *p = (TGB376_App*)(pDwnStart+sizeof ( TGB376_header ) 
                                  +sizeof(TGB376_CF)
                                  +((TADDR == currentAddrType)?
                                        (sizeof(TGB376_ARCandTaddr)):
                                        (sizeof(T4ADDRTGB376_ARCandTaddr)))
                                  +sizeof(TGB376_AFNInfo));

    p->DA1 = da.DA1;
    p->DA2 = da.DA2;
    p->DT1 = ((TFN*)fn)->DT1;
    p->DT2 = ((TFN*)fn)->DT2;
    if(NULL != attachData)
        memcpy (p+1,attachData,atSize);
}

void GB376_1::MakeAFNInfo(  BYTE afn, BYTE seq)
{
    TGB376_AFNInfo *p = (TGB376_AFNInfo*)(pDwnStart+sizeof ( TGB376_header )
            +sizeof(TGB376_CF)
            +((TADDR == currentAddrType)?
                  (sizeof(TGB376_ARCandTaddr)):
                  (sizeof(T4ADDRTGB376_ARCandTaddr))));

    p->AFN = afn;
    p->SEQ = seq;
}

void GB376_1::MakeTail(char* target,DWORD Size)
{
    TGB376_Tail *p = (TGB376_Tail*)(pDwnStart+Size+sizeof(TGB376_header));

    p->cs =  ByteCheckSum ((BYTE*)target,Size);
    p->end = 0x16;
}

void GB376_1::checkHeadAGetLen( WORD *len)
{
     TGB376_header * p = (TGB376_header*)pUpStart;
     if(p->end68!=0x68||
             p->start68!=0x68)
     {
         *len = 0;
     }
     memcpy (len,p->L1,2);
     *len = ((*len - 2) >>2);
}

void GB376_1::checkCtrl(void)
{
    TGB376_CF *p = (TGB376_CF*) (TGB376_CF*)(pUpStart+sizeof ( TGB376_header ) );
    byLastCtrl = *p;
}

void GB376_1::checkAndGetAddr(BYTE _ATT[ ],BYTE *addrGrpFlg)
{
    if(TADDR == currentAddrType)
    {
        TGB376_ARCandTaddr *p = (TGB376_ARCandTaddr*)(pUpStart+sizeof ( TGB376_header )
                                                      +sizeof(TGB376_CF));
        *addrGrpFlg = p->AandTAddrFlag;
        memcpy (_ATT,p->bcdARC,4);
    }
    else
    {
        T4ADDRTGB376_ARCandTaddr *p = (T4ADDRTGB376_ARCandTaddr*)(pUpStart+sizeof ( TGB376_header )
                                                                  +sizeof(TGB376_CF));
        *addrGrpFlg = p->AandTAddrFlag;
        memcpy (_ATT,p->bcdARC,6);
    }
}
void GB376_1::checkAndGetAfnSeq(BYTE *afn,BYTE *seq)
{
        TGB376_AFNInfo *p = (TGB376_AFNInfo*)(pUpStart+sizeof ( TGB376_header )
                                              +sizeof(TGB376_CF)
                                              +((TADDR == currentAddrType)?
                                                    (sizeof(TGB376_ARCandTaddr)):
                                                    (sizeof(T4ADDRTGB376_ARCandTaddr))));
        *afn = p->AFN;
        *seq = p->SEQ;
}
void GB376_1::checkAppAndGetAttachData(char*attachData,DWORD atSize,TDA *da,void *fn)
{
    TGB376_App *p = (TGB376_App*)((pUpStart+sizeof ( TGB376_header )
                                   +sizeof(TGB376_CF)
                                   +((TADDR == currentAddrType)?
                                         (sizeof(TGB376_ARCandTaddr)):
                                         (sizeof(T4ADDRTGB376_ARCandTaddr))))
                                  +sizeof(TGB376_AFNInfo));
     da->DA1 = p->DA1;
     da->DA2 = p->DA2;
     ((TFN*)fn)->DT1 = p->DT1;
     ((TFN*)fn)->DT2 = p->DT2;
     if(attachData != NULL)
     {
           memcpy (attachData,p+1,atSize);
     }
}
bool GB376_1::checkCs(int len)
{
    TGB376_Tail *p = (TGB376_Tail*)(pUpStart+len+sizeof(TGB376_header));
    if( ByteCheckSum ((BYTE*)checkSumPoint(),len) != p->cs )
    {
        return false;
    }
    return true;
}

DWORD GB376_1::LengthWithAttach(DWORD len,int count)
{
    int length = 0;
    int i;
    length += sizeof(TGB376_header);
    length += sizeof(TGB376_CF);
    length += (currentAddrType == TADDR)?sizeof(TGB376_ARCandTaddr):sizeof(T4ADDRTGB376_ARCandTaddr);
    length +=sizeof(TGB376_AFNInfo);
    for(i = 0;  i < count ; i++)
    {
        length += sizeof(TGB376_App);
        length += len;
    }
     length += sizeof(TGB376_Tail);
    return length;
}

char* GB376_1::checkSumPoint(void)
{
    char * p;

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

DWORD GB376_1::checkSumLen(DWORD len)
{
    return (len - sizeof(TGB376_header) - sizeof(TGB376_Tail));
}

bool GB376_1::AATIsNotEixst(void)
{
    for(int i = 0;  i < 6; i++)
    {
        if(0x00 != theAAT[i])
        {
            return false;
        }
    }
    return true;
}
