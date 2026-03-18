#ifndef NXNUMBERBOX_H
#define NXNUMBERBOX_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXNumberBoxPrivate;
class NX_EXPORT NXNumberBox : public QWidget
{
    Q_OBJECT
  Q_Q_CREATE(NXNumberBox)
  Q_PROPERTY_CREATE_H(bool, IsWrapping)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, Decimals)
    Q_PROPERTY_CREATE_H(double, Value)
    Q_PROPERTY_CREATE_H(double, Minimum)
    Q_PROPERTY_CREATE_H(double, Maximum)
    Q_PROPERTY_CREATE_H(double, Step)

  public:
    explicit NXNumberBox(QWidget* parent = nullptr);
    ~NXNumberBox() override;

    void stepUp() noexcept;
    void stepDown() noexcept;

Q_SIGNALS:
  void valueChanged(double value);

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void leaveEvent(QEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // NXNUMBERBOX_H
