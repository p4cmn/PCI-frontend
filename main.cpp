#include <QApplication>
#include "view\MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QHostAddress host = QHostAddress::LocalHost;
  int port = 1234;

  MainWindow w(host, port);
  w.show();

  return a.exec();
}
