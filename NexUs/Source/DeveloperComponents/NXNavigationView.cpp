#include "NXNavigationView.h"

#include <QDebug>
#include <QHeaderView>
#include <QMimeData>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>

#include "NXMenu.h"
#include "NXNavigationModel.h"
#include "NXNavigationNode.h"
#include "NXNavigationStyle.h"
#include "NXScrollBar.h"
#include "NXTheme.h"
#include "NXToolTip.h"
QColor gIndicatorColor = NXThemeColor(nxTheme->getThemeMode(), PrimaryNormal);

NXNavigationView::NXNavigationView(QWidget *parent)
    : QTreeView(parent)
    , _pIsLeftButtonPressedToggleNavigation(false)
{
  setObjectName("NXNavigationView");
  setStyleSheet("#NXNavigationView{background-color:transparent;}");
  setAnimated(true);
  setHeaderHidden(true);
  setRootIsDecorated(false);
  setExpandsOnDoubleClick(false);
  setAutoScroll(false);
  setMouseTracking(true);
  setSelectionMode(QAbstractItemView::NoSelection);

  // 滚动条设置
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  NXScrollBar *vScrollBar = new NXScrollBar(this);
  connect(vScrollBar, &NXScrollBar::rangeAnimationFinished, this, [=]() { doItemsLayout(); });
  setVerticalScrollBar(vScrollBar);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  NXScrollBar *floatVScrollBar = new NXScrollBar(vScrollBar, this);
  floatVScrollBar->setIsAnimation(true);
  floatVScrollBar->installEventFilter(this);

  _navigationStyle = new NXNavigationStyle(this->style());
  _navigationStyle->setNavigationView(this);
  setStyle(_navigationStyle);

  QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
  QScroller *scroller            = QScroller::scroller(this->viewport());
  QScrollerProperties properties = scroller->scrollerProperties();
  properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
  properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
  properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
  properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
  properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
  properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
  scroller->setScrollerProperties(properties);

  connect(scroller,
          &QScroller::stateChanged,
          this,
          [=](QScroller::State newstate)
  {
    if (newstate == QScroller::Pressed)
    {
      _navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
      viewport()->update();
    }
    else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
    {
      _navigationStyle->setPressIndex(QModelIndex());
    }
  });

  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &NXNavigationView::customContextMenuRequested, this, &NXNavigationView::onCustomContextMenuRequested);
}

NXNavigationView::~NXNavigationView() { }

NXToolTip *NXNavigationView::getCompactToolTip() const { return _compactToolTip; }

void NXNavigationView::navigationNodeStateChange(QVariantMap data)
{
  this->_navigationStyle->navigationNodeStateChange(data);
}

void NXNavigationView::setNavigationNodeDragAndDropEnable(bool isEnable)
{
  setDragEnabled(isEnable);
  setAcceptDrops(isEnable);
  setDropIndicatorShown(isEnable);
  setDragDropMode(isEnable ? QAbstractItemView::InternalMove : QAbstractItemView::NoDragDrop);
}

QAbstractItemView::DropIndicatorPosition
NXNavigationView::_dropIndicatorPosition(const QModelIndex& dropTargetIndex) const
{
  if (!dropTargetIndex.isValid()) return QAbstractItemView::OnViewport;

  const QRect& itemRect   = visualRect(dropTargetIndex);
  const QPoint& cursorPos = mapFromGlobal(QCursor::pos());

  if (cursorPos.y() - 9 < itemRect.top()) { return QAbstractItemView::AboveItem; }
  else if (cursorPos.y() + 10 > itemRect.bottom()) { return QAbstractItemView::BelowItem; }
  else
  {
    return QAbstractItemView::OnItem;
  }
}

void NXNavigationView::onCustomContextMenuRequested(const QPoint& pos)
{
  if (!_pNavigationBarPrivate->_pIsAllowPageOpenInNewWindow) { return; }
  QModelIndex posIndex = indexAt(pos);
  if (!posIndex.isValid()) { return; }
  NXNavigationNode *posNode = static_cast<NXNavigationNode *>(posIndex.internalPointer());
  if (!posNode->getIsExpanderNode() && !posNode->getIsCategoryNode())
  {
    NXMenu menu;
    menu.setMenuItemHeight(27);
    QAction *openAction = menu.addNXIconAction(NXIconType::ObjectGroup, "在新窗口中打开");
    connect(openAction, &QAction::triggered, this, [=]() { Q_EMIT navigationOpenNewWindow(posNode->getNodeKey()); });
    QAction *closeAction = menu.addNXIconAction(NXIconType::FilmSlash, "关闭当前导航窗口");
    connect(
        closeAction, &QAction::triggered, this, [=]() { Q_EMIT navigationCloseCurrentWindow(posNode->getNodeKey()); });
    menu.exec(mapToGlobal(pos));
  }
}

void NXNavigationView::mousePressEvent(QMouseEvent *event)
{
  QTreeView::mousePressEvent(event);
  if (event->button() == Qt::LeftButton && _pIsLeftButtonPressedToggleNavigation)
  {
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) { Q_EMIT navigationClicked(index); }
    _navigationStyle->setPressIndex(QModelIndex());
  }
}

void NXNavigationView::mouseMoveEvent(QMouseEvent *event)
{
  _doCompactToolTip();
  QTreeView::mouseMoveEvent(event);
}

void NXNavigationView::mouseDoubleClickEvent(QMouseEvent *event)
{
  _navigationStyle->setPressIndex(indexAt(event->pos()));
  viewport()->update();
  QTreeView::mouseDoubleClickEvent(event);
}

void NXNavigationView::mouseReleaseEvent(QMouseEvent *event)
{
  QTreeView::mouseReleaseEvent(event);
  if (event->button() == Qt::LeftButton && !_pIsLeftButtonPressedToggleNavigation)
  {
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) { Q_EMIT navigationClicked(index); }
    _navigationStyle->setPressIndex(QModelIndex());
  }
}

void NXNavigationView::dragEnterEvent(QDragEnterEvent *event)
{
  event->setAccepted(event->mimeData()->hasFormat("application/x-nxnavigation-node"));
}

void NXNavigationView::dragLeaveEvent(QDragLeaveEvent *event)
{
  NXNavigationModel *model = qobject_cast<NXNavigationModel *>(this->model());
  model->setProperty("NXDropTargetIndex", QModelIndex());
  model->setProperty("NXDropIndicatorPosition", DropIndicatorPosition::OnViewport);
  viewport()->update();
  event->accept();
}

void NXNavigationView::dragMoveEvent(QDragMoveEvent *event)
{
  // QTreeView::dragMoveEvent(event);
  //  触发PE_IndicatorItemViewItemDrop，dropIndicatorPosition()
  //  但是对于PageNode来说，不会触发QStyle::PE_IndicatorItemViewItemDrop，
  //  且当前点击拖拽的PageNode，触发dropIndicatorPosition()Above和Below时效果不好
  //  所以这里需要重写dragMoveEvent，自定义dropIndicatorPosition方法和绘制指示器
  NXNavigationModel *model = qobject_cast<NXNavigationModel *>(this->model());
  if (!event->mimeData()->hasFormat("application/x-nxnavigation-node"))
  {
    event->ignore();
    return;
  }
  const QModelIndex& targetIndex          = indexAt(event->position().toPoint());
  const QModelIndex& targetParentIndex    = targetIndex.parent();
  DropIndicatorPosition dropindicationPos = _dropIndicatorPosition(targetIndex);
  model->setProperty("NXDropTargetIndex", targetIndex);
  model->setProperty("NXDropIndicatorPosition", dropindicationPos);

  NXNavigationNode *draggedNode           = model->getSelectedNode();
  const QModelIndex& draggedIndex         = draggedNode->getModelIndex();
  const QModelIndex& draggedParentIndex   = draggedIndex.parent();
  const QModelIndex& draggedPreviousIndex = model->index(draggedIndex.row() - 1, 0, draggedParentIndex);
  const QModelIndex& draggedNextIndex     = model->index(draggedIndex.row() + 1, 0, draggedParentIndex);
  bool canProceedWithDragDrop             = _canProceedWithDragDrop(
      dropindicationPos, draggedIndex, targetIndex, draggedPreviousIndex, draggedNextIndex, targetParentIndex, model);
  model->setProperty("NXCanProceedWithDragDrop", canProceedWithDragDrop);
  if (canProceedWithDragDrop)
  {
    if (dropindicationPos == QAbstractItemView::OnItem)
    {
      gIndicatorColor = static_cast<NXNavigationNode *>(targetIndex.internalPointer())->getIsExpanderNode()
                            ? NXThemeColor(nxTheme->getThemeMode(), BasicText)
                            : (nxTheme->getThemeMode() == NXThemeType::ThemeMode::Light ? QColor(0x7D, 0x52, 0x84)
                                                                                        : QColor(0x66, 0x3D, 0x74));
    }
    else
    {
      gIndicatorColor = NXThemeColor(nxTheme->getThemeMode(), PrimaryNormal);
    }
    // QColor(0x7D, 0x52, 0x84)、QColor(0x42, 0x22, 0x56)、QColor(0x9B, 0x8E, 0xA9)、QColor(0x7E, 0x52,
    // 0x7F)、QColor(0x66, 0x3D, 0x74)
  }
  else
  {
    gIndicatorColor = NXThemeColor(nxTheme->getThemeMode(), BasicText);
  }
  event->accept();
  viewport()->update();
}

void NXNavigationView::dropEvent(QDropEvent *event)
{
  NXNavigationModel *model = qobject_cast<NXNavigationModel *>(this->model());
  if (!model->property("NXCanProceedWithDragDrop").toBool())
  {
    event->ignore();
    return;
  }
  DropIndicatorPosition dropindicationPos = model->property("NXDropIndicatorPosition").value<DropIndicatorPosition>();
  const QModelIndex& targetIndex          = model->property("NXDropTargetIndex").value<QModelIndex>();
  NXNavigationNode *targetNode            = static_cast<NXNavigationNode *>(targetIndex.internalPointer());

  NXNavigationNode *draggedNode  = model->getSelectedNode();
  const QModelIndex draggedIndex = draggedNode->getModelIndex();
  if (dropindicationPos == QAbstractItemView::OnItem)
  {
    if (targetNode->getIsExpanderNode())
    {
      model->setProperty("NXDropTargetIndex", QModelIndex());
      model->setProperty("NXDropIndicatorPosition", DropIndicatorPosition::OnViewport);
      event->ignore();
      return;
    }
    draggedNode->swapVisual(targetNode);
    model->swapNodes(draggedNode->getNodeKey(), targetNode->getNodeKey());
    Q_EMIT navigationPositionSwapped(draggedIndex, targetIndex);
    _navigationStyle->setPressIndex(QModelIndex());
  }
  else
  {
    model->dropMimeData(event->mimeData(), Qt::MoveAction, targetIndex.row(), 0, targetIndex.parent());
  }
  model->setProperty("NXDropTargetIndex", QModelIndex());
  model->setProperty("NXDropIndicatorPosition", DropIndicatorPosition::OnViewport);
  event->acceptProposedAction();
}

void NXNavigationView::paintEvent(QPaintEvent *event)
{
  QTreeView::paintEvent(event);
  NXNavigationModel *model           = qobject_cast<NXNavigationModel *>(this->model());
  const QModelIndex& dropTargetIndex = model->property("NXDropTargetIndex").value<QModelIndex>();
  if (!dropTargetIndex.isValid()) return;
  QStyleOptionViewItem option;
  option.initFrom(this);
  option.rect = visualRect(dropTargetIndex);

  QPainter painter(viewport());
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(QPen(gIndicatorColor, 2));

  switch (model->property("NXDropIndicatorPosition").value<DropIndicatorPosition>())
  {
    case QAbstractItemView::AboveItem :
    {
      int startY = option.rect.top() + 1;
      int startX = option.rect.left() + 6;
      painter.drawEllipse(QPoint(startX, startY), 4, 4);
      painter.drawLine(QPoint(startX, startY), QPoint(option.rect.width() + 6, startY));
      break;
    }
    case QAbstractItemView::BelowItem :
    {
      int startY = option.rect.bottom();
      int startX = option.rect.left() + 6;
      painter.drawEllipse(QPoint(startX, startY), 4, 4);
      painter.drawLine(QPoint(startX, startY), QPoint(option.rect.width() + 6, startY));
      break;
    }
    case QAbstractItemView::OnItem :
    {
      painter.drawRect(option.rect.adjusted(1, 1, -1, -1));
      break;
    }
    default : break;
  }
}

bool NXNavigationView::eventFilter(QObject *watched, QEvent *event)
{
  switch (event->type())
  {
    case QEvent::MouseMove :
    case QEvent::HoverMove :
    {
      _doCompactToolTip();
      break;
    }
    default :
    {
      break;
    }
  }
  return QAbstractItemView::eventFilter(watched, event);
}

bool NXNavigationView::_canProceedWithDragDrop(QAbstractItemView::DropIndicatorPosition dropIndicatorPosition,
                                               const QModelIndex& draggedIndex,
                                               const QModelIndex& targetIndex,
                                               const QModelIndex& draggedPreviousIndex,
                                               const QModelIndex& draggedNextIndex,
                                               const QModelIndex& targetParentIndex,
                                               NXNavigationModel *model)
{
  return !(draggedIndex == targetIndex ||
           (draggedPreviousIndex.isValid() && draggedPreviousIndex == targetIndex &&
            dropIndicatorPosition == QAbstractItemView::BelowItem) ||
           (draggedNextIndex.isValid() && draggedNextIndex == targetIndex &&
            dropIndicatorPosition == QAbstractItemView::AboveItem) ||
           ((draggedIndex.row() == model->rowCount(targetParentIndex) - 1) && !targetIndex.isValid()) ||
           (targetIndex == draggedIndex.parent() && !model->getSelectedNode()->getParentNode()->getIsRootNode()));
}

void NXNavigationView::_doCompactToolTip()
{
  if (_pNavigationBarPrivate->_currentDisplayMode == NXNavigationType::NavigationDisplayMode::Compact)
  {
    if (!_compactToolTip) { _compactToolTip = new NXToolTip(this); }
    const QPoint& cursorPos     = mapFromGlobal(QCursor::pos());
    const QModelIndex& posIndex = indexAt(cursorPos);
    if (!posIndex.isValid())
    {
      _compactToolTip->hide();
      return;
    }
    NXNavigationNode *posNode = static_cast<NXNavigationNode *>(posIndex.internalPointer());
    _compactToolTip->setToolTip(posNode->getNodeTitle());
    _compactToolTip->setOffSetX(10);
    _compactToolTip->updatePos();
    _compactToolTip->show();
  }
  else
  {
    if (_compactToolTip)
    {
      _compactToolTip->deleteLater();
      _compactToolTip = nullptr;
    }
  }
}
