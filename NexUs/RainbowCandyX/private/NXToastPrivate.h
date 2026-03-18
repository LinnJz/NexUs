#ifndef NXTOASTPRIVATE_H
#define NXTOASTPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXToast.h"
class NXToastPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXToast)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, DisplayMsec)

public:
    explicit NXToastPrivate(QObject* parent = nullptr);
    ~NXToastPrivate() override;

private:
    NXThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{6};
    NXToast::ToastType _toastType{NXToast::Info};
    QString _text;
};

#endif // NXTOASTPRIVATE_H
