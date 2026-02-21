#include "NXAppBarPrivate.h"

#include "NXToolButton.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#endif
#include <QDebug>
#include <QGuiApplication>
#include <QLabel>
#include <QMenu>
#include <QPropertyAnimation>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>
#include <QWindow>
#include "NXAppBar.h"
#include "NXIconButton.h"
#include "NXNavigationBar.h"
#include "NXText.h"

NXAppBarPrivate::NXAppBarPrivate(QObject *parent)
    : QObject { parent }
{
}

NXAppBarPrivate::~NXAppBarPrivate() { }

void NXAppBarPrivate::onMinButtonClicked()
{
  Q_Q(NXAppBar);
  q->window()->showMinimized();
}

void NXAppBarPrivate::onMaxButtonClicked()
{
  Q_Q(NXAppBar);
  bool isMaximized = q->window()->isMaximized();
  isMaximized ? q->window()->showNormal() : q->window()->showMaximized();
#ifndef Q_OS_WIM
  _changeMaxButtonAwesome(!isMaximized);
#endif
}

void NXAppBarPrivate::onCloseButtonClicked()
{
  Q_Q(NXAppBar);
  if (_pIsDefaultClosed) { q->window()->close(); }
  else
  {
    Q_EMIT q_ptr->closeButtonClicked();
  }
}

void NXAppBarPrivate::onStayTopButtonClicked()
{
#ifdef Q_OS_WIN
  HWND hwnd = (HWND) _currentWinID;
  ::SetWindowPos(hwnd, _pIsStayTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
  Q_Q(NXAppBar);
  if (q->window()->isVisible())
  {
    q->window()->windowHandle()->setFlag(Qt::WindowStaysOnTopHint, _pIsStayTop);
    q->window()->update();
  }
  else
  {
    q->window()->setWindowFlag(Qt::WindowStaysOnTopHint, _pIsStayTop);
  }
#endif
  _stayTopButton->setIsSelected(_pIsStayTop);
  _stayTopButton->update();
}

void NXAppBarPrivate::_changeMaxButtonAwesome(bool isMaximized)
{
  if (isMaximized) { _maxButton->setNXIcon(NXIconType::WindowRestore); }
  else
  {
    _maxButton->setNXIcon(NXIconType::Square);
  }
}

void NXAppBarPrivate::_showAppBarMenu(QPoint point)
{
  Q_Q(const NXAppBar);
  if (_pCustomMenu) { _pCustomMenu->exec(point); }
  else
  {
#ifdef Q_OS_WIN
    QScreen *screen = qApp->screenAt(QCursor::pos());
    if (!screen) { screen = QGuiApplication::primaryScreen(); }
    if (!screen) { return; }
    const QPoint origin = screen->geometry().topLeft();
    auto nativePos      = QPointF(QPointF(point - origin) * screen->devicePixelRatio()).toPoint() + origin;
    HWND hwnd           = reinterpret_cast<HWND>(q->window()->winId());
    const HMENU hMenu   = ::GetSystemMenu(hwnd, FALSE);
    if (q->window()->isMaximized() || q->window()->isFullScreen())
    {
      ::EnableMenuItem(hMenu, SC_MOVE, MFS_DISABLED);
      ::EnableMenuItem(hMenu, SC_RESTORE, MFS_ENABLED);
    }
    else
    {
      ::EnableMenuItem(hMenu, SC_MOVE, MFS_ENABLED);
      ::EnableMenuItem(hMenu, SC_RESTORE, MFS_DISABLED);
    }
    if (!_pIsFixedSize && !q->window()->isMaximized() && !q->window()->isFullScreen())
    {
      ::EnableMenuItem(hMenu, SC_SIZE, MFS_ENABLED);
      ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_ENABLED);
    }
    else
    {
      ::EnableMenuItem(hMenu, SC_SIZE, MFS_DISABLED);
      ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_DISABLED);
    }
    const int result =
        ::TrackPopupMenu(hMenu,
                         (TPM_RETURNCMD | (QGuiApplication::isRightToLeft() ? TPM_RIGHTALIGN : TPM_LEFTALIGN)),
                         nativePos.x(),
                         nativePos.y(),
                         0,
                         hwnd,
                         nullptr);
    if (result != FALSE) { ::PostMessageW(hwnd, WM_SYSCOMMAND, result, 0); }
#endif
  }
}

void NXAppBarPrivate::_updateCursor(int edges)
{
  Q_Q(const NXAppBar);
  switch (edges)
  {
    case 0 :
    {
      q->window()->setCursor(Qt::ArrowCursor);
      break;
    }
    case Qt::LeftEdge :
    case Qt::RightEdge :
    {
      q->window()->setCursor(Qt::SizeHorCursor);
      break;
    }
    case Qt::TopEdge :
    case Qt::BottomEdge :
    {
      q->window()->setCursor(Qt::SizeVerCursor);
      break;
    }
    case Qt::LeftEdge | Qt::TopEdge :
    case Qt::RightEdge | Qt::BottomEdge :
    {
      q->window()->setCursor(Qt::SizeFDiagCursor);
      break;
    }
    case Qt::RightEdge | Qt::TopEdge :
    case Qt::LeftEdge | Qt::BottomEdge :
    {
      q->window()->setCursor(Qt::SizeBDiagCursor);
      break;
    }
    default :
    {
      break;
    }
  }
}

bool NXAppBarPrivate::_containsCursorToItem(QWidget *item)
{
  Q_Q(const NXAppBar);
  if (!item || !item->isVisible()) { return false; }
  auto point  = item->window()->mapFromGlobal(QCursor::pos());
  QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
  if (item == q)
  {
    for (int i = 0; i < _clientWidgetList.count(); i++)
    {
      if (_containsCursorToItem(_clientWidgetList[i])) { return false; }
    }
    for (int i = 0; i < _customAreaWidgetList.count(); i++)
    {
      QWidget *customAreaWidget = _customAreaWidgetList[i];
      if (_containsCursorToItem(customAreaWidget))
      {
        QObject *customAreaHitTestObject = _customAreaHitTestObjectList[i];
        if (customAreaHitTestObject)
        {
          bool isContainsInAppBar = false;
          QMetaObject::invokeMethod(customAreaHitTestObject,
                                    _customAreaHitTestFunctionNameList[i].toLocal8Bit().constData(),
                                    Qt::AutoConnection,
                                    Q_RETURN_ARG(bool, isContainsInAppBar));
          return isContainsInAppBar;
        }
        else
        {
          return false;
        }
      }
    }
  }
  else if (item == _maxButton) { rect.adjust(0, 8, 0, 0); }
  if (rect.contains(point)) { return true; }
  return false;
}

void NXAppBarPrivate::_onThemeModeChange(NXThemeType::ThemeMode themeMode)
{
  if (themeMode == NXThemeType::Light) { _themeChangeButton->setNXIcon(NXIconType::MoonStars); }
  else
  {
    _themeChangeButton->setNXIcon(NXIconType::SunBright);
  }
}

int NXAppBarPrivate::_calculateMinimumWidth()
{
  Q_Q(NXAppBar);
  int width = 0;
  if (_titleLabel->isVisible())
  {
    width += _titleLabel->width();
    width += 10;
  }
  if (_iconLabel->isVisible())
  {
    width += _iconLabel->width();
    width += 10;
  }
  bool isHasNavigationBar = false;
  if (q->parentWidget()->findChild<NXNavigationBar *>())
  {
    isHasNavigationBar = true;
    width += 305;
  }
  else
  {
    width += 5;
  }

  int customWidgetWidth = 0;
  for (int i = 0; i < _customAreaWidgetList.count(); i++)
  {
    customWidgetWidth += _customAreaWidgetList[i]->minimumWidth();
  }
  if (isHasNavigationBar)
  {
    if (customWidgetWidth > 300) { width += customWidgetWidth - 300; }
  }
  else
  {
    width += customWidgetWidth;
  }

  QList<QAbstractButton *> buttonList = q->findChildren<QAbstractButton *>();
  for (auto button : buttonList)
  {
    // fixed bug scrollarea add components，double click will let window size change
    if (button->isVisible() && button->objectName() != "NavigationButton" && button->parentWidget() == q)
    {
      width += button->width();
    }
  }
  return width;
}

QVBoxLayout *NXAppBarPrivate::_createVLayout(QWidget *widget)
{
  if (!widget) { return nullptr; }
  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0, 0, 0, 0);
  vLayout->setSpacing(0);
  if (widget == _iconLabel || widget == _titleLabel) { vLayout->addSpacing(6); }
  vLayout->addWidget(widget);
  vLayout->addStretch();
  return vLayout;
}
