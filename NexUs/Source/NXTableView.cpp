#include "NXTableView.h"

#include <QHeaderView>
#include <QMouseEvent>

#include "DeveloperComponents/NXTableViewStyle.h"
#include "NXScrollBar.h"
#include "private/NXTableViewPrivate.h"

NXTableView::NXTableView(QWidget *parent)
    : QTableView(parent)
    , d_ptr(new NXTableViewPrivate())
{
  Q_D(NXTableView);
  d->q_ptr = this;
  setMouseTracking(true);
  setObjectName("NXTableView");
  setStyleSheet(QStringLiteral("QTableView{background-color:transparent;}"
                               "QHeaderView{background-color:transparent;border:0px;}"));
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

void
NXTableView::setBorderRadius(int radius)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setBorderRadius(radius);
  update();
}

int
NXTableView::getBorderRadius() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getBorderRadius();
}

void
NXTableView::setCheckIndicatorWidth(int width)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setCheckIndicatorWidth(width);
  update();
}

int
NXTableView::getCheckIndicatorWidth() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getCheckIndicatorWidth();
}

void
NXTableView::setHeaderMargin(int headerMargin)
{
  Q_D(NXTableView);
  if (headerMargin >= 0)
  {
    d->_tableViewStyle->setHeaderMargin(headerMargin);
    doItemsLayout();
  }
}

int
NXTableView::getHeaderMargin() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getHeaderMargin();
}

void
NXTableView::setIsHoverRowEffectEnable(bool enable)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setIsHoverRowEffectEnable(enable);
  update();
}

bool
NXTableView::getIsHoverRowEffectEnable() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getIsHoverRowEffectEnable();
}

void
NXTableView::setDefaultPadding(int padding)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setDefaultPadding(padding);
  update();
}

int
NXTableView::getDefaultPadding() const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getDefaultPadding();
}

void
NXTableView::setColumnPadding(int column, int padding)
{
  Q_D(NXTableView);
  d->_tableViewStyle->setColumnPadding(column, padding);
  update();
}

int
NXTableView::columnPadding(int column) const
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->columnPadding(column);
}

void
NXTableView::clearColumnPadding(int column)
{
  Q_D(NXTableView);
  d->_tableViewStyle->clearColumnPadding(column);
  update();
}

void
NXTableView::showEvent(QShowEvent *event)
{
  Q_EMIT tableViewShow();
  QTableView::showEvent(event);
}

void
NXTableView::hideEvent(QHideEvent *event)
{
  Q_EMIT tableViewHide();
  QTableView::hideEvent(event);
}

void
NXTableView::mouseMoveEvent(QMouseEvent *event)
{
  Q_D(NXTableView);
  if (selectionBehavior() == QAbstractItemView::SelectRows)
  {
    const QModelIndex &currentIndex  = indexAt(event->pos());
    const QModelIndex &oldHoverIndex = d->_tableViewStyle->getCurrentHoverIndex();
    if (currentIndex.isValid() && currentIndex.row() != oldHoverIndex.row())
    {
      QRect rowRect = visualRect(currentIndex);
      rowRect.setX(0);
      rowRect.setWidth(viewport()->width());
      update(rowRect);
    }
    d->_tableViewStyle->setCurrentHoverIndex(currentIndex);
    Q_EMIT hoverIndexChanged(currentIndex);
  }
  QTableView::mouseMoveEvent(event);
}

void
NXTableView::leaveEvent(QEvent *event)
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
