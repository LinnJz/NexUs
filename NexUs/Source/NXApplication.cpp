#include "NXApplication.h"

#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QWidget>
#include <utility>
#include "DeveloperComponents/NXWinShadowHelper.h"
#include "NXTheme.h"
#include "private/NXApplicationPrivate.h"

NXApplication::NXApplication(QObject *parent)
    : QObject { parent }
    , d_ptr(new NXApplicationPrivate())
{
  Q_D(NXApplication);
  d->q_ptr               = this;
  d->_pMicaImagePath     = ":/Resource/Image/MicaBase.png";
  d->_pWindowDisplayMode = NXApplicationType::Normal;
  d->_themeMode          = nxTheme->getThemeMode();
  connect(nxTheme, &NXTheme::themeModeChanged, d, &NXApplicationPrivate::onThemeModeChanged);
}

NXApplication::~NXApplication() { }

void NXApplication::setWindowDisplayMode(NXApplicationType::WindowDisplayMode windowDisplayType)
{
  Q_D(NXApplication);
  auto lastDisplayMode = d->_pWindowDisplayMode;
  if (lastDisplayMode == windowDisplayType) { return; }
  if (lastDisplayMode == NXApplicationType::NXMica) { d->_resetAllMicaWidget(); }
  switch (windowDisplayType)
  {
    case NXApplicationType::Normal :
    {
      break;
    }
    case NXApplicationType::NXMica :
    {
      d->_pWindowDisplayMode = windowDisplayType;
      d->_initMicaBaseImage(QImage(d->_pMicaImagePath));
      break;
    }
    default :
    {
      break;
    }
  }
#ifdef Q_OS_WIN
  for (auto widget : d->_micaWidgetList)
  {
    NXWinShadowHelper::getInstance()->setWindowDisplayMode(widget, windowDisplayType, lastDisplayMode);
    NXWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), d->_themeMode == NXThemeType::Light);
  }
#endif
  if (windowDisplayType != NXApplicationType::NXMica)
  {
    d->_pWindowDisplayMode = windowDisplayType;
    Q_EMIT pWindowDisplayModeChanged();
  }
}

NXApplicationType::WindowDisplayMode NXApplication::getWindowDisplayMode() const
{
  Q_D(const NXApplication);
  return d->_pWindowDisplayMode;
}

void NXApplication::setMicaImagePath(const QString& micaImagePath)
{
  Q_D(NXApplication);
  d->_pMicaImagePath = micaImagePath;
  d->_initMicaBaseImage(QImage(micaImagePath));
  Q_EMIT pMicaImagePathChanged();
}

QString NXApplication::getMicaImagePath() const
{
  Q_D(const NXApplication);
  return d->_pMicaImagePath;
}

void NXApplication::init()
{
  Q_D(NXApplication);
  Q_INIT_RESOURCE(NexUs);
  QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
  QFontDatabase::addApplicationFont(":/Resource/Font/NXAwesome.ttf");
  QFontDatabase::addApplicationFont(":/Resource/Font/segoe_slboot.ttf");
  // 默认字体
  QFont font = qApp->font();
  font.setPixelSize(13);
  font.setFamily("Microsoft YaHei");
  font.setHintingPreference(QFont::PreferNoHinting);
  qApp->setFont(font);
#ifdef Q_OS_WIN
  nxWinHelper->initWinAPI();
#endif
  d->syncSystemTheme();
}

void NXApplication::syncWindowDisplayMode(QWidget *widget, bool isSync)
{
  Q_D(NXApplication);
  if (!widget) { return; }
  if (isSync)
  {
    d->_micaWidgetList.append(widget);
    widget->installEventFilter(d);
  }
  else
  {
    d->_micaWidgetList.removeOne(widget);
    widget->removeEventFilter(d);
  }
  switch (d->_pWindowDisplayMode)
  {
    case NXApplicationType::Normal :
    case NXApplicationType::NXMica :
    {
      if (isSync)
      {
        if (d->_pWindowDisplayMode == NXApplicationType::WindowDisplayMode::NXMica) { d->_updateMica(widget, false); }
      }
      break;
    }
    default :
    {
#ifdef Q_OS_WIN
      if (isSync)
      {
        NXWinShadowHelper::getInstance()->setWindowDisplayMode(
            widget, d->_pWindowDisplayMode, NXApplicationType::Normal);
        NXWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), d->_themeMode == NXThemeType::Light);
      }
      else
      {
        NXWinShadowHelper::getInstance()->setWindowDisplayMode(
            widget, NXApplicationType::Normal, d->_pWindowDisplayMode);
        NXWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), true);
      }
#endif
      break;
    }
  }
}

bool NXApplication::containsCursorToItem(QWidget *item)
{
  if (!item || !item->isVisible()) { return false; }
  auto point  = item->window()->mapFromGlobal(QCursor::pos());
  QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
  if (rect.contains(point)) { return true; }
  return false;
}
