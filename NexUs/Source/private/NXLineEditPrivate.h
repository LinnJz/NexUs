#ifndef NXLINEEDITPRIVATE_H
#define NXLINEEDITPRIVATE_H

#include <QMargins>
#include <QObject>
#include <QVariantMap>

#include "NXDef.h"
class NXEvent;
class NXLineEdit;
class NXLineEditStyle;

class NXLineEditPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXLineEdit)
  Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
  Q_PROPERTY_CREATE_D(bool, IsClearButtonEnable)

public:
  explicit NXLineEditPrivate(QObject *parent = nullptr);
  ~NXLineEditPrivate();
  Q_INVOKABLE void onWMWindowClickedEvent(const QVariantMap &data);
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  NXThemeType::ThemeMode _themeMode;
  qreal _textSpacing { 0.5 };
  NXEvent *_focusEvent { nullptr };
  NXLineEditStyle *_lineEditStyle { nullptr };
};

#endif // NXLINEEDITPRIVATE_H
