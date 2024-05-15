#include "mainwindow.h"

#include <QVBoxLayout>
#include <QTimer>

#include "info/cpuinfo.h"
#include "info/loadavginfo.h"
#include "info/memoryinfo.h"
#include "info/uptimeinfo.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  initUI();
  initMonitoring();
}

MainWindow::~MainWindow() {}

void MainWindow::initUI() {
  this->setFixedSize(this->sizeHint());

  auto main_widget = new QWidget;
  auto main_layout = new QVBoxLayout;

  cpu_widget_ = new CPUWidget(this);
  memory_widget_ = new MemoryWidget(this);

  main_layout->addWidget(cpu_widget_);
  main_layout->addWidget(memory_widget_);
  main_layout->addStretch(1);

  main_widget->setLayout(main_layout);

  this->setCentralWidget(main_widget);

  auto palette = this->palette();
  palette.setColor(QPalette::ColorRole::Window, QColor(30, 30, 30));
  palette.setColor(QPalette::ColorRole::AlternateBase, QColor(50, 50, 50));
  palette.setColor(QPalette::ColorRole::WindowText, QColor(225, 225, 225));

  this->setPalette(palette);
}

void MainWindow::initMonitoring() {
  monitoring_timeout_ = new QTimer(this);
  monitoring_timeout_->setInterval(1000);

  auto cpu_info_ptr = new CPUInfo;
  sys_info_v_.push_back(cpu_info_ptr);

  auto load_avg_info_ptr = new LoadAvgInfo;
  sys_info_v_.push_back(load_avg_info_ptr);

  auto memory_info_ptr = new MemoryInfo;
  sys_info_v_.push_back(memory_info_ptr);

  auto uptime_info_ptr = new UptimeInfo;
  sys_info_v_.push_back(uptime_info_ptr);

  for (auto monitor : sys_info_v_) {
    QObject::connect(monitoring_timeout_, &QTimer::timeout,
                     monitor, &AbstractSysInfo::process);
  }

  QObject::connect(cpu_info_ptr, &CPUInfo::sendCPUStats, 
                   cpu_widget_, &CPUWidget::setCPUStats);

  QObject::connect(memory_info_ptr, &MemoryInfo::sendRamUsage, 
                   memory_widget_, &MemoryWidget::setRAMUsage);

  QObject::connect(load_avg_info_ptr, &LoadAvgInfo::sendLoadAvg,
                   cpu_widget_, &CPUWidget::setLoadAvg);

  QObject::connect(uptime_info_ptr, &UptimeInfo::sendUptime,
                   cpu_widget_, &CPUWidget::setUptime);

  monitoring_timeout_->start();
}