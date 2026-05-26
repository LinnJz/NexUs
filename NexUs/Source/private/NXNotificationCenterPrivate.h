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
  explicit NXNotificationCenterPrivate(QObject *parent = nullptr);
  ~NXNotificationCenterPrivate();

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  bool _isVisible { false };
  NXThemeType::ThemeMode _themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
  int _shadowBorderWidth { 0 };
#else
  int _shadowBorderWidth { 8 };
#endif
  int _scrollOffset { 0 };
  QList<NXNotificationCenter::NotificationItem> _items;
  QPropertyAnimation *_slideAnimation { nullptr };
  QWidget *_anchorWidget { nullptr };

  int _hitTestCard(QPoint pos) const;
  QRect _clearAllButtonRect() const;
  bool _hitTestClearAll(QPoint pos) const;
};

#endif // NXNOTIFICATIONCENTERPRIVATE_H
