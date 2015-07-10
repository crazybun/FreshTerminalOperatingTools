#include "gb376_1.h"

void GB376_1::AFN01 (_UCHAR *content, TFN fn, _ULONG *size)
{
    switch(currentComType)
    {
    case DownComm:
    {    char *p;
        TDA     da = {0,1};
        _UCHAR x= (theSeq & 0xf0);
        theSeq = (x|((theSeq+1)&0xf));
       *size = LengthWithAttach (0,1);
        p = MakeHeader ((char*)content,*size);
        p = MakeControlFiled (p,0x40);
        p = MakeAddr (p,theAAT,0x08);
        p = MakeAFNInfo (p,0x01,theSeq);
        p = MakeApp (p,NULL,0,da,fn);
        MakeTail (p,checkSumPoint ((char*)content),checkSumLen (*size));
    }
    break;
    default:
        break;
    }
}
