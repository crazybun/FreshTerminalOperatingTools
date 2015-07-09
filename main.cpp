#include "enterwidget.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath("./plugins");
    EnterWidget w;
    w.show();
    return a.exec();
}
