#ifndef NXSTACKBARPRIVATE_H
#define NXSTACKBARPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXSnackbar.h"

class NXSnackbarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSnackbar)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, DisplayMsec)

public:
  explicit NXSnackbarPrivate(QObject *parent = nullptr);
  ~NXSnackbarPrivate();

private:
  bool _isActionHover { false };
  bool _isActionPressed { false };
  bool _isClosing { false };
  NXThemeType::ThemeMode _themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
  int _shadowBorderWidth { 0 };
#else
  int _shadowBorderWidth { 6 };
#endif
  NXSnackbar::SnackbarType _snackbarType { NXSnackbar::Info };
  int _baseBottomY { 0 };
  int _centerX { 0 };
  QRect _actionRect;
  QString _text;
  QString _actionText;
  std::function<void()> _doClose;
};

#endif // NXSTACKBARPRIVATE_H
