#ifndef CPUINFO_H
#define CPUINFO_H

#include "info/abstractsysinfo.h"
#include "utils.h"

class CPUInfo : public AbstractSysInfo {
  Q_OBJECT
 public:
   explicit CPUInfo(QObject* parent = nullptr);

 signals:
  void sendCPUStats(const QVector<CPUStats>& cpu_stats);

 private slots:
  void process() override;
};

#endif // CPUINFO_H