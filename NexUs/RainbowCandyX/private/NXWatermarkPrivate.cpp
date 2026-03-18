#include "NXWatermarkPrivate.h"
#include <QPainter>

NXWatermarkPrivate::NXWatermarkPrivate(QObject *parent)
    : QObject(parent)
{
}

NXWatermarkPrivate::~NXWatermarkPrivate() { }

void NXWatermarkPrivate::rebuildPattern() noexcept
{
  Q_Q(NXWatermark);
  _patternDirty = false;

  bool hasText  = !_pText.isEmpty();
  bool hasImage = !_pImage.isNull();
  if (!hasText && !hasImage)
  {
    _patternPixmap = QPixmap();
    return;
  }

  QFont font;
  font.setPixelSize(_pFontPixelSize);
  QFontMetrics fm(font);

  int contentWidth  = 0;
  int contentHeight = 0;
  if (hasText)
  {
    contentWidth  = fm.horizontalAdvance(_pText);
    contentHeight = fm.height();
  }
  if (hasImage)
  {
    int imgW = _pImageWidth > 0 ? _pImageWidth : _pImage.width();
    int imgH = _pImageHeight > 0 ? _pImageHeight : _pImage.height();
    if (hasText)
    {
      contentWidth = qMax(contentWidth, imgW);
      contentHeight += 4 + imgH;
    }
    else
    {
      contentWidth  = imgW;
      contentHeight = imgH;
    }
  }

  if (contentWidth <= 0 || contentHeight <= 0)
  {
    _patternPixmap = QPixmap();
    return;
  }

  int cellWidth  = contentWidth + _pGapX;
  int cellHeight = contentHeight + _pGapY;

  qreal radians     = qDegreesToRadians(_pRotation);
  int rotatedWidth  = qCeil(qAbs(cellWidth * qCos(radians)) + qAbs(cellHeight * qSin(radians)));
  int rotatedHeight = qCeil(qAbs(cellWidth * qSin(radians)) + qAbs(cellHeight * qCos(radians)));

  qreal dpr = q->devicePixelRatioF();
  QPixmap tile(QSize(rotatedWidth, rotatedHeight) * dpr);
  tile.setDevicePixelRatio(dpr);
  tile.fill(Qt::transparent);

  QPainter painter(&tile);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
  painter.translate(rotatedWidth / 2.0, rotatedHeight / 2.0);
  painter.rotate(_pRotation);
  painter.translate(-cellWidth / 2.0, -cellHeight / 2.0);

  int yOffset = 0;
  if (hasImage)
  {
    int imgW = _pImageWidth > 0 ? _pImageWidth : _pImage.width();
    int imgH = _pImageHeight > 0 ? _pImageHeight : _pImage.height();
    int imgX = (contentWidth - imgW) / 2;
    painter.setOpacity(_pOpacity);
    painter.drawImage(QRect(imgX, 0, imgW, imgH), _pImage);
    yOffset = imgH + 4;
  }

  if (hasText)
  {
    QColor color = _pTextColor.isValid() ? _pTextColor : NXThemeColor(_themeMode, BasicText);
    color.setAlphaF(color.alphaF() * _pOpacity);
    painter.setOpacity(1.0);
    painter.setFont(font);
    painter.setPen(color);
    painter.drawText(QRect(0, yOffset, contentWidth, fm.height()), Qt::AlignCenter, _pText);
  }

  painter.end();
  _patternPixmap = tile;
}
