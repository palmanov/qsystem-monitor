#ifndef ABSTRACTSYSINFO_H
#define ABSTRACTSYSINFO_H

#include <QObject>

class AbstractSysInfo : public QObject {
  Q_OBJECT
 public:
   explicit AbstractSysInfo(QObject* parent = nullptr) : QObject(parent) {}

 public slots:
   virtual void process() = 0;
};

#endif // ABSTRACTSYSINFO_H