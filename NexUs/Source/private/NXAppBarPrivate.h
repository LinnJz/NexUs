#ifndef NXAPPBARPRIVATE_H
#define NXAPPBARPRIVATE_H
#include <QObject>

#include "NXDef.h"
class QMenu;
class QLabel;
class QScreen;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;
class NXText;
class NXAppBar;
class NXIconButton;
class NXToolButton;

class NXAppBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXAppBar)
  Q_PROPERTY_CREATE_D(bool, IsCustomModule)
  Q_PROPERTY_CREATE_D(bool, IsStayTop)
  Q_PROPERTY_CREATE_D(bool, IsFixedSize)
  Q_PROPERTY_CREATE_D(bool, IsDefaultClosed)
  Q_PROPERTY_CREATE_D(bool, IsOnlyAllowMinAndClose)
  bool _isHoverMaxButton { false };
  Q_PRIVATE_CREATE_D(QMenu *, CustomMenu)
  Q_PROPERTY_CREATE_D(int, AppBarHeight)

public:
  explicit NXAppBarPrivate(QObject *parent = nullptr);
  ~NXAppBarPrivate() override;
  Q_SLOT void onMinButtonClicked();
  Q_SLOT void onMaxButtonClicked();
  Q_SLOT void onCloseButtonClicked();
  Q_SLOT void onStayTopButtonClicked();

private:
  int _lastMinTrackWidth { 0 };
  int _edges { 0 };
  int _margins { 8 };
  int _win7Margins { 0 };
  NXThemeType::ThemeMode _themeMode;
  NXAppBarType::ButtonFlags _buttonFlags;
  qint64 _currentWinID { 0 };
  quint64 _clickTimer { 0 };
  QHBoxLayout *_mainLayout { nullptr };
  QVBoxLayout *_iconLabelLayout { nullptr };
  QVBoxLayout *_titleLabelLayout { nullptr };

  NXToolButton *_routeBackButton { nullptr };
  NXToolButton *_routeForwardButton { nullptr };
  NXToolButton *_navigationButton { nullptr };
  NXToolButton *_themeChangeButton { nullptr };
  NXToolButton *_stayTopButton { nullptr };
  NXToolButton *_minButton { nullptr };
  NXToolButton *_maxButton { nullptr };
  NXIconButton *_closeButton { nullptr };
  QScreen *_lastScreen { nullptr };
  NXText *_titleLabel { nullptr };
  QLabel *_iconLabel { nullptr };

  QList<QWidget *> _customAreaWidgetList { nullptr, nullptr, nullptr };
  QList<QObject *> _customAreaHitTestObjectList { nullptr, nullptr, nullptr };
  QStringList _customAreaHitTestFunctionNameList { "", "", "" };
  QList<QWidget *> _clientWidgetList;

  void _changeMaxButtonAwesome(bool isMaximized);
  void _showAppBarMenu(QPoint point);
  void _updateCursor(int edges);
  bool _containsCursorToItem(QWidget *item);
  void _onThemeModeChange(NXThemeType::ThemeMode themeMode);
  int _calculateMinimumWidth();
  QVBoxLayout *_createVLayout(QWidget *widget);
  QHBoxLayout *_createHLayout(QWidget *widget);
};

#endif // NXAPPBARPRIVATE_H
