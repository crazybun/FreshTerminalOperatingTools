
#include "someprotocoltask.h"
static QList<TTaskStruct>  FileUpgradeTask;
static QList<TTaskStruct>  &CurrentTask = FileUpgradeTask;
static QList<QString>        AFNList;
static QList<enum ProtocolStack> PStack;
static void TaskAdd(QString tName,                            //任务名称
                    QList<QString> aFNList,                          //对应规约将要执行的功能码
                    //功能码命名规范
                    //终端上行规约  AFNXXFXXX 如AFN01F001代表功能码AFN01F1
                    //当为AFNXXF000,为要主站决定
                    QList<enum ProtocolStack> PStack, //规约嵌套栈0为最底层规约
                    enum CommType      commType,            //上行OR下行？
                    DWORD exceptionJumpIndex = 0x00                   //异常跳转
        )
{
    TTaskStruct *taskItem = new TTaskStruct;
    taskItem->tName = tName;
    taskItem->aFN     = aFNList;
    taskItem->PStack = PStack;
    taskItem->commType = commType;
    taskItem->exceptionJumpIndex = exceptionJumpIndex ;
    CurrentTask.append (*taskItem);
}
void InitTask_UpgradeFile_Com (int index)
{
    CurrentTask = FileUpgradeTask;
    PStack <<(enum ProtocolStack)index;
    AFNList <<"AFN02F001";
    TaskAdd ("终端链接",AFNList,PStack,UpComm);
    AFNList.clear ();
    AFNList<<"AFN00F001";
    TaskAdd ("主站回复",AFNList,PStack,DownComm);
    AFNList.clear ();
    AFNList<<"AFN09F001";
    TaskAdd ("版本召回",AFNList,PStack,DownComm) ;
    AFNList.clear ();
    AFNList<<"AFN09F001";
    TaskAdd ("版本回复",AFNList,PStack,UpComm,0x0C);
    AFNList.clear ();
    AFNList<<"AFN0FF001head";
    TaskAdd ("发送文件头",AFNList,PStack,DownComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN0FF001head";
    TaskAdd ("文件头回复",AFNList,PStack,UpComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN0FF001content";
    TaskAdd ("发送文件体",AFNList,PStack,DownComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN0FF001content";
    TaskAdd ("发送文件体",AFNList,PStack,UpComm,0x04);
    AFNList.clear ();
    AFNList<<"AFN0FF001tail";
    TaskAdd ("发送文件尾",AFNList,PStack,DownComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN0FF001tail";
    TaskAdd ("文件体回尾",AFNList,PStack,UpComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN01F001";
    TaskAdd ("发送复位",AFNList,PStack,DownComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN00F001";
    TaskAdd ("复位回复",AFNList,PStack,UpComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN04F003";
    TaskAdd ("设置参数",AFNList,PStack,DownComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN00F001";
    TaskAdd ("参数回复",AFNList,PStack,UpComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN01F001";
    TaskAdd ("发送复位",AFNList,PStack,DownComm,0x00);
    AFNList.clear ();
    AFNList<<"AFN00F001";
    TaskAdd ("复位回复",AFNList,PStack,UpComm,0xff);
    AFNList.clear ();
    AFNList<<"AFNFFFFFF";
    TaskAdd ("升级完成",AFNList,PStack,DownComm,0x00);
    AFNList.clear ();
}
void InitTask_UpgradeFile_Com_client_HB (void)
{
    CurrentTask = FileUpgradeTask;
    PStack << _GB13;
    AFNList<<"AFN09F001";
    TaskAdd ("版本召回",AFNList,PStack,DownComm) ;             //0x00
    AFNList.clear ();
    AFNList<<"AFN09F001";
    TaskAdd ("版本回复",AFNList,PStack,UpComm,0xff);          //0x01
    AFNList.clear ();
    AFNList<<"AFN0FF001head";
    TaskAdd ("发送文件头",AFNList,PStack,DownComm,0x00);   //0x02
    AFNList.clear ();
    AFNList<<"AFN0FF001head";
    TaskAdd ("文件头回复",AFNList,PStack,UpComm,0x00);     //0x03
    AFNList.clear ();
    AFNList<<"AFN0FF001content";
    TaskAdd ("发送文件体",AFNList,PStack,DownComm,0x00);   //0x04
    AFNList.clear ();
    AFNList<<"AFN0FF001content";
    TaskAdd ("发送文件体",AFNList,PStack,UpComm,0x04);     //0x05
    AFNList.clear ();
    AFNList<<"AFN0FF001tail";
    TaskAdd ("发送文件尾",AFNList,PStack,DownComm,0x00);   //0x06
    AFNList.clear ();
    AFNList<<"AFN0FF001tail";
    TaskAdd ("文件体回尾",AFNList,PStack,UpComm,0x00);         //0x07
    AFNList.clear ();
    AFNList<<"AFN01F001";
    TaskAdd ("发送复位",AFNList,PStack,DownComm,0x00);        //0x08
    AFNList.clear ();
    AFNList<<"AFN00F001";
    TaskAdd ("复位回复",AFNList,PStack,UpComm,0x00);        //0x09
    AFNList.clear ();
    AFNList<<"AFNFFFFFF";
    TaskAdd ("升级完成",AFNList,PStack,DownComm,0x00);        //0x0A
    AFNList.clear ();
}
void InitTask_UpgradeFile_CSG (void)
{
    CurrentTask = FileUpgradeTask;
    PStack << _CSG_2015;
    AFNList << "AFN02E0001000";
    TaskAdd ("终端链接",AFNList,PStack,UpComm,0);               //0
    AFNList.clear ();
    AFNList <<"AFN00E0000000";
    TaskAdd ("主站回复",AFNList,PStack,DownComm,0);       //1
    AFNList.clear ();
    AFNList <<"AFN0AE0000100";
    TaskAdd ("通讯方式招回",AFNList,PStack,DownComm,0);         //2
    AFNList.clear ();
    AFNList <<"AFN0AE0000100";
    TaskAdd ("通讯方式回复",AFNList,PStack,UpComm,0);             //3
    AFNList.clear ();
    AFNList <<"AFN0AE0000B01";
    TaskAdd ("版本招回",AFNList,PStack,DownComm,0);         //4
    AFNList.clear ();
    AFNList <<"AFN0AE0000B01";
    TaskAdd ("版本回复",AFNList,PStack,UpComm,0x0e);             //5
    AFNList.clear ();
    AFNList<<"AFN0FE3010003";
    TaskAdd ("查询文件信息",AFNList,PStack,DownComm,0);      //6
    AFNList.clear ();
    AFNList<<"AFN0FE3010003";
    TaskAdd ("文件信息回复",AFNList,PStack,UpComm,0);            //7
    AFNList.clear ();
    AFNList <<"AFN0FE3010001";
    TaskAdd ("文件传输启动",AFNList,PStack,DownComm,0);         //8
    AFNList.clear ();
    AFNList<<"AFN0FE3010001";
    TaskAdd ("文件传输启动OK",AFNList,PStack,UpComm,0);            //9
    AFNList.clear ();
    AFNList <<"AFN0FE3010002";
    TaskAdd ("传输文件内容",AFNList,PStack,DownComm,0);     //0xA
    AFNList.clear ();
    AFNList<<"AFN0FE3010002";
    TaskAdd  ("传输文件内容OK",AFNList,PStack,UpComm,0xA);        //0xB
    AFNList.clear ();
    AFNList<<"AFN0FE3010003";
    TaskAdd ("查询文件信息",AFNList,PStack,DownComm,0);      //0X0C
    AFNList.clear ();
    AFNList<<"AFN0FE3010003";
    TaskAdd ("文件信息回复",AFNList,PStack,UpComm,0);            //0X0D
    AFNList.clear ();
    AFNList <<"AFN04E0000100";
    TaskAdd ("通讯方式下发",AFNList,PStack,DownComm,0);         //0x0E
    AFNList.clear ();
    AFNList <<"AFN04E0000100";
    TaskAdd ("升级完成",AFNList,PStack,UpComm,0xff);             //0x0F
    AFNList.clear ();
}
void InitTask_UpgradeFile_Http(void)
{
    CurrentTask = FileUpgradeTask;
    PStack <<_GB09<<_SDMASTER;
    AFNList <<"AFN09F001"<<"sdMasterSend";
    TaskAdd ("版本召回",AFNList,PStack,DownComm,0);         //0
    AFNList.clear ();
    AFNList <<"AFN09F001"<<"sdMasterRecv";
    TaskAdd ("版本回复",AFNList,PStack,UpComm,0xff);          //1
    AFNList.clear ();
    PStack.clear ();
    PStack <<_WH<<_SDMASTER;
    AFNList <<"SendFileHead"<<"sdMasterSend";
    TaskAdd ("发送文件头",AFNList,PStack,DownComm,0);       //2
    AFNList.clear ();
    AFNList <<"RecvFileHead"<<"sdMasterRecv";
    TaskAdd ("文件头回复",AFNList,PStack,UpComm,2);     //3
    AFNList.clear ();
    AFNList <<"SendFileBody"<<"sdMasterSend";
    TaskAdd ("发送文件体",AFNList,PStack,DownComm,0);     //4
    AFNList.clear ();
    AFNList <<"RecvFileBody"<<"sdMasterRecv";
    TaskAdd ("文件体回复",AFNList,PStack,UpComm,4);     //5
    AFNList.clear ();
    AFNList <<"SendFileTail"<<"sdMasterSend";
    TaskAdd ("发送文件尾",AFNList,PStack,DownComm,0);     //7
    AFNList.clear ();
    AFNList <<"RecvFileTail"<<"sdMasterRecv";
    TaskAdd ("文件尾回复",AFNList,PStack,UpComm,0);     //8
    PStack.clear ();
    PStack <<_GB09<<_SDMASTER;
    AFNList.clear ();
    AFNList <<"AFN01F001"<<"sdMasterSend";
    TaskAdd ("发送复位",AFNList,PStack,DownComm,0);     //9
    AFNList.clear ();
    AFNList <<"AFN00F001"<<"sdMasterRecv";
    TaskAdd ("复位确认",AFNList,PStack,UpComm,0xff);     //10
}
QList<TTaskStruct>   GetFileUpgradeTaskList(void)
{
    return FileUpgradeTask;
}
