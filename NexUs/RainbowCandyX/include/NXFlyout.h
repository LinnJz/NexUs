#ifndef NXFLYOUT_H
#define NXFLYOUT_H

#include <QWidget>

#include "NXProperty.h"
class NXFlyoutPrivate;
class NX_EXPORT NXFlyout : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXFlyout)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(bool, IsLightDismiss)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Content)

  public:
    explicit NXFlyout(QWidget* parent = nullptr);
    ~NXFlyout() override;

    void setContentWidget(QWidget *widget) noexcept;
    void showFlyout(QWidget *target) noexcept;
    void closeFlyout() noexcept;

Q_SIGNALS:
    void closed();

protected:
  void paintEvent(QPaintEvent* event) override;
};

#endif // NXFLYOUT_H
