#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <cmath>

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>

#include "cpumonitor.h"
#include "utils.h"

constexpr int kItemsCount = 45;
constexpr int kItemsSpacing = 3;
constexpr int kItemW = 5;
constexpr int kItemH = 10;

constexpr QColor kSysColor(235, 85, 69);
constexpr QColor kUserColor(107, 212, 95);
constexpr QColor kIOWaitColor(248, 216, 74);
constexpr QColor kGuestColor(178, 96, 234);
constexpr QColor kIdleColor(70, 70, 70);

struct CPUStatsPercent {
  float sys = 0.f;
  float user = 0.f;
  float iowait = 0.f;
  float steal = 0.f;
  float idle = 0.f;
  float guest = 0.f;
};

class CPUStatsWidget : public QWidget {
  Q_OBJECT
 public:
  explicit CPUStatsWidget(QWidget* parent = nullptr) 
      : QWidget(parent) {
    auto cpu_info = parseProcCpuInfo({"siblings"});
    const auto siblings = cpu_info["siblings"].toInt();
    this->setMinimumSize(kItemsCount * kItemW + (kItemsCount - 1) * kItemsSpacing, (siblings - 1) * kItemsSpacing + kItemH * siblings);
  }

 public slots:
  void setCPUStats(const QVector<CPUStats>& stats) { 
    static auto prev_cpu_stats = stats;
    static auto prev_total_time_arr = QVector<int>(stats.size(), 0);
    cpu_stats_percent_.resize(stats.size());

    for (size_t i = 1; i < stats.size(); i++) {
      const auto& stat = stats[i];
      const auto& prev_stat = prev_cpu_stats[i];
      auto& prev_total_time = prev_total_time_arr[i];

      auto& stat_perc = cpu_stats_percent_[i];

      const auto total = stat.user + stat.nice + stat.system 
        + stat.idle + stat.iowait + stat.irq + stat.softirq 
        + stat.steal + stat.guest + stat.guest_nice;

      stat_perc.sys = 100.f * (stat.system - prev_stat.system) / (total - prev_total_time);
      stat_perc.user = 100.f * (stat.user - prev_stat.user) / (total - prev_total_time);
      stat_perc.iowait = 100.f * (stat.iowait - prev_stat.iowait) / (total - prev_total_time);
      stat_perc.steal = 100.f * (stat.steal - prev_stat.steal) / (total - prev_total_time);
      stat_perc.guest = 100.f * (stat.guest - prev_stat.guest) / (total - prev_total_time);
      stat_perc.idle = 100.f * (stat.idle - prev_stat.idle) / (total - prev_total_time);

      prev_total_time = total;
    }

    prev_cpu_stats = stats;

    update();
  }

 protected:
  void paintEvent(QPaintEvent* e) override {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);

    int item_h = kItemH;
    int item_w = kItemW;

    for (size_t i = 1; i < cpu_stats_percent_.size(); i++) {
      const double item_percent = 100.0 / kItemsCount;

      const int system_items_count = std::round(cpu_stats_percent_[i].sys / item_percent);
      const int user_items_count = std::round(cpu_stats_percent_[i].user / item_percent);
      const int iowait_items_count = std::round(cpu_stats_percent_[i].iowait / item_percent);
      const int guest_items_count = std::round(cpu_stats_percent_[i].guest / item_percent);
      const int idle_items_count = kItemsCount - (system_items_count + user_items_count + iowait_items_count + guest_items_count);

      int x_offset = 0;
      for (int k = 0; k < system_items_count; k++) {
        painter.setBrush(kSysColor);
        painter.drawRoundedRect(x_offset, (i - 1) * kItemsSpacing + kItemH * (i - 1), kItemW, kItemH, 3, 3);
        x_offset += kItemsSpacing + kItemW;
      }

      for (int k = 0; k < user_items_count; k++) {
        painter.setBrush(kUserColor);
        painter.drawRoundedRect(x_offset, (i - 1) * kItemsSpacing + kItemH * (i - 1), kItemW, kItemH, 3, 3);
        x_offset += kItemsSpacing + kItemW;
      }

      for (int k = 0; k < iowait_items_count; k++) {
        painter.setBrush(kIOWaitColor);
        painter.drawRoundedRect(x_offset, (i - 1) * kItemsSpacing + kItemH * (i - 1), kItemW, kItemH, 3, 3);
        x_offset += kItemsSpacing + kItemW;
      }

      for (int k = 0; k < guest_items_count; k++) {
        painter.setBrush(kGuestColor);
        painter.drawRoundedRect(x_offset, (i - 1) * kItemsSpacing + kItemH * (i - 1), kItemW, kItemH, 3, 3);
        x_offset += kItemsSpacing + kItemW;
      }

      for (int k = 0; k < idle_items_count; k++) {
        painter.setBrush(kIdleColor);
        painter.drawRoundedRect(x_offset, (i - 1) * kItemsSpacing + kItemH * (i - 1), kItemW, kItemH, 3, 3);
        x_offset += kItemsSpacing + kItemW;
      }
    }
  }

 private:
  QVector<CPUStatsPercent> cpu_stats_percent_;
};

enum class CPUState {
  kSys = 0,
  kUser,
  kIOWait,
  kGuest
};

class CPUStateLabel : public QLabel {
 public:
  explicit CPUStateLabel(CPUState state, const QString& str, QWidget* parent = nullptr)
      : QLabel(str, parent),
        state_(state) {
  }

 protected:
  void paintEvent(QPaintEvent* e) override {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor brush_color = kIdleColor;
    switch (state_) {
      case CPUState::kSys: brush_color = kSysColor; break;
      case CPUState::kIOWait: brush_color = kIOWaitColor; break;
      case CPUState::kGuest: brush_color = kGuestColor; break;
      case CPUState::kUser: brush_color = kUserColor; break;
      default: break;
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush({brush_color});
    painter.drawRoundedRect(0, 2, kItemW, kItemH, 3, 3);

    painter.setPen(QPen({Qt::white}, 2.0));
    const QRect rectangle = QRect(kItemW + 4, 0, this->width() - kItemW + 4, this->height());
    painter.drawText(rectangle, Qt::AlignLeft | Qt::AlignTop, this->text());

    QWidget::paintEvent(e);
  }

 private:
  CPUState state_;
};

class CPUWidget : public QWidget {
  Q_OBJECT
 public:
  explicit CPUWidget(QWidget* parent = nullptr) 
      : QWidget(parent) {
    cpu_stats_widget_ = new CPUStatsWidget;
    cpu_util_label_ = new QLabel;
    cpu_util_label_->setMinimumWidth(96);

    QFont font = cpu_util_label_->font();
    font.setPointSize(20);
    font.setWeight(QFont::Weight::DemiBold);
    cpu_util_label_->setFont(font);

    QHBoxLayout* first_layout = new QHBoxLayout;
    first_layout->addWidget(cpu_util_label_);

    font.setPointSize(10);
    { 
      QVBoxLayout* l = new QVBoxLayout;
      cpu_sys_label_ = new QLabel("0%");
      cpu_sys_label_->setFont(font);

      l->addWidget(new CPUStateLabel(CPUState::kSys, "SYS"));
      l->addWidget(cpu_sys_label_);
      first_layout->addLayout(l);
    }

    { 
      QVBoxLayout* l = new QVBoxLayout;
      cpu_user_label_ = new QLabel("0%");
      cpu_user_label_->setFont(font);

      l->addWidget(new CPUStateLabel(CPUState::kUser, "USER"));
      l->addWidget(cpu_user_label_);
      first_layout->addLayout(l);
    }

    { 
      QVBoxLayout* l = new QVBoxLayout;
      cpu_iowait_label_ = new QLabel("0%");
      cpu_iowait_label_->setFont(font);

      l->addWidget(new CPUStateLabel(CPUState::kIOWait, "IOWAIT"));
      l->addWidget(cpu_iowait_label_);
      first_layout->addLayout(l);
    }

    { 
      QVBoxLayout* l = new QVBoxLayout;
      cpu_guest_label_ = new QLabel("0%");
      cpu_guest_label_->setFont(font);

      l->addWidget(new CPUStateLabel(CPUState::kGuest, "GUEST"));
      l->addWidget(cpu_guest_label_);
      first_layout->addLayout(l);
    }

    QHBoxLayout* third_layout = new QHBoxLayout;

    {
      QVBoxLayout* l = new QVBoxLayout;
      uptime_label_ = new QLabel();
      uptime_label_->setFont(font);

      l->addWidget(new QLabel("UPTIME"));
      l->addWidget(uptime_label_);
      third_layout->addLayout(l);
    }

    {
      QVBoxLayout* l = new QVBoxLayout;
      avg_load_label_ = new QLabel();
      avg_load_label_->setFont(font);

      l->addWidget(new QLabel("LOAD"));
      l->addWidget(avg_load_label_);
      third_layout->addLayout(l);
    }

    QVBoxLayout* l = new QVBoxLayout;
    l->addLayout(first_layout);
    l->addWidget(cpu_stats_widget_, 1);
    l->addLayout(third_layout);
    this->setLayout(l);
  }

 public slots:
  void setCPUStats(const QVector<CPUStats>& stats) {
    cpu_stats_widget_->setCPUStats(stats);

    static auto prev_cpu_stats = stats[0];
    static auto prev_total_time = 0;

    const auto& stat = stats[0];

    CPUStatsPercent stat_perc;

    const auto total = stat.user + stat.nice + stat.system 
      + stat.idle + stat.iowait + stat.irq + stat.softirq 
      + stat.steal + stat.guest + stat.guest_nice;

    stat_perc.sys = 100.f * (stat.system - prev_cpu_stats.system) / (total - prev_total_time);
    stat_perc.user = 100.f * (stat.user - prev_cpu_stats.user) / (total - prev_total_time);
    stat_perc.iowait = 100.f * (stat.iowait - prev_cpu_stats.iowait) / (total - prev_total_time);
    stat_perc.steal = 100.f * (stat.steal - prev_cpu_stats.steal) / (total - prev_total_time);
    stat_perc.idle = 100.f * (stat.idle - prev_cpu_stats.idle) / (total - prev_total_time);

    cpu_util_label_->setText(QString("%1%").arg(static_cast<int>(100.0 - stat_perc.idle)));
    cpu_sys_label_->setText(QString("%1%").arg(static_cast<int>(stat_perc.sys)));
    cpu_user_label_->setText(QString("%1%").arg(static_cast<int>(stat_perc.user)));
    cpu_iowait_label_->setText(QString("%1%").arg(static_cast<int>(stat_perc.iowait)));
    cpu_guest_label_->setText(QString("%1%").arg(static_cast<int>(stat_perc.steal)));

    prev_total_time = total;
    prev_cpu_stats = stats[0];
  }

  void setUptime(uint uptime) {
    uint remaining_seconds = uptime;
    int days = uptime / 86400;
    remaining_seconds = uptime % 86400;
    int hours = remaining_seconds / 3600;
    remaining_seconds %= 3600;
    int minutes = remaining_seconds / 60;
    remaining_seconds %= 60;

    QString uptime_str = QString("%1 days, %2:%3:%4")
                              .arg(days)
                              .arg(hours, 1, 10, QChar('0'))
                              .arg(minutes, 2, 10, QChar('0'))
                              .arg(remaining_seconds, 2, 10, QChar('0'));
    uptime_label_->setText(uptime_str);
  }

  void setLoadAvg(const QVector<double>& load_avg) {
    avg_load_label_->setText(QString("%1 %2 %3")
                              .arg(load_avg[0])
                              .arg(load_avg[1])
                              .arg(load_avg[2]));
  }

 protected:
  void paintEvent(QPaintEvent* e) override {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(50, 50, 50));

    painter.drawRoundedRect(this->rect(), 4, 4);

    QWidget::paintEvent(e);
  }

 private:
  QLabel* cpu_util_label_;
  QLabel* cpu_sys_label_;
  QLabel* cpu_user_label_;
  QLabel* cpu_iowait_label_;
  QLabel* cpu_guest_label_;

  CPUStatsWidget* cpu_stats_widget_;

  QLabel* uptime_label_;
  QLabel* avg_load_label_;
};

#endif // CPUWIDGET_H