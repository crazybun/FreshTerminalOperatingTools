#ifndef COMPARASETTING_H
#define COMPARASETTING_H

#include <QWidget>
#include <BaseType.h>
#include <QFile>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
namespace Ui {
class ComParaSetting;
}

class ComParaSetting : public QWidget
{
    Q_OBJECT
    
public:
    explicit ComParaSetting(QWidget *parent = 0);
    ~ComParaSetting();
    
private slots:
    void on_serialRdBttn_toggled(bool checked);

    void on_clientRdBttn_toggled(bool checked);

    void on_serverRdbttn_toggled(bool checked);

    void on_sureBttn_clicked();

    void on_cancelBttn_clicked();


    void on_inPCbx_activated(const QString &arg1);

private:
    Ui::ComParaSetting *ui;
     TComParaSettings *pCommPara;
     QFile   paraSettingsFile;
    QSerialPortInfo serialPortInfo;
     void SettingWidgetByPara(void);
     void SettingParaByWidget(void);
};

#endif // COMPARASETTING_H
