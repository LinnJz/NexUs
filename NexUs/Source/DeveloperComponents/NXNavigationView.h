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
  NXToolTip *getCompactToolTip() const;
  void navigationNodeStateChange(QVariantMap data);
  void setNavigationNodeDragAndDropEnable(bool isEnable);
  Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);
Q_SIGNALS:
  Q_SIGNAL void navigationPositionSwapped(const QModelIndex& from, const QModelIndex& to);
  Q_SIGNAL void navigationClicked(const QModelIndex& index);
  Q_SIGNAL void navigationOpenNewWindow(const QString& nodeKey);
  Q_SIGNAL void navigationCloseCurrentWindow(const QString& nodeKey);

protected:
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void dragEnterEvent(QDragEnterEvent *event) override;
  virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
  virtual void dropEvent(QDropEvent *event) override;
  virtual void dragMoveEvent(QDragMoveEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
  virtual bool eventFilter(QObject *watched, QEvent *event) override;

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
