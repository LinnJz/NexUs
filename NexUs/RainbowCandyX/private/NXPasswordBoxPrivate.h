#ifndef NXPASSWORDBOXPRIVATE_H
#define NXPASSWORDBOXPRIVATE_H

#include <QObject>

#include "NXDef.h"
class QAction;
class NXEvent;
class NXPasswordBox;
class NXPasswordBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXPasswordBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(bool, IsPasswordVisible)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit NXPasswordBoxPrivate(QObject* parent = nullptr);
    ~NXPasswordBoxPrivate() override;
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
    NXThemeType::ThemeMode _themeMode;
    NXEvent* _focusEvent{nullptr};
    QAction* _toggleAction{nullptr};
    qreal _textSpacing{0.5};
};

#endif // NXPASSWORDBOXPRIVATE_H
