#include "gb376_1.h"
#include <QTime>
void GB376_1::AFN0f (TFN fn, DWORD *size,T0FTYPE   fileType)
{

    switch(currentComType)
    {
    case DownComm:
    {
        TDA     da = {0,0};
        char buf[2048];
        int bufferSize;
        switch (fileType)
        {
        case FILE_HEAD:
        {
            if(proType == GB09)
            {
                bufferSize =    sizeof(FileTransfer)+sizeof(fileNameDomain)+sizeof(TPW);
                *size = LengthWithAttach (bufferSize,1);
                memset(buf,0,bufferSize);

                FileTransfer *pTrans = (FileTransfer*)buf;
                pTrans->fileTags = 0x01;
                pTrans->fillePro = 0x00;
                pTrans->fileCmd = 0x00;
                pTrans->sumSegment = fileSegCnt+2;
                pTrans->fileOffset = upProgress;
                pTrans->dataLOfn = bufferSize - sizeof(TPW);
                fileNameDomain  * _fp = (fileNameDomain*)(buf+sizeof(FileTransfer));
                memset(_fp,0,sizeof(fileNameDomain));
                memcpy(_fp->filename,fileName,strlen (fileName));
                BYTE *__p = _fp->filename+strlen (fileName);
                __p[0] = 0x00;
                __p[1] = 0xe4;
                __p[2] = 0xce;

                qint64 _len = upFile.size ();

                memcpy (&__p[3],&_len,4);              //拷贝4字节的文件长度

                _fp->fTime[0] = Dec2Bcd(QTime::currentTime ().second ()%60);                                 //时间
                _fp->fTime[1] = Dec2Bcd(QTime::currentTime ().minute ()%60);
                _fp->fTime[2] = Dec2Bcd(QTime::currentTime ().hour ()%24);
                _fp->fTime[3] =  Dec2Bcd(QTime::currentTime ().toString ("dd").toInt ());
                _fp->fTime[4] = Dec2Bcd(QTime::currentTime ().toString ("MM").toInt ());
                _fp->fTime[5] = Dec2Bcd(QDateTime::currentDateTime ().toString ("yyyy").toInt () - 2000);
                break;
            }
        }
        case FILE_TAIL:
        {

            if(proType == GB09)
            {
                memset(buf,0,2048);
                bufferSize =    sizeof(FileTransfer)+sizeof(fileNameDomain)+sizeof(TPW);
                *size = LengthWithAttach (bufferSize,1);
                FileTransfer *pTrans = (FileTransfer*)buf;
                pTrans->fileTags = 0x01;
                pTrans->fillePro = 0x01;
                pTrans->fileCmd = 0x00;
                pTrans->fileOffset = upProgress;
                pTrans->sumSegment = fileSegCnt+2;
                fileNameDomain  * _fp = (fileNameDomain*)(buf+sizeof(FileTransfer)-2);
                memset(_fp,0,sizeof(fileNameDomain));
                memcpy(_fp->filename,fileName,strlen (fileName));
                BYTE *__p = _fp->filename+strlen (fileName);
                __p[0] = 0x00;                                                                                 //真的感觉蛋疼难道是压缩类型之类的东西？
                __p[1] = 0xe4;
                __p[2] = 0xce;
                qint64 _len = upFile.size ();
                memcpy (&__p[3],&_len,4);              //拷贝4字节的文件长度
                _fp->fTime[0] = Dec2Bcd(QTime::currentTime ().second ()%60);                                 //时间
                _fp->fTime[1] = Dec2Bcd(QTime::currentTime ().minute ()%60);
                _fp->fTime[2] = Dec2Bcd(QTime::currentTime ().hour ()%24);
                _fp->fTime[3] =  Dec2Bcd(QTime::currentTime ().toString ("dd").toInt ());
                _fp->fTime[4] = Dec2Bcd(QTime::currentTime ().toString ("MM").toInt ());
                _fp->fTime[5] = Dec2Bcd(QDateTime::currentDateTime ().toString ("yyyy").toInt () - 2000);
                __p = _fp->fTime+6;
                __p[0] = this->fileCrc16&0xff;
                __p[1] = (this->fileCrc16>>8)&0xff;

                break;
            }
        }
        case FILE_CONTENT:
        {
            memset (buf,0,2048);
            upFile.seek (upProgress*fileSegSize);
            qint64 theReadSize = upFile.read (buf+sizeof(FileTransfer),fileSegSize);
            if(theReadSize <= 0)
            {
                SetNoException();
                return;
            }
            if(proType == GB13)
            {
                ReverseMem ((BYTE*)buf+sizeof(FileTransfer),theReadSize);
                if((upFile.size () - upProgress*fileSegSize <= 2*fileSegSize )
                        ||(fileType == FILE_HEAD))
                {//当前已经到达末尾帧
                    SetNoException ();
                }
                else
                {
                    SetExceptionRepeat ();
                }
            }
            else
            {
                if(upFile.atEnd ())
                {
                    SetNoException ();
                }
                else
                {
                    SetExceptionRepeat ();
                }
            }
            bufferSize  =      sizeof(FileTransfer)+(int)theReadSize+sizeof(TPW);
            *size = LengthWithAttach (bufferSize,1);
            FileTransfer *pTrans = (FileTransfer*)buf;
            pTrans->fileTags = 0x01;
            if(fileType == FILE_TAIL)
            {
                pTrans->fillePro = 0x01;
            }
            else
                pTrans->fillePro = 0x00;
            pTrans->fileCmd = 0x00;
            pTrans->sumSegment = fileSegCnt+2;
            pTrans->fileOffset = upProgress;
            pTrans->dataLOfn = theReadSize;
        }
            break;
        default:
            break;
        }
        MakeApp (buf,bufferSize,da,&fn);
    }
        break;
    case UpComm:
    {
        TDA da;
        DWORD uIndex;
        switch (fileType)
        {
        case FILE_HEAD:
        case FILE_TAIL:
        case FILE_CONTENT:
        {
            checkAppAndGetAttachData ((char*)&uIndex,sizeof(DWORD),&da,&fn);
            upProgress = uIndex;
            upProgress++;
        }
            break;
            break;
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
}
