#ifndef NXQRCODEPRIVATE_H
#define NXQRCODEPRIVATE_H

#include <QObject>
#include <QVector>

#include "NXDef.h"

class NXQRCode;

class NXQRCodePrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXQRCode)
  Q_PROPERTY_CREATE_D(QColor, ForegroundColor)
  Q_PROPERTY_CREATE_D(QColor, BackgroundColor)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, QuietZone)

public:
  int _moduleCount { 0 };
  int _ecLevel { 1 }; // 0=L, 1=M, 2=Q, 3=H
  NXThemeType::ThemeMode _themeMode;
  QString _text;
  explicit NXQRCodePrivate(QObject *parent = nullptr);
  ~NXQRCodePrivate();

  void encode(const QString &text);

  QVector<QVector<bool>> _modules;
};

#endif // NXQRCODEPRIVATE_H
