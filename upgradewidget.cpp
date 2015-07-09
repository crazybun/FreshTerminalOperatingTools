#include "upgradewidget.h"
#include "ui_upgradewidget.h"
#include <QProgressBar>
#include <QMetaType>
#include <QCheckBox>
UpgradeWidget::UpgradeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpgradeWidget)
{
    ui->setupUi(this);
    ui->uPauseBttn->setDisabled (true);

    initTableWidget ();
    pComPro = NULL;
    pTConfig = new TerminalConfig [MAXCONFIG];
    connect (this,SIGNAL(TableAdd()),this,SLOT(AddTerAddr2Table()));
    TableAdd ();
    qRegisterMetaType<TUpgradeUIStruct>("TUpgradeUIStruct");

}
void UpgradeWidget::paintEvent (QPaintEvent *e)
{
    e = e;

    theTableWiget->resize (this->size ().width (),this->size ().height ()-ui->groupBox->size ().height ()-20);
    ui->groupBox->setGeometry (this->size ().width ()-10-ui->groupBox->width (),
                                                    this->size ().height ()-10-ui->groupBox->height (),
                                                    ui->groupBox->width (),
                                                    ui->groupBox->height ());
   reSizeTableColumn ();
}
void UpgradeWidget::reSizeTableColumn ()
{
    int x = (theTableWiget->width ())/(theTableColumnCount);
    if( x >= 50)
    {
        for( int i = 0 ; i < theTableColumnCount ; i++ )
        theTableWiget ->  setColumnWidth ( i , x );
    }
    else
    {
        for( int i = 0 ; i < theTableColumnCount; i++ )
        theTableWiget -> setColumnWidth ( i , 50 );
    }
}

void UpgradeWidget::initTableWidget ()
{
    theHeaders   <<"终端名称"
                 <<"升级状态"
                 <<"升级进度"
                 <<"开始时间"
                 <<"完成时间"
                 <<"升前版本 |  日期 |  时间"
                 <<"升后版本 |  日期 |  时间"
                 <<"终端类型"
                 <<"终端状态";

    theTableWiget = new QTableWidget ( this );
    theTableWiget -> setEditTriggers( QAbstractItemView::NoEditTriggers );
    theTableWiget -> autoFillBackground ();
    theTableColumnCount = 9;
    theTableWiget -> setColumnCount (theTableColumnCount);
    reSizeTableColumn ();
    theTableWiget -> setHorizontalHeaderLabels (theHeaders);
}
UpgradeWidget::~UpgradeWidget()
{
    delete ui;
}

void UpgradeWidget::on_uStartBttn_clicked()
{
    if(!pComPro)
    {
        pComPro = new CommProcess(this);
        enum TaskInitStatus tmpStatus;
        if((tmpStatus= pComPro->GetTaskInitStatus ()) != INITSUCCESS)
        {     
            const char* errMsg = "未设置当前错误信息";
            switch(tmpStatus)
            {
            case PARAERROR:
                errMsg = ("参数错误！");
                break;
            case  SERIALINITERROR:
                errMsg = ("串口打开失败！");
                break;
            case    TCPLISTENERROR:
                errMsg = ("TCP监听端口失败！");
                break;
            case    BINDERROR:
                errMsg = ("绑定端口失败！");
                break;
            case   TERMINALADDRERROR:
                errMsg = ("终端地址未设置！");
                break;
            case   UPFILENOTEXIST:
                errMsg = ("升级文件未找到！");
                break;
            case  UDPNOTSEVER:
                errMsg = ("请在参数设置中选择服务器模式");
            default:
                break;
            }
            CommonPlaceMessageBoxWork ("错误",errMsg);
            delete pComPro;
            pComPro = NULL;
            return;
        }
    }
    else
    {
        pComPro->ThreadWork ();
    }
    emit SettingProcSignal (false);
    ui->uStartBttn->setDisabled (true);
    ui->uPauseBttn->setEnabled (true);
    ui->terAddrABttn->setDisabled (true);
    ui->terAddrClrBttn->setDisabled (true);
}

void UpgradeWidget::on_uPauseBttn_clicked()
{
    emit SettingProcSignal (true);
    ui->uStartBttn->setDisabled (false);
    ui->uPauseBttn->setEnabled (false);
     ui->terAddrABttn->setDisabled (false);
     ui->terAddrClrBttn->setDisabled (false);
     if(pComPro)
     {
         pComPro->ThreadPause ();
     }
}

void UpgradeWidget::on_terAddrABttn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle ("错误");
    msgBox.setText ("指定终端地址");
    msgBox.setInformativeText ("导入EXCEL文件？");
    msgBox.setStandardButtons (QMessageBox::Yes|QMessageBox::No);

    if(QMessageBox::Yes == msgBox.exec ())
    {
        excelProc *excelObject = new excelProc(this);
        if(!excelObject->isPermit ())
        {
            QMessageBox ex;
            ex.setWindowTitle ("错误");
            ex.setText ("未找到配置文件");
            ex.exec ();
            return;
        }
        excelObject->ExcleInit ();
        excelObject->QueryExcelValue ();
        excelObject->Write2File ();
        emit TableAdd ();
    }
    else
    {
        TerAddrDialog *pTerD = new TerAddrDialog(this);
         pTerD->show();
    }

}
void UpgradeWidget::AddTerAddr2Table ()
{
    int lines = 0; 
     GetTerConfig (pTConfig);
     for(int i = 0 ; pTConfig[i].IsUsed; i++)
     {
         lines++;
     }
     this->theTableWiget->setRowCount(lines); //设置表格行
     for(int i = 0 ; pTConfig[i].IsUsed ;i++)
     {

         this->theTableWiget->setItem(i,0,new QTableWidgetItem(pTConfig[i].ARCandTAddr));
         this->theTableWiget->item (i,0)->setFont (QFont("Helvetica [Cronyx]", 10));
         this->theTableWiget->item (i,0)->setTextColor (QColor(255,69,0));
         this->theTableWiget->setItem(i,1,new QTableWidgetItem(""));
         this->theTableWiget->setCellWidget (i,2,new QProgressBar(this->theTableWiget));
         this->theTableWiget->cellWidget (i,2)->resize (this->theTableWiget->width ()/this->theTableColumnCount,
                                       this->theTableWiget->cellWidget (i,2)->height ());
         QProgressBar *Tb;
         Tb = (QProgressBar*)this->theTableWiget->cellWidget (i,2);
         Tb->setMaximum (100);

         this->theTableWiget->setItem(i,3,new QTableWidgetItem(""));
         this->theTableWiget->setItem(i,4,new QTableWidgetItem(""));
         this->theTableWiget->setItem(i,5,new QTableWidgetItem(""));
         this->theTableWiget->setItem(i,6,new QTableWidgetItem(""));
         this->theTableWiget->setItem (i,7,new QTableWidgetItem(""));
         this->theTableWiget->setItem (i,8,new QTableWidgetItem(""));
     }
}

void UpgradeWidget::UiFresh (TUpgradeUIStruct UIs)
{
    int i = 0; 
    for(; i< theTableWiget->rowCount ();i++)
    {
        if(0 == memcmp(UIs.ATT,
                       theTableWiget->item (i,0)->text().toLatin1 ().data (),
                       theTableWiget->item (i,0)->text().length ()))
        {
            this->theTableWiget->item (i,0)->setTextColor (QColor(46,139,87));
            this->theTableWiget->item (i,0)->setFont (QFont("Helvetica [Cronyx]", 10));
            QProgressBar *Tb;
            Tb = (QProgressBar*)this->theTableWiget->cellWidget (i,2);
            Tb->setValue (UIs.upProgress);
            this->theTableWiget->item (i,1)->setText (UIs.UpStatus);
            this->theTableWiget->item (i,3)->setText (UIs.terStartTime);
            this->theTableWiget->item (i,4)->setText (UIs.finishTime);
            this->theTableWiget->item (i,5)->setText (UIs.VersionBeforUpgrade);
            this->theTableWiget->item (i,6)->setText (UIs.VersionAfterUpgrade);
            this->theTableWiget->item (i,7)->setText (UIs.TerType);
            this->theTableWiget->item (i,8)->setText (UIs.TerStatus);
            return;
        }
    }
    if(i >= theTableWiget->rowCount ())
    {//不存在就添加
        theTableWiget->setRowCount (theTableWiget->rowCount ()+1);
        this->theTableWiget->setItem(i,0,new QTableWidgetItem((char*)UIs.ATT));
        this->theTableWiget->setItem(i,1,new QTableWidgetItem(""));
        this->theTableWiget->setCellWidget (i,2,new QProgressBar(this->theTableWiget));
        this->theTableWiget->cellWidget (i,2)->resize (this->theTableWiget->width ()/this->theTableColumnCount,
                                                                 this->theTableWiget->cellWidget (i,2)->height ());\
        QProgressBar *Tb;
        Tb = (QProgressBar*)this->theTableWiget->cellWidget (i,2);
        this->theTableWiget->setItem(i,3,new QTableWidgetItem(""));
        this->theTableWiget->setItem(i,4,new QTableWidgetItem(""));
        this->theTableWiget->setItem(i,5,new QTableWidgetItem(""));
        this->theTableWiget->setItem(i,6,new QTableWidgetItem(""));
        this->theTableWiget->setItem (i,7,new QTableWidgetItem(""));
        this->theTableWiget->setItem (i,8,new QTableWidgetItem(""));
        Tb->setValue (UIs.upProgress);
        this->theTableWiget->item (i,1)->setText (UIs.UpStatus);
        this->theTableWiget->item (i,3)->setText (UIs.terStartTime);
        this->theTableWiget->item (i,4)->setText (UIs.finishTime);
        this->theTableWiget->item (i,5)->setText (UIs.VersionBeforUpgrade);
        this->theTableWiget->item (i,6)->setText (UIs.VersionAfterUpgrade);
        this->theTableWiget->item (i,7)->setText (UIs.TerType);
        this->theTableWiget->item (i,8)->setText (UIs.TerStatus);

    }

}
void UpgradeWidget::on_terAddrClrBttn_clicked()
{
    QMessageBox ex;
    ex.setWindowTitle ("警告");
    ex.setText ("确认清空终端地址？");
    ex.setStandardButtons (QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes ==  ex.exec ())
    {
        QFile cFile(CONFIGNAME);
        if(cFile.exists ())
        {
            cFile.open (QIODevice::Truncate|QIODevice::WriteOnly);
        }
        cFile.close ();
         emit TableAdd ();
    }
}
