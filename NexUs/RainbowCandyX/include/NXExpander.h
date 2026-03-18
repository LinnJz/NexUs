#ifndef NXEXPANDER_H
#define NXEXPANDER_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXExpanderPrivate;
class NX_EXPORT NXExpander : public QWidget
{
    Q_OBJECT
  Q_Q_CREATE(NXExpander)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, AnimationDuration)
  Q_PROPERTY_CREATE_H(NXIconType::IconName, HeaderIcon)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, SubTitle)

  public:
    enum ExpandDirection
    {
        Down = 0,
        Up
    };
    Q_ENUM(ExpandDirection)

    explicit NXExpander(QWidget* parent = nullptr);
    explicit NXExpander(const QString& title, QWidget* parent = nullptr);
    ~NXExpander() override;

    void setExpandDirection(ExpandDirection direction) noexcept;
    ExpandDirection getExpandDirection() const noexcept;

    void setContentWidget(QWidget *widget) noexcept;
    QWidget *getContentWidget() const noexcept;

    void setHeaderWidget(QWidget *widget) noexcept;

    void setIsExpanded(bool expanded) noexcept;
    bool getIsExpanded() const noexcept;

Q_SIGNALS:
  void expandStateChanged(bool expanded);

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  bool event(QEvent* event) override;
};

#endif // NXEXPANDER_H
