#ifndef PROTOCOLBASE_H
#define PROTOCOLBASE_H
#include "BaseType.h"
class ProtocolBase
{
public:

    ProtocolBase();
    void virtual CombinFrame(char* afnstr,BYTE*content,DWORD *size) {afnstr = afnstr,content = content,size = size; }
    void virtual ParseFrame(char*afnstr,BYTE*content,DWORD *size) { afnstr = afnstr,content = content,size = size;}
    enum ProtocolException virtual  ExceptionProc(void) ;
    void virtual StrDate_ProtocolDateSwitch(const BYTE*) ;
    void GetSwDisDate(char* disDate)
    {
        if(NULL!= disDate)
        {
            memcpy (disDate,swDisDate,strlen((char*)swDisDate));
        }
    }
    void GetSwDisTime( char* disTime)
    {
        if(NULL!= disTime)
        {
            memcpy (disTime,swDisTime,strlen((char*)swDisTime));
        }
    }
    void virtual GetAAT(char* aat)
    {
        aat = aat;
    }
private:

protected:
    char *            pDwnAppPtr;
    char *            pUpAppPtr;
    char *            pDwnStart;                                       //下行开始处
    char *            pUpStart;                                        //上行开始处
    BYTE              theSeq;
    BYTE              swDisDate[32];
    BYTE              swDisTime[32];
    enum ProtocolException theException;
    
    void virtual MakeHeader( int len) { len = len; }
    
    void virtual MakeControlFiled( BYTE x) { x = x; }
    
    void virtual MakeAddr( BYTE AAT[], BYTE addrGrpFlag ) {AAT = AAT,addrGrpFlag = addrGrpFlag;}
    
    void virtual MakeApp(char*attachData, DWORD atSize,TDA da,void* p) {attachData = attachData;
                                                                     atSize = atSize,da = da,p = p;}
    
    void virtual MakeAFNInfo(  BYTE afn, BYTE seq) { afn = afn; seq = seq;}
    
    void virtual MakeTail(char* target,DWORD Size) { target = target;Size = Size;}
    
    void virtual checkHeadAGetLen( WORD *len) {  len = len ;}
    
    void virtual checkCtrl(void) { }
    
    void virtual checkAndGetAddr(BYTE  _ATT[ ],BYTE *addrGrpFlg) {_ATT = _ATT,addrGrpFlg = addrGrpFlg; }

    void virtual checkAndGetAfnSeq(BYTE *afn,BYTE *seq) {afn = afn,seq = seq; }
    
    void virtual checkAppAndGetAttachData( char*attachData, DWORD atSize,TDA *da,void *fn) { attachData = attachData;
                                                                                             atSize = atSize,da = da,fn = fn;}
    
    bool virtual checkCs(int len){ len = len;return false;}
    
    DWORD inline virtual LengthWithAttach(DWORD len,int count){len = len;count = count; return 0;}
    
    char virtual *   checkSumPoint(void){return NULL; }
    
    DWORD virtual checkSumLen(DWORD len){ len = len;return 0;}

    void  virtual needConfirmation(void)
    {//GB13-GB09 CSG2015
        theSeq |= 0x10;
    }
    
    void SetNoException(void)
    {
        theException = NOEXCEPTION;
    }
    void SetExceptionJump(void)
    {
        theException = EXCEPTIONJUMP;
    }
    void SetExceptionRepeat(void)
    {
        theException = EXCEPTIONREPEAT;
    }
    void SetExceptionWait(void)
    {
        theException = EXCEPTIONWAIT;
    }
    void SetExceptionEnsure(void)
    {
         theException = EXCEPTIONENSURE;
    }
    void SetExceptionOverProc(void)
    {
         theException = EXCEPTIONOVERPROC;
    }
};

#endif // PROTOCOLBASE_H
