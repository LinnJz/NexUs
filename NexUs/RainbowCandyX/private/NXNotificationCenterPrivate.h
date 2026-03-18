#ifndef NXNOTIFICATIONCENTERPRIVATE_H
#define NXNOTIFICATIONCENTERPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXNotificationCenter.h"
class QPropertyAnimation;
class NXNotificationCenterPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXNotificationCenter)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, PanelWidth)

public:
    explicit NXNotificationCenterPrivate(QObject* parent = nullptr);
    ~NXNotificationCenterPrivate() override;

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QList<NXNotificationCenter::NotificationItem> _items;
    QPropertyAnimation* _slideAnimation{nullptr};
    bool _isVisible{false};
    NXThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{8};
    int _scrollOffset{0};
    QWidget* _anchorWidget{nullptr};

    int _hitTestCard(const QPoint& pos) const;
    QRect _clearAllButtonRect() const;
    bool _hitTestClearAll(const QPoint& pos) const;
};

#endif // NXNOTIFICATIONCENTERPRIVATE_H
