#ifndef NXFLYOUTPRIVATE_H
#define NXFLYOUTPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXFlyout;
class QVBoxLayout;
class QGraphicsOpacityEffect;

class NXFlyoutPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXFlyout)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, Content)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(bool, IsLightDismiss)

public:
  explicit NXFlyoutPrivate(QObject *parent = nullptr);
  ~NXFlyoutPrivate();

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  NXThemeType::ThemeMode _themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
  int _shadowBorderWidth { 0 };
#else
  int _shadowBorderWidth { 8 };
#endif
  QVBoxLayout *_mainLayout { nullptr };
  QWidget *_contentWidget { nullptr };
  QGraphicsOpacityEffect *_opacityEffect { nullptr };
  void _doShowAnimation();
};

#endif // NXFLYOUTPRIVATE_H
