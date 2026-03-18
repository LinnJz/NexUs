#ifndef NXDROPDOWNBUTTONPRIVATE_H
#define NXDROPDOWNBUTTONPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXProperty.h"

class NXMenu;
class NXDropDownButton;
class NXDropDownButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXDropDownButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(NXIconType::IconName, NXIcon)
public:
    explicit NXDropDownButtonPrivate(QObject* parent = nullptr);
    ~NXDropDownButtonPrivate() override;

private:
    NXThemeType::ThemeMode _themeMode;
    bool _isHover{false};
    bool _isPressed{false};
    NXMenu* _menu{nullptr};
};

#endif // NXDROPDOWNBUTTONPRIVATE_H
