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
  Q_PROPERTY_CREATE_D(bool, IsClearButtonEnabled)

public:
  explicit NXLineEditPrivate(QObject *parent = nullptr);
  ~NXLineEditPrivate() override;
  Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  NXThemeType::ThemeMode _themeMode;
  qreal _textSpacing { 0.5 };
  QMargins _paddings;
  NXEvent *_focusEvent { nullptr };
  NXLineEditStyle *_lineEditStyle { nullptr };
};

#endif // NXLINEEDITPRIVATE_H
