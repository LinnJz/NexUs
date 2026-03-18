#ifndef NXRATINGCONTROL_H
#define NXRATINGCONTROL_H

#include <QWidget>

#include "NXProperty.h"
class NXRatingControlPrivate;
class NX_EXPORT NXRatingControl : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXRatingControl)
    Q_PROPERTY_CREATE_H(int, MaxRating)
    Q_PROPERTY_CREATE_H(int, Rating)
    Q_PROPERTY_CREATE_H(int, StarSize)
    Q_PROPERTY_CREATE_H(int, Spacing)
    Q_PROPERTY_CREATE_H(bool, IsReadOnly)
public:
    explicit NXRatingControl(QWidget* parent = nullptr);
    ~NXRatingControl() override;

Q_SIGNALS:
  void ratingChanged(int rating);

protected:
  void paintEvent(QPaintEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void leaveEvent(QEvent* event) override;
  QSize sizeHint() const override;
};

#endif // NXRATINGCONTROL_H
