#ifndef NXTAGPRIVATE_H
#define NXTAGPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXTag.h"
class NXTagPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXTag)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, TagText)
    Q_PROPERTY_CREATE_D(bool, IsClosable)
    Q_PROPERTY_CREATE_D(bool, IsCheckable)
    Q_PROPERTY_CREATE_D(bool, IsChecked)
public:
    explicit NXTagPrivate(QObject* parent = nullptr);
    ~NXTagPrivate() override;

private:
    NXTag::TagColor _tagColor{NXTag::Default};
    NXThemeType::ThemeMode _themeMode;
    int _closeButtonWidth{16};
    int _checkIconWidth{16};
    int _padding{8};
    QColor _getBackgroundColor() const;
    QColor _getForegroundColor() const;
};

#endif // NXTAGPRIVATE_H
