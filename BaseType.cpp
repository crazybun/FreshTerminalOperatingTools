#include "BaseType.h"

void MakeCrc16Tab(WORD *Crc16Table)
{
WORD wRc;
WORD i,j;

    for(i=0;i<256;i++)
    {
        wRc = (WORD)(i<<8);
        for(j=0;j<8;j++)
        {
            if(wRc&0x8000)
            {
                wRc <<= 1;
                wRc ^= 0x1021;
            }
            else
                wRc <<= 1;
        }
       Crc16Table[i]=wRc;
    }
}

//计算循环冗余码
WORD Crc16(WORD crc,WORD *Crc16Table,void *pvBuf,DWORD count)
{
BYTE *p = (BYTE *)pvBuf;
    while (count--)
    {
        crc = (crc << 8)^Crc16Table[(crc>>8)^*p];
        p++;
    }
    return (crc);
}
WORD fileCrc16Calc (DWORD fileLen,BYTE *fileBuffer)
{
    int index = 0; 
    WORD  Crc16Table[256];                                                                                                                   //制作Crc校验表
    WORD fileCrc16 = 0; 
    WORD crcSize = MAXCRCSIZE;
    MakeCrc16Tab(Crc16Table);
    while(1)
    {
        if( fileLen-index*MAXCRCSIZE <= MAXCRCSIZE)
        {
            crcSize =  fileLen -index*MAXCRCSIZE;
        }
        fileCrc16 = Crc16 (fileCrc16,
                                        Crc16Table,fileBuffer+index*MAXCRCSIZE,
                                 crcSize);
        if(fileLen-index*MAXCRCSIZE <= MAXCRCSIZE)
        {
            break;
        }
        index++;
    }
    return fileCrc16;
}
WORD _Crc16(BYTE *fileBuffer,DWORD fileLen)
{
    WORD crc = 0; 
    WORD tmp = 0; 
    for (DWORD  i = 0;  i < fileLen; i++)
    {
        crc = crc ^ ( (fileBuffer[i]<< 8) );
        for (int j = 0;  j <8 ; j++)
        {
            tmp = (crc << 1);
            if ((crc & 0x8000) != 0)
            {
                tmp = (tmp ^ 0x1021);
            }
            crc = tmp;
        }
    }
    return crc;
}

void InsertConfig(TerminalConfig configArr[],TerminalConfig newConfig,enum TerAddrType type)
{
    int max = 0 ;
    int judge = 0; 
    int i = 0; 
    int addrcount = 8;
    while(configArr[max].IsUsed)
    {
        max++;
    }
    if(type == FADDR)
    {
        addrcount = 12;
    }
    else if(type == TADDR)
    {
        addrcount = 8;
    }
    else if(type == CSGADDR)
    {
        addrcount = 12;
    }
    for( ;configArr[i].IsUsed;)
    {
        if((judge =memcmp (configArr[i].ARCandTAddr,newConfig.ARCandTAddr,addrcount))<0)     //地址比较实际上就是从高位比起高位小于肯定是没话说往后排。
        {
            i++;
        }
        else if(judge > 0)
        {
            for(int j = max;j>i;j--)
            {
                configArr[j] = configArr[j-1];
            }
            configArr[i] = newConfig;
            return;
        }
        else
        {
            return;         //不添加重复地址
        }
    }
    configArr[i] = newConfig;
}

DWORD Dec2Bcd(int x)
{
    DWORD BcdCode = 0; 
    int bitCount = 0; 
    while(x)
    {
        BcdCode += pow(16,bitCount++)*(x%10);
        x/=10;
    }
    return BcdCode;
}
DWORD Bcd2Dec(DWORD x)
{
    DWORD DecCode = 0; 
    int bitCount = 0; 
    while(x)
    {
        DecCode += pow(10,bitCount++)*(x%0x10);
        x/=0x10;
    }
    return DecCode;
}
BYTE inline c2b(BYTE xx)
{
    int x = xx;
    if((x >= '0') && (x <='9') )
    {
        x=x-'0';
        return x;
    }
    if((x >='a')&&(x <= 'f'))
    {
        x = x -'a'+10;
        return x;
    }
    if((x >='A')&&(x <= 'F'))
    {
        x = x -'A'+10;
        return x;
    }
    return 0xff;
}
void TerCAddr2BAddr(BYTE CAAT[],BYTE BAddr[])
{
    BAddr[0] |= c2b(CAAT[2]);
    BAddr[0] <<=4;
    BAddr[0] |= c2b(CAAT[3]);
    BAddr[1] |= c2b(CAAT[1]);
        BAddr[0] <<=4;
    BAddr[1] |= c2b(CAAT[0]);
    for(int i = 0;  i < 4;i++)
    {
        BAddr[i+2] |=c2b( CAAT[11-2*i -1]);
        BAddr[i+2]  <<= 4;
        BAddr[i+2] |= c2b(CAAT[11-2*i]);
    }
}
int TerAddrCmp(BYTE _AAT[],BYTE __AAT[],enum TerAddrType type)   //终端地址比较
{
    BYTE tAAT[13];
    int nAddrNum = 0; 
    if((FADDR == type)
            ||(CSGADDR == type))
    {
        nAddrNum = 12;
    }
    else if(TADDR== type)
    {
        nAddrNum = 8;
    }
    for(int i = 0 ; i< nAddrNum;i++)
    {
        if(i%2==0)
        {
            if(__AAT[i/2]/16 >=10)
            {
                tAAT[i] = __AAT[i/2]/16 -10+'A';
            }
            else
            {
                tAAT[i] = __AAT[i/2]/16+'0';
            }

        }
        else
        {
            if(__AAT[i/2]%16 >= 10)
            {
                tAAT[i] = __AAT[i/2]%16  -10+ 'A';
            }
            else
            {
                tAAT[i] = __AAT[i/2]%16+'0';
            }

        }
    }
    tAAT[nAddrNum] = '\0';
    for(int i = 0 ; i < nAddrNum; i++)
    {
        if(_AAT[i] != tAAT[i])
        {
            return _AAT[i] - tAAT[i];
        }
    }
   return 0;
}
//查找到最后那个字符或字符串出现的地方
char *Sfilter( char *str1,const char *str2)
{
    char *p;
    if(NULL == str1 || NULL == str2)
    {
        return NULL;
    }
    while(NULL!=(p =strstr(str1,str2)))
    {
        str1 = p+1;
    }
    return str1;
}
bool GetParaSettings (  TParaSettings **pParaSettings_)
{
    QFile comCfgFile,terCfgFile;

    comCfgFile.setFileName (COMPARASFILENMAE);
    terCfgFile.setFileName (TERPARASFILENAME);
    if(!comCfgFile.exists ()||!terCfgFile.exists ())
    {
        return false;
    }
    *pParaSettings_ = new TParaSettings;
    comCfgFile.open (QIODevice::ReadOnly);
    memcpy (&(*pParaSettings_)->comPara,comCfgFile.readAll (),sizeof(TComParaSettings));
    terCfgFile.open (QIODevice::ReadOnly);
    memcpy (&(*pParaSettings_)->terPara,terCfgFile.readAll (),sizeof(TTerParaSettings));
    return true;
}
static int TerAddrCmp_str(const void * pA, const void * pB)
{
    TerminalConfig* pTerA = (TerminalConfig*)pA;
    TerminalConfig* pTerB = (TerminalConfig*)pB;

    return memcmp (pTerA->ARCandTAddr,
                   pTerB->ARCandTAddr,
                   strlen(pTerA->ARCandTAddr));
}
bool GetTerConfig (TerminalConfig* pTConfig)
{
    memset(pTConfig,0,sizeof(TerminalConfig)*MAXCONFIG);
    QFile configFile;
    configFile.setFileName (CONFIGNAME);
    if(configFile.exists ())
    {
        size_t index;
        configFile.open (QIODevice::ReadWrite);
        QByteArray ba =  configFile.readAll ();
        memcpy (pTConfig,ba.data (),ba.size ());  //读出所有配置
        configFile.close ();
        for(index = 0; pTConfig[index].IsUsed; index++);
        if(0 == index)
        {
            return false;
        }
        qsort (&pTConfig[0],                     //进行排序方便进行2分搜索
               index,
               (size_t)(sizeof(TerminalConfig)),TerAddrCmp_str);
        return true;
    }
    return false;
}
void SPECData2String (QByteArray datagram,QString & ss)
{
    int i = 0; 
    for(char * p = datagram.data (); i<datagram.size(); i++)
    {
        if((p[i]&0xff )<= 0xf)
        {
            ss+=QString::number (0,10);
        }
        ss += QString::number (p[i]&0xff,16);
        ss = ss.toUpper ();
       if(i != datagram.size ()-1)
        ss += QString(" ");
    }
}
void addrTrans (QByteArray datagram,QString & ss)
{
    int i = 0; 
    for(char * p = datagram.data (); i<datagram.size(); i++)
    {
        if((p[i]&0xff )<= 0xf)
        {
            ss+=QString::number (0,10);
        }
        ss += QString::number (p[i]&0xff,16);
        ss = ss.toUpper ();
    }
}
unsigned char SW2Hex(int x)
{
    return  (x%10)+(x/10)*16;
}
void String2SPECData(QString ss,QByteArray & datagram)
{
    int index = 0; 
    bool yes;
    char x[3] = {};
    char buf[2048];
    char* p = ss.toLatin1 ().data ();
    memset (buf,0,2048);
    while('\0' != p[0])
    {
        if((p[0] >= '0'&&p[0]<='9')&&
                (p[1] >= '0'&&p[1]<='9'))
        {
            memcpy (x,p,2);
            QString s(x);
            buf[index++] = SW2Hex (s.toInt ());
        }
        else if(' ' != p[0])
        {
            memcpy (x,p,2);
            QString s(x);
            buf[index++] = s.toInt (&yes,16);
        }
        (' ' == p[0])?(p += 1):(p += 2);
    }
    datagram.resize (index);
    memcpy (datagram.data(),buf,index);
}
void StrString2SPECData(const char* strString,QByteArray & datagram)
{
    int index = 0; 
    bool yes;
    char x[3] = {};
    char buf[2048];
    const char* p = strString;
    memset (buf,0,2048);
    while('\0' != p[0])
    {
        if((p[0] >= '0'&&p[0]<='9')&&
                (p[1] >= '0'&&p[1]<='9'))
        {
            memcpy (x,p,2);
            QString s(x);
            buf[index++] = SW2Hex (s.toInt ());
        }
        else if(' ' != p[0])
        {
            memcpy (x,p,2);
            QString s(x);
            buf[index++] = s.toInt (&yes,16);
        }
        (' ' == p[0])?(p += 1):(p += 2);
    }
    datagram.resize (index);
    memcpy (datagram.data(),buf,index);
}
BYTE ByteCheckSum(const BYTE* buf,int index)
{
    BYTE tmp = 0; 
    for(int i = 0 ; i < index ;  i++)
    {
        tmp += buf[i];
    }
    return tmp;
}
void ReverseMem(BYTE *str,DWORD stringLen)
{
    for(int i = 0;  i < (int)(stringLen/2); i++)
    {
        BYTE tmp;
        tmp = str[i];
        str[i] = str[stringLen - i-1];
        str[stringLen-i-1] =tmp;
    }
}

void CommonPlaceMessageBoxWork(const char * Titil,const char * msg)
{
    QMessageBox ex;
    ex.setWindowTitle (Titil);
    ex.setText (msg);
    ex.exec ();
}
