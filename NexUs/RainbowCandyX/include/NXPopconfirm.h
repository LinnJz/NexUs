#ifndef NXPOPCONFIRM_H
#define NXPOPCONFIRM_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXPopconfirmPrivate;
class NX_EXPORT NXPopconfirm : public QWidget
{
    Q_OBJECT
  Q_Q_CREATE(NXPopconfirm)
  Q_PROPERTY_CREATE_H(bool, IsLightDismiss)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(NXIconType::IconName, Icon)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Content)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, ConfirmButtonText)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, CancelButtonText)

  public:
    explicit NXPopconfirm(QWidget* parent = nullptr);
    ~NXPopconfirm() override;

    void showPopconfirm(QWidget *target) noexcept;
    void closePopconfirm() noexcept;

Q_SIGNALS:
  void confirmed();
  void cancelled();
  void closed();

protected:
 void paintEvent(QPaintEvent* event) override;
};

#endif // NXPOPCONFIRM_H
