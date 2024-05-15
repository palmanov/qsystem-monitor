#include "info/cpuinfo.h"

#include "utils.h"

CPUInfo::CPUInfo(QObject* parent) : AbstractSysInfo(parent) {}

void CPUInfo::process() {
  auto cpu_proc_stat = parseCPUProcStat();
  emit sendCPUStats(cpu_proc_stat);
}
