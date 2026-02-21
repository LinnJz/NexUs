#ifndef NXMESSAGECARDPRIVATE_H
#define NXMESSAGECARDPRIVATE_H

#include <QObject>

#include "NXDef.h"

class NXMessageCard;

class NXMessageCardPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXMessageCard)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(bool, IsCompleted)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, Content)
  Q_PROPERTY_CREATE_D(int, TitlePixelSize)
  Q_PROPERTY_CREATE_D(int, ContentPixelSize)

public:
  explicit NXMessageCardPrivate(QObject *parent = nullptr);
  ~NXMessageCardPrivate();

  NXThemeType::ThemeMode _themeMode;
};

#endif // NXMESSAGECARDPRIVATE_H
