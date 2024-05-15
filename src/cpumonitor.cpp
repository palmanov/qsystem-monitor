#include "cpumonitor.h"

#include <QFile>
#include <QDebug>

#include "utils.h"

CPUMonitor::CPUMonitor(QObject* parent)
    : AbstractMonitor(parent) {
  const auto cpu_info = parseProcCpuInfo({"cpu cores", "siblings"});
  cores_ = cpu_info["cpu cores"].toUInt();
  siblings_ = cpu_info["siblings"].toUInt();
}

void CPUMonitor::process() {
  auto cpu_proc_stat = parseCPUProcStat();
  emit sendCPUStats(cpu_proc_stat);
}

