#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QThread>
#include <QResizeEvent>

#include "abstractmonitor.h"

#include "cpuwidget.h"
#include "ramwidget.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr,
                      Qt::WindowFlags flags = Qt::WindowFlags());
  ~MainWindow();
  
 private:
  void InitUI();
  void InitMonitoring();

 private:
  QVector<AbstractMonitor*> monitors_;

  QLabel* threads_label_;
  QLabel* avg_load_label_;
  QLabel* uptime_label_;

  QTimer* monitoring_timeout_;

  CPUWidget* cpu_stats_widget_;
  RAMStatWidget* ram_stat_widget_;
};

#endif // MAINWINDOW_H
