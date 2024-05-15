#ifndef UPTIMEINFO_H
#define UPTIMEINFO_H

#include "info/abstractsysinfo.h"

class UptimeInfo : public AbstractSysInfo {
  Q_OBJECT
 public:
   explicit UptimeInfo(QObject *parent = nullptr) : AbstractSysInfo(parent) {}

 signals:
  void sendUptime(uint uptime);

 private slots:
  void process() override;
};

#endif // UPTIMEINFO_H