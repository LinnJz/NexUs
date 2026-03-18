#ifndef NXAPPBAR_H
#define NXAPPBAR_H

#include <QWidget>

#include "NXDef.h"

#ifdef Q_OS_WIN
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#    define Q_TAKEOVER_NATIVEEVENT_H                                                          \
      bool nativeEvent(const QByteArray& eventType, void *message, qintptr *result) override;
#  else
#    define Q_TAKEOVER_NATIVEEVENT_H                                                       \
      bool nativeEvent(const QByteArray& eventType, void *message, long *result) override;
#  endif
#else
#  define Q_TAKEOVER_NATIVEEVENT_H
#endif

#ifdef Q_OS_WIN
#  define NXAPPBAR_HANDLE(NXAppBar)                                               \
    if (NXAppBar)                                                                 \
    {                                                                             \
      int ret = NXAppBar->takeOverNativeEvent(eventType, message, result);        \
      if (ret == -1) { return QWidget::nativeEvent(eventType, message, result); } \
      return (bool) ret;                                                          \
    }                                                                             \
    return QWidget::nativeEvent(eventType, message, result);
#endif

#ifdef Q_OS_WIN
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#    define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, NXAppBar) \
      bool CLASS::nativeEvent(const QByteArray& eventType, void *message, qintptr *result) { NXAPPBAR_HANDLE(NXAppBar) }
#  else
#    define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, NXAppBar)                                                               \
      bool CLASS::nativeEvent(const QByteArray& eventType, void *message, long *result) { NXAPPBAR_HANDLE(NXAppBar) }
#  endif
#else
#  define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, NXAppBar)
#endif
class QMenu;
class NXAppBarPrivate;

class NX_EXPORT NXAppBar : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXAppBar)
  Q_PROPERTY_CREATE_H(bool, IsStayTop)
  Q_PROPERTY_CREATE_H(bool, IsFixedSize)
  Q_PROPERTY_CREATE_H(bool, IsDefaultClosed)
  Q_PROPERTY_CREATE_H(bool, IsOnlyAllowMinAndClose)
  Q_PROPERTY_CREATE_H(int, AppBarHeight)

public:
  explicit NXAppBar(QWidget *parent = nullptr);
  ~NXAppBar() override;

  void setCustomWidget(NXAppBarType::CustomArea customArea,
                       QWidget *customWidget,
                       QObject *hitTestObject             = nullptr,
                       const QString& hitTestFunctionName = {}) noexcept;
  QWidget *getCustomWidget(NXAppBarType::CustomArea customArea) const noexcept;
  void setCustomMenu(QMenu *customMenu) noexcept;
  QMenu *getCustomMenu() const noexcept;

  void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true) noexcept;
  void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags) noexcept;
  NXAppBarType::ButtonFlags getWindowButtonFlags() const noexcept;

  void setRouteBackButtonEnable(bool isEnable) noexcept;
  void setRouteForwardButtonEnable(bool isEnable) noexcept;

  void closeWindow() noexcept;
#ifdef Q_OS_WIN
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  int takeOverNativeEvent(const QByteArray& eventType, void *message, qintptr *result);
#  else
  int takeOverNativeEvent(const QByteArray& eventType, void *message, long *result);
#  endif
#endif
Q_SIGNALS:
  void routeBackButtonClicked();
  void routeForwardButtonClicked();
  void navigationButtonClicked();
  void themeChangeButtonClicked();
  void closeButtonClicked();
  void customWidgetChanged();
  void customMenuChanged();

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
#ifdef Q_OS_WIN
  void paintEvent(QPaintEvent *event) override;
#endif
};

#endif // NXAPPBAR_H
