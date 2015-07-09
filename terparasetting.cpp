#include "terparasetting.h"
#include "ui_terparasetting.h"
#include <QTextCodec>
#include <QFileDialog>
TerParaSetting::TerParaSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerParaSetting)
{
    ui->setupUi(this);

    //  codec = QTextCodec::codecForName ("system");
    //   QTextCodec::setCodecForLocale(codec);
    ui->mGrpBox->hide ();
    ui->uGrpBox->hide ();
    ui->pGrpBox->hide ();
    ui->speTerTypeCbx->setDisabled (true);
    pTerminalPara = new TTerParaSettings;
    memset(pTerminalPara,0,sizeof(TTerParaSettings));
    paraSettingsFile.setFileName (TERPARASFILENAME);
    if(paraSettingsFile.exists ())
    {
        paraSettingsFile.open (QIODevice::ReadWrite);
    }
    SettingWidgetByPara ();
}

TerParaSetting::~TerParaSetting()
{
    delete ui;
}

void TerParaSetting::SettingWidgetByPara (void)
{
    if(paraSettingsFile.exists ())
    {
        memcpy (pTerminalPara,paraSettingsFile.readAll (),sizeof( TTerParaSettings));
        theFileName = QString(pTerminalPara-> filePathName);
        ui->uFileLed->setText (theFileName);
        ui->mIpLed->setText (pTerminalPara-> mIp);
        ui->mPortLed->setText (QString::number (pTerminalPara-> mPort));
        ui->bakIPLed->setText (pTerminalPara-> bIp);
        ui->bakPortLed->setText (QString::number (pTerminalPara-> bPort));
        ui->apnLed->setText (pTerminalPara-> apn);
        ui->swReleaseDLed->setText (pTerminalPara-> swReleaseDate);
        ui->swReleaseTLed->setText (pTerminalPara-> swReleaseTime);
        ui->swVersionLed->setText (pTerminalPara-> swVersion);
        ui->TerUpPcbx->setCurrentIndex (pTerminalPara-> terProtocol);
        ui->masterPCbx->setCurrentIndex (pTerminalPara-> masterProtocol);
        ui->meterUPCbx->setCurrentIndex (pTerminalPara-> meterProtocol);
        ui->terAddrCCbx->setCurrentIndex (pTerminalPara-> terAddrType);
        ui->terAddrAssCbx->setCurrentIndex (pTerminalPara->terAddrOperateType);
        ui->terTypeCbx->setCurrentIndex (pTerminalPara->terType);
        ui->overTimerLed->setText (QString::number (pTerminalPara->overTimeGap));
        ui->SRDelayLed->setText (QString::number (pTerminalPara->SRDelayTime));
        ui->FileSSLed->setText (QString::number (pTerminalPara->dwFileSegSize));
        if(pTerminalPara->terType == SPETERMINAL)
        {
            ui->speTerTypeCbx->setEnabled (true);
        }
        ui->speTerTypeCbx->setCurrentIndex (pTerminalPara->speTerType);
        if(pTerminalPara-> isChecked[0])
        {
            ui->uParaChbx->setChecked (true);
            ui->uGrpBox->show();
        }
        if(pTerminalPara-> isChecked[1])
        {
            ui->spParaChbx->setChecked (true);
            ui->pGrpBox->show();
        }
        if(pTerminalPara-> isChecked[2])
        {
            ui->miscParaChbx->setChecked (true);
            ui->mGrpBox->show();
        }
        if(pTerminalPara->overTimechecked)
        {
            ui->overTimeChBx->setChecked (true);
            ui->overTimerLed->show ();
        }
    }
}

void TerParaSetting::SettingParaByWidget ()
{
    memset (pTerminalPara-> filePathName,0,256);
    memcpy (pTerminalPara-> filePathName,theFileName.toLatin1 ().data (),theFileName.size ());
    memcpy (pTerminalPara-> mIp,ui->mIpLed->text ().toLatin1 ().data (),32);
    memcpy (pTerminalPara-> bIp,ui->bakIPLed->text ().toLatin1 ().data (),32);
    memcpy (pTerminalPara-> apn,ui->apnLed->text ().toLatin1 ().data (),32);
    memcpy (pTerminalPara-> swReleaseTime,ui->swReleaseTLed->text ().toLatin1 ().data(),32);
    memcpy (pTerminalPara-> swReleaseDate,ui->swReleaseDLed->text ().toLatin1 ().data(),32);
    memcpy (pTerminalPara-> swVersion,ui->swVersionLed->text().toLatin1 ().data (),32);
    pTerminalPara-> mPort = ui->mPortLed->text ().toUShort ();
    pTerminalPara-> bPort = ui->bakPortLed->text().toUShort ();
    pTerminalPara->SRDelayTime = ui->SRDelayLed->text ().toULong ();
    pTerminalPara->dwFileSegSize = ui->FileSSLed->text ().toULong ();
    pTerminalPara-> terProtocol = (enum TerProtocol)ui->TerUpPcbx->currentIndex ();
    pTerminalPara-> terAddrType = (enum TerAddrType)ui -> terAddrCCbx->currentIndex ();
    pTerminalPara-> masterProtocol = (enum MasterProtocol)ui->masterPCbx->currentIndex ();
    pTerminalPara-> meterProtocol = (enum MeterProtocol)ui->meterUPCbx->currentIndex ();
    pTerminalPara->terAddrOperateType = (enum TerminalAddrOperateType)ui->terAddrAssCbx->currentIndex ();
    pTerminalPara->terType = (enum TerminalType)ui->terTypeCbx->currentIndex ();
    pTerminalPara->speTerType = (enum SpeTerType)ui->speTerTypeCbx->currentIndex ();
    pTerminalPara->overTimeGap = ui->overTimerLed->text ().toULong ();
    pTerminalPara-> isChecked[0] = (ui->uParaChbx->isChecked ()?true:false);
    pTerminalPara-> isChecked[1] = (ui->spParaChbx->isChecked ()?true:false);
    pTerminalPara-> isChecked[2] = (ui->miscParaChbx->isChecked ()?true:false);
    pTerminalPara->overTimechecked = (ui->overTimeChBx->isChecked ()?true:false);
    if(paraSettingsFile.isOpen ())
    {
        paraSettingsFile.close ();
    }
    paraSettingsFile.open (QIODevice::WriteOnly);
    paraSettingsFile.write ((const char*)pTerminalPara,sizeof( TTerParaSettings));
    paraSettingsFile.close ();
}
void TerParaSetting::on_asUpFileBttn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName((QWidget *)this->parent (),
                                                    tr(" 打开升级文件"), "*", tr("升级文件"));
    //theFileName = codec->toUnicode (fileName.toLatin1 ());
    theFileName = fileName;
    ui->uFileLed->setText (fileName);
}

void TerParaSetting::on_sureBttn_clicked()
{
    SettingParaByWidget ();
    this->close ();
    ((QWidget*)this->parent ())->close ();
}

void TerParaSetting::on_cancelBttn_clicked()
{
    this->close ();
    ((QWidget*)this->parent ())->close ();
}

void TerParaSetting::on_masterPCbx_activated(const QString &arg1)
{
    QString ss("新联");
    if(0 == arg1.compare (ss))
    {
        this->hide ();
        XLMasterWin *xlMasterWin = new XLMasterWin(&pTerminalPara->xlMasterPara);
        xlMasterWin->setWindowModality (Qt::ApplicationModal);
        xlMasterWin->show ();
        connect (xlMasterWin,SIGNAL(XLSettingFinished()),this,SLOT(show()));
    }
}

void TerParaSetting::on_terTypeCbx_activated(const QString &arg1)
{
    QString ss("专变终端");
    if(0 == arg1.compare (ss))
    {
        ui->speTerTypeCbx->setEnabled (true);
    }
    else
    {
        ui->speTerTypeCbx->setDisabled (true);
    }
}

void TerParaSetting::on_overTimeChBx_clicked(bool checked)
{
    ui->overTimerLed->setVisible (checked);
}
