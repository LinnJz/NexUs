#ifndef NXMENUPRIVATE_H
#define NXMENUPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "NXProperty.h"
class NXMenu;
class NXMenuStyle;

class NXMenuPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXMenu)
  Q_PROPERTY_CREATE(int, AnimationImagePosY)

public:
  explicit NXMenuPrivate(QObject *parent = nullptr);
  ~NXMenuPrivate();

private:
  bool _isCloseAnimation { false };
  QPoint _mousePressPoint;
  QPixmap _animationPix;
  NXMenuStyle *_menuStyle { nullptr };
};

#endif // NXMENUPRIVATE_H
