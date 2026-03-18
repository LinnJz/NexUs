#ifndef NXINFOBADGEPRIVATE_H
#define NXINFOBADGEPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXInfoBadge.h"
class NXInfoBadgePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXInfoBadge)
    Q_PROPERTY_CREATE_D(int, Value)
    Q_PROPERTY_CREATE_D(NXIconType::IconName, NXIcon)
public:
    explicit NXInfoBadgePrivate(QObject* parent = nullptr);
    ~NXInfoBadgePrivate() override;

private:
    NXInfoBadge::BadgeMode _badgeMode{NXInfoBadge::Dot};
    NXInfoBadge::Severity _severity{NXInfoBadge::Attention};
    int _maxValue{99};
    QWidget* _target{nullptr};
    NXThemeType::ThemeMode _themeMode;
    void _updatePosition();
    QColor _getSeverityColor() const;
};

#endif // NXINFOBADGEPRIVATE_H
