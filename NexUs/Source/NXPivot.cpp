#include "NXPivot.h"

#include <QCursor>
#include <QPainter>
#include <QScroller>
#include <QVBoxLayout>
#include <QtGlobal>

#include "DeveloperComponents/NXPivotModel.h"
#include "DeveloperComponents/NXPivotStyle.h"
#include "DeveloperComponents/NXPivotView.h"
#include "private/NXPivotPrivate.h"

namespace
{
Q_ALWAYS_INLINE int
NormalizeInsertIndex(const NXPivotModel *model, int index) noexcept
{
  return qBound(0, index, model->getPivotListCount());
}

Q_ALWAYS_INLINE bool
IsValidPivotIndex(const NXPivotModel *model, int index) noexcept
{
  return index >= 0 && index < model->getPivotListCount();
}
} // namespace

NXPivot::NXPivot(QWidget *parent)
    : QWidget { parent }
    , d_ptr(new NXPivotPrivate())
{
  Q_D(NXPivot);
  d->q_ptr = this;
  setFixedHeight(40);
  setObjectName("NXPivot");
  setStyleSheet(QStringLiteral("#NXPivot{background-color:transparent;}"));
  setMouseTracking(true);

  d->_listView = new NXPivotView(this);
  d->_listView->setMinimumHeight(0);
  d->_listView->setFlow(QListView::LeftToRight);
  d->_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  d->_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  d->_listModel = new NXPivotModel(this);
  d->_listView->setModel(d->_listModel);
  d->_listStyle = new NXPivotStyle(d->_listView->style());
  d->_listView->setStyle(d->_listStyle);
  d->_listView->setPivotStyle(d->_listStyle);

  QFont textFont = this->font();
  textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
  textFont.setPixelSize(20);
  d->_listView->setFont(textFont);

  QScroller::grabGesture(d->_listView->viewport(), QScroller::LeftMouseButtonGesture);
  QScroller *scroller            = QScroller::scroller(d->_listView->viewport());
  QScrollerProperties properties = scroller->scrollerProperties();
  properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
  properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
  properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
  properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
  properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
  scroller->setScrollerProperties(properties);

  connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate)
  {
    if (newstate == QScroller::Pressed)
    {
      d->_listView->setPressIndexAndUpdate(d->_listView->indexAt(d->_listView->mapFromGlobal(QCursor::pos())));
    }
    else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
    {
      d->_listView->setPressIndexAndUpdate(QModelIndex());
    }
  });
  connect(d->_listView, &NXPivotView::clicked, this, [=](const QModelIndex &index)
  {
    if (!index.isValid())
    {
      return;
    }
    d->_listView->setCurrentIndex(index);
    if (index.row() != d->_listView->getCommittedIndex().row())
    {
      d->_listView->doCurrentIndexChangedAnimation(index);
      d->_listView->setCommittedIndexAndUpdate(index);
      Q_EMIT pCurrentIndexChanged();
    }
    Q_EMIT pivotClicked(index.row());
  });
  connect(d->_listView, &NXPivotView::doubleClicked, this, [=](const QModelIndex &index)
  {
    Q_EMIT pivotDoubleClicked(index.row());
  });
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->addWidget(d->_listView);
}

NXPivot::~NXPivot()
{
  Q_D(NXPivot);
  delete d->_listStyle;
}

void
NXPivot::appendPivot(const QPixmap &pixmap) noexcept
{
  appendPivot(QString {}, pixmap);
}

void
NXPivot::appendPivot(const QString &pivotTitle, const QPixmap &pixmap) noexcept
{
  Q_D(NXPivot);
  const int index = d->_listModel->getPivotListCount();
  d->_listModel->appendPivot(pivotTitle, pixmap);
  setCurrentIndex(index);
  Q_EMIT pivotInserted(index);
}

void
NXPivot::insertPivot(int index, const QPixmap &pixmap) noexcept
{
  insertPivot(index, QString {}, pixmap);
}

void
NXPivot::insertPivot(int index, const QString &pivotTitle, const QPixmap &pixmap) noexcept
{
  Q_D(NXPivot);
  const int insertIndex = NormalizeInsertIndex(d->_listModel, index);
  d->_listModel->insertPivot(insertIndex, pivotTitle, pixmap);
  setCurrentIndex(insertIndex);
  Q_EMIT pivotInserted(insertIndex);
}

void
NXPivot::removePivot(int index) noexcept
{
  Q_D(NXPivot);
  if (!IsValidPivotIndex(d->_listModel, index)) [[unlikely]]
  {
    return;
  }

  const int currentIndex = getCurrentIndex();
  d->_listModel->removePivot(index);
  const int count = d->_listModel->getPivotListCount();

  if (count == 0)
  {
    d->_listView->setCommittedIndexAndUpdate(QModelIndex {});
    if (d->_listView->currentIndex().isValid())
    {
      d->_listView->setCurrentIndex(QModelIndex());
    }
    Q_EMIT pivotRemoved(index);
    return;
  }

  int nextCurrentIndex = currentIndex;
  if (currentIndex < 0 || currentIndex == index)
  {
    nextCurrentIndex = qMin(index, count - 1);
  }
  else if (currentIndex > index)
  {
    nextCurrentIndex = currentIndex - 1;
  }
  setCurrentIndex(nextCurrentIndex);

  Q_EMIT pivotRemoved(index);
}

void
NXPivot::setPivot(int index, const QPixmap &pixmap) noexcept
{
  Q_D(NXPivot);
  if (!IsValidPivotIndex(d->_listModel, index)) [[unlikely]]
  {
    return;
  }
  d->_listModel->setPivot(index, pixmap);
  Q_EMIT pivotChanged(index);
}

void
NXPivot::setPivot(int index, const QString &pivotTitle, const QPixmap &pixmap) noexcept
{
  Q_D(NXPivot);
  if (!IsValidPivotIndex(d->_listModel, index)) [[unlikely]]
  {
    return;
  }
  d->_listModel->setPivot(index, pivotTitle, pixmap);
  Q_EMIT pivotChanged(index);
}

void
NXPivot::setTextPixelSize(int textPixelSize) noexcept
{
  Q_D(NXPivot);
  QFont textFont = this->font();
  textFont.setPixelSize(textPixelSize);
  d->_listView->setFont(textFont);
}

int
NXPivot::getTextPixelSize() const noexcept
{
  Q_D(const NXPivot);
  return d->_listView->font().pixelSize();
}

void
NXPivot::setHoverValidIndexCursor(Qt::CursorShape shape) noexcept
{
  Q_D(NXPivot);
  d->_listView->setHoverValidIndexCursor(shape);
  d->_listView->refreshHoverState();
}

Qt::CursorShape
NXPivot::getHoverValidIndexCursor() const noexcept
{
  Q_D(const NXPivot);
  return d->_listView->getHoverValidIndexCursor();
}

void
NXPivot::setCurrentIndex(int currentIndex) noexcept
{
  Q_D(NXPivot);
  if (currentIndex < 0 || currentIndex >= d->_listModel->getPivotListCount()) [[unlikely]]
  {
    return;
  }

  const QModelIndex index = d->_listModel->index(currentIndex, 0);
  if (!index.isValid()) [[unlikely]]
  {
    return;
  }

  d->_listView->setCurrentIndex(index);
  if (index.row() != d->_listView->getCommittedIndex().row())
  {
    d->_listView->doCurrentIndexChangedAnimation(index);
    d->_listView->setCommittedIndexAndUpdate(index);
    Q_EMIT pCurrentIndexChanged();
  }
}

int
NXPivot::getCurrentIndex() const noexcept
{
  Q_D(const NXPivot);
  return d->_listView->getCommittedIndex().row();
}

void
NXPivot::setMarkWidth(int markWidth) noexcept
{
  Q_D(NXPivot);
  d->_listView->setMarkWidth(markWidth);
  d->_listView->viewport()->update();
  Q_EMIT pMarkWidthChanged();
}

int
NXPivot::getMarkWidth() const noexcept
{
  Q_D(const NXPivot);
  return d->_listView->getMarkWidth();
}

void
NXPivot::setTextNormalColor(const QColor &color) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setTextNormalColor(color);
}

QColor
NXPivot::getTextNormalColor() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getTextNormalColor();
}

void
NXPivot::setTextPressedColor(const QColor &color) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setTextPressedColor(color);
}

QColor
NXPivot::getTextPressedColor() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getTextPressedColor();
}

void
NXPivot::setTextFocusColor(const QColor &color) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setTextFocusColor(color);
}

QColor
NXPivot::getTextFocusColor() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getTextFocusColor();
}

void
NXPivot::setMarkColor(const QColor &color) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setMarkColor(color);
}

QColor
NXPivot::getMarkColor() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getMarkColor();
}

void
NXPivot::setMarkBackgroundColor(const QColor &color) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setMarkBackgroundColor(color);

  const QModelIndex hoverIndex = d->_listView->getHoverIndex();
  if (hoverIndex.isValid())
  {
    const QRect hoverRect = d->_listView->visualRect(hoverIndex).adjusted(-2, -2, 2, 2);
    if (hoverRect.isValid())
    {
      d->_listView->viewport()->update(hoverRect);
    }
  }
}

QColor
NXPivot::getMarkBackgroundColor() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getMarkBackgroundColor();
}

void
NXPivot::setPivotHorizontalSpacing(int PivotHorizontalSpacing) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setPivotHorizontalSpacing(PivotHorizontalSpacing);
  d->_listView->doItemsLayout();
  Q_EMIT pPivotHorizontalSpacingChanged();
}

int
NXPivot::getPivotHorizontalSpacing() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getPivotHorizontalSpacing();
}

void
NXPivot::setPivotVerticalSpacing(int PivotVerticalSpacing) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setPivotVerticalSpacing(PivotVerticalSpacing);
  d->_listView->doItemsLayout();
  Q_EMIT pPivotVerticalSpacingChanged();
}

int
NXPivot::getPivotVerticalSpacing() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getPivotVerticalSpacing();
}

void
NXPivot::setTextIconSpacing(int textIconSpacing) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setTextIconSpacing(textIconSpacing);
  d->_listView->doItemsLayout();
  d->_listView->viewport()->update();
}

int
NXPivot::getTextIconSpacing() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getTextIconSpacing();
}

void
NXPivot::setDisplayMode(NXPivotType::DisplayMode DisplayMode) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setDisplayMode(DisplayMode);
  d->_listView->doItemsLayout();
  d->_listView->viewport()->update();
}

NXPivotType::DisplayMode
NXPivot::getDisplayMode() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getDisplayMode();
}

void
NXPivot::setMarkFlags(NXPivotType::MarkFlags markFlags) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setMarkFlags(markFlags);
  d->_listView->doItemsLayout();
  d->_listView->viewport()->update();
}

NXPivotType::MarkFlags
NXPivot::getMarkFlags() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getMarkFlags();
}

void
NXPivot::setOverlayTextOffsetFromCenterAnchor(QPoint OverlayTextOffsetFromCenterAnchor) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setOverlayTextOffsetFromCenterAnchor(OverlayTextOffsetFromCenterAnchor);
  d->_listView->viewport()->update();
}

QPoint
NXPivot::getOverlayTextOffsetFromCenterAnchor() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getOverlayTextOffsetFromCenterAnchor();
}

void
NXPivot::setPivotFixedSize(QSize pivotFixedSize) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setPivotFixedSize(pivotFixedSize);
  d->_listView->viewport()->update();
}

QSize
NXPivot::getPivotFixedSize() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getPivotFixedSize();
}

void
NXPivot::setIsPivotFixedSize(bool isFixedSize) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setIsPivotFixedSize(isFixedSize);
  d->_listView->viewport()->update();
}

bool
NXPivot::getIsPivotFixedSize() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getIsPivotFixedSize();
}

void
NXPivot::setIsHoverBackgroundEnabled(bool enabled) noexcept
{
  Q_D(NXPivot);
  d->_listStyle->setIsHoverBackgroundEnabled(enabled);
  d->_listView->viewport()->update();
}

bool
NXPivot::getIsHoverBackgroundEnabled() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getIsHoverBackgroundEnabled();
}

void
NXPivot::setIsAutoAdaptivePivotHeight(bool IsAutoAdaptivePivotHeight) noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->setIsAutoAdaptivePivotHeight(IsAutoAdaptivePivotHeight);
}

bool
NXPivot::getIsAutoAdaptivePivotHeight() const noexcept
{
  Q_D(const NXPivot);
  return d->_listStyle->getIsAutoAdaptivePivotHeight();
}
