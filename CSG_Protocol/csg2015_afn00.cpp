#include "csg2015.h"

void csg2015::AFN00 (DWORD uDI, DWORD *size)
{
     BYTE theafn = 0x00;
    switch(currentComType)
    {
    case DownComm:
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
    case UpComm:
        break;
    default:
        break;
    }
}
