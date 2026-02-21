#ifndef NXSCROLLPAGE_H
#define NXSCROLLPAGE_H

#include <QWidget>

#include "NXDef.h"
class NXScrollArea;
class NXText;
class QHBoxLayout;
class NXScrollPagePrivate;

class NX_EXPORT NXScrollPage : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXScrollPage)
  Q_PROPERTY_CREATE_Q_H(QWidget *, CustomWidget)
public:
  explicit NXScrollPage(QWidget *parent = nullptr);
  ~NXScrollPage() override;

  void addCentralWidget(QWidget *centralWidget,
                        bool isWidgetResizeable              = true,
                        bool isVerticalGrabGesture           = true,
                        qreal mousePressEventDNXy            = 0.5,
                        Qt::ScrollBarPolicy vScrollBarPolicy = Qt::ScrollBarAsNeeded,
                        Qt::ScrollBarPolicy hScrollBarPolicy = Qt::ScrollBarAlwaysOff);

  void navigation(int widgetIndex, bool isLogRoute = true);

  void setPageTitleSpacing(int spacing);
  int getPageTitleSpacing() const;
  void setTitleVisible(bool isVisible);
};

#endif // NXSCROLLPAGE_H
