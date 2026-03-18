#ifndef NXSPLASHSCREEN_H
#define NXSPLASHSCREEN_H

#include <QWidget>

#include "NXProperty.h"

class NXSplashScreenPrivate;
class NX_EXPORT NXSplashScreen : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXSplashScreen)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, Minimum)
    Q_PROPERTY_CREATE_H(int, Maximum)
    Q_PROPERTY_CREATE_H(int, Value)
    Q_PROPERTY_CREATE_H(bool, IsShowProgressBar)
    Q_PROPERTY_CREATE_H(bool, IsShowProgressRing)
    Q_PROPERTY_CREATE_H(bool, IsClosable)
public:
    explicit NXSplashScreen(QWidget* parent = nullptr);
    explicit NXSplashScreen(const QPixmap& logo, QWidget* parent = nullptr);
    ~NXSplashScreen() override;

    void setLogo(const QPixmap& logo) noexcept;
    void setTitle(const QString& title) noexcept;
    void setSubTitle(const QString& subTitle) noexcept;
    void setStatusText(const QString& text) noexcept;

    void show();
    void close();
    void finish(QWidget *mainWindow) noexcept;

Q_SIGNALS:
  void closed();

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // NXSPLASHSCREEN_H
