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
  Q_PROPERTY_CREATE_Q_H(int, HeaderMargin)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, CheckIndicatorWidth)
  Q_PROPERTY_CREATE_Q_H(bool, IsSelectionEffectsEnabled)
  Q_PROPERTY_CREATE_Q_H(bool, IsHoverEffectsEnabled)

public:
  explicit NXTableView(QWidget *parent = nullptr);
  ~NXTableView();

  void setModel(QAbstractItemModel *model) override;
  void setHorizontalPadding(int column, int padding);
  int getHorizontalPadding(int column) const;

Q_SIGNALS:
  Q_SIGNAL void tableViewShow();
  Q_SIGNAL void tableViewHide();
  Q_SIGNAL void hoverIndexChanged(const QModelIndex& index);

protected:
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;

private:
  void _syncHorizontalPaddings(int columnCount);
};

#endif // NXTABLEVIEW_H
