#ifndef ABSTRACTMONITOR_H
#define ABSTRACTMONITOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class AbstractMonitor : public QObject {
  Q_OBJECT
 public:
  explicit AbstractMonitor(QObject* parent = nullptr)
      : QObject(parent) {}

   public slots:
    virtual void process() = 0;
};

#endif  // ABSTRACTMONITOR_H