#ifndef NXINFOBADGE_H
#define NXINFOBADGE_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXInfoBadgePrivate;
class NX_EXPORT NXInfoBadge : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXInfoBadge)
    Q_PROPERTY_CREATE_H(int, Value)
    Q_PROPERTY_CREATE_H(NXIconType::IconName, NXIcon)
public:
    enum BadgeMode
    {
        Dot = 0,
        Value_,
        Icon
    };
    Q_ENUM(BadgeMode)

    explicit NXInfoBadge(QWidget* parent = nullptr);
    explicit NXInfoBadge(int value, QWidget* parent = nullptr);
    explicit NXInfoBadge(NXIconType::IconName icon, QWidget* parent = nullptr);
    ~NXInfoBadge() override;

    void setBadgeMode(BadgeMode mode) noexcept;
    BadgeMode getBadgeMode() const noexcept;

    void setMaxValue(int maxValue) noexcept;
    int getMaxValue() const noexcept;

    enum Severity
    {
        Attention = 0,
        Informational,
        Success,
        Caution,
        Critical
    };
    Q_ENUM(Severity)

    void setSeverity(Severity severity) noexcept;
    Severity getSeverity() const noexcept;

    void attachTo(QWidget *target) noexcept;

protected:
  bool eventFilter(QObject* watched, QEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  QSize sizeHint() const override;
};

#endif // NXINFOBADGE_H
