#ifndef NXRIBBONBAR_H
#define NXRIBBONBAR_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class QStackedWidget;
class NXRibbonGroup;
class NXRibbonTabBar;
class NXRibbonBarPrivate;

class NX_EXPORT NXRibbonBar : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXRibbonBar)
  Q_PROPERTY_CREATE_H(int, CurrentIndex)

public:
  explicit NXRibbonBar(QWidget *parent = nullptr);
  ~NXRibbonBar();

  void bindTabBar(NXRibbonTabBar *tabBar);
  NXRibbonTabBar *tabBar() const;

  QWidget *addTab(const QString &title);
  NXRibbonGroup *addGroup(QWidget *page, const QString &title);

  int tabCount() const;
  QString tabText(int index) const;

  void setCollapsed(bool collapsed);
  bool isCollapsed() const;

  void setPinned(bool pinned);
  bool isPinned() const;

  void setAnimationDuration(int durationMs);
  int getAnimationDuration() const;

  void showPinContextMenu(QPoint globalPos);
  Q_SIGNAL void tabClicked(int index);
  Q_SIGNAL void collapsedChanged(bool collapsed);
  Q_SIGNAL void pinnedChanged(bool pinned);

protected:
  void paintEvent(QPaintEvent *event) override;
  QSize sizeHint() const override;
  bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // NXRIBBONBAR_H
