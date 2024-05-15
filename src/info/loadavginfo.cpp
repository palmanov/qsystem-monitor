#include "info/loadavginfo.h"

#include <QFile>

void LoadAvgInfo::process() {
  QFile file("/proc/loadavg");
  file.open(QFile::ReadOnly);

  QVector<double> load_avg(3);
  const QList<QByteArray> fields = file.readLine().simplified().split(' ');
  for (size_t i = 0; i < load_avg.size(); i++)
    load_avg[i] = fields[i].toDouble();

  emit sendLoadAvg(load_avg);

  const auto threads_str = fields[3].split('/');
  const int running_threads = threads_str[0].toInt();
  const int total_threads = threads_str[1].toInt();

  emit sendThreads(running_threads, total_threads);
}