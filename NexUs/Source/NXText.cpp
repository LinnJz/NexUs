#include "NXText.h"

#include <QContextMenuEvent>
#include <QPainter>

#include "NXTheme.h"
#include "private/NXTextPrivate.h"
Q_PROPERTY_CREATE_CPP(NXText, bool, IsAllowClick)
Q_PROPERTY_CREATE_CPP(NXText, NXTextType::DisplayMode, DisplayMode)

NXText::NXText(QWidget *parent)
    : QLabel(parent)
    , d_ptr(new NXTextPrivate())
{
  Q_D(NXText);
  d->q_ptr            = this;
  d->_pIsAllowClick   = false;
  d->_pIsWrapAnywhere = false;
  d->_pTextStyle      = NXTextType::NoStyle;
  d->_pNXIcon         = NXIconType::None;
  d->_pDisplayMode    = NXTextType::FollowStyle;

  setObjectName("NXText");
  setStyleSheet(QStringLiteral("#NXText{background-color:transparent;}"));
  QFont textFont = font();
  textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
  textFont.setPixelSize(28);
  setFont(textFont);
  setWordWrap(true);
  d->_themeMode = nxTheme->getThemeMode();
  d->onThemeChanged(nxTheme->getThemeMode());
  connect(nxTheme, &NXTheme::themeModeChanged, d, &NXTextPrivate::onThemeChanged);
}

NXText::NXText(const QString& text, QWidget *parent)
    : NXText(parent)
{
  setText(text);
}

NXText::NXText(const QString& text, int pixelSize, QWidget *parent)
    : NXText(text, parent)
{
  QFont font = this->font();
  font.setPixelSize(pixelSize);
  setFont(font);
}

NXText::~NXText() { }

void NXText::setIsWrapAnywhere(bool isWrapAnywhere) noexcept
{
  Q_D(NXText);
  setWordWrap(isWrapAnywhere);
  d->_pIsWrapAnywhere = isWrapAnywhere;
}

bool NXText::getIsWrapAnywhere() const noexcept
{
  Q_D(const NXText);
  return d->_pIsWrapAnywhere;
}

void NXText::setTextPixelSize(int size) noexcept
{
  QFont font = this->font();
  font.setPixelSize(size);
  setFont(font);
}

int NXText::getTextPixelSize() const noexcept { return this->font().pixelSize(); }

void NXText::setTextPointSize(int size) noexcept
{
  QFont font = this->font();
  font.setPointSize(size);
  setFont(font);
}

int NXText::getTextPointSize() const noexcept { return this->font().pointSize(); }

void NXText::setTextStyle(NXTextType::TextStyle textStyle) noexcept
{
  Q_D(NXText);
  QFont textFont = font();
  d->_pTextStyle = textStyle;
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
  default :
  {
    break;
  }
  }
  setFont(textFont);
}

NXTextType::TextStyle NXText::getTextStyle() const noexcept
{
  Q_D(const NXText);
  return d->_pTextStyle;
}

void NXText::setNXIcon(NXIconType::IconName icon) noexcept
{
  Q_D(NXText);
  d->_pNXIcon = icon;
  update();
  Q_EMIT pNXIconChanged();
}

NXIconType::IconName NXText::getNXIcon() const noexcept
{
  Q_D(const NXText);
  return d->_pNXIcon;
}

void NXText::mouseReleaseEvent(QMouseEvent *event)
{
  if (d_ptr->_pIsAllowClick && event->button() == Qt::LeftButton) { Q_EMIT clicked(); }
  QLabel::mouseReleaseEvent(event);
}

void NXText::enterEvent(QEnterEvent *event)
{
  if (d_ptr->_pIsAllowClick) { setCursor(QCursor(Qt::PointingHandCursor)); }
  QLabel::enterEvent(event);
}

void NXText::leaveEvent(QEvent *event)
{
  if (d_ptr->_pIsAllowClick) { setCursor(QCursor(Qt::ArrowCursor)); }
  QLabel::leaveEvent(event);
}

void NXText::paintEvent(QPaintEvent *event)
{
  Q_D(NXText);
  if (palette().color(QPalette::WindowText) != NXThemeColor(d->_themeMode, BasicText))
  {
    d->onThemeChanged(d->_themeMode);
  }
  QPainter painter(this);
  painter.save();
  painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
  if (!d->drawByDisplayMode(painter))
  {
    painter.restore();
    QLabel::paintEvent(event);
    return;
  }
  painter.restore();
}
