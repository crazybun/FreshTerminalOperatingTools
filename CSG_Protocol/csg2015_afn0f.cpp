#include "csg2015.h"
//File transform.
void csg2015::AFN0f (DWORD uDI, DWORD *size)
{
     BYTE theafn = 0x0f;
    switch(currentComType)
    {
    case DownComm:
    {
        TDA da = {0,0};

        switch (uDI) {
        case 0xE3010001:
        {
            TCSG_E3010001 content;
            *size = LengthWithAttach (sizeof(TCSG_E3010001),1);
            MakeHeader (*size);
            MakeControlFiled (0x40);
            MakeAddr (theAAT,0x08);
            MakeAFNInfo (theafn,++theSeq);
            memset(&content,0,sizeof(TCSG_E3010001));
            content.dwFileSize = (DWORD)upFile.size ();
            memcpy(content.szFileName,fileName,strlen(fileName));
            content.wCrc16 = fileCrc16;
            content.wFileProperties = UPGRADE_FILE_TYPE;
            content.wFileSegSum = fileSegCnt;
            MakeApp ((char*)&content,sizeof(TCSG_E3010001),da,&uDI);
            MakeTail (checkSumPoint (),checkSumLen (*size));
            QByteArray ba = QByteArray((char*)pDwnStart,*size);
            QString ss;
            SPECData2String (ba,ss);
            qDebug() << ss;
        }
        break;
        case 0xE3010002:
        {
            TCSG_E3010002 content;
            char buf[2048];
            memset(buf,0,2048);
            memset(&content,0,sizeof(TCSG_E3010002));
             upFile.seek(upProgress*fileSegSize);
             content.wFileSegNum = upProgress;
             content.wFileSegSize = upFile.read (buf,fileSegSize);
            *size = LengthWithAttach (sizeof(TCSG_E3010002)+2+content.wFileSegSize,1);
            MakeHeader (*size);
            MakeControlFiled (0x40);
            MakeAddr (theAAT,0x08);
            MakeAFNInfo (theafn,++theSeq);
            MakeApp ((char*)&content,sizeof(TCSG_E3010002),da,&uDI);
            memcpy (pDwnContent,buf,content.wFileSegSize);
            WORD _segcrc16 =(WORD) fileCrc16Calc (content.wFileSegSize,(BYTE*)buf);
           *((WORD*)&pDwnContent[content.wFileSegSize]) = _segcrc16;
           pDwnContent += (content.wFileSegSize+2);
            qDebug() << (DWORD)(pDwnContent -pDwnStart);
            MakeTail (checkSumPoint (),checkSumLen (*size));
            QByteArray ba = QByteArray((char*)pDwnStart,*size);
            QString ss;
            SPECData2String (ba,ss);
            qDebug() << ss;
        }
        break;
        case 0xE3010003:
        {
           TCSG_E3010003 content;
            *size = LengthWithAttach (sizeof(TCSG_E3010003),1);
            MakeHeader (*size);
            MakeControlFiled (0x40);
            MakeAddr (theAAT,0x08);
            MakeAFNInfo (theafn,++theSeq);
            memset(&content,0,sizeof(TCSG_E3010003));
            content.FD1.dwFileSize = (DWORD)upFile.size ();
            memcpy(content.FD1.szFileName,fileName,strlen(fileName));
            content.FD1.wCrc16 = fileCrc16;
            content.FD1.wFileProperties = UPGRADE_FILE_TYPE;
            content.FD1.wFileSegSum = (upFile.size()/fileSegSize)+(((upFile.size ()/fileSegSize)==0)?0:1);
            content.wFileStartSegNo = 0;
            content.wFileEndSegNo = 0;
            MakeApp ((char*)&content,sizeof(TCSG_E3010003),da,&uDI);
            MakeTail (checkSumPoint (),checkSumLen (*size));
            QByteArray ba = QByteArray((char*)pDwnStart,*size);
            QString ss;
            SPECData2String (ba,ss);
            qDebug() << ss;
        }
        default:
            break;
        }
        break;
    }
    case UpComm:
        TDA da;
        WORD len;
        BYTE afn;
        DWORD uuDI ;
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
        if(afn != 0xf)
        {
           SetExceptionJump ();
            return;
        }
        switch (uDI) {
        case 0xE3010001:
        {
               char edc;
               QByteArray ba = QByteArray((char*)pUpStart,*size);
               QString ss;
               SPECData2String (ba,ss);
               qDebug() << ss;
               checkAppAndGetAttachData (&edc,sizeof(char),&da,&uuDI);
               if(edc == 0x00)
               {
                   SetNoException ();
               }
               else
               {
                   SetExceptionRepeat ();
               }
        }
        break;
        case 0xE3010002:
        {
            char edc;
            QByteArray ba = QByteArray((char*)pUpStart,*size);
            QString ss;
            SPECData2String (ba,ss);
            qDebug() << ss;
            checkAppAndGetAttachData (&edc,sizeof(char),&da,&uuDI);
            if(edc == 0x00)
            {
                SetExceptionJump ();
                upProgress++;
                if(upProgress >= fileSegCnt)
                {
                   SetNoException ();
                }
            }
            else
            {
                SetExceptionRepeat ();
            }
        }
        break;
        case 0xE3010003:
        {
            TCSG_E3010003 content;
            QByteArray ba = QByteArray((char*)pUpStart,*size);
            QString ss;
            SPECData2String (ba,ss);
            qDebug() << ss;
            checkAppAndGetAttachData((char*)&content,sizeof(TCSG_E3010003),&da,&uuDI);
            upProgress  =(WORD) content.wFileEndSegNo;
            if(upProgress == 0)
            {
                upProgress = content.FD1.dwFileSize/fileSegSize;
            }
            if(upProgress >= fileSegCnt  - 1)
            {
                SetExceptionJump ();
            }
            else
            {
               SetNoException ();
            }
        }
        default:
            break;
        }
            break;
        default:
            break;
    }
}
