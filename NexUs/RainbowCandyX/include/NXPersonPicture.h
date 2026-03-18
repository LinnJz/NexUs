#ifndef NXPERSONPICTURE_H
#define NXPERSONPICTURE_H

#include <QPixmap>
#include <QWidget>

#include "NXProperty.h"
class NXPersonPicturePrivate;
class NX_EXPORT NXPersonPicture : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXPersonPicture)
    Q_PROPERTY_CREATE_H(int, PictureSize)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, DisplayName)
    Q_PROPERTY_CREATE_2_H(const QPixmap&, QPixmap, Picture)

public:
    explicit NXPersonPicture(QWidget* parent = nullptr);
    ~NXPersonPicture() override;
    QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent* event) override;
};

#endif // NXPERSONPICTURE_H
