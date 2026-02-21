#include "NXText.h"

#include <optional>

#include <QContextMenuEvent>
#include <QPainter>

#include "NXTheme.h"
#include "private/NXTextPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(NXText, bool, IsAllowClick)
Q_PROPERTY_CREATE_Q_CPP(NXText, NXWidgetType::BorderFlags, BorderFlag)

NXText::NXText(QWidget *parent)
    : QLabel(parent)
    , d_ptr(new NXTextPrivate())
{
  Q_D(NXText);
  d->q_ptr            = this;
  d->_pTextStyle      = NXTextType::NoStyle;
  d->_pNXIcon         = NXIconType::None;
  d->_pIsAllowClick   = false;
  d->_pIsWrapAnywhere = false;

  setObjectName("NXText");
  setStyleSheet("#NXText{background-color:transparent;}");
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

void NXText::setBorderStyle(int pixelSize, NXWidgetType::BorderFlags borderFlag, QColor color)
{
  Q_D(NXText);
  d->_pBorderFlag = borderFlag;
  d->_borderColor = color;
  d->_borderPx    = pixelSize;
}

void NXText::setIsWrapAnywhere(bool isWrapAnywhere)
{
  Q_D(NXText);
  setWordWrap(isWrapAnywhere);
  d->_pIsWrapAnywhere = isWrapAnywhere;
}

bool NXText::getIsWrapAnywhere() const
{
  Q_D(const NXText);
  return d->_pIsWrapAnywhere;
}

void NXText::setTextPixelSize(int size)
{
  QFont font = this->font();
  font.setPixelSize(size);
  setFont(font);
}

int NXText::getTextPixelSize() const { return this->font().pixelSize(); }

void NXText::setTextPointSize(int size)
{
  QFont font = this->font();
  font.setPointSize(size);
  setFont(font);
}

int NXText::getTextPointSize() const { return this->font().pointSize(); }

void NXText::setTextStyle(NXTextType::TextStyle textStyle,
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

NXTextType::TextStyle NXText::getTextStyle() const
{
  Q_D(const NXText);
  return d->_pTextStyle;
}

void NXText::setNXIcon(NXIconType::IconName elaIcon)
{
  Q_D(NXText);
  d->_pNXIcon = elaIcon;
  update();
  Q_EMIT pNXIconChanged();
}

NXIconType::IconName NXText::getNXIcon() const
{
  Q_D(const NXText);
  return d->_pNXIcon;
}

void NXText::mouseReleaseEvent(QMouseEvent *event)
{
  Q_D(NXText);
  if (d_ptr->_pIsAllowClick && event->button() == Qt::LeftButton) { Q_EMIT clicked(); }
  QLabel::mouseReleaseEvent(event);
}

void NXText::enterEvent(QEnterEvent *event)
{
  Q_D(const NXText);
  if (d_ptr->_pIsAllowClick) { setCursor(QCursor(Qt::PointingHandCursor)); }
  QLabel::enterEvent(event);
}

void NXText::leaveEvent(QEvent *event)
{
  Q_D(const NXText);
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
  if (d->_pNXIcon != NXIconType::None)
  {
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    QFont iconFont = QFont("NXAwesome");
    iconFont.setPixelSize(this->font().pixelSize());
    painter.setFont(iconFont);
    painter.setPen(NXThemeColor(d->_themeMode, BasicText));
    painter.drawText(rect(), Qt::AlignCenter, QChar((unsigned short) d->_pNXIcon));
    painter.restore();
  }
  else
  {
    if (wordWrap() && d->_pIsWrapAnywhere)
    {
      QPainter painter(this);
      painter.save();
      painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
      painter.setPen(NXThemeColor(d->_themeMode, BasicText));
      painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere, text());
      painter.restore();
    }
    else
    {
      QRect contentRect = rect().adjusted(
          contentsMargins().left(), contentsMargins().top(), -contentsMargins().right(), -contentsMargins().bottom());
      QPainter painter(this);
      painter.save();
      painter.setPen(QPen(d->_borderColor, d->_borderPx));
      QFontMetrics fontMetrics(font());
      int textWidth = fontMetrics.horizontalAdvance(text());
      if (d->_pBorderFlag & NXWidgetType::TopBorder)
      {
        painter.drawLine(QPoint(contentRect.center().x() - textWidth / 2, contentRect.top()),
                         QPoint(contentRect.center().x() + textWidth / 2, contentRect.top()));
      }

      if (d->_pBorderFlag & NXWidgetType::BottomBorder)
      {
        painter.drawLine(QPoint(contentRect.center().x() - textWidth / 2, contentRect.bottom()),
                         QPoint(contentRect.center().x() + textWidth / 2, contentRect.bottom()));
      }

      if (d->_pBorderFlag & NXWidgetType::LeftBorder)
      {
        painter.drawLine(QPoint(contentRect.left(), contentRect.center().y() - fontMetrics.height() / 2),
                         QPoint(contentRect.left(), contentRect.center().y() + fontMetrics.height() / 2));
      }

      if (d->_pBorderFlag & NXWidgetType::RightBorder)
      {
        painter.drawLine(QPoint(contentRect.right(), contentRect.center().y() - fontMetrics.height() / 2),
                         QPoint(contentRect.right(), contentRect.center().y() + fontMetrics.height() / 2));
      }
      painter.restore();
      QLabel::paintEvent(event);
    }
  }
}
