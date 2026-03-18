#ifndef NXTEACHINGTIPPRIVATE_H
#define NXTEACHINGTIPPRIVATE_H

#include <QObject>
#include <functional>

#include "NXDef.h"
#include "NXTeachingTip.h"
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QPropertyAnimation;
class NXTeachingTipPrivate : public QObject
{
    Q_OBJECT
  Q_D_CREATE(NXTeachingTip)
  Q_PROPERTY_CREATE(qreal, Opacity)
  Q_PROPERTY_CREATE_D(QPixmap, HeroImage)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, SubTitle)
  Q_PROPERTY_CREATE_D(QString, Content)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(NXIconType::IconName, TipIcon)
  Q_PROPERTY_CREATE_D(bool, IsLightDismiss)

public:
    explicit NXTeachingTipPrivate(QObject* parent = nullptr);
    ~NXTeachingTipPrivate() override;

private:
    struct ActionItem
    {
        QString text;
        std::function<void()> callback;
    };

    bool _closeButtonVisible { true };
    int _shadowBorderWidth { 8 };
    int _tailSize { 8 };
    NXTeachingTip::TailPosition _tailPosition { NXTeachingTip::Auto };
    NXThemeType::ThemeMode _themeMode;

    QList<ActionItem> _actions;
    QRect _closeButtonRect;

    QWidget* _target{nullptr};

    QPoint _calculatePosition() const;
    NXTeachingTip::TailPosition _calculateAutoTailPosition() const;
    void _doShowAnimation();
};

#endif // NXTEACHINGTIPPRIVATE_H
