#include <QApplication>
#include <QThread>
#include <QDateTime>
#include <QFontDatabase>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  qRegisterMetaType<QVector<double>>("QVector<double>");

  MainWindow main_window;
  main_window.showMinimized();

  app.exec();
}