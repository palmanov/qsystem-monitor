#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "info/abstractsysinfo.h"

#include "cpuwidget.h"
#include "memorywidget.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr,
                      Qt::WindowFlags flags = Qt::WindowFlags());
  ~MainWindow();
  
 private:
  void initUI();
  void initMonitoring();

 private:
  QVector<AbstractSysInfo*> sys_info_v_;

  QTimer* monitoring_timeout_;

  CPUWidget* cpu_widget_;
  MemoryWidget* memory_widget_;
};

#endif // MAINWINDOW_H
