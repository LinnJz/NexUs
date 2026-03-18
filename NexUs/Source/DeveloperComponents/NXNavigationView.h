#ifndef NXNAVIGATIONVIEW_H
#define NXNAVIGATIONVIEW_H

#include <QObject>
#include <QTreeView>
#include "NXDef.h"
#include "private/NXNavigationBarPrivate.h"
class NXScrollBar;
class NXNavigationStyle;
class NXToolTip;
class NXNavigationModel;

class NXNavigationView : public QTreeView
{
  Q_OBJECT
  Q_PROPERTY_CREATE(bool, IsLeftButtonPressedToggleNavigation)
  Q_PRIVATE_CREATE(const NXNavigationBarPrivate *, NavigationBarPrivate)

public:
  explicit NXNavigationView(QWidget *parent = nullptr);
  ~NXNavigationView() override;
  NXToolTip *getCompactToolTip() const noexcept;
  void navigationNodeStateChange(const QVariantMap& data) noexcept;
  void setNavigationNodeDragAndDropEnable(bool isEnable) noexcept;
  Q_SLOT void onCustomContextMenuRequested(const QPoint& pos) noexcept;

Q_SIGNALS:
  void navigationPositionSwapped(const QModelIndex& from, const QModelIndex& to);
  void navigationClicked(const QModelIndex& index);
  void navigationOpenNewWindow(const QString& nodeKey);
  void navigationCloseCurrentWindow(const QString& nodeKey);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  NXNavigationStyle *_navigationStyle { nullptr };
  NXToolTip *_compactToolTip { nullptr };
  bool _canProceedWithDragDrop(QAbstractItemView::DropIndicatorPosition dropIndicatorPosition,
                               const QModelIndex& draggedIndex,
                               const QModelIndex& targetIndex,
                               const QModelIndex& draggedPreviousIndex,
                               const QModelIndex& draggedNextIndex,
                               const QModelIndex& targetParentIndex,
                               NXNavigationModel *model);
  QAbstractItemView::DropIndicatorPosition _dropIndicatorPosition(const QModelIndex& dropTargetIndex) const;
  void _doCompactToolTip();
};

#endif // NXNAVIGATIONVIEW_H
