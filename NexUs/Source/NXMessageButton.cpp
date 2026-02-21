#include "NXMessageButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "NXMessageBar.h"
#include "NXTheme.h"
#include "private/NXMessageButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(NXMessageButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NXMessageButton, QString, BarTitle);
Q_PROPERTY_CREATE_Q_CPP(NXMessageButton, QString, BarText);
Q_PROPERTY_CREATE_Q_CPP(NXMessageButton, int, DisplayMsec);
Q_PROPERTY_CREATE_Q_CPP(NXMessageButton, QWidget *, MessageTargetWidget)
Q_PROPERTY_CREATE_Q_CPP(NXMessageButton, NXMessageBarType::MessageMode, MessageMode);
Q_PROPERTY_CREATE_Q_CPP(NXMessageButton, NXMessageBarType::PositionPolicy, PositionPolicy);

NXMessageButton::NXMessageButton(QWidget *parent)
    : QPushButton(parent)
    , d_ptr(new NXMessageButtonPrivate())
{
  Q_D(NXMessageButton);
  d->q_ptr          = this;
  d->_pBorderRadius = 3;
  setMouseTracking(true);
  setFixedSize(80, 38);
  QFont font = this->font();
  font.setPixelSize(15);
  setFont(font);
  setText("Message");
  setObjectName("NXMessageButton");
  setStyleSheet("#NXMessageButton{background-color:transparent;}");
  d->_pDisplayMsec         = 2000;
  d->_pMessageMode         = NXMessageBarType::Success;
  d->_pPositionPolicy      = NXMessageBarType::TopRight;
  d->_themeMode            = nxTheme->getThemeMode();
  d->_pMessageTargetWidget = parent;
  connect(
      nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
  connect(this, &NXMessageButton::clicked, d, &NXMessageButtonPrivate::_showMessage);
  connect(this, &NXMessageButton::showMessage, d, &NXMessageButtonPrivate::_showMessage);
}

NXMessageButton::NXMessageButton(const QString& text, QWidget *parent)
    : NXMessageButton(parent)
{
  setText(text);
}

NXMessageButton::~NXMessageButton() { }

void NXMessageButton::disconnectInternalSignalOfClicked()
{
  Q_D(NXMessageButton);
  disconnect(this, &NXMessageButton::clicked, d, &NXMessageButtonPrivate::_showMessage);
}

void NXMessageButton::mousePressEvent(QMouseEvent *event)
{
  Q_D(NXMessageButton);
  if (event->button() == Qt::LeftButton) { d->_isLeftButtonPress = true; }
  QPushButton::mousePressEvent(event);
}

void NXMessageButton::mouseReleaseEvent(QMouseEvent *event)
{
  Q_D(NXMessageButton);
  d->_isLeftButtonPress = false;
  QPushButton::mouseReleaseEvent(event);
}

void NXMessageButton::paintEvent(QPaintEvent *event)
{
  Q_D(NXMessageButton);
  QPainter painter(this);
  painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
  // 高性能阴影
  nxTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

  // 背景绘制
  painter.save();
  QRect foregroundRect(d->_penBorderWidth + d->_shadowBorderWidth,
                       d->_penBorderWidth + d->_shadowBorderWidth,
                       width() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth),
                       height() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth));
  painter.setPen(QPen(NXThemeColor(d->_themeMode, BasicBorder), d->_penBorderWidth));
  painter.setBrush(isEnabled()
                       ? underMouse() ? NXThemeColor(d->_themeMode, BasicHover) : NXThemeColor(d->_themeMode, BasicBase)
                       : NXThemeColor(d->_themeMode, BasicDisable));
  painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

  // 文字绘制
  painter.setPen(isEnabled() ? d->_isLeftButtonPress ? NXThemeColor(d->_themeMode, BasicTextPress)
                                                     : NXThemeColor(d->_themeMode, BasicText)
                             : NXThemeColor(d->_themeMode, BasicTextDisable));
  painter.drawText(rect(), Qt::AlignCenter, text());
  painter.restore();
}
