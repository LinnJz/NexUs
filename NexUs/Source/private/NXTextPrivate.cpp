#include "NXTextPrivate.h"

#include <algorithm>

#include <QFontMetricsF>
#include <QPainter>
#include <QTextDocument>
#include <QTextOption>

#include "NXText.h"
#include "NXTheme.h"

NXTextPrivate::NXTextPrivate(QObject *parent)
    : QObject { parent }
{
}

NXTextPrivate::~NXTextPrivate() { }

void NXTextPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode) noexcept
{
  Q_Q(NXText);
  _themeMode       = themeMode;
  QPalette palette = q->palette();
  palette.setColor(QPalette::WindowText, NXThemeColor(_themeMode, BasicText));
  q->setPalette(palette);
}

std::unique_ptr<QTextDocument>
NXTextPrivate::_createDocument(const QString& htmlText, qreal maxWidth, Qt::Alignment alignment) const noexcept
{
  Q_Q(const NXText);
  std::unique_ptr<QTextDocument> doc = std::make_unique<QTextDocument>();
  doc->setDocumentMargin(0);
  doc->setDefaultFont(q->font());
  QTextOption option;
  option.setWrapMode(_pIsWrapAnywhere ? QTextOption::WrapAnywhere : QTextOption::WrapAtWordBoundaryOrAnywhere);
  option.setAlignment(alignment);
  doc->setDefaultTextOption(option);
  doc->setDefaultStyleSheet(QStringLiteral("body{color:%1;}").arg(NXThemeColor(_themeMode, BasicText).name()));
  doc->setHtml(htmlText);
  qreal textWidth = doc->idealWidth();
  if (maxWidth > 0) { textWidth = std::min(textWidth, maxWidth); }
  if (textWidth <= 0) { textWidth = std::max<qreal>(1.0, maxWidth); }
  doc->setTextWidth(textWidth);
  return doc;
}

void NXTextPrivate::_drawNXIcon(QPainter& painter, const QRectF& targetRect) const noexcept
{
  Q_Q(const NXText);
  if (_pNXIcon == NXIconType::None) { return; }
  QFont iconFont(QStringLiteral("NXAwesome"));
  iconFont.setPixelSize(q->font().pixelSize());
  painter.save();
  painter.setFont(iconFont);
  painter.setPen(NXThemeColor(_themeMode, BasicText));
  painter.drawText(targetRect, Qt::AlignCenter, QChar((unsigned short) _pNXIcon));
  painter.restore();
}

void NXTextPrivate::_drawTextCentered(QPainter& painter,
                                      const QRectF& targetRect,
                                      const QString& htmlText) const noexcept
{
  _drawTextDocumentHorizontal(painter, targetRect, htmlText, Qt::AlignCenter);
}

void NXTextPrivate::_drawTextDocumentHorizontal(QPainter& painter,
                                                const QRectF& targetRect,
                                                const QString& htmlText,
                                                Qt::Alignment alignment) const noexcept
{
  if (htmlText.isEmpty() || targetRect.isEmpty()) { return; }
  std::unique_ptr<QTextDocument> doc = _createDocument(htmlText, targetRect.width(), alignment);
  QSizeF docSize                     = doc->size();
  QPointF topLeft(targetRect.left() + (targetRect.width() - docSize.width()) / 2.0,
                  targetRect.top() + (targetRect.height() - docSize.height()) / 2.0);
  painter.save();
  painter.translate(topLeft);
  doc->drawContents(&painter, QRectF(QPointF(0, 0), docSize));
  painter.restore();
}

bool NXTextPrivate::drawByDisplayMode(QPainter& painter) const noexcept
{
  Q_Q(const NXText);
  if (_pDisplayMode == NXTextType::FollowStyle || _pDisplayMode == NXTextType::TextOnly) { return false; }

  QRectF viewport        = q->contentsRect();
  const QString htmlText = q->text();
  const bool hasText     = !htmlText.isEmpty();
  const bool hasIcon     = _pNXIcon != NXIconType::None;
  const qreal spacing    = 6.0;

  QFont iconFont(QStringLiteral("NXAwesome"));
  iconFont.setPixelSize(q->font().pixelSize());
  QFontMetricsF iconMetrics(iconFont);
  const QSizeF iconSize(hasIcon ? iconMetrics.horizontalAdvance(QChar((unsigned short) _pNXIcon)) : 0,
                        hasIcon ? iconMetrics.height() : 0);

  if (_pDisplayMode == NXTextType::IconOnly)
  {
    _drawNXIcon(painter, viewport);
    return true;
  }

  if (_pDisplayMode == NXTextType::TextBesideIcon)
  {
    if (hasText && hasIcon)
    {
      qreal textMaxWidth                 = std::max<qreal>(1.0, viewport.width() - iconSize.width() - spacing);
      std::unique_ptr<QTextDocument> doc = _createDocument(htmlText, textMaxWidth, Qt::AlignLeft);
      QSizeF textSize                    = doc->size();
      qreal totalWidth                   = iconSize.width() + spacing + textSize.width();
      qreal totalHeight                  = std::max(iconSize.height(), textSize.height());
      QRectF iconRect(viewport.left() + (viewport.width() - totalWidth) / 2.0,
                      viewport.top() + (viewport.height() - totalHeight) / 2.0 +
                          (totalHeight - iconSize.height()) / 2.0,
                      iconSize.width(), iconSize.height());
      QRectF textRect(iconRect.right() + spacing,
                      viewport.top() + (viewport.height() - totalHeight) / 2.0 +
                          (totalHeight - textSize.height()) / 2.0,
                      textSize.width(), textSize.height());
      _drawNXIcon(painter, iconRect);
      _drawTextDocumentHorizontal(painter, textRect, htmlText, Qt::AlignLeft);
      return true;
    }
    if (hasText)
    {
      _drawTextCentered(painter, viewport, htmlText);
      return true;
    }
    if (hasIcon)
    {
      _drawNXIcon(painter, viewport);
      return true;
    }
    return true;
  }

  if (_pDisplayMode == NXTextType::TextUnderIcon)
  {
    QSizeF textSize(0, 0);
    if (hasText)
    {
      std::unique_ptr<QTextDocument> doc =
          _createDocument(htmlText, std::max<qreal>(1.0, viewport.width()), Qt::AlignCenter);
      textSize = doc->size();
    }

    if (hasIcon && hasText)
    {
      qreal totalHeight = iconSize.height() + spacing + textSize.height();
      qreal startY      = viewport.top() + (viewport.height() - totalHeight) / 2.0;
      qreal centerX     = viewport.left() + viewport.width() / 2.0;
      QRectF iconRect(centerX - iconSize.width() / 2.0, startY, iconSize.width(), iconSize.height());
      QRectF textRect(centerX - textSize.width() / 2.0, startY + iconSize.height() + spacing, textSize.width(),
                      textSize.height());
      _drawNXIcon(painter, iconRect);
      _drawTextDocumentHorizontal(painter, textRect, htmlText, Qt::AlignCenter);
    }
    else if (hasIcon) { _drawNXIcon(painter, viewport); }
    else if (hasText) { _drawTextCentered(painter, viewport, htmlText); }
    return true;
  }

  return false;
}
