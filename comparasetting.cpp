#include "comparasetting.h"
#include "ui_comparasetting.h"
const DWORD BudaRate[] = {1200,2400,4800,9600,19200,38400,115200,0xffffffff};
ComParaSetting::ComParaSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComParaSetting)
{
    ui->setupUi(this);
    ui->serialGrpBox->hide ();
    ui->clientGrpBox->hide ();
    ui->serverGrpBox->hide();
    ui->inPCbx->hide ();
    ui->label_5->hide ();
    pCommPara = new TComParaSettings;
    paraSettingsFile.setFileName (COMPARASFILENMAE);
    if(paraSettingsFile.exists ())
    {
        paraSettingsFile.open (QIODevice::ReadWrite);
    }
    SettingWidgetByPara ();
    on_inPCbx_activated (ui->inPCbx->currentText ());
}

ComParaSetting::~ComParaSetting()
{
    delete ui;
}

void ComParaSetting::on_serialRdBttn_toggled(bool checked)
{
    if(checked)
    {
        ui->serialGrpBox->show ();
    }
    ui->clientGrpBox->hide ();
    ui->serverGrpBox->hide ();
    ui->inPCbx->hide ();
    ui->label_5->hide ();
}
void ComParaSetting::on_clientRdBttn_toggled(bool checked)
{
    if(checked)
    {
        ui->clientGrpBox->show ();
        ui->inPCbx->show ();
        ui->label_5->show();
    }
    ui->serialGrpBox->hide ();
    ui->serverGrpBox->hide ();

}

void ComParaSetting::on_serverRdbttn_toggled(bool checked)
{
    if(checked)
    {
        ui->serverGrpBox->show ();
          ui->label_5->show();
    }
    ui->clientGrpBox->hide ();
    ui->serialGrpBox->hide ();
    ui->inPCbx->show();
}
void ComParaSetting::SettingWidgetByPara (void)
{
    if(paraSettingsFile.exists ())
    memcpy (pCommPara,paraSettingsFile.readAll (),sizeof(TComParaSettings));

   for(int i = 0 ; i < serialPortInfo. availablePorts ().size();i++)
    {
        ui->serNumCbx->addItem (serialPortInfo.availablePorts ().at (i).portName ());
    }
   for(int i = 0;  0xffffffff != BudaRate[i];i++)
   {
       if(pCommPara-> baudRate == BudaRate[i])
       {
            ui->BuadNumCbx->setCurrentIndex (i);
            break;
       }
   }
   ui->cSBCbx->setCurrentIndex ((pCommPara-> checkBit==0)?(0):(pCommPara-> checkBit-1));
   ui->sBCbx->setCurrentIndex (pCommPara-> stopBit-1);
   ui->dataBitCbx->setCurrentIndex ((int)pCommPara->DataBit-5);
   ui->serverIPLed->setText (pCommPara-> mIP);
   ui->serverURLLed->setText(pCommPara->urlLink);
   ui->orgFrameDTime->setText (QString::number (pCommPara->uLOrFrameDebugTime));
   ui->sPortLed->setText (QString::number (pCommPara-> sPort));
   ui->cSPortLed->setText (QString::number (pCommPara-> mPort));
   ui->inPCbx->setCurrentIndex (pCommPara-> InPType);
   switch(this->pCommPara-> comType)
   {
         case SERIAL:
        ui->serialRdBttn->setChecked (true);
        ui->serialGrpBox->show ();
          break;
   case CLIENT:
       ui->clientRdBttn->setChecked (true);
       ui->clientGrpBox->show ();
       break;
    case SERVER:
       ui->serverRdbttn->setChecked (true);
       ui->serverGrpBox->show ();
       break;
      default:
       break;
   }
}
void ComParaSetting::SettingParaByWidget ()
{
    if(paraSettingsFile.exists ())
    memcpy (pCommPara,paraSettingsFile.readAll (),sizeof(TComParaSettings));
    if(ui->serialRdBttn->isChecked ())
    {
        this->pCommPara-> comType = SERIAL;
    }
    if(ui->clientRdBttn->isChecked ())
    {
        this->pCommPara-> comType = CLIENT;
    }
    if(ui->serverRdbttn->isChecked ())
    {
        this->pCommPara-> comType = SERVER;
    }
    memcpy(pCommPara-> serialName,ui->serNumCbx->currentText ().toLatin1 ().data(),32);
    pCommPara-> baudRate = BudaRate[ui->BuadNumCbx->currentIndex ()];
    pCommPara-> checkBit = (enum CheckType)((ui->cSBCbx->currentIndex () == 0 )?(0):(ui->cSBCbx->currentIndex ()+1));
    pCommPara-> stopBit = ui->sBCbx->currentIndex ()+1;
    pCommPara->DataBit =(enum DataBit) (ui->dataBitCbx->currentIndex ()+5);
    memcpy (pCommPara-> mIP,ui->serverIPLed->text ().toLatin1 ().data (),32);
    memcpy (pCommPara->urlLink,ui->serverURLLed->text ().toLatin1 ().data (),256);
    pCommPara->uLOrFrameDebugTime = ui->orgFrameDTime->text ().toULong ();
    pCommPara-> sPort = ui->sPortLed->text ().toUShort ();
    pCommPara-> mPort = ui->cSPortLed->text().toUShort ();
    pCommPara-> InPType = (enum InProtocolType)ui->inPCbx->currentIndex ();
    if(paraSettingsFile.isOpen ())
    {
        paraSettingsFile.close ();
    }
    paraSettingsFile.open (QIODevice::WriteOnly);
    paraSettingsFile.write ((const char*)pCommPara,sizeof(TComParaSettings));
    paraSettingsFile.close ();
}
void ComParaSetting::on_sureBttn_clicked()
{
    SettingParaByWidget ();
    this->close ();
    ((QWidget*)this->parent ())->close ();
}


void ComParaSetting::on_cancelBttn_clicked()
{
    this->close ();
    ((QWidget*)this->parent ())->close ();
}



void ComParaSetting::on_inPCbx_activated(const QString &arg1)
{
    if(QString("HTTP") == arg1)
    {
        ui->serverURLLed->setEnabled (true);
        ui->orgFrameDTime->setEnabled (true);
        ui->serverIPLed->setDisabled (true);
        ui->cSPortLed->setDisabled (true);
    }
    else
    {
        ui->orgFrameDTime->setEnabled (false);
        ui->serverURLLed->setEnabled (false);
        ui->serverIPLed->setDisabled (false);
        ui->cSPortLed->setDisabled (false);
    }
}
