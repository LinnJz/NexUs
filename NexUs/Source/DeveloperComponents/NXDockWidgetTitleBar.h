#ifndef NXDOCKWIDGETTITLEBAR_H
#define NXDOCKWIDGETTITLEBAR_H

#include <QDockWidget>
#include <QLabel>
#include <QWidget>

#include "NXDef.h"
class NXText;
class NXIconButton;
class NXDockWidget;

class NXDockWidgetTitleBar : public QWidget
{
  Q_OBJECT
public:
  explicit NXDockWidgetTitleBar(QWidget *parent = nullptr);
  ~NXDockWidgetTitleBar();

  Q_SLOT void onFloatButtonClicked();
  Q_SLOT void onCloseButtonClicked();

protected:
  virtual void paintEvent(QPaintEvent *event) override;

private:
  friend class NXDockWidgetPrivate;
  NXThemeType::ThemeMode _themeMode;
  NXDockWidget *_dockWidget { nullptr };
  QLabel *_iconLabel { nullptr };
  NXText *_titleLabel { nullptr };
  NXIconButton *_floatButton { nullptr };
  NXIconButton *_closeButton { nullptr };
  void _setVisibleFromFeatures(QDockWidget::DockWidgetFeatures features);
};

#endif // NXDOCKWIDGETTITLEBAR_H
