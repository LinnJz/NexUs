#ifndef NXFLYOUTPRIVATE_H
#define NXFLYOUTPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXFlyout;
class QVBoxLayout;
class QGraphicsOpacityEffect;
class NXFlyoutPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXFlyout)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Content)
    Q_PROPERTY_CREATE_D(bool, IsLightDismiss)

public:
    explicit NXFlyoutPrivate(QObject* parent = nullptr);
    ~NXFlyoutPrivate() override;

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QVBoxLayout* _mainLayout{nullptr};
    QWidget* _contentWidget{nullptr};
    QGraphicsOpacityEffect* _opacityEffect{nullptr};
    NXThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{8};
    void _doShowAnimation();
};

#endif // NXFLYOUTPRIVATE_H
