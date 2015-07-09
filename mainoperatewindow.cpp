#include "mainoperatewindow.h"
#include "ui_mainoperatewindow.h"
#include <QMessageBox>
#include <QLabel>
MainOperateWindow::MainOperateWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainOperateWindow)
{
    ui->setupUi(this);
    theMdiArea = new QMdiArea(this);
    theMdiArea->setBackground (QBrush(Qt::NoBrush));
    theMdiArea->setStyleSheet ("background:url(:/new/prefix1/Tres/flfd.png);");
    this->setCentralWidget (theMdiArea);
    theComParaS = new ComParaSetting();
    theTerParaS = new TerParaSetting();
    theUpgradeWidget = new UpgradeWidget();
    connect (theUpgradeWidget,SIGNAL(SettingProcSignal(bool)),this,SLOT(SettingPermitOrProhibited(bool)));
    theProPraseWidget = new ProPraseWidget();

}

MainOperateWindow::~MainOperateWindow()
{
    delete ui;
}
void MainOperateWindow::paintEvent (QPaintEvent * e)
{
    e = e;
}
void MainOperateWindow::SubWindowAdd (QWidget *w)
{
    for(int i = 0; i< theMdiArea->subWindowList().size ();i++)
    {
        if(0 == theMdiArea->subWindowList().at (i)->windowTitle ().compare (w->windowTitle ()))
        {
           if(theMdiArea->subWindowList ().at (i)->isHidden ())
           {//Show when it hidden
               theMdiArea-> subWindowList().at (i)->widget ()->show ();
           }
           else
           {//Focus when it choosen while not hidden.
               theMdiArea->subWindowList ().at (i)->widget ()->setFocus ();
           }

            return;
        }
    }
    QMdiSubWindow *qms = new QMdiSubWindow();
    qms->setWidget (w);
    theMdiArea->addSubWindow (qms);
    w->show ();
}
void MainOperateWindow::on_actionComPara_triggered()
{
    SubWindowAdd (theComParaS);
}
void MainOperateWindow::on_actionTerPara_triggered()
{
    SubWindowAdd (theTerParaS);
}

void MainOperateWindow::on_actionUpgrade_triggered()
{
    if(!SettingWidgetProcBefOperate ())
        return;
    SubWindowAdd (theUpgradeWidget);
}

bool MainOperateWindow::SettingWidgetProcBefOperate ()
{
    if(!theComParaS->isHidden ()
            ||!theTerParaS->isHidden ())
    {
        CommonPlaceMessageBoxWork ("警告","设置窗口未关闭！");
        if(!theComParaS->isHidden ())
        theComParaS->setFocus ();
        if(!theUpgradeWidget->isHidden ())
        theTerParaS->setFocus ();
        return false;
    }
    return true;
}
void MainOperateWindow::SettingPermitOrProhibited (bool isEnabled)
{
    if(isEnabled)
    {
        ui->actionComPara->setEnabled (true);
        ui->actionTerPara->setEnabled (true);
    }
    else
    {
        ui->actionComPara->setDisabled (true);
        ui->actionTerPara->setDisabled (true);
    }
}

void MainOperateWindow::on_actionProParse_triggered()
{
    if(!SettingWidgetProcBefOperate ())
        return;
    SubWindowAdd (theProPraseWidget);
}
