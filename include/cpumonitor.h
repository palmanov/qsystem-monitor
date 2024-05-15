#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include "abstractmonitor.h"

#include "utils.h"

class CPUMonitor : public AbstractMonitor {
  Q_OBJECT
 public:
  explicit CPUMonitor(QObject* parent = nullptr);

 signals:
  void sendCPUStats(const QVector<CPUStats>& cpu_stats);

 private slots:
  void process() override;

 private:
  int getSiblingsCount();

 private:
  /// Количество ядер
  uint cores_ = 0;
  /// Количество потоков
  uint siblings_ = 0;

  int prev_idle_time_;
  int prev_total_time_;
};

#endif  // CPUMONITOR_H