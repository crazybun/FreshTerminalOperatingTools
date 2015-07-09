#ifndef MAINOPERATEWINDOW_H
#define MAINOPERATEWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QMdiArea>
#include <QLabel>
#include <QMdiSubWindow>
#include "comparasetting.h"
#include "terparasetting.h"
#include "upgradewidget.h"
#include "proprasewidget.h"

namespace Ui {
class MainOperateWindow;
}

class MainOperateWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainOperateWindow(QWidget *parent = 0);
    ~MainOperateWindow();
public slots:
    void SettingPermitOrProhibited(bool isEnabled);
private slots:
    void on_actionComPara_triggered();

    void on_actionTerPara_triggered();

    void on_actionUpgrade_triggered();

    void on_actionProParse_triggered();

private:
    Ui::MainOperateWindow *ui;
    QMdiArea           *theMdiArea;
    QLabel                *theImagLabel;
    ComParaSetting  *theComParaS;
    TerParaSetting    *theTerParaS;
    UpgradeWidget     *theUpgradeWidget;
    ProPraseWidget *theProPraseWidget;
    void SubWindowAdd(QWidget *w);
    bool SettingWidgetProcBefOperate(void);
    void paintEvent (QPaintEvent *);
};

#endif // MAINOPERATEWINDOW_H
