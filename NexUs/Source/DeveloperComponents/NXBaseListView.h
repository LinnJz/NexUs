#ifndef NXBASELISTVIEW_H
#define NXBASELISTVIEW_H

#include <QListView>
#include <QModelIndex>

class NXScrollBar;

class NXBaseListView : public QListView
{
  Q_OBJECT

public:
  explicit NXBaseListView(QWidget *parent = nullptr);
  ~NXBaseListView();
Q_SIGNALS:
  void mousePress(const QModelIndex& index);
  void mouseRelease(const QModelIndex& index);
  void mouseDoubleClick(const QModelIndex& index);

protected:
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // NXBASELISTVIEW_H
