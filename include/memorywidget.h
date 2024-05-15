#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>

class MemoryWidget : public QWidget {
  Q_OBJECT
public:
  MemoryWidget(QWidget* parent = nullptr);

 public slots:
  void setRAMUsage(uint free, uint total, uint cached);

 protected:
  void paintEvent(QPaintEvent* e) override;

 private:
  QLabel* free_label_;
  QLabel* used_label_;
  QLabel* cached_label_;
};

#endif  // RAMWIDGET_H