#ifndef NXFRAMEWORK_NEXUS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_
#define NXFRAMEWORK_NEXUS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_

#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QWidget>

#include "NXDef.h"

class NXDrawerContainer : public QWidget
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QS_SET_CREF(QPixmap), ContainerPix)
  Q_PROPERTY_CREATE(qreal, Opacity)
  Q_PRIVATE_CREATE(int, BorderRadius)

public:
  explicit NXDrawerContainer(QWidget *parent = nullptr);
  ~NXDrawerContainer();

  void addWidget(QWidget *widget);
  void removeWidget(QWidget *widget);

  void doDrawerAnimation(bool isExpand);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  bool _isShowBorder { true };
  NXThemeType::ThemeMode _themeMode;
  QList<QWidget *> _drawerWidgetList;
  QVBoxLayout *_mainLayout { nullptr };
  QVBoxLayout *_containerLayout { nullptr };
  QWidget *_containerWidget { nullptr };
};

#endif //NXFRAMEWORK_NEXUS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_
