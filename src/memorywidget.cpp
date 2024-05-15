#include "memorywidget.h"

MemoryWidget::MemoryWidget(QWidget* parent) : QWidget(parent) {
  QFont font = this->font();
  font.setPointSize(10);
  font.setWeight(QFont::Weight::DemiBold);

  QHBoxLayout* main_layout = new QHBoxLayout;
  {
    QVBoxLayout* l = new QVBoxLayout;
    free_label_ = new QLabel;
    free_label_->setFont(font);

    l->addWidget(new QLabel("FREE"));
    l->addWidget(free_label_);
    main_layout->addLayout(l);
  }

  {
    QVBoxLayout* l = new QVBoxLayout;
    used_label_ = new QLabel;
    used_label_->setFont(font);

    l->addWidget(new QLabel("USED"));
    l->addWidget(used_label_);
    main_layout->addLayout(l);
  }

  {
    QVBoxLayout* l = new QVBoxLayout;
    cached_label_ = new QLabel;
    cached_label_->setFont(font);

    l->addWidget(new QLabel("CACHED"));
    l->addWidget(cached_label_);
    main_layout->addLayout(l);
  }

  this->setLayout(main_layout);
}

void MemoryWidget::setRAMUsage(uint free, uint total, uint cached) {
  free_label_->setText(
      QString("%1 G").arg(QString::number(free >> 20, 'f', 1)));
  used_label_->setText(QString("%1 G").arg(
      QString::number((total - free - cached) >> 20, 'f', 1)));
  cached_label_->setText(
      QString("%1 G").arg(QString::number(cached >> 20, 'f', 1)));
}

void MemoryWidget::paintEvent(QPaintEvent* e) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(50, 50, 50));

  painter.drawRoundedRect(this->rect(), 4, 4);

  QWidget::paintEvent(e);
}