#ifndef MEMORYINFO_H
#define MEMORYINFO_H

#include "info/abstractsysinfo.h"

class MemoryInfo : public AbstractSysInfo {
  Q_OBJECT
 public:
   explicit MemoryInfo(QObject* parent = nullptr) : AbstractSysInfo(parent) {}

 signals:
  void sendRamUsage(uint free, uint total, uint cached);

 private slots:
  void process() override;
};

#endif // MEMORYINFO_H