#include "xlmasterwin.h"
#include "ui_xlmasterwin.h"

XLMasterWin::XLMasterWin(TXLMasterPara *para,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XLMasterWin)
{
    ui->setupUi(this);
    this->setWindowFlags (Qt::FramelessWindowHint);
    QPalette palette = this->palette ();
    palette.setBrush (this->backgroundRole (),QBrush(QImage(BACKGROUNDIMAGIN)));
    this->setPalette (palette);
    memcpy(&this->masterPara,para,sizeof(TXLMasterPara));
    pPara = para;
    ui->userLed->setText (QString(QLatin1String(this->masterPara.userName)));
    ui->passwdLed->setText (QString(QLatin1String(this->masterPara.passwd)));
    ui->chInfoLed->setText (QString(QLatin1String(this->masterPara.channelNum)));
    ui->heartBeatLed->setText (QString(QLatin1String(this->masterPara.heartBeatTime)));
    ui->pVersionLed->setText (QString(QLatin1String(this->masterPara.pVersion)));
    ui->TransWLed->setText (QString(QLatin1String(this->masterPara.tranWms)));
}

XLMasterWin::~XLMasterWin()
{
    delete ui;
}

void XLMasterWin::on_sureBttn_clicked()
{
    QsSwitchToChar (ui->userLed->text (),this->masterPara.userName);
    QsSwitchToChar (ui->passwdLed->text (),this->masterPara.passwd);
    QsSwitchToChar (ui->chInfoLed->text (),this->masterPara.channelNum);
    QsSwitchToChar (ui->heartBeatLed->text (),this->masterPara.heartBeatTime);
    QsSwitchToChar (ui->TransWLed->text (),this->masterPara.tranWms);
    QsSwitchToChar (ui->pVersionLed->text (),this->masterPara.pVersion);
    memcpy (pPara,&this->masterPara,sizeof(TXLMasterPara));
    this->close ();
    emit XLSettingFinished ();
}

void XLMasterWin::on_cancelBttn_clicked()
{
    this->close ();
    emit XLSettingFinished ();
}
