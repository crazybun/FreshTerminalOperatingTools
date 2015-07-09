#include "proprasewidget.h"
#include "ui_proprasewidget.h"

ProPraseWidget::ProPraseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProPraseWidget)
{
    ui->setupUi(this);
}
void  ProPraseWidget::paintEvent (QPaintEvent *e)
{
    e = e;
    ui->groupBox->setGeometry (this->size ().width ()-10-ui->groupBox->width (),
                                                    this->size ().height ()-10-ui->groupBox->height (),
                                                    ui->groupBox->width (),
                                                    ui->groupBox->height ());
}
ProPraseWidget::~ProPraseWidget()
{
    delete ui;
}
