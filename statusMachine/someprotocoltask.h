#ifndef SOMEPROTOCOLTASK_H
#define SOMEPROTOCOLTASK_H
#include "../BaseType.h"
#include <QList>
#include <QString>

typedef struct _task_struct{
    QString tName;                                                       //任务名称
    QList<QString> aFN;                                            //对应规约将要执行的功能码
                                                                                  //功能码命名规范
                                                                                 //终端上行规约  AFNXXFXXX 如AFN01F001代表功能码AFN01F1
                                                                                //当为AFNXXF000,为要主站决定
   QList<enum ProtocolStack> PStack;                    //规约嵌套栈0为最底层规约
    enum CommType      commType;                         //上行OR下行？
    DWORD exceptionJumpIndex;                            //异常跳转的Index
}TTaskStruct;
void InitTask_UpgradeFile_Com (int index);
void InitTask_UpgradeFile_CSG (void);
void InitTask_UpgradeFile_Http(void);
void InitTask_UpgradeFile_Com_client_HB (void);
QList<TTaskStruct>   GetFileUpgradeTaskList(void);
#endif // SOMEPROTOCOLTASK_H
