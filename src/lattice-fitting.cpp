#include <QApplication>
#include "LatticeFittingWindow.hpp"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  QGuiApplication::setApplicationDisplayName("Lattice fitting");
  LatticeFittingWindow window;
  window.show();

  return a.exec();
}
