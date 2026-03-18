#ifndef NXICON_H
#define NXICON_H
#include <QIcon>

#include "LinnSingleton.h"
#include "NXDef.h"

class NX_EXPORT NXIcon
{
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXIcon))

private:
  explicit NXIcon();
  ~NXIcon();

public:
  QIcon getNXIcon(NXIconType::IconName awesome) noexcept;
  QIcon getNXIcon(NXIconType::IconName awesome, const QColor& iconColor) noexcept;
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize) noexcept;
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize, const QColor& iconColor) noexcept;
  QIcon getNXIcon(NXIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight) noexcept;
  QIcon getNXIcon(
      NXIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, const QColor& iconColor) noexcept;
};

#endif // NXICON_H
