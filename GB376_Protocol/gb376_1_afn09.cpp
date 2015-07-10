#include "gb376_1.h"
void GB376_1::AFN09( TFN fn, DWORD *size)
{
    switch(currentComType)
    {
    case DownComm:
        switch(GetFN (fn.DT1,fn.DT2))
        {
        case 1:
        {
            TDA     da = {0,0};
            theSeq = 0x70;
            *size = LengthWithAttach (0,1);
            MakeApp (NULL,0,da,&fn);
        }
            break;
        default:
            break;
        }
        break;
    case UpComm:
        switch(GetFN (fn.DT1,fn.DT2))
        {
        case 1:
        {
            TDA da;
            TTerminalVersion tVersion;
            checkAppAndGetAttachData ((char*)&tVersion,sizeof(TTerminalVersion),&da,&fn);
            if((0 == memcmp (tVersion.terSWVersoin,theSoftVersion,4)
                &&(0 == memcmp (tVersion.sWDisDate,sWDisDate_,3))))
            {
                SetExceptionJump ();
            }
            else
            {
                upProgress = 0;
                memcpy (theLastSoftVersion,tVersion.terSWVersoin,4);
                ReverseMem (tVersion.sWDisDate,3);
                QString ss;
                memcpy (swDisDate,QString::number (2000+Bcd2Dec ((DWORD)tVersion.sWDisDate[0])).toLatin1 ().data (),4);
                swDisDate[4] = '-';
                memcpy (&swDisDate[5],ss.sprintf ("%02ld",Bcd2Dec(tVersion.sWDisDate[1])).toLatin1 ().data (),2);
                swDisDate[7] = '-';
                memcpy (&swDisDate[8],QString::number (Bcd2Dec ((DWORD)tVersion.sWDisDate[2])).toLatin1 ().data (),2);
                SetNoException ();
                mempcpy (swDisTime,tVersion.TerVConfigCode,2);
                swDisTime[2] = ':';
                mempcpy (&swDisTime[3],&tVersion.TerVConfigCode[2],2);
                swDisTime[5] = ':';
                mempcpy (&swDisTime[6],&tVersion.TerVConfigCode[4],2);
            }
        }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
