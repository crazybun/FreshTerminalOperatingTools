
#include "gb376_1.h"
void GB376_1::AFN04( TFN fn, DWORD *size)
{
    switch(currentComType)
    {
    case DownComm:
        switch(GetFN (fn.DT1,fn.DT2))
        {
        case 03:
        {
            TDA     da = {0,0};
            theSeq = 0x70;
            *size = LengthWithAttach (sizeof(MasterIPPort),1);
            MakeApp ((char*)&AFN04F3,sizeof(MasterIPPort),da,&fn);
        }
            break;
        default:
            break;
        }
        break;
    case UpComm:
    default:
        break;
    }
}

