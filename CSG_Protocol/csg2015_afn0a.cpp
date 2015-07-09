#include "csg2015.h"
void csg2015::AFN0a ( DWORD uDI, DWORD *size)
{
     BYTE theafn = 0x0a;
    switch(currentComType)
    {
    case DownComm:
        switch (uDI) {
        case 0xE0000B01:
        case 0xE0000100:
        {
            TDA da = {0,0};
            *size = LengthWithAttach (0,1);
            MakeHeader (*size);
            MakeControlFiled (0x40);
            MakeAddr (theAAT,0x08);
            MakeAFNInfo (theafn,++theSeq);
            MakeApp (NULL,0,da,&uDI);
            MakeTail (checkSumPoint (),checkSumLen (*size));

        }
            break;
        default:
            break;
        }
        break;
    case UpComm:
        TDA da;
        WORD len;
        BYTE afn;
        DWORD uuDI;
        checkHeadAGetLen (&len);
        if(0 == len)
        {
            return;
        }
        if(!checkCs (*size))
        {
            SetExceptionRepeat ();
            return;
        }
        checkCtrl ();
        checkAndGetAddr (theAAT,&theaAddrGrpFlag);
        checkAndGetAfnSeq (&afn,&theSeq);
        if(afn != 0x0a)
        {
            SetExceptionRepeat ();
            return;
        }
        switch (uDI) {
        case 0xE0000B01:
        {
            TCSG_E0000B01 tVersion;
            QByteArray ba;
            QString ss;
            checkAppAndGetAttachData ((char*)&tVersion,sizeof(TCSG_E0000B01),&da,&uuDI);
            ba.resize (3);
            memset(ba.data(),0,3);
            memcpy (ba.data (),(char*)&tVersion.wCCCC,2);
            //BIN转字符串所以要逆转一下。
            ReverseMem ((BYTE*)ba.data (),2);
            addrTrans (ba,ss);
            if(0 == memcmp (ss.toLatin1 ().data (),theSoftVersion,4))
            {//相等就跳转
                SetExceptionJump ();
            }
            else
            {//不相等则向下执行
                memcpy (theLastSoftVersion,ss.toLatin1 ().data (),4);
                SetNoException ();
            }
        }
            break;
       case 0xE0000100:
        {
            TCSG_E0000100 tCom;
            checkAppAndGetAttachData ((char*)&tCom,sizeof(TCSG_E0000100),&da,&uuDI);
            byComType = tCom.byComType;
            SetNoException ();
        }
        default:
            break;
        }
        break;
    default:
        break;
    }
}
