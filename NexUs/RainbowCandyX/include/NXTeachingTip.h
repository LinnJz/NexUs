#ifndef NXTEACHINGTIP_H
#define NXTEACHINGTIP_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXTeachingTipPrivate;
class NX_EXPORT NXTeachingTip : public QWidget
{
    Q_OBJECT
  Q_Q_CREATE(NXTeachingTip)
  Q_PROPERTY_CREATE_H(bool, IsLightDismiss)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(NXIconType::IconName, TipIcon)
    Q_PROPERTY_CREATE_2_H(const QPixmap&, QPixmap, HeroImage)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, SubTitle)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Content)

  public:
    enum TailPosition
    {
        Auto = 0,
        Top,
        Bottom,
        Left,
        Right
    };
    Q_ENUM(TailPosition)

    explicit NXTeachingTip(QWidget* parent = nullptr);
    ~NXTeachingTip() override;

    void setTailPosition(TailPosition position) noexcept;
    TailPosition getTailPosition() const noexcept;

    void setTarget(QWidget *target) noexcept;
    QWidget *getTarget() const noexcept;

    void setCloseButtonVisible(bool visible) noexcept;

    void addAction(const QString& text, const std::function<void()>& callback) noexcept;
    void clearActions() noexcept;

    void showTip() noexcept;
    void closeTip() noexcept;

Q_SIGNALS:
  void closed();
  void closeButtonClicked();

protected:
  void paintEvent(QPaintEvent* event) override;
  bool eventFilter(QObject* watched, QEvent* event) override;
  void showEvent(QShowEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
};

#endif // NXTEACHINGTIP_H
