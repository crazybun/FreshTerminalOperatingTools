#ifndef TERPARASETTING_H
#define TERPARASETTING_H

#include <QWidget>
#include <QFile>
#include <QPalette>
#include "BaseType.h"
#include "xlmasterwin.h"
namespace Ui {
class TerParaSetting;
}

class TerParaSetting : public QWidget
{
    Q_OBJECT
    
public:
    explicit TerParaSetting(QWidget *parent = 0);
    ~TerParaSetting();
    
private slots:
    void on_asUpFileBttn_clicked();

    void on_sureBttn_clicked();

    void on_cancelBttn_clicked();

    void on_masterPCbx_activated(const QString &arg1);

    void on_terTypeCbx_activated(const QString &arg1);

    void on_overTimeChBx_clicked(bool checked);

private:
    Ui::TerParaSetting *ui;
   TTerParaSettings *pTerminalPara;
    QFile   paraSettingsFile;
    QString theFileName;
    QTextCodec *codec;
    void SettingWidgetByPara(void);
    void SettingParaByWidget(void);
};

#endif // TERPARASETTING_H
