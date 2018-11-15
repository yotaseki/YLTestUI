#include "YOLOTestUI.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YOLOTestUI w;
    w.show();

    return a.exec();
}
