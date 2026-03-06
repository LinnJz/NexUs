#ifndef NXTABWIDGET_H
#define NXTABWIDGET_H

#include <QTabWidget>

#include "NXDef.h"

class NXTabWidgetPrivate;

class NX_EXPORT NXTabWidget : public QTabWidget
{
  friend class NXCustomTabWidget;

  Q_OBJECT
  Q_Q_CREATE(NXTabWidget)
  Q_PROPERTY_CREATE_Q_H(bool, IsSelectedIndicatorVisible)
  Q_PROPERTY_CREATE_Q_H(bool, IsTabTransparent);
  Q_PROPERTY_CREATE_Q_H(bool, IsContainerAcceptDrops);
  Q_PROPERTY_CREATE_Q_H(int, TabCornerRadius)
  Q_PROPERTY_CREATE_Q_H(NXTabBarType::TabBarStyle, TabBarStyle)
  Q_PROPERTY_CREATE_Q_H(QSize, TabSize)

public:
  explicit NXTabWidget(QWidget *parent = nullptr);
  ~NXTabWidget() override;
  void setTabPosition(TabPosition position);

protected:
  virtual void paintEvent(QPaintEvent *event) override;
  virtual void dragEnterEvent(QDragEnterEvent *event) override;
  virtual void dropEvent(QDropEvent *event) override;
  virtual void tabInserted(int index);
};

#endif // NXTABWIDGET_H
