#include "NXTableWidget.h"

#include <QHeaderView>
#include <QMouseEvent>

#include "DeveloperComponents/NXTableWidgetStyle.h"
#include "NXScrollBar.h"
#include "NXTableWidgetPrivate.h"

NXTableWidget::NXTableWidget(QWidget *parent)
    : QTableWidget(parent)
    , d_ptr(new NXTableWidgetPrivate())
{
  Q_D(NXTableWidget);
  d->q_ptr = this;
  setMouseTracking(true);
  setObjectName("NXTableWidget");
  setStyleSheet(QStringLiteral("QTableWidget{background-color:transparent;}"
                "QHeaderView{background-color:transparent;border:0px;}"));
  setShowGrid(false);
  setVerticalScrollBar(new NXScrollBar(this));
  setHorizontalScrollBar(new NXScrollBar(this));
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  d->_tableWidgetStyle = new NXTableWidgetStyle(style());
  setStyle(d->_tableWidgetStyle);
}

NXTableWidget::~NXTableWidget()
{
  Q_D(NXTableWidget);
  delete d->_tableWidgetStyle;
}

void NXTableWidget::insertRows(int row, int count) noexcept
{
  if (row < 0 || count <= 0 || row > rowCount()) { return; }

  for (int i = 0; i < count; ++i) { insertRow(row); }
}

void NXTableWidget::removeRows(int row, int count) noexcept
{
  if (row < 0 || count <= 0 || row >= rowCount()) { return; }

  int actualCount = qMin(count, rowCount() - row);

  for (int i = row + actualCount - 1; i >= row; --i) { removeRow(i); }
}

void NXTableWidget::insertColumns(int column, int count) noexcept
{
  if (column < 0 || count <= 0 || column > columnCount()) { return; }

  for (int i = 0; i < count; ++i) { insertColumn(column); }
}

void NXTableWidget::removeColumns(int column, int count) noexcept
{
  if (column < 0 || count <= 0 || column >= columnCount()) { return; }

  int actualCount = qMin(count, columnCount() - column);

  for (int i = column + actualCount - 1; i >= column; --i) { removeColumn(i); }
}

void NXTableWidget::setItemText(int row, int column, const QString& text) noexcept
{
  if (row < 0 || row >= rowCount() || column < 0 || column >= columnCount()) { return; }

  QTableWidgetItem *currentItem = item(row, column);
  if (currentItem) { currentItem->setText(text); }
  else
  {
    QTableWidgetItem *newItem = new QTableWidgetItem(text);
    setItem(row, column, newItem);
  }
}

QString NXTableWidget::getItemText(int row, int column) const noexcept
{
  if (row < 0 || row >= rowCount() || column < 0 || column >= columnCount()) { return QString(); }

  QTableWidgetItem *currentItem = item(row, column);
  return currentItem ? currentItem->text() : QString();
}

void NXTableWidget::setRowData(int row, const QStringList& data) noexcept
{
  if (row < 0 || row >= rowCount()) { return; }

  int maxCol = qMin(data.size(), columnCount());
  for (int col = 0; col < maxCol; ++col) { setItemText(row, col, data[col]); }
}

QStringList NXTableWidget::getRowData(int row) const noexcept
{
  QStringList data;
  if (row < 0 || row >= rowCount()) { return data; }

  for (int col = 0; col < columnCount(); ++col) { data.append(getItemText(row, col)); }
  return data;
}

void NXTableWidget::setItemHeight(int itemHeight) noexcept
{
  Q_D(NXTableWidget);
  if (itemHeight > 0)
  {
    d->_tableWidgetStyle->setItemHeight(itemHeight);
    doItemsLayout();
  }
}

int NXTableWidget::getItemHeight() const noexcept
{
  Q_D(const NXTableWidget);
  return d->_tableWidgetStyle->getItemHeight();
}

void NXTableWidget::setHeaderMargin(int headerMargin) noexcept
{
  Q_D(NXTableWidget);
  if (headerMargin >= 0)
  {
    d->_tableWidgetStyle->setHeaderMargin(headerMargin);
    doItemsLayout();
  }
}

int NXTableWidget::getHeaderMargin() const noexcept
{
  Q_D(const NXTableWidget);
  return d->_tableWidgetStyle->getHeaderMargin();
}

void NXTableWidget::setIsTransparent(bool isTransparent) noexcept
{
  Q_D(NXTableWidget);
  d->_tableWidgetStyle->setIsTransparent(isTransparent);
  update();
}

bool NXTableWidget::getIsTransparent() const noexcept
{
  Q_D(const NXTableWidget);
  return d->_tableWidgetStyle->getIsTransparent();
}

void NXTableWidget::showEvent(QShowEvent *event)
{
  Q_EMIT tableWidgetShow();
  QTableWidget::showEvent(event);
}

void NXTableWidget::hideEvent(QHideEvent *event)
{
  Q_EMIT tableWidgetHide();
  QTableWidget::hideEvent(event);
}

void NXTableWidget::mouseMoveEvent(QMouseEvent *event)
{
  Q_D(NXTableWidget);
  if (selectionBehavior() == QAbstractItemView::SelectRows)
  {
    const QModelIndex& currentIndex  = indexAt(event->pos());
    const QModelIndex& oldHoverIndex = d->_tableWidgetStyle->getCurrentHoverIndex();
    d->_tableWidgetStyle->setCurrentHoverIndex(currentIndex);
    if (currentIndex.isValid() && currentIndex.row() != oldHoverIndex.row())
    {
      QRect rowRect = visualRect(oldHoverIndex);
      rowRect.setX(0);
      rowRect.setWidth(viewport()->width());

      rowRect = visualRect(currentIndex);
      rowRect.setX(0);
      rowRect.setWidth(viewport()->width());
      update(rowRect);
    }
    Q_EMIT hoverIndexChanged(currentIndex);
  }
  QTableWidget::mouseMoveEvent(event);
}

void NXTableWidget::leaveEvent(QEvent *event)
{
  Q_D(NXTableWidget);
  if (selectionBehavior() == QAbstractItemView::SelectRows)
  {
    d->_tableWidgetStyle->setCurrentHoverIndex({});
    update();
  }
  QTableWidget::leaveEvent(event);
}
