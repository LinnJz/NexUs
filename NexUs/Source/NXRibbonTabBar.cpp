#include "NXRibbonTabBar.h"

#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>

#include "NXTheme.h"
#include "private/NXRibbonTabBarPrivate.h"

NXRibbonTabBar::NXRibbonTabBar(QWidget *parent)
    : QWidget { parent }
    , d_ptr(new NXRibbonTabBarPrivate())
{
  Q_D(NXRibbonTabBar);
  d->q_ptr          = this;
  d->_pCurrentIndex = 0;

  setObjectName("NXRibbonTabBar");
  setMouseTracking(true);
  setAttribute(Qt::WA_StyledBackground, false);
  setFixedHeight(32);

  d->_themeMode = nxTheme->getThemeMode();
  connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode)
  {
    d->_themeMode = themeMode;
    update();
  });
}

NXRibbonTabBar::~NXRibbonTabBar()
{
}

int
NXRibbonTabBar::appendTab(const QString &title)
{
  Q_D(NXRibbonTabBar);
  d->_tabTitles.append(title);
  update();
  updateGeometry();
  return d->_tabTitles.size() - 1;
}

void
NXRibbonTabBar::removeTab(int index)
{
  Q_D(NXRibbonTabBar);
  if (index < 0 || index >= d->_tabTitles.size())
  {
    return;
  }
  d->_tabTitles.removeAt(index);
  if (d->_pCurrentIndex >= d->_tabTitles.size())
  {
    d->_pCurrentIndex = qMax(0, d->_tabTitles.size() - 1);
    Q_EMIT pCurrentIndexChanged();
  }
  update();
  updateGeometry();
}

void
NXRibbonTabBar::clear()
{
  Q_D(NXRibbonTabBar);
  d->_tabTitles.clear();
  d->_pCurrentIndex = 0;
  d->_hoveredIndex  = -1;
  update();
  updateGeometry();
}

int
NXRibbonTabBar::tabCount() const
{
  Q_D(const NXRibbonTabBar);
  return d->_tabTitles.size();
}

QString
NXRibbonTabBar::tabText(int index) const
{
  Q_D(const NXRibbonTabBar);
  if (index < 0 || index >= d->_tabTitles.size())
  {
    return {};
  }
  return d->_tabTitles.at(index);
}

void
NXRibbonTabBar::setTabText(int index, const QString &title)
{
  Q_D(NXRibbonTabBar);
  if (index < 0 || index >= d->_tabTitles.size())
  {
    return;
  }
  d->_tabTitles[index] = title;
  update();
  updateGeometry();
}

void
NXRibbonTabBar::setCurrentIndex(int currentIndex)
{
  Q_D(NXRibbonTabBar);
  if (currentIndex < 0 || currentIndex >= d->_tabTitles.size())
  {
    return;
  }
  if (d->_pCurrentIndex == currentIndex)
  {
    return;
  }
  d->_pCurrentIndex = currentIndex;
  Q_EMIT pCurrentIndexChanged();
  update();
}

int
NXRibbonTabBar::getCurrentIndex() const
{
  Q_D(const NXRibbonTabBar);
  return d->_pCurrentIndex;
}

void
NXRibbonTabBar::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  Q_D(NXRibbonTabBar);
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  QFont tabFont = font();
  tabFont.setPixelSize(14);
  painter.setFont(tabFont);

  QList<QRect> rects = d->tabRects();
  for (int i = 0; i < rects.size(); ++i)
  {
    const QRect &r       = rects.at(i);
    const QString &title = d->_tabTitles.at(i);
    bool isSelected      = (i == d->_pCurrentIndex);
    bool isHovered       = (i == d->_hoveredIndex);

    if (isSelected)
    {
      painter.setPen(Qt::NoPen);
      painter.setBrush(NXThemeColor(d->_themeMode, PrimaryNormal));
      QRect markRect(r.x() + 8, r.bottom() - 2, r.width() - 16, 2);
      painter.drawRect(markRect);
      painter.setPen(NXThemeColor(d->_themeMode, BasicText));
    }
    else if (isHovered)
    {
      painter.setPen(Qt::NoPen);
      painter.setBrush(NXThemeColor(d->_themeMode, BasicHover));
      painter.drawRoundedRect(r.adjusted(4, 4, -4, -4), 4, 4);
      painter.setPen(NXThemeColor(d->_themeMode, BasicText));
    }
    else
    {
      painter.setPen(NXThemeColor(d->_themeMode, BasicTextNoFocus));
    }
    painter.drawText(r, Qt::AlignCenter, title);
  }
}

void
NXRibbonTabBar::mouseMoveEvent(QMouseEvent *event)
{
  Q_D(NXRibbonTabBar);
  int idx = d->tabAt(event->pos());
  if (idx != d->_hoveredIndex)
  {
    d->_hoveredIndex = idx;
    update();
  }
  QWidget::mouseMoveEvent(event);
}

void
NXRibbonTabBar::mousePressEvent(QMouseEvent *event)
{
  Q_D(NXRibbonTabBar);
  if (event->button() == Qt::LeftButton)
  {
    int idx = d->tabAt(event->pos());
    if (idx >= 0)
    {
      const bool isReclick = (idx == d->_pCurrentIndex);
      if (!isReclick)
      {
        d->_pCurrentIndex = idx;
        Q_EMIT pCurrentIndexChanged();
        update();
      }
      else
      {
        Q_EMIT tabReclicked(idx);
      }
      Q_EMIT tabClicked(idx);
    }
  }
  QWidget::mousePressEvent(event);
}

void
NXRibbonTabBar::leaveEvent(QEvent *event)
{
  Q_D(NXRibbonTabBar);
  if (d->_hoveredIndex != -1)
  {
    d->_hoveredIndex = -1;
    update();
  }
  QWidget::leaveEvent(event);
}

QSize
NXRibbonTabBar::sizeHint() const
{
  Q_D(const NXRibbonTabBar);
  QFont tabFont = font();
  tabFont.setPixelSize(14);
  QFontMetrics fm(tabFont);
  int total = 16;
  for (const QString &t : d->_tabTitles)
  {
    total += fm.horizontalAdvance(t) + 28;
  }
  return QSize(qMax(total, 80), 32);
}

NXRibbonTabBarPrivate::NXRibbonTabBarPrivate(QObject *parent)
    : QObject { parent }
{
}

NXRibbonTabBarPrivate::~NXRibbonTabBarPrivate()
{
}

QList<QRect>
NXRibbonTabBarPrivate::tabRects() const
{
  QList<QRect> rects;
  if (_tabTitles.isEmpty())
  {
    return rects;
  }
  QFont tabFont = q_ptr->font();
  tabFont.setPixelSize(14);
  QFontMetrics fm(tabFont);
  int x = 16;
  for (const QString &t : _tabTitles)
  {
    int w = fm.horizontalAdvance(t) + 28;
    rects.append(QRect(x, 0, w, q_ptr->height()));
    x += w;
  }
  return rects;
}

int
NXRibbonTabBarPrivate::tabAt(QPoint pos) const
{
  QList<QRect> rects = tabRects();
  for (int i = 0; i < rects.size(); ++i)
  {
    if (rects.at(i).contains(pos))
    {
      return i;
    }
  }
  return -1;
}
