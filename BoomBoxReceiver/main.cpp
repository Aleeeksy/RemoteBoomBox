#include <QApplication>
#include "boomboxreceiver.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  BoomBoxReceiver w;
  w.show();

  return a.exec();
}
