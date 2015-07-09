#ifndef EXCELPROC_H
#define EXCELPROC_H
#include <QAxObject>
#include<QAxWidget>
#include<QFile>
#include <QObject>
#include "BaseType.h"
class excelProc
{
private:
    QFile configFile;
    TerminalConfig *pTConfig;
    TParaSettings  *pParaSettings;
    enum TerAddrType theTerAddrType;
    QString excelFileName;
    QAxObject *pAppExcel;
     QAxObject *pAppWorkBooks;
     QAxObject *pAppWorkBook;
     QAxObject *pAppWorkSheet;
     bool exePermit;
public:
    excelProc(QObject *parent);
    void ExcleInit(void);
    void QueryExcelValue(void);
    void Write2File(void);
    QList<TerminalConfig> tList;
    bool isPermit(void)
    {
        return this->exePermit;
    }
};

#endif // EXCELPROC_H
