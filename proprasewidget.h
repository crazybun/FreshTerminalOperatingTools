#ifndef PROPRASEWIDGET_H
#define PROPRASEWIDGET_H

#include <QWidget>

namespace Ui {
class ProPraseWidget;
}

class ProPraseWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProPraseWidget(QWidget *parent = 0);
    ~ProPraseWidget();
    
private:
    Ui::ProPraseWidget *ui;
    void paintEvent (QPaintEvent *e);
};

#endif // PROPRASEWIDGET_H
