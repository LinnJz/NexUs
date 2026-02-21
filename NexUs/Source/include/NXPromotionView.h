#ifndef NXPROMOTIONVIEW_H
#define NXPROMOTIONVIEW_H

#include <QPixmap>
#include <QWidget>

#include "NXProperty.h"
class NXPromotionCard;
class NXPromotionViewPrivate;

class NX_EXPORT NXPromotionView : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXPromotionView)
  Q_PROPERTY_CREATE_Q_H(bool, IsAutoScroll)
  Q_PROPERTY_CREATE_Q_H(int, CardExpandWidth)
  Q_PROPERTY_CREATE_Q_H(int, CardCollapseWidth)
  Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
  Q_PROPERTY_CREATE_Q_H(int, AutoScrollInterval)
public:
  explicit NXPromotionView(QWidget *parent = nullptr);
  ~NXPromotionView();

  void appendPromotionCard(NXPromotionCard *card);

protected:
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void resizeEvent(QResizeEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXPROMOTIONVIEW_H
