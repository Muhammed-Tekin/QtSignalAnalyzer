#include "SignalInterface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SignalInterface w;
    w.show();
    return QApplication::exec();
}
