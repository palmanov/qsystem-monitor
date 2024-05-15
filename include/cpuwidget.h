#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <cmath>

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>

#include "utils.h"

class CPUStatsWidget;

class CPUWidget : public QWidget {
  Q_OBJECT
 public:
  explicit CPUWidget(QWidget* parent = nullptr);

 public slots:
  void setCPUStats(const QVector<CPUStats>& stats);

  void setUptime(uint uptime);

  void setLoadAvg(const QVector<double>& load_avg);

 protected:
  void paintEvent(QPaintEvent* e) override;

 private:
  QLabel* cpu_util_label_;
  QLabel* cpu_sys_label_;
  QLabel* cpu_user_label_;
  QLabel* cpu_iowait_label_;
  QLabel* cpu_guest_label_;

  CPUStatsWidget* cpu_stats_widget_;

  QLabel* uptime_label_;
  QLabel* avg_load_label_;
};

#endif // CPUWIDGET_H