#include "PinWindowUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PinWindowUI w;
    w.show();
    return a.exec();
}
