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
  Q_PROPERTY_CREATE(bool, AlignParentMenuHeight)
public:
  explicit NXMenuPrivate(QObject *parent = nullptr);
  ~NXMenuPrivate() override;

private:
  bool _isCloseAnimation { false };
  QPixmap _animationPix;
  QPoint _mousePressPoint;
  NXMenuStyle *_menuStyle { nullptr };
};

#endif // NXMENUPRIVATE_H
