#ifndef NXCUSTOMWIDGET_H
#define NXCUSTOMWIDGET_H

#include <QDialog>

#include "NXAppBar.h"

class QVBoxLayout;

class NXCustomWidget : public QDialog
{
  Q_OBJECT
  Q_TAKEOVER_NATIVEEVENT_H
public:
  explicit NXCustomWidget(QWidget *parent = nullptr);
  ~NXCustomWidget() override;

  void setCentralWidget(QWidget *widget);

protected:
  virtual void paintEvent(QPaintEvent *event) override;
  QVBoxLayout *_mainLayout { nullptr };
  NXAppBar *_appBar { nullptr };
  QWidget *_centralWidget { nullptr };

private:
  NXThemeType::ThemeMode _themeMode;
  NXApplicationType::WindowDisplayMode _windowDisplayMode;
};

#endif // NXCUSTOMWIDGET_H
