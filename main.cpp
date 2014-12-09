#include "enterwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EnterWidget w;
    w.show();
    
    return a.exec();
}
