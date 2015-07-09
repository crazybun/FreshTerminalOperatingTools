

#include "csg2015.h"

void csg2015::AFN04 (DWORD uDI, DWORD *size)
{
    BYTE theafn = 0x04;
    if(currentComType == DownComm)
    {
        TDA da = {0,0};
        char* pContent = 0;
        DWORD dwAppSize = 0;
        switch(uDI)
        {
        case 0xE0000100:
        {

            TCSG_E0000100 content;
            *size = LengthWithAttach (sizeof(TCSG_E0000100),1);
            content.byComType = byComType;
            content.MasterAddr.theIPPort.dwMasterIP = SwitchIP;
            content.MasterAddr.theIPPort.wPort = SwitchPort;
            pContent = (char*)&content;
            dwAppSize = sizeof(TCSG_E0000100);
        }
       break;
       default:
            break;
        }
        MakeHeader (*size);
        MakeControlFiled (0x40);
        MakeAddr (theAAT,0x08);
        MakeAFNInfo (theafn,++theSeq);
        MakeApp (pContent,dwAppSize,da,&uDI);
        MakeTail (checkSumPoint (),checkSumLen (*size));
    }
    else
    {
        TDA da;
        WORD len;
        BYTE afn;
        DWORD uDI;
        char edc;
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
        if(afn != 0x04)
        {
            SetExceptionRepeat ();
        }
        checkAppAndGetAttachData (&edc,1,&da,&uDI);
        if(edc == 0x00)
        {
            SetExceptionJump ();
        }
        else
        {
            SetExceptionRepeat ();
        }
    }


}
