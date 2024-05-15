#include "info/memoryinfo.h"

#include <QFile>
#include <QTextStream>

void MemoryInfo::process() {
  uint64_t mem_total = 0;
  uint64_t mem_free = 0;
  uint64_t mem_cached = 0;

  QFile file("/proc/meminfo");
  if (!file.open(QFile::ReadOnly))
    return;

  auto contents = file.readAll();
  QTextStream in(&contents);

  while (!in.atEnd()) {
    const auto str = in.readLine();
    const auto fields = str.simplified().split(' ');

    if (fields[0] == "MemTotal:") mem_total = fields[1].toUInt();
    if (fields[0] == "MemFree:") mem_free = fields[1].toUInt();
    if (fields[0] == "Cached:") mem_cached += fields[1].toUInt();
    if (fields[0] == "Buffers:") mem_cached += fields[1].toUInt();
  }

  emit sendRamUsage(mem_free, mem_total, mem_cached);
}