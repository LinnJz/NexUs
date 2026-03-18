#ifndef NXSPLITBUTTONPRIVATE_H
#define NXSPLITBUTTONPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXProperty.h"
class NXMenu;
class NXSplitButton;
class NXSplitButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXSplitButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(NXIconType::IconName, NXIcon)

public:
    explicit NXSplitButtonPrivate(QObject* parent = nullptr);
    ~NXSplitButtonPrivate();

private:
    static const int _dropdownWidth = 30;
    NXMenu* _menu{nullptr};
    bool _isLeftHovered{false};
    bool _isRightHovered{false};
    bool _isLeftPressed{false};
    bool _isRightPressed{false};
    NXThemeType::ThemeMode _themeMode;
};

#endif // NXSPLITBUTTONPRIVATE_H
