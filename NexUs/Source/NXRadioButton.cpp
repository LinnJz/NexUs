#include "NXRadioButton.h"

#include <optional>
#include "DeveloperComponents/NXRadioButtonStyle.h"
#include "NXTheme.h"
#include "private/NXRadioButtonPrivate.h"

NXRadioButton::NXRadioButton(QWidget *parent)
    : QRadioButton(parent)
    , d_ptr(new NXRadioButtonPrivate())
{
  Q_D(NXRadioButton);
  setMouseTracking(true);
  setCursor(Qt::PointingHandCursor);
  d->q_ptr = this;
  setFixedHeight(20);
  QFont font = this->font();
  font.setPixelSize(15);
  setFont(font);
  setStyle(new NXRadioButtonStyle(style()));
  d->onThemeChanged(nxTheme->getThemeMode());
  connect(nxTheme, &NXTheme::themeModeChanged, d, &NXRadioButtonPrivate::onThemeChanged);
}

NXRadioButton::NXRadioButton(const QString& text, QWidget *parent)
    : NXRadioButton(parent)
{
  setText(text);
}

NXRadioButton::~NXRadioButton() { delete this->style(); }

void NXRadioButton::setTextStyle(NXTextType::TextStyle textStyle,
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

  Q_D(NXRadioButton);
  QFont textFont = font();
  d->_textStyle  = textStyle;
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

NXTextType::TextStyle NXRadioButton::getTextStyle() const
{
  Q_D(const NXRadioButton);
  return d->_textStyle;
}

void NXRadioButton::paintEvent(QPaintEvent *event)
{
  Q_D(NXRadioButton);
  if (palette().color(QPalette::WindowText) != NXThemeColor(d->_themeMode, BasicText))
  {
    d->onThemeChanged(d->_themeMode);
  }
  QRadioButton::paintEvent(event);
}
