#include "NXCheckBox.h"

#include <optional>

#include "DeveloperComponents/NXCheckBoxStyle.h"
#include "private/NXCheckBoxPrivate.h"

NXCheckBox::NXCheckBox(QWidget *parent)
    : QCheckBox(parent)
    , d_ptr(new NXCheckBoxPrivate())
{
  Q_D(NXCheckBox);
  d->q_ptr = this;
  setMouseTracking(true);
  setObjectName("NXCheckBox");
  d->_checkBoxStyle = new NXCheckBoxStyle(style());
  setStyle(d->_checkBoxStyle);
  QFont font = this->font();
  font.setPixelSize(15);
  setFont(font);
}

NXCheckBox::NXCheckBox(const QString& text, QWidget *parent)
    : NXCheckBox(parent)
{
  setText(text);
}

NXCheckBox::~NXCheckBox() { delete this->style(); }

void NXCheckBox::setTextStyle(NXTextType::TextStyle textStyle,
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
  Q_D(NXCheckBox);
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

NXTextType::TextStyle NXCheckBox::getTextStyle() const
{
  Q_D(const NXCheckBox);
  return d->_textStyle;
}

void NXCheckBox::setCheckBorderRadius(int borderRadius)
{
  Q_D(const NXCheckBox);
  d->_checkBoxStyle->setCheckBorderRadius(borderRadius);
}

int NXCheckBox::getCheckBorderRadius() const
{
  Q_D(const NXCheckBox);
  return d->_checkBoxStyle->getCheckBorderRadius();
}

void NXCheckBox::setCheckIndicatorWidth(int indicatorWidth)
{
  Q_D(const NXCheckBox);
  d->_checkBoxStyle->setCheckIndicatorWidth(indicatorWidth);
}

int NXCheckBox::getCheckIndicatorWidth() const
{
  Q_D(const NXCheckBox);
  return d->_checkBoxStyle->getCheckIndicatorWidth();
}
