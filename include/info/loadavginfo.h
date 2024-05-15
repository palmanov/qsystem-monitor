#ifndef LOADAVGINFO_H
#define LOADAVGINFO_H

#include "info/abstractsysinfo.h"

class LoadAvgInfo : public AbstractSysInfo {
  Q_OBJECT
 public:
   explicit LoadAvgInfo(QObject* parent = nullptr) : AbstractSysInfo(parent) {}

 signals:
  void sendLoadAvg(const QVector<double>& load_avg);
  void sendThreads(int running_threads, int total_threads);

 private slots:
  void process() override;
};

#endif // LOADAVGINFO_H