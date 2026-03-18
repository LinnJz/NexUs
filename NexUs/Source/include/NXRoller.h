#ifndef NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_
#define NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_

#include <QWidget>
#include "NXProperty.h"

class NXRollerPrivate;

class NX_EXPORT NXRoller : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXRoller)
  Q_PROPERTY_CREATE_H(bool, IsContainer)
  Q_PROPERTY_CREATE_H(bool, IsEnableLoop)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, ItemHeight)
  Q_PROPERTY_CREATE_H(int, MaxVisibleItems)
  Q_PROPERTY_CREATE_H(int, CurrentIndex)
  Q_PROPERTY_CREATE_2_H(const QStringList&, QStringList, ItemList)

public:
  explicit NXRoller(QWidget *parent = nullptr);
  ~NXRoller() override;

  void setCurrentData(const QString& data) noexcept;
  QString getCurrentData() const noexcept;

Q_SIGNALS:
  void currentDataChanged(const QString& data);

protected:
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event);
  void leaveEvent(QEvent *event);
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_
