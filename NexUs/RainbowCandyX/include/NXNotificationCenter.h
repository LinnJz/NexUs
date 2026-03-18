#ifndef NXNOTIFICATIONCENTER_H
#define NXNOTIFICATIONCENTER_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXNotificationCenterPrivate;
class NX_EXPORT NXNotificationCenter : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXNotificationCenter)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, PanelWidth)

public:
    struct NotificationItem
    {
        QString title;
        QString content;
        QString timestamp;
        NXIconType::IconName icon = NXIconType::None;
    };

    explicit NXNotificationCenter(QWidget* parent = nullptr);
    ~NXNotificationCenter();

    void addNotification(const NotificationItem& item) noexcept;
    void clearAll() noexcept;
    int getNotificationCount() const noexcept;

    void showPanel(QWidget *anchor) noexcept;
    void hidePanel() noexcept;
    bool isPanelVisible() const noexcept;

Q_SIGNALS:
  void notificationClicked(int index);
  void panelVisibilityChanged(bool visible);

protected:
  void paintEvent(QPaintEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
};

#endif // NXNOTIFICATIONCENTER_H
