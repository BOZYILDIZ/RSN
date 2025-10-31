#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  // Create and show main window
  MainWindow window;
  window.show();

  return app.exec();
}
