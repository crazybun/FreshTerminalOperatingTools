#include "excelproc.h"
#include <QFileDialog>
#include <QDebug>
#include <QtAlgorithms>
#include <QMessageBox>

excelProc::excelProc(QObject *parent)
{
    if(!GetParaSettings(&pParaSettings))
    {
        CommonPlaceMessageBoxWork ("错误！","终端地址未配置！");
        this->exePermit = false;
        return;
    }
    this->theTerAddrType = pParaSettings->terPara.terAddrType;
    this->pTConfig = new TerminalConfig[MAXCONFIG];
    configFile.setFileName (CONFIGNAME);
    if(!configFile.exists ())
    {
        configFile.open (QIODevice::WriteOnly);
    }
    else
    {
        configFile.open (QIODevice::ReadWrite);
        QByteArray ba =  configFile.readAll ();
        memcpy (pTConfig,ba.data (),ba.size ());  //读出所有配置
    }
   excelFileName = QString("TerConfig.xls");
    QFile excelFile(excelFileName);
    if(!excelFile.exists ())
    {
         excelFileName = QFileDialog::getOpenFileName ((QWidget*)parent," Excel 文件", "*.xlsx", "配置文件(*.xls)");
         excelFile.setFileName (excelFileName);
    }
    if(!excelFile.exists ())
    {
        this->exePermit = false;
        return;
    }
    this->exePermit =true;

}
void excelProc::ExcleInit (void)
{
    this->pAppExcel = NULL;
    this->pAppWorkBooks = NULL;
    this->pAppWorkBook = NULL;
    this->pAppWorkSheet  =NULL;
    pAppExcel  = new QAxObject("Excel.Application");
    if(!this->pAppExcel)
    {
        return;
    }
    pAppExcel->dynamicCall("SetVisible(bool)", false);
    pAppWorkBooks = pAppExcel->querySubObject("WorkBooks");
    pAppWorkBook = pAppWorkBooks->querySubObject ("Open(QString, QVariant)",this->excelFileName);
    pAppWorkSheet = pAppWorkBook->querySubObject("WorkSheets(int)", 1);//打开第一个sheet

}
void excelProc::QueryExcelValue (void)
{
    QAxObject * uRange = pAppWorkSheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = uRange->querySubObject("Rows");
    QAxObject * columns = uRange->querySubObject("Columns");
    /*获取行数和列数*/
    int intRowStart = uRange->property("Row").toInt();
    int intColStart = uRange->property("Column").toInt();
    int intCols = columns->property("Count").toInt();
    int intRows = rows->property("Count").toInt();
    /*获取excel内容*/
    for (int i = intRowStart+1; i < intRowStart + intRows; i++)  //行
    {
        for (int j = intColStart; j < intColStart + intCols; j++)  //列
        {
            QAxObject * cell = pAppWorkSheet->querySubObject("Cells(int,int)", i, j );  //获取单元格
            char *p =cell->dynamicCall ("Value2()").toByteArray ().data ();
            if('0'<=p[0]&& p[0] <='9')
            {
                TerminalConfig tmpConfig ;
                memset (&tmpConfig,0,sizeof(TerminalConfig));
                tmpConfig.AddrType = HEX_ADDR;
                if(this->theTerAddrType == FADDR ||
                        this->theTerAddrType == CSGADDR)
                {
                    memcpy(tmpConfig.ARCandTAddr,p,12);
                }
                else if(this->theTerAddrType == TADDR)
                {
                    memcpy(tmpConfig.ARCandTAddr,p,8);
                }
                tmpConfig.IsUsed = true;
                InsertConfig (this->pTConfig,tmpConfig,this->theTerAddrType);
                delete cell;
            }
        }
    }
   pAppWorkBook->dynamicCall("Close (Boolean)",false);
   this->pAppExcel->dynamicCall ("Quit ()");
   delete this->pAppExcel;//一定要记得删除，要不线程中会一直打开excel.exe
}

void excelProc::Write2File (void)
{
    this->configFile.seek (0)  ;                                                     //清除
    this->configFile.write ((char*)this->pTConfig,sizeof(TerminalConfig)*MAXCONFIG);
    this->configFile.close ();
}
