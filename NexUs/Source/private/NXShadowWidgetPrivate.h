#ifndef NX_SHADOW_WIDGET_PRIVATE_H_
#define NX_SHADOW_WIDGET_PRIVATE_H_
#include <functional>

#include <QObject>

#include "NXDef.h"

class QPainter;
class QWidget;

class NXShadowGraphicsEffect;
class NXShadowWidget;

class NXShadowWidgetPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXShadowWidget)

public:
  explicit NXShadowWidgetPrivate(QObject *parent = nullptr);
  ~NXShadowWidgetPrivate();

private:
  NXShadowGraphicsEffect *_shadowEffect { nullptr };
  std::function<void(QPainter *, QWidget *)> _customDraw;
};
#endif // !NX_SHADOW_WIDGET_PRIVATE_H_
