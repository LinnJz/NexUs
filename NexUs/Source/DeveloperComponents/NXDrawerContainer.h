#ifndef NXFRAMEWORK_NEXUS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_
#define NXFRAMEWORK_NEXUS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_

#include <QWidget>

#include <QVBoxLayout>
#include "NXDef.h"

#include <QGraphicsOpacityEffect>

class NXDrawerContainer : public QWidget
{
  Q_OBJECT
  Q_PROPERTY_CREATE(qreal, Opacity)
  Q_PRIVATE_CREATE(QPixmap, ContainerPix)
  Q_PRIVATE_CREATE(int, BorderRadius)
public:
  explicit NXDrawerContainer(QWidget *parent = nullptr);
  ~NXDrawerContainer() override;

  void addWidget(QWidget *widget);
  void removeWidget(QWidget *widget);

  void doDrawerAnimation(bool isExpand);

protected:
  virtual void paintEvent(QPaintEvent *event) override;

private:
  NXThemeType::ThemeMode _themeMode;
  bool _isShowBorder { true };

  QVBoxLayout *_mainLayout { nullptr };
  QVBoxLayout *_containerLayout { nullptr };
  QWidget *_containerWidget { nullptr };
  QList<QWidget *> _drawerWidgetList;

  int _calculateContainerMinimumHeight() const;
};

#endif // NXFRAMEWORK_NEXUS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_
