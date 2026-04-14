#ifndef NXPIVOTVIEW_H
#define NXPIVOTVIEW_H

#include <QListView>
#include <QModelIndex>

#include "NXProperty.h"
class NXPivotStyle;

class NXPivotView : public QListView
{
  Q_OBJECT
  Q_PROPERTY_CREATE(int, MarkWidth)
  Q_PROPERTY_CREATE(int, MarkX)
  Q_PRIVATE_CREATE(Qt::CursorShape, HoverValidIndexCursor)
  Q_PRIVATE_CREATE(NXPivotStyle *, PivotStyle)
  Q_PRIVATE_CREATE_2(const QModelIndex &, QModelIndex, PressIndex)
  Q_PRIVATE_CREATE_2(const QModelIndex &, QModelIndex, HoverIndex)
  Q_PRIVATE_CREATE_2(const QModelIndex &, QModelIndex, CommittedIndex)

public:
  explicit NXPivotView(QWidget *parent = nullptr);
  ~NXPivotView() override;
  void doCurrentIndexChangedAnimation(const QModelIndex &index);
  void setPressIndexAndUpdate(const QModelIndex &index) noexcept;
  void setCommittedIndexAndUpdate(const QModelIndex &index) noexcept;
  void refreshHoverState() noexcept;

protected:
  void wheelEvent(QWheelEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  void _setHoverIndexInternal(const QModelIndex &index) noexcept;
  void _updateMarkRegion() noexcept;
  void _updateIndexRect(const QModelIndex &index) noexcept;
  void _updateIndexPair(const QModelIndex &first, const QModelIndex &second) noexcept;
  void _syncHoverCursor() noexcept;

  bool _isAnimationFinished { true };
};

#endif // NXPIVOTVIEW_H
