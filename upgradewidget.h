#ifndef UPGRADEWIDGET_H
#define UPGRADEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QScrollArea>
#include "BaseType.h"
#include "commprocess.h"
#include "teraddrdialog.h"
#include "excelproc.h"
namespace Ui {
class UpgradeWidget;
}

class UpgradeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UpgradeWidget(QWidget *parent = 0);
    ~UpgradeWidget();
    void initTableWidget(void);
 public slots:
     void AddTerAddr2Table(void);
     void UiFresh(TUpgradeUIStruct UIs);
signals:
    void SettingProcSignal(bool isEnabled );
    void TableAdd(void);
private slots:
    void on_uStartBttn_clicked();

    void on_uPauseBttn_clicked();

    void on_terAddrABttn_clicked();

    void on_terAddrClrBttn_clicked();

private:
    Ui::UpgradeWidget *ui;
    QScrollArea    *theScrollArea;
    QTableWidget *theTableWiget;
    QStringList       theHeaders;
    qint8               theTableColumnCount;
    TerminalConfig *pTConfig;

    CommProcess *pComPro;
    void reSizeTableColumn(void);
    void paintEvent (QPaintEvent * e);
};

#endif // UPGRADEWIDGET_H
