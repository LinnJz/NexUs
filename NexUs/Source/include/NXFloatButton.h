#ifndef NXFLOATBUTTON_H
#define NXFLOATBUTTON_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXFloatButtonPrivate;
class NXMenu;

class NX_EXPORT NXFloatButton : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXFloatButton)
  Q_PROPERTY_CREATE_H(int, ButtonSize)
  Q_PROPERTY_CREATE_H(int, Margin)

public:
  enum Position
  {
    BottomRight = 0,
    BottomLeft,
    TopRight,
    TopLeft
  };
  Q_ENUM(Position)

  explicit NXFloatButton(QWidget *parent = nullptr);
  explicit NXFloatButton(NXIconType::IconName icon, QWidget *parent = nullptr);
  explicit NXFloatButton(NXIconType::IconName icon, Position position, QWidget *parent = nullptr);
  ~NXFloatButton();

  void setIcon(NXIconType::IconName icon);
  NXIconType::IconName getIcon() const;

  void setPosition(Position position);
  Position getPosition() const;

  void setMenu(NXMenu *menu);
  NXMenu *getMenu() const;
  Q_SIGNAL void clicked();

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  bool eventFilter(QObject *watched, QEvent *event) override;
  bool event(QEvent *event) override;
};

#endif // NXFLOATBUTTON_H
