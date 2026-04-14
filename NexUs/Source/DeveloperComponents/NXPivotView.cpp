#include "NXPivotView.h"

#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QtGlobal>

#include "NXPivotStyle.h"
#include "NXScrollBar.h"

NXPivotView::NXPivotView(QWidget *parent)
    : QListView(parent)
{
  _pMarkX                 = 0;
  _pMarkWidth             = 40;
  _pPressIndex            = {};
  _pHoverIndex            = {};
  _pCommittedIndex        = {};
  _pHoverValidIndexCursor = Qt::PointingHandCursor;
  setObjectName("NXPivotView");
  setStyleSheet(QStringLiteral("#NXPivotView{background-color:transparent;}"));
  setMouseTracking(true);
  setVerticalScrollBar(new NXScrollBar(this));
  setHorizontalScrollBar(new NXScrollBar(this));
  setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

NXPivotView::~NXPivotView()
{
}

void
NXPivotView::doCurrentIndexChangedAnimation(const QModelIndex &index)
{
  if (!this->isVisible())
  {
    return;
  }
  NXPivotType::MarkFlags markFlags = _pPivotStyle->getMarkFlags();
  if (!(markFlags & (NXPivotType::MarkTopIndicator | NXPivotType::MarkBottomIndicator)))
  {
    return;
  }
  _isAnimationFinished = false;

  QPropertyAnimation *markAnimation = new QPropertyAnimation(this, "pMarkX");
  connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]()
  {
    _updateMarkRegion();
  });
  connect(markAnimation, &QPropertyAnimation::finished, this, [=]()
  {
    _isAnimationFinished = true;
    _updateMarkRegion();
  });
  markAnimation->setDuration(300);
  markAnimation->setEasingCurve(QEasingCurve::InOutSine);
  if (int centerX = visualRect(index).center().x(); _pCommittedIndex.row() >= 0)
  {
    markAnimation->setStartValue(_pMarkX);
    markAnimation->setEndValue(centerX - _pMarkWidth / 2);
  }
  else
  {
    markAnimation->setStartValue(centerX);
    markAnimation->setEndValue(centerX - _pMarkWidth / 2);

    QPropertyAnimation *markWidthAnimation = new QPropertyAnimation(this, "pMarkWidth");
    markWidthAnimation->setDuration(300);
    markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markWidthAnimation->setStartValue(0);
    markWidthAnimation->setEndValue(_pMarkWidth);
    markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  }
  _updateMarkRegion();
  markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void
NXPivotView::setPressIndexAndUpdate(const QModelIndex &index) noexcept
{
  if (_pPressIndex == index)
  {
    return;
  }
  const QModelIndex oldIndex = _pPressIndex;
  _pPressIndex               = index;
  _updateIndexPair(oldIndex, index);
}

void
NXPivotView::setCommittedIndexAndUpdate(const QModelIndex &index) noexcept
{
  if (_pCommittedIndex == index)
  {
    return;
  }
  const QModelIndex oldIndex = _pCommittedIndex;
  _pCommittedIndex           = index;
  _updateIndexPair(oldIndex, index);
}

void
NXPivotView::refreshHoverState() noexcept
{
  const QPoint viewportPos         = viewport()->mapFromGlobal(QCursor::pos());
  const QModelIndex nextHoverIndex = viewport()->rect().contains(viewportPos) ? indexAt(viewportPos) : QModelIndex {};
  _setHoverIndexInternal(nextHoverIndex);
  _syncHoverCursor();
}

void
NXPivotView::_updateMarkRegion() noexcept
{
  const QRect viewportRect = viewport()->rect();
  const int markBandHeight = 12;
  viewport()->update(QRect(viewportRect.left(), qMax(viewportRect.top(), viewportRect.bottom() - markBandHeight),
                           viewportRect.width(), markBandHeight + 1));

  viewport()->update(QRect(viewportRect.left(), viewportRect.top(), viewportRect.width(), markBandHeight + 1));
}

void
NXPivotView::mouseDoubleClickEvent(QMouseEvent *event)
{
  setPressIndexAndUpdate(indexAt(event->pos()));
  QListView::mouseDoubleClickEvent(event);
}

void
NXPivotView::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    setPressIndexAndUpdate(QModelIndex {});
  }
  QListView::mouseReleaseEvent(event);
}

void
NXPivotView::mouseMoveEvent(QMouseEvent *event)
{
  QListView::mouseMoveEvent(event);
  _setHoverIndexInternal(indexAt(event->pos()));
  _syncHoverCursor();
}

void
NXPivotView::leaveEvent(QEvent *event)
{
  _setHoverIndexInternal(QModelIndex {});
  _syncHoverCursor();
  QListView::leaveEvent(event);
}

void
NXPivotView::wheelEvent(QWheelEvent *event)
{
  QListView::wheelEvent(event);
  event->accept();
}

void
NXPivotView::paintEvent(QPaintEvent *event)
{
  QListView::paintEvent(event);
  if (_pCommittedIndex.isValid())
  {
    NXPivotType::MarkFlags markFlags = _pPivotStyle->getMarkFlags();
    if (!(markFlags & (NXPivotType::MarkTopIndicator | NXPivotType::MarkBottomIndicator)))
    {
      return;
    }
    QWidget *widget    = viewport();
    QRect viewPortRect = widget->rect();

    QPainter painter(widget);

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(_pPivotStyle->getMarkColor());

    QRect currentIndexRect = visualRect(_pCommittedIndex);
    qreal halfWidth        = _pMarkWidth / 2.0;
    qreal centerX          = _isAnimationFinished ? currentIndexRect.center().x() : _pMarkX + halfWidth;

    QPainterPath path;
    QPolygonF polygon;
    if (markFlags.testFlag(NXPivotType::MarkBottomIndicator))
    {
      QPointF A(centerX, viewPortRect.bottom() - 4.8);
      polygon = { A,                            // 顶部中心
                  A + QPointF(2.0, 2.8),        // 右侧内点
                  A + QPointF(halfWidth, 5.0),  // 最右侧点
                  A + QPointF(2.0, 5.0),        // 右侧底部内点
                  A + QPointF(0.0, 2.0),        // 底部中心尖点
                  A + QPointF(-2.0, 5.0),       // 左侧底部内点（对称补全）
                  A + QPointF(-halfWidth, 5.0), // 最左侧点
                  A + QPointF(-2.0, 2.8) };     // 左侧内点
      path.addPolygon(polygon);
    }
    if (markFlags.testFlag(NXPivotType::MarkTopIndicator))
    {
      QPointF A_Mirror(centerX, viewPortRect.top() + 5.2);
      polygon = { A_Mirror,                             // 底部中心（原顶部中心）
                  A_Mirror + QPointF(2.0, -2.8),        // 右侧内点（Y取反）
                  A_Mirror + QPointF(halfWidth, -5.0),  // 最右侧点（Y取反）
                  A_Mirror + QPointF(2.0, -5.0),        // 右侧顶部内点（Y取反）
                  A_Mirror + QPointF(0.0, -2.0),        // 顶部中心尖点（Y取反）
                  A_Mirror + QPointF(-2.0, -5.0),       // 左侧顶部内点（Y取反，对称）
                  A_Mirror + QPointF(-halfWidth, -5.0), // 最左侧点（Y取反，对称）
                  A_Mirror + QPointF(-2.0, -2.8) };     // 左侧内点（Y取反，对称）
      path.addPolygon(polygon);
    }
    painter.drawPath(path);
    painter.restore();
  }
}

void
NXPivotView::_setHoverIndexInternal(const QModelIndex &index) noexcept
{
  if (_pHoverIndex == index)
  {
    return;
  }
  const QModelIndex oldIndex = _pHoverIndex;
  _pHoverIndex               = index;
  _updateIndexPair(oldIndex, index);
}

void
NXPivotView::_updateIndexPair(const QModelIndex &first, const QModelIndex &second) noexcept
{
  _updateIndexRect(first);
  if (second != first)
  {
    _updateIndexRect(second);
  }
}

void
NXPivotView::_updateIndexRect(const QModelIndex &index) noexcept
{
  if (!index.isValid())
  {
    return;
  }
  const QRect itemRect = visualRect(index);
  if (!itemRect.isValid())
  {
    return;
  }
  viewport()->update(itemRect.adjusted(-2, -2, 2, 2));
}

void
NXPivotView::_syncHoverCursor() noexcept
{
  if (_pHoverIndex.isValid())
  {
    viewport()->setCursor(_pHoverValidIndexCursor);
  }
  else
  {
    viewport()->unsetCursor();
  }
}
