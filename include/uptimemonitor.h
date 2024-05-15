#ifndef UPTIMEMONITOR_H
#define UPTIMEMONITOR_H

#include "abstractmonitor.h"

class UptimeMonitor : public AbstractMonitor {
  Q_OBJECT
 public:
  explicit UptimeMonitor(QObject* parent = nullptr)
      : AbstractMonitor(parent) {}

 signals:
  void sendUptime(uint uptime);

 private slots:
  void process() override;
};

#endif // UPTIMEMONITOR_H