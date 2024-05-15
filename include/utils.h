#ifndef UTILS_H
#define UTILS_H

#include <QMap>
#include <QSet>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

struct CPUStats {
  uint32_t user = 0;
  uint32_t nice = 0;
  uint32_t system = 0;
  uint32_t idle = 0;
  uint32_t iowait = 0;
  uint32_t irq = 0;
  uint32_t softirq = 0;
  uint32_t steal = 0;
  uint32_t guest = 0;
  uint32_t guest_nice = 0;
};

static QMap<QString, QString> parseProcCpuInfo(const QSet<QString>& keys) {
  if (keys.isEmpty())
    return {};

  QFile f("/proc/cpuinfo");
  if (!f.open(QIODevice::ReadOnly))
    return {};

  QMap<QString, QString> result;

  auto contents = f.readAll();
  QTextStream in(&contents);
  while (!in.atEnd()) {
    const auto fields = QString(in.readLine()).split('\t');
    for (const auto& key : keys) {
      if (!result.contains(key) && fields[0].contains(key)) {
        result.insert(fields[0], fields[1].right(fields[1].length() - 2));
      }
    }
  }

  return result;
}

static QVector<CPUStats> parseCPUProcStat() {
  QFile f("/proc/stat");
  if (!f.open(QIODevice::ReadOnly))
    return {};

  QVector<CPUStats> cpu_stats;

  auto contents = f.readAll();
  QTextStream in(&contents);
  while (!in.atEnd()) {
    const auto fields = in.readLine().simplified().split(' ');
    if (fields[0].contains("cpu")) {
      CPUStats info;
      info.user       = fields[1].toULong();
      info.nice       = fields[2].toULong();
      info.system     = fields[3].toULong();
      info.idle       = fields[4].toULong();
      info.iowait     = fields[5].toULong();
      info.irq        = fields[6].toULong();
      info.softirq    = fields[7].toULong();
      info.steal      = fields[8].toULong();
      info.guest      = fields[9].toULong();
      info.guest_nice = fields[10].toULong();

      cpu_stats.append(info);
    }
  }

  return cpu_stats;
}

#endif // UTILS_H