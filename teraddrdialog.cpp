#include "teraddrdialog.h"
#include "ui_teraddrdialog.h"
#include <QDebug>
TerAddrDialog::TerAddrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerAddrDialog)
{
    ui->setupUi(this);
    ui->decRdbox->setChecked (true);
    if(!GetParaSettings(&pParaSettings))
    {
        CommonPlaceMessageBoxWork ("错误", "终端未配置!");
        return;
    }
    pTConfig = new TerminalConfig[MAXCONFIG];
    memset (pTConfig,0,sizeof(TerminalConfig)*MAXCONFIG );
    GetTerConfig(pTConfig);
    if(pParaSettings->terPara.terAddrType == CSGADDR)
        ui->admCodeLed->setInputMask ("000000");
    else
        ui->admCodeLed->setInputMask ("0000");
    if(pParaSettings->terPara.terAddrType == FADDR)
    {
        ui->upAddrLed->setInputMask ("HHHHHHHHHH");
        ui->dwAddrLed->setInputMask ("HHHHHHHHHH");
    }
    else if(pParaSettings->terPara.terAddrType == CSGADDR)
    {
        ui->upAddrLed->setInputMask ("HHHHHHHH");
        ui->dwAddrLed->setInputMask ("HHHHHHHH");
    }
    else
    {
        ui->upAddrLed->setInputMask ("HHHHH");
        ui->dwAddrLed->setInputMask ("HHHHH");
    }

    connect (this,SIGNAL(TerAdd()),this->parent (),SLOT(AddTerAddr2Table ()));
}

TerAddrDialog::~TerAddrDialog()
{
    delete pTConfig;
    delete ui;
}

void TerAddrDialog::on_buttonBox_accepted()
{
    int count;
    int max = 0;
    bool yes;
    enum AddrType aType;
    while(pTConfig[max].IsUsed)
    {
        max++;
    }
    AdminCode = ui->admCodeLed->text ();
    UpAddr = ui->upAddrLed->text ();
    DwAddr = ui->dwAddrLed->text();
    if(ui->hexRdBox->isChecked ())
    {
        aType = HEX_ADDR;
        count =  (int)(DwAddr.toULong (&yes,16) - UpAddr.toULong (&yes,16));

    }
    else
    {
        aType = DEC_ADDR;
        count =  (int)(DwAddr.toULong (&yes,10) - UpAddr.toULong (&yes,10));
    }
    if(count < 0)
    {
        CommonPlaceMessageBoxWork ("错误","非法地址");
        return;
    }
    for(int i = 0 ; (i+max < MAXCONFIG)&&(i<=count);i++)
    {
        int a = ((aType == HEX_ADDR)?(16):(10));
        unsigned long x =UpAddr.toULong (&yes,a);
        x+=i;
        QString ss= AdminCode+QString("%1").arg (x,pParaSettings->terPara.terAddrType == FADDR?8:4,16,QChar('0'));
        memcpy(pTConfig[i+max].ARCandTAddr,
                ss.toLatin1 ().data (),
                ss.length ());
        pTConfig[i+max].IsUsed= true;
        pTConfig[i+max].AddrType = HEX_ADDR;

    }
    this->configFile.setFileName (CONFIGNAME);
    this->configFile.open (QIODevice::ReadWrite);
    this->configFile.seek (0)  ;
    this->configFile.write ((char*)this->pTConfig,sizeof(TerminalConfig)*MAXCONFIG);
    this->configFile.close ();
    emit TerAdd ();
}

void TerAddrDialog::on_buttonBox_rejected()
{

}
