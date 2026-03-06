#include "NXTableView.h"

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QMouseEvent>

#include "DeveloperComponents/NXTableViewStyle.h"
#include "NXScrollBar.h"
#include "NXTableViewPrivate.h"

NXTableView::NXTableView(QWidget *parent)
    : QTableView(parent)
    , d_ptr(new NXTableViewPrivate())
{
  Q_D(NXTableView);
  d->q_ptr = this;
  setMouseTracking(true);
  setObjectName("NXTableView");
  setStyleSheet("QTableView{background-color:transparent;}"
                "QHeaderView{background-color:transparent;border:0px;}");
  setShowGrid(false);
  setVerticalScrollBar(new NXScrollBar(this));
  setHorizontalScrollBar(new NXScrollBar(this));
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  d->_tableViewStyle = new NXTableViewStyle(style());
  setStyle(d->_tableViewStyle);
}

NXTableView::~NXTableView()
{
  Q_D(NXTableView);
  delete d->_tableViewStyle;
}

void NXTableView::setHorizontalPadding(int column, int padding)
{
  Q_D(NXTableView);
  if (column < 0) { return; }
  int columnCount = model() ? model()->columnCount() : 0;
  _syncHorizontalPaddings(qMax(column + 1, columnCount));
  d->_tableViewStyle->setHorizontalPadding(column, padding);
  update();
}

int NXTableView::getHorizontalPadding(int column) const { return d_ptr->_tableViewStyle->getHorizontalPadding(column); }

void NXTableView::setModel(QAbstractItemModel *model)
{
  Q_D(NXTableView);
  if (QAbstractItemModel *oldModel = this->model())
  {
    QObject::disconnect(d->_modelResetConnection);
    QObject::disconnect(d->_columnsInsertedConnection);
    QObject::disconnect(d->_columnsRemovedConnection);
    QObject::disconnect(d->_layoutChangedConnection);
  }

  QTableView::setModel(model);

  if (model)
  {
    d->_modelResetConnection      = connect(model,
                                       &QAbstractItemModel::modelReset,
                                       this,
                                       [this]()
    {
      _syncHorizontalPaddings(this->model() ? this->model()->columnCount() : 0);
      update();
    });
    d->_columnsInsertedConnection = connect(model,
                                            &QAbstractItemModel::columnsInserted,
                                            this,
                                            [this]()
    {
      _syncHorizontalPaddings(this->model() ? this->model()->columnCount() : 0);
      update();
    });
    d->_columnsRemovedConnection  = connect(model,
                                           &QAbstractItemModel::columnsRemoved,
                                           this,
                                           [this]()
    {
      _syncHorizontalPaddings(this->model() ? this->model()->columnCount() : 0);
      update();
    });
    d->_layoutChangedConnection   = connect(model,
                                          &QAbstractItemModel::layoutChanged,
                                          this,
                                          [this]()
    {
      _syncHorizontalPaddings(this->model() ? this->model()->columnCount() : 0);
      update();
    });
  }

  _syncHorizontalPaddings(model ? model->columnCount() : 0);
}

void NXTableView::setHeaderMargin(int headerMargin)
{
  Q_D(NXTableView);
  if (headerMargin >= 0)
  {
    d->_tableViewStyle->setHeaderMargin(headerMargin);
    doItemsLayout();
  }
}

int NXTableView::getHeaderMargin() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getHeaderMargin();
}

void NXTableView::setBorderRadius(int radius)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setBorderRadius(radius);
  update();
}

int NXTableView::getBorderRadius() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getBorderRadius();
}

void NXTableView::setCheckIndicatorWidth(int width)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setCheckIndicatorWidth(width);
  update();
}

int NXTableView::getCheckIndicatorWidth() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getCheckIndicatorWidth();
}

void NXTableView::setIsSelectionEffectsEnabled(bool enabled)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setIsSelectionEffectsEnabled(enabled);
  update();
}

bool NXTableView::getIsSelectionEffectsEnabled() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getIsSelectionEffectsEnabled();
}

void NXTableView::setIsHoverEffectsEnabled(bool enabled)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setIsHoverEffectsEnabled(enabled);
  update();
}

bool NXTableView::getIsHoverEffectsEnabled() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getIsHoverEffectsEnabled();
}

void NXTableView::showEvent(QShowEvent *event)
{
  Q_EMIT tableViewShow();
  QTableView::showEvent(event);
}

void NXTableView::hideEvent(QHideEvent *event)
{
  Q_EMIT tableViewHide();
  QTableView::hideEvent(event);
}

void NXTableView::mouseMoveEvent(QMouseEvent *event)
{
  Q_D(NXTableView);
  if (selectionBehavior() == QAbstractItemView::SelectRows)
  {
    const QModelIndex& currentIndex = indexAt(event->pos());
    d->_tableViewStyle->setCurrentHoverIndex(currentIndex);
    update();
    Q_EMIT hoverIndexChanged(currentIndex);
  }
  QTableView::mouseMoveEvent(event);
}

void NXTableView::leaveEvent(QEvent *event)
{
  Q_D(NXTableView);
  if (selectionBehavior() == QAbstractItemView::SelectRows)
  {
    d->_tableViewStyle->setCurrentHoverIndex(QModelIndex {});
    update();
    Q_EMIT hoverIndexChanged(QModelIndex {});
  }
  QTableView::leaveEvent(event);
}

void NXTableView::_syncHorizontalPaddings(int columnCount)
{
  Q_D(NXTableView);
  d->_tableViewStyle->syncHorizontalPaddings(columnCount);
}
