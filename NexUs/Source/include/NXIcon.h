#ifndef NXICON_H
#define NXICON_H
#include <QIcon>

#include "LinnSingleton.h"
#include "NXDef.h"
#include "NXProperty.h"

class NX_EXPORT NXIcon
{
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXIcon))

private:
  explicit NXIcon();
  ~NXIcon();

public:
  QIcon getNXIcon(NXIconType::IconName awesome);
  QIcon getNXIcon(NXIconType::IconName awesome, const QColor &iconColor);
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize);
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize, const QColor &iconColor);
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight);
  QIcon
  getNXIcon(NXIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, const QColor &iconColor);
};

#endif // NXICON_H
