#include "mainwindow.h"

#include <QGridLayout>
#include <QLabel>

#include "cpumonitor.h"
#include "loadavgmonitor.h"
#include "rammonitor.h"
#include "uptimemonitor.h"



MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      threads_label_(new QLabel),
      avg_load_label_(new QLabel),
      uptime_label_(new QLabel) {
  auto cpu_info = parseProcCpuInfo({"siblings"});
  cpu_stats_widget_ = new CPUWidget(this);
  ram_stat_widget_ = new RAMStatWidget(this);

  InitUI();
  InitMonitoring();
}

MainWindow::~MainWindow() {}

void MainWindow::InitUI() {
  this->setFixedSize(this->sizeHint());

  auto main_widget = new QWidget;
  auto main_layout = new QVBoxLayout;

  main_layout->addWidget(cpu_stats_widget_);
  main_layout->addWidget(ram_stat_widget_);
  main_layout->addStretch(1);

  main_widget->setLayout(main_layout);

  this->setCentralWidget(main_widget);

  auto palette = this->palette();
  palette.setColor(QPalette::ColorRole::Window, QColor(30, 30, 30));
  palette.setColor(QPalette::ColorRole::AlternateBase, QColor(50, 50, 50));
  palette.setColor(QPalette::ColorRole::WindowText, QColor(225, 225, 225));

  this->setPalette(palette);
}

void MainWindow::InitMonitoring() {
  monitoring_timeout_ = new QTimer(this);
  monitoring_timeout_->setInterval(1000);

  auto cpu_monitor_ptr = new CPUMonitor;
  monitors_.push_back(cpu_monitor_ptr);

  auto load_avg_monitor_ptr = new LoadAvgMonitor;
  monitors_.push_back(load_avg_monitor_ptr);

  auto ram_monitor_ptr = new RAMMonitor;
  monitors_.push_back(ram_monitor_ptr);

  auto uptime_monitor_ptr = new UptimeMonitor;
  monitors_.push_back(uptime_monitor_ptr);

  for (auto monitor : monitors_) {
    QObject::connect(monitoring_timeout_, &QTimer::timeout,
                     monitor, &AbstractMonitor::process);
  }

  QObject::connect(cpu_monitor_ptr, &CPUMonitor::sendCPUStats, 
                   cpu_stats_widget_, &CPUWidget::setCPUStats);

  QObject::connect(ram_monitor_ptr, &RAMMonitor::sendRamUsage,
                   ram_stat_widget_, &RAMStatWidget::setRAMUsage);

  QObject::connect(load_avg_monitor_ptr, &LoadAvgMonitor::sendThreads, this,
                   [=](int running_threads, int total_threads) {
                     threads_label_->setText(
                         QString("Threads: %1 running, %2 total")
                             .arg(running_threads)
                             .arg(total_threads));
                   });

  QObject::connect(load_avg_monitor_ptr, &LoadAvgMonitor::sendLoadAvg, this,
                   [=](const QVector<double> &load_avg) {
                     avg_load_label_->setText(QString("Load average: %1 %2 %3")
                                                  .arg(load_avg[0])
                                                  .arg(load_avg[1])
                                                  .arg(load_avg[2]));
                   });

  QObject::connect(load_avg_monitor_ptr, &LoadAvgMonitor::sendLoadAvg,
                   cpu_stats_widget_, &CPUWidget::setLoadAvg);

  QObject::connect(
      uptime_monitor_ptr, &UptimeMonitor::sendUptime, cpu_stats_widget_, &CPUWidget::setUptime);

  QObject::connect(
      uptime_monitor_ptr, &UptimeMonitor::sendUptime, this, [=](uint uptime) {
        uint remaining_seconds = uptime;
        int days = uptime / 86400;
        remaining_seconds = uptime % 86400;
        int hours = remaining_seconds / 3600;
        remaining_seconds %= 3600;
        int minutes = remaining_seconds / 60;
        remaining_seconds %= 60;

        QString uptime_str = QString("Uptime: %1 days, %2:%3:%4")
                                 .arg(days)
                                 .arg(hours, 2, 10, QChar('0'))
                                 .arg(minutes, 2, 10, QChar('0'))
                                 .arg(remaining_seconds, 2, 10, QChar('0'));
        uptime_label_->setText(uptime_str);
      });

  monitoring_timeout_->start();
}