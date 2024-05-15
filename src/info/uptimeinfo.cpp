#include "info/uptimeinfo.h"

#include <QFile>

void UptimeInfo::process() {
  uint uptime = 0;

  QFile file("/proc/uptime");
  if (!file.open(QFile::ReadOnly))
    return;

  const QList<QByteArray> fields = file.readLine().simplified().split(' ');
  uptime = fields[0].toDouble();

  emit sendUptime(uptime);
}