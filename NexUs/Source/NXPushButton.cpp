#include "NXPushButton.h"

#include <QPainter>
#include <QPainterPath>

#include "NXTheme.h"
#include "private/NXPushButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(NXPushButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NXPushButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NXPushButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NXPushButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NXPushButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NXPushButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(NXPushButton, QColor, DarkPressColor)

NXPushButton::NXPushButton(QWidget *parent)
    : QPushButton(parent)
    , d_ptr(new NXPushButtonPrivate())
{
  Q_D(NXPushButton);
  d->q_ptr               = this;
  d->_pBorderRadius      = 3;
  d->_themeMode          = nxTheme->getThemeMode();
  d->_pLightDefaultColor = NXThemeColor(NXThemeType::Light, BasicBase);
  d->_pDarkDefaultColor  = NXThemeColor(NXThemeType::Dark, BasicBase);
  d->_pLightHoverColor   = NXThemeColor(NXThemeType::Light, BasicHover);
  d->_pDarkHoverColor    = NXThemeColor(NXThemeType::Dark, BasicHover);
  d->_pLightPressColor   = NXThemeColor(NXThemeType::Light, BasicPress);
  d->_pDarkPressColor    = NXThemeColor(NXThemeType::Dark, BasicPress);
  d->_lightTextColor     = NXThemeColor(NXThemeType::Light, BasicText);
  d->_darkTextColor      = NXThemeColor(NXThemeType::Dark, BasicText);
  setMouseTracking(true);
  setFixedHeight(38);
  QFont font = this->font();
  font.setPixelSize(15);
  setFont(font);
  setObjectName("NXPushButton");
  setStyleSheet("#NXPushButton{background-color:transparent;border:none;outline:none;}");
  connect(
      nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

NXPushButton::NXPushButton(const QString& text, QWidget *parent)
    : NXPushButton(parent)
{
  setText(text);
}

NXPushButton::~NXPushButton() { }

void NXPushButton::setLightTextColor(QColor color)
{
  Q_D(NXPushButton);
  d->_lightTextColor = color;
}

QColor NXPushButton::getLightTextColor() const
{
  Q_D(const NXPushButton);
  return d->_lightTextColor;
}

void NXPushButton::setDarkTextColor(QColor color)
{
  Q_D(NXPushButton);
  d->_darkTextColor = color;
}

QColor NXPushButton::getDarkTextColor() const
{
  Q_D(const NXPushButton);
  return d->_darkTextColor;
}

void NXPushButton::setTextStyle(NXTextType::TextStyle textStyle,
                                std::optional<int> pixelSize,
                                std::optional<QFont::Weight> weight)
{
  if (textStyle < NXTextType::NoStyle || textStyle > NXTextType::CustomStyle)
  {
    qWarning() << "Warning: Invalid textStyle provided. Please use a valid NXTextType::TextStyle enum value.";
    return;
  }
  if (textStyle != NXTextType::CustomStyle && (pixelSize.has_value() || weight.has_value()))
  {
    qWarning() << "Warning: To use pixelSize and weight, set textStyle to NXTextType::CustomStyle.";
    return;
  }
  QFont textFont = this->font();
  switch (textStyle)
  {
    case NXTextType::NoStyle :
    {
      break;
    }
    case NXTextType::Caption :
    {
      textFont.setPixelSize(12);
      break;
    }
    case NXTextType::Body :
    {
      textFont.setPixelSize(13);
      break;
    }
    case NXTextType::BodyStrong :
    {
      textFont.setPixelSize(13);
      textFont.setWeight(QFont::DemiBold);
      break;
    }
    case NXTextType::Subtitle :
    {
      textFont.setPixelSize(20);
      textFont.setWeight(QFont::DemiBold);
      break;
    }
    case NXTextType::Title :
    {
      textFont.setPixelSize(28);
      textFont.setWeight(QFont::DemiBold);
      break;
    }
    case NXTextType::TitleLarge :
    {
      textFont.setPixelSize(40);
      textFont.setWeight(QFont::DemiBold);
      break;
    }
    case NXTextType::Display :
    {
      textFont.setPixelSize(48);
      textFont.setWeight(QFont::DemiBold);
      break;
    }
    case NXTextType::CustomStyle :
    {
      if (pixelSize.has_value()) { textFont.setPixelSize(pixelSize.value()); }
      if (weight.has_value()) { textFont.setWeight(weight.value()); }
      break;
    }
    default :
    {
      break;
    }
  }
  setFont(textFont);
}

void NXPushButton::setNXIcon(NXIconType::IconName icon)
{
  Q_D(NXPushButton);
  d->_icon    = icon;
  d->_hasIcon = (icon != NXIconType::None);
  update();
}

void NXPushButton::setNXIcon(NXIconType::IconName icon, int iconSize)
{
  Q_D(NXPushButton);
  d->_icon     = icon;
  d->_iconSize = iconSize;
  d->_hasIcon  = (icon != NXIconType::None);
  update();
}

void NXPushButton::mousePressEvent(QMouseEvent *event)
{
  Q_D(NXPushButton);
  d->_isPressed = true;
  QPushButton::mousePressEvent(event);
}

void NXPushButton::mouseReleaseEvent(QMouseEvent *event)
{
  Q_D(NXPushButton);
  d->_isPressed = false;
  QPushButton::mouseReleaseEvent(event);
}

void NXPushButton::paintEvent(QPaintEvent *event)
{
  Q_D(NXPushButton);
  QPainter painter(this);
  painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
  // 高性能阴影
  nxTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

  // 背景绘制
  painter.save();
  QRect foregroundRect(d->_shadowBorderWidth,
                       d->_shadowBorderWidth,
                       width() - 2 * (d->_shadowBorderWidth),
                       height() - 2 * d->_shadowBorderWidth);
  if (d->_themeMode == NXThemeType::Light)
  {
    painter.setPen(NXThemeColor(NXThemeType::Light, BasicBorder));
    painter.setBrush(isEnabled() ? d->_isPressed ? d->_pLightPressColor
                                                 : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor)
                                 : NXThemeColor(d->_themeMode, BasicDisable));
  }
  else
  {
    painter.setPen(Qt::NoPen);
    painter.setBrush(isEnabled() ? d->_isPressed ? d->_pDarkPressColor
                                                 : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor)
                                 : NXThemeColor(d->_themeMode, BasicDisable));
  }
  painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
  // 底边线绘制
  if (!d->_isPressed)
  {
    painter.setPen(NXThemeColor(d->_themeMode, BasicBaseLine));
    painter.drawLine(foregroundRect.x() + d->_pBorderRadius,
                     height() - d->_shadowBorderWidth + 1,
                     foregroundRect.width() - d->_pBorderRadius,
                     height() - d->_shadowBorderWidth + 1);
  }
  // 文字绘制
  painter.setPen(isEnabled() ? d->_themeMode == NXThemeType::Light ? d->_lightTextColor : d->_darkTextColor
                             : NXThemeColor(d->_themeMode, BasicTextDisable));
  painter.drawText(foregroundRect, Qt::AlignCenter, text());
  painter.restore();
}
