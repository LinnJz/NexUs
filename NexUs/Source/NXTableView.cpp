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
  setStyleSheet("QTableView{background-color:transparent;}"
                "QHeaderView{background-color:transparent;border:0px;}");
  setShowGrid(false);
  setVerticalScrollBar(new NXScrollBar(this));
  setHorizontalScrollBar(new NXScrollBar(this));
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  d->_pTableViewStyle = new NXTableViewStyle(style());
  setStyle(d->_pTableViewStyle);
  this->installEventFilter(this);
}

NXTableView::~NXTableView()
{
  Q_D(NXTableView);
  delete d->_pTableViewStyle;
}

void NXTableView::setBorderRadius(int radius)
{
  Q_D(NXTableView);
  d->_pTableViewStyle->setBorderRadius(radius);
  update();
}

int NXTableView::getBorderRadius() const
{
  Q_D(const NXTableView);
  return d->_pTableViewStyle->getBorderRadius();
}

void NXTableView::setIsDrawAlternateRowsEnabled(bool enabled)
{
  Q_D(NXTableView);
  d->_pTableViewStyle->setIsDrawAlternateRowsEnabled(enabled);
  update();
}

bool NXTableView::getIsDrawAlternateRowsEnabled() const
{
  Q_D(const NXTableView);
  return d->_pTableViewStyle->getIsDrawAlternateRowsEnabled();
}

void NXTableView::setIsSelectionEffectsEnabled(bool enabled)
{
  Q_D(NXTableView);
  d->_pTableViewStyle->setIsSelectionEffectsEnabled(enabled);
  update();
}

bool NXTableView::getIsSelectionEffectsEnabled() const
{
  Q_D(const NXTableView);
  return d->_pTableViewStyle->getIsSelectionEffectsEnabled();
}

void NXTableView::setIsHoverEffectsEnabled(bool enabled)
{
  Q_D(NXTableView);
  d->_pTableViewStyle->setIsHoverEffectsEnabled(enabled);
  update();
}

bool NXTableView::getIsHoverEffectsEnabled() const
{
  Q_D(const NXTableView);
  return d->_pTableViewStyle->getIsHoverEffectsEnabled();
}

void NXTableView::setIndexWidget(const QModelIndex& index, QWidget *widget)
{
  QTableView::setIndexWidget(index, widget);
  if (auto *indexWidget = qobject_cast<NXModelIndexWidget *>(widget)) { Q_EMIT indexWidgetAdded(indexWidget); }
}

QWidget *NXTableView::indexWidget(const QModelIndex& index) const { return QTableView::indexWidget(index); }

QList<NXModelIndexWidget *> NXTableView::getIndexWidgets() const
{
  QList<NXModelIndexWidget *> widgets;
  for (int row = 0; row < model()->rowCount(); ++row)
  {
    for (int col = 0; col < model()->columnCount(); ++col)
    {
      if (auto *widget = qobject_cast<NXModelIndexWidget *>(indexWidget(model()->index(row, col))))
      {
        widgets.append(widget);
      }
    }
  }
  return widgets;
}

NXModelIndexWidget *NXTableView::getIndexWidget(const QModelIndex& index) const
{
  return qobject_cast<NXModelIndexWidget *>(indexWidget(index));
}

/*
void NXTableView::updateIndexWidgets()
{
    for (int row = 0; row < model()->rowCount(); ++row) {
        for (int col = 0; col < model()->columnCount(); ++col) {
            QModelIndex currentIndex = model()->index(row, col);
            if (auto* widget = getIndexWidget(currentIndex)) {
                widget->setIndex(currentIndex);
            }
        }
    }
}
*/
void NXTableView::setHorizontalHeaderFontSize(int pixelSize)
{
  QFont tableHeaderFont = horizontalHeader()->font();
  tableHeaderFont.setPixelSize(pixelSize);
  horizontalHeader()->setFont(tableHeaderFont);
}

void NXTableView::setVerticalHeaderFontSize(int pixelSize)
{
  QFont tableHeaderFont = verticalHeader()->font();
  tableHeaderFont.setPixelSize(pixelSize);
  verticalHeader()->setFont(tableHeaderFont);
}

void NXTableView::setFontSize(int pixelSize)
{
  QFont tableFont = this->font();
  tableFont.setPixelSize(pixelSize);
  this->setFont(tableFont);
}

void NXTableView::adjustHeaderColumnIconRect(const QHash<int, coords>& adjusts)
{
  Q_D(NXTableView);
  d->_pTableViewStyle->adjustHeaderColumnIconRect(adjusts);
}

void NXTableView::adjustColummTextRect(const QHash<int, coords>& adjusts)
{
  Q_D(NXTableView);
  d->_pTableViewStyle->adjustColummTextRect(adjusts);
}

void NXTableView::setHeaderMargin(int headerMargin)
{
  Q_D(NXTableView);
  if (headerMargin >= 0)
  {
    d->_pTableViewStyle->setHeaderMargin(headerMargin);
    doItemsLayout();
  }
}

int NXTableView::getHeaderMargin() const
{
  Q_D(const NXTableView);
  return d->_pTableViewStyle->getHeaderMargin();
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
    d->_pTableViewStyle->setCurrentHoverIndex(currentIndex);
    Q_EMIT currentHoverIndexChanged(currentIndex);
    update();
  }
  QTableView::mouseMoveEvent(event);
}

void NXTableView::mousePressEvent(QMouseEvent *event)
{
  Q_D(NXTableView);
  QTableView::mousePressEvent(event);
}

void NXTableView::mouseReleaseEvent(QMouseEvent *event) { QTableView::mouseReleaseEvent(event); }

void NXTableView::leaveEvent(QEvent *event)
{
  Q_D(NXTableView);
  if (selectionBehavior() == QAbstractItemView::SelectRows)
  {
    d->_pTableViewStyle->setCurrentHoverIndex(QModelIndex());
    Q_EMIT currentHoverIndexChanged(QModelIndex());
    update();
  }
  QTableView::leaveEvent(event);
}

/*
void NXTableView::rowsInserted(const QModelIndex& parent, int start, int end)
{
    QTableView::rowsInserted(parent, start, end);
    // 行插入后可能需要更新索引widget
    updateIndexWidgets();
}

void NXTableView::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end)
{
    QTableView::rowsAboutToBeRemoved(parent, start, end);
}

void NXTableView::reset()
{
    QTableView::reset();
    // 模型重置后更新所有索引widget
    updateIndexWidgets();
}
*/
NXModelIndexWidget::NXModelIndexWidget(NXTableView *parent)
    : QWidget(parent)
    , d_ptr(new NXModelIndexWidgetPrivate())
{
  Q_D(NXModelIndexWidget);
  d->q_ptr = this;
  setAttribute(Qt::WA_Hover, true);
  setMouseTracking(true);
}

NXModelIndexWidget::NXModelIndexWidget(const QModelIndex& index, NXTableView *parent)
    : NXModelIndexWidget(parent)
{
  setIndex(index);
}

NXModelIndexWidget::~NXModelIndexWidget() { }

const QModelIndex& NXModelIndexWidget::getIndex() const
{
  Q_D(const NXModelIndexWidget);
  return d->_pIndex;
}

void NXModelIndexWidget::setIndex(const QModelIndex& index)
{
  Q_D(NXModelIndexWidget);
  if (d->_pIndex != index)
  {
    d->_pIndex = index;
    Q_EMIT indexChanged(index);
    update(); // 索引改变时重绘
  }
}

bool NXModelIndexWidget::isValid() const
{
  Q_D(const NXModelIndexWidget);
  return d->_pIndex.isValid();
}

int NXModelIndexWidget::row() const
{
  Q_D(const NXModelIndexWidget);
  return d->_pIndex.row();
}

int NXModelIndexWidget::column() const
{
  Q_D(const NXModelIndexWidget);
  return d->_pIndex.column();
}

void NXModelIndexWidget::enterEvent(QEnterEvent *event)
{
  Q_EMIT entered();
  QWidget::enterEvent(event);
}

void NXModelIndexWidget::leaveEvent(QEvent *event)
{
  Q_EMIT leaved();
  QWidget::leaveEvent(event);
}

void NXModelIndexWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) { Q_EMIT clicked(); }
  QWidget::mousePressEvent(event);
}

void NXModelIndexWidget::mouseReleaseEvent(QMouseEvent *event) { QWidget::mouseReleaseEvent(event); }

void NXModelIndexWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) { Q_EMIT doubleClicked(); }
  QWidget::mouseDoubleClickEvent(event);
}

void NXModelIndexWidget::paintEvent(QPaintEvent *event) { QWidget::paintEvent(event); }

bool NXModelIndexWidget::event(QEvent *event)
{
  switch (event->type())
  {
    case QEvent::HoverEnter : Q_EMIT entered(); break;
    case QEvent::HoverLeave : Q_EMIT leaved(); break;
    default                 : break;
  }
  return QWidget::event(event);
}
