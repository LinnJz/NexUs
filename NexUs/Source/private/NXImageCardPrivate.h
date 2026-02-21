#ifndef NXIMAGECARDPRIVATE_H
#define NXIMAGECARDPRIVATE_H

#include <QImage>
#include <QObject>

#include "NXDef.h"

class NXImageCard;

class NXImageCardPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXImageCard)
  Q_PROPERTY_CREATE_D(QImage, CardImage);
  Q_PROPERTY_CREATE_D(bool, IsPreserveAspectCrop)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
  explicit NXImageCardPrivate(QObject *parent = nullptr);
  ~NXImageCardPrivate() override;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXIMAGECARDPRIVATE_H
