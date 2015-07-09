#ifndef ENTERWIDGET_H
#define ENTERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPalette>
#include <QPainter>
#include <QPixmap>
#include <QStyleOption>
#include "BaseType.h"
#include "mainoperatewindow.h"
namespace Ui {
class EnterWidget;
}

class EnterWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit EnterWidget(QWidget *parent = 0);
    ~EnterWidget();
public slots:
    void TimerOutSlot(void);
private:
    Ui::EnterWidget *ui;
    QTimer * theTimer;
    DWORD theStartTime;
    MainOperateWindow *theMOWin;
    void paintEvent (QPaintEvent *e);
};

#endif // ENTERWIDGET_H
