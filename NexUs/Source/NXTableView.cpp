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

void NXTableView::setHorizontalPadding(int column, int padding) noexcept
{
  Q_D(NXTableView);
  if (column < 0) { return; }
  int columnCount = model() ? model()->columnCount() : 0;
  d->_tableViewStyle->syncHorizontalPaddings(qMax(column + 1, columnCount));
  d->_tableViewStyle->setHorizontalPadding(column, padding);
  update();
}

int NXTableView::getHorizontalPadding(int column) const noexcept
{
  return d_ptr->_tableViewStyle->getHorizontalPadding(column);
}

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
    d->_modelResetConnection = connect(model, &QAbstractItemModel::modelReset, this, [this]()
    {
      d_ptr->_tableViewStyle->syncHorizontalPaddings(this->model() ? this->model()->columnCount() : 0);
      update();
    });

    d->_columnsInsertedConnection = connect(model, &QAbstractItemModel::columnsInserted, this, [this]()
    {
      d_ptr->_tableViewStyle->syncHorizontalPaddings(this->model() ? this->model()->columnCount() : 0);
      update();
    });

    d->_columnsRemovedConnection = connect(model, &QAbstractItemModel::columnsRemoved, this, [this]()
    {
      d_ptr->_tableViewStyle->syncHorizontalPaddings(this->model() ? this->model()->columnCount() : 0);
      update();
    });

    d->_layoutChangedConnection = connect(model, &QAbstractItemModel::layoutChanged, this, [this]()
    {
      d_ptr->_tableViewStyle->syncHorizontalPaddings(this->model() ? this->model()->columnCount() : 0);
      update();
    });
  }
  d->_tableViewStyle->syncHorizontalPaddings(model ? model->columnCount() : 0);
}

void NXTableView::setHeaderMargin(int headerMargin) noexcept
{
  Q_D(NXTableView);
  if (headerMargin >= 0)
  {
    d->_tableViewStyle->setHeaderMargin(headerMargin);
    doItemsLayout();
  }
}

int NXTableView::getHeaderMargin() const noexcept
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getHeaderMargin();
}

void NXTableView::setBorderRadius(int radius) noexcept
{
  Q_D(NXTableView);
  d->_tableViewStyle->setBorderRadius(radius);
  update();
}

int NXTableView::getBorderRadius() const noexcept
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getBorderRadius();
}

void NXTableView::setCheckIndicatorWidth(int width) noexcept
{
  Q_D(NXTableView);
  d->_tableViewStyle->setCheckIndicatorWidth(width);
  update();
}

int NXTableView::getCheckIndicatorWidth() const noexcept
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getCheckIndicatorWidth();
}

void NXTableView::setIsSelectionEffectsEnabled(bool enabled) noexcept
{
  Q_D(NXTableView);
  d->_tableViewStyle->setIsSelectionEffectsEnabled(enabled);
  update();
}

bool NXTableView::getIsSelectionEffectsEnabled() const noexcept
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getIsSelectionEffectsEnabled();
}

void NXTableView::setIsHoverEffectsEnabled(bool enabled) noexcept
{
  Q_D(NXTableView);
  d->_tableViewStyle->setIsHoverEffectsEnabled(enabled);
  update();
}

bool NXTableView::getIsHoverEffectsEnabled() const noexcept
{
  Q_D(const NXTableView);
  return d->_tableViewStyle->getIsHoverEffectsEnabled();
}

QRect NXTableView::headerCheckIndicatorRect(int section) const noexcept
{
  Q_D(const NXTableView);
  QHeaderView *header = horizontalHeader();
  if (!header || section < 0 || section >= header->count() || !model() ||
      !model()->headerData(section, Qt::Horizontal, Qt::CheckStateRole).isValid()) [[unlikely]]
  {
    return QRect {};
  }

  NXTableViewStyle *style = d->_tableViewStyle;
  int padding             = style->getHorizontalPadding(section);
  int cw                  = style->getCheckIndicatorWidth();

  int x = header->sectionViewportPosition(section);
  int w = header->sectionSize(section);
  int h = header->height();
  QRect sectionRect(x, 0, w, h); // 相对于表头

  // 按绘制逻辑计算复选框矩形（与 NXTableViewStyle::drawControl 中 CE_HeaderSection 一致）
  QRect checkRect(sectionRect.left() + padding + 3, sectionRect.center().y() - cw / 2, cw, cw);

  QPoint offset = header->mapTo(this, QPoint(0, 0));
  checkRect.translate(offset);
  return checkRect;
}

QRect NXTableView::cellCheckIndicatorRect(int row, int column) const noexcept
{
  Q_D(const NXTableView);
  if (!model() || row < 0 || column < 0 || row >= model()->rowCount() || column >= model()->columnCount()) [[unlikely]]
  {
    return QRect {};
  }

  QModelIndex index = model()->index(row, column);
  if (!index.isValid()) [[unlikely]] { return QRect {}; }

  QVariant checkStateData = model()->data(index, Qt::CheckStateRole);
  if (!checkStateData.isValid()) [[unlikely]] { return QRect {}; }

  QRect cellRect = visualRect(index);
  if (!cellRect.isValid()) [[unlikely]] { return QRect {}; }

  NXTableViewStyle *style = d->_tableViewStyle;
  int padding             = style->getHorizontalPadding(column);

  QStyleOptionViewItem option;
  option.initFrom(this);
  option.rect       = cellRect;
  option.index      = index;
  option.features   = QStyleOptionViewItem::HasCheckIndicator;
  option.checkState = checkStateData.value<Qt::CheckState>();

  QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &option, this);
  if (checkRect.isNull()) return QRect();

  // 应用水平内边距（与 NXTableViewStyle::drawControl 中 CE_ItemViewItem 分支一致）
  checkRect.adjust(padding, 0, padding, 0);

  QPoint offset = viewport()->mapTo(this, QPoint(0, 0));
  checkRect.translate(offset);
  return checkRect;
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
    const QModelIndex& currentIndex  = indexAt(event->pos());
    const QModelIndex& oldHoverIndex = d->_tableViewStyle->getCurrentHoverIndex();
    d->_tableViewStyle->setCurrentHoverIndex(currentIndex);
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
