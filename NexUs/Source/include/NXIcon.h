#ifndef NXICON_H
#define NXICON_H
#include <QIcon>

#include "NXDef.h"
#include "singleton.h"

class NX_EXPORT NXIcon
{
  SINGLETON_CREATE_H(NXIcon)

private:
  explicit NXIcon();
  ~NXIcon();

public:
  QIcon getNXIcon(NXIconType::IconName awesome);
  QIcon getNXIcon(NXIconType::IconName awesome, QColor iconColor);
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize);
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize, QColor iconColor);
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight);
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor);
};

#endif // NXICON_H
