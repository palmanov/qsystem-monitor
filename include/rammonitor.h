#ifndef RAMMONITOR_H
#define RAMMONITOR_H

#include "abstractmonitor.h"

class RAMMonitor : public AbstractMonitor {
  Q_OBJECT
 public:
  explicit RAMMonitor(QObject* parent = nullptr)
      : AbstractMonitor(parent) {}

 signals:
  void sendRamUsage(uint free, uint total, uint cached);

 private slots:
  void process() override;
};

#endif  // RAMMONITOR_H