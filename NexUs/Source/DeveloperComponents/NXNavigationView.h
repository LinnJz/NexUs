#ifndef NXNAVIGATIONVIEW_H
#define NXNAVIGATIONVIEW_H

#include <QObject>
#include <QTreeView>

#include "NXDef.h"
#include "private/NXNavigationBarPrivate.h"
class NXScrollBar;
class NXNavigationStyle;
class NXToolTip;

class NXNavigationView : public QTreeView
{
  Q_OBJECT
  Q_PRIVATE_CREATE(const NXNavigationBarPrivate *, NavigationBarPrivate)

public:
  explicit NXNavigationView(QWidget *parent = nullptr);
  ~NXNavigationView();
  NXToolTip *getCompactToolTip() const;
  void setIsDragDropEnable(bool enable);
  void navigationNodeStateChange(const QVariantMap &data);

  Q_SLOT void onCustomContextMenuRequested(QPoint pos);
  Q_SIGNAL void navigationClicked(const QModelIndex &index);
  Q_SIGNAL void navigationOpenNewWindow(const QString &nodeKey);
  Q_SIGNAL void navigationCloseCurrentWindow(const QString &nodeKey);

protected:
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  NXNavigationStyle *_navigationStyle { nullptr };
  NXToolTip *_compactToolTip { nullptr };
  void _doCompactToolTip();
};

#endif // NXNAVIGATIONVIEW_H
