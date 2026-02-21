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
  Q_SIGNAL void mousePress(const QModelIndex& index);
  Q_SIGNAL void mouseRelease(const QModelIndex& index);
  Q_SIGNAL void mouseDoubleClick(const QModelIndex& index);

protected:
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // NXBASELISTVIEW_H
