#ifndef NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_
#define NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_

#include <QWidget>
#include "NXProperty.h"

class NXRollerPrivate;

class NX_EXPORT NXRoller : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXRoller)
  Q_PROPERTY_CREATE_Q_H(bool, IsContainer)
  Q_PROPERTY_CREATE_Q_H(bool, IsEnableLoop)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
  Q_PROPERTY_CREATE_Q_H(int, MaxVisibleItems)
  Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
  Q_PROPERTY_CREATE_Q_H(QStringList, ItemList)
public:
  explicit NXRoller(QWidget *parent = nullptr);
  ~NXRoller() override;

  void setCurrentData(const QString& data);
  QString getCurrentData() const;
Q_SIGNALS:
  Q_SIGNAL void currentDataChanged(const QString& data);

protected:
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void leaveEvent(QEvent *event);
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_
