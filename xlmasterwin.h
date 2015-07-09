#ifndef XLMASTERWIN_H
#define XLMASTERWIN_H

#include <QWidget>
#include"BaseType.h"
namespace Ui {
class XLMasterWin;
}

class XLMasterWin : public QWidget
{
    Q_OBJECT
    
public:
    explicit XLMasterWin(TXLMasterPara *para,  QWidget *parent = 0);
    inline void QsSwitchToChar(QString s,char *pCh)
    {
        QByteArray ba;
        ba = s.toLatin1();
        strcpy(pCh,ba.data());
    }
    ~XLMasterWin();

private slots:
    void on_sureBttn_clicked();
    void on_cancelBttn_clicked();
signals:
    void XLSettingFinished();
private:
    Ui::XLMasterWin *ui;
    TXLMasterPara masterPara;
    TXLMasterPara *pPara;
};

#endif // XLMASTERWIN_H
