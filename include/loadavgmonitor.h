#ifndef LOADAVGMONITOR_H
#define LOADAVGMONITOR_H

#include "abstractmonitor.h"

class LoadAvgMonitor : public AbstractMonitor {
  Q_OBJECT
 public:
  explicit LoadAvgMonitor(QObject* parent = nullptr)
      : AbstractMonitor(parent) {}

 signals:
  void sendLoadAvg(const QVector<double>& load_avg);
  void sendThreads(int running_threads, int total_threads);

 private slots:
  void process() override;
};

#endif  // LOADAVGMONITOR_H