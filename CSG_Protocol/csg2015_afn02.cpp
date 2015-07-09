
#include "csg2015.h"

void csg2015::AFN02 (DWORD uDI, DWORD *size)
{
    switch(currentComType)
    {
    case DownComm:
        break;
    case UpComm:
    {
        TDA da;
        WORD len;
        BYTE afn;
        DWORD uDI;
        checkHeadAGetLen (&len);
        if(0 == len)
        {
            return;
        }
        if(!checkCs (*size))
        {
            SetExceptionJump ();
        }
        checkCtrl ();
        checkAndGetAddr (theAAT,&theaAddrGrpFlag);
        checkAndGetAfnSeq (&afn,&theSeq);
        checkAppAndGetAttachData (NULL,0,&da,&uDI);          //完全不用关心终端规约版本
    }
        break;
    default:
        break;
    }
}
