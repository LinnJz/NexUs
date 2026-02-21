#ifndef NXAPPLICATIONPRIVATE_H
#define NXAPPLICATIONPRIVATE_H

#include <QColor>
#include <QIcon>
#include <QObject>

#include "NXDef.h"
class NXApplication;

class NXApplicationPrivate : public QObject
{
  friend class NXMicaBaseInitObject;
  Q_OBJECT
  Q_D_CREATE(NXApplication)
  Q_PROPERTY_CREATE_D(QString, MicaImagePath)
  Q_PROPERTY_CREATE_D(NXApplicationType::WindowDisplayMode, WindowDisplayMode)
public:
  explicit NXApplicationPrivate(QObject *parent = nullptr);
  ~NXApplicationPrivate() override;
  Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode);

  Q_SLOT void onSystemPaletteChanged();
  void syncSystemTheme();

Q_SIGNALS:
  Q_SIGNAL void initMicaBase(const QImage& img);

protected:
  virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
  NXThemeType::ThemeMode _themeMode;
  QList<QWidget *> _micaWidgetList;
  QImage _lightBaseImage;
  QImage _darkBaseImage;
  void _initMicaBaseImage(const QImage& img);
  QRect _calculateWindowVirtualGeometry(QWidget *widget);
  void _updateMica(QWidget *widget, bool isProcessEvent = true);
  void _updateAllMicaWidget();
  void _resetAllMicaWidget();
  bool _isSystemDarkMode() const;
};

#endif // NXAPPLICATIONPRIVATE_H
