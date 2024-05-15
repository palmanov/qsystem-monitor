#include "uptimemonitor.h"

#include <QFile>
#include <QDebug>

void UptimeMonitor::process() {
  uint uptime = 0;

  QFile file("/proc/uptime");
  if (!file.open(QFile::ReadOnly))
    return;

  const QList<QByteArray> fields = file.readLine().simplified().split(' ');
  uptime = fields[0].toDouble();

  emit sendUptime(uptime);
}