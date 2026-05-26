#ifndef NXINFOBAR_PRIVATE_H
#define NXINFOBAR_PRIVATE_H

#include <functional>

#include <QObject>
#include <QRect>

#include "NXDef.h"

class NXInfoBar;

class NXInfoBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXInfoBar)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, Message)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(NXInfoBarType::InfoBarSeverity, Severity)
  Q_PROPERTY_CREATE_D(NXIconType::IconName, InfoBarIcon)
  Q_PROPERTY_CREATE_D(bool, IsClosable)

public:
  struct ActionItem
  {
    QString text;
    std::function<void()> callback;
  };

  bool _isClosing { false };
  NXThemeType::ThemeMode _themeMode;
  int _closeButtonHover { false };
  int _hoverActionIndex { -1 };
  QRect _closeButtonRect;
  QList<ActionItem> _actions;
  QList<QRect> _actionButtonRects;

  explicit NXInfoBarPrivate(QObject *parent = nullptr);
  ~NXInfoBarPrivate();


  QColor _getSeverityColor() const;
  NXIconType::IconName _getSeverityIcon() const;
  void _doCloseAnimation();
};

#endif // NXINFOBAR_PRIVATE_H
