#include "gb376_1.h"
void GB376_1::AFN00 (TFN fn, DWORD *size)
{
    switch(currentComType)
    {
    case DownComm:
    {
        TDA     da = {0,0};
        *size = LengthWithAttach (0,1);
        MakeApp (NULL,0,da,&fn);
    }
        break;
    default:
        if(LastSendAfn != 0x04)
            SetExceptionJump ();
        else
            SetNoException ();
        break;
    }
}

void GB376_1::AFN01 (TFN fn, DWORD *size)
{
    switch(currentComType)
    {
    case DownComm:
    {
        TDA     da = {0,0};
        *size = LengthWithAttach (0,1);
        needConfirmation ();
        MakeApp (NULL,0,da,&fn);
    }
        break;
    default:
        break;
    }
}

void GB376_1::AFN02 (TFN fn, DWORD *size)
{
    switch(currentComType)
    {
    case UpComm:
    {
        fn = fn;
        size = size;
    }
        break;
    default:
        break;
    }
}

