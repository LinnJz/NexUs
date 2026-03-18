#ifndef NXTABLEVIEW_H
#define NXTABLEVIEW_H

#include <QTableView>

#include "NXProperty.h"

class QAbstractItemModel;
class NXTableViewPrivate;

class NX_EXPORT NXTableView : public QTableView
{
  Q_OBJECT
  Q_Q_CREATE(NXTableView)
  Q_PROPERTY_CREATE_H(int, HeaderMargin)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, CheckIndicatorWidth)
  Q_PROPERTY_CREATE_H(bool, IsSelectionEffectsEnabled)
  Q_PROPERTY_CREATE_H(bool, IsHoverEffectsEnabled)

public:
  explicit NXTableView(QWidget *parent = nullptr);
  ~NXTableView();

  void setModel(QAbstractItemModel *model) override;
  void setHorizontalPadding(int column, int padding) noexcept;
  int getHorizontalPadding(int column) const noexcept;

  QRect headerCheckIndicatorRect(int section) const noexcept;
  QRect cellCheckIndicatorRect(int row, int column) const noexcept;

Q_SIGNALS:
  void tableViewShow();
  void tableViewHide();
  void hoverIndexChanged(const QModelIndex& index);

protected:
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;
};

#endif // NXTABLEVIEW_H
