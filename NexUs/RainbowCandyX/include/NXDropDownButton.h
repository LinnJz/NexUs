#ifndef NXDROPDOWNBUTTON_H
#define NXDROPDOWNBUTTON_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXMenu;
class NXDropDownButtonPrivate;
class NX_EXPORT NXDropDownButton : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXDropDownButton)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(NXIconType::IconName, NXIcon)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Text)

  public:
    explicit NXDropDownButton(QWidget* parent = nullptr);
    ~NXDropDownButton() override;

    void setMenu(NXMenu* menu) noexcept;
    NXMenu *getMenu() const noexcept;

protected:
    bool event(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
};

#endif // NXDROPDOWNBUTTON_H
