#include "enterwidget.h"
#include "ui_enterwidget.h"

EnterWidget::EnterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnterWidget)
{
    ui->setupUi(this);
    //不显示标题栏
    this->setWindowFlags (Qt::FramelessWindowHint);
    theTimer = new QTimer();
    theTimer->start (10);
    theStartTime = 0; 
    theMOWin = new MainOperateWindow();
    connect (this->theTimer,SIGNAL(timeout()),this,SLOT(TimerOutSlot()));
}

EnterWidget::~EnterWidget()
{
    delete ui;
}
void EnterWidget::TimerOutSlot (void)
{
    static qreal Opacity = 0.0;
    static bool even = true;
    if(even)
    {
        Opacity+=0.02;
    }
    else
    {
        Opacity-=0.02;
    }
    if((Opacity >= 1.0) || (Opacity <= 0.00))
    {
        even =!even;
    }
    this->setWindowOpacity (Opacity);
    if(++theStartTime>=100)
    {
        this->close ();
        theMOWin->show ();
        disconnect (this->theTimer,SIGNAL(timeout()),0,0);
    }
}
void EnterWidget::paintEvent (QPaintEvent *e)
{
    e = e;
}
