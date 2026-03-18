#ifndef NXPERSONPICTUREPRIVATE_H
#define NXPERSONPICTUREPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "NXDef.h"
class NXPersonPicture;
class NXPersonPicturePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXPersonPicture)
    Q_PROPERTY_CREATE_D(int, PictureSize)
    Q_PROPERTY_CREATE_D(QString, DisplayName)
    Q_PROPERTY_CREATE_D(QPixmap, Picture)

public:
    explicit NXPersonPicturePrivate(QObject* parent = nullptr);
    ~NXPersonPicturePrivate() override;

private:
    NXThemeType::ThemeMode _themeMode;
};

#endif // NXPERSONPICTUREPRIVATE_H
