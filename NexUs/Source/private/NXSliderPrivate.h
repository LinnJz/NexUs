#ifndef NX_SLIDERPRIVATE_H
#define NX_SLIDERPRIVATE_H

#include <QObject>

#include "NXProperty.h"
class NXToolTip;
class NXSlider;
class NXSliderStyle;

class NXSliderPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSlider)
public:
  explicit NXSliderPrivate(QObject *parent = nullptr);
  ~NXSliderPrivate();

private:
  bool _mousePressed { false };
  NXToolTip *_valueToolTip { nullptr };
  NXSliderStyle *_sliderStyle { nullptr };
};


#endif //! NX_SLIDERPRIVATE_H
