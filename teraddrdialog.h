#ifndef TERADDRDIALOG_H
#define TERADDRDIALOG_H

#include <QDialog>
#include "BaseType.h"
#include <QMessageBox>
#include <QFile>
namespace Ui {
class TerAddrDialog;
}
class UpgradeWidget;
class TerAddrDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TerAddrDialog(QWidget *parent = 0);
    ~TerAddrDialog();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
signals:
    void TerAdd(void);
private:
    Ui::TerAddrDialog *ui;
    TerminalConfig *pTConfig;
    TParaSettings *pParaSettings;
    QString AdminCode;
    QString UpAddr;
    QString DwAddr;
    QFile configFile;
};

#endif // TERADDRDIALOG_H
