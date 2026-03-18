#ifndef NXPIVOTVIEW_H
#define NXPIVOTVIEW_H

#include <QListView>
#include <QModelIndex>

#include "NXProperty.h"
class NXPivotStyle;

class NXPivotView : public QListView
{
  Q_OBJECT
  Q_PRIVATE_CREATE(bool, IsAnimationFinished)
  Q_PRIVATE_CREATE(int, MarkWidth)
  Q_PROPERTY_CREATE(int, MarkX)
  Q_PROPERTY_CREATE(int, MarkAnimationWidth)
  Q_PRIVATE_CREATE_2(const QModelIndex&, QModelIndex, CurrentIndex)
  Q_PRIVATE_CREATE(QRect, CurrentIndexRect)
  Q_PRIVATE_CREATE(NXPivotStyle *, PivotStyle)

public:
  explicit NXPivotView(QWidget *parent = nullptr);
  ~NXPivotView() override;
  void doCurrentIndexChangedAnimation(const QModelIndex& index);

protected:
  void wheelEvent(QWheelEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXPIVOTVIEW_H
