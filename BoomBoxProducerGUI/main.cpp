#include "boomboxproducerwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BoomBoxProducerWidget w;
    w.show();

    return a.exec();
}
