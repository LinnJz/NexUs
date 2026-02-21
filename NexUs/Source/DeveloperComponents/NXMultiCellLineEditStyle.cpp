#include "NXMultiCellLineEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "NXTheme.h"

NXMultiCellLineEditStyle::NXMultiCellLineEditStyle(QStyle *style)
{
  _pBorderRadius = 6;
  _themeMode             = nxTheme->getThemeMode();
  connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NXMultiCellLineEditStyle::~NXMultiCellLineEditStyle() { }

void NXMultiCellLineEditStyle::drawPrimitive(PrimitiveElement element,
                                             const QStyleOption *option,
                                             QPainter *painter,
                                             const QWidget *widget) const
{
  switch (element)
  {
    case PE_Widget :
    {
      QStyleOptionFrame frame;
      const QStyleOptionFrame *fopt = qstyleoption_cast<const QStyleOptionFrame *>(option);
      if (fopt)
      {
        frame = *fopt;
      }
      else if (option)
      {
        frame.rect  = option->rect;
        frame.state = option->state;
        frame.palette = option->palette;
      }

      if (!frame.rect.isNull())
      {
        QRect lineEditRect = frame.rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(NXThemeColor(_themeMode, BasicBorder));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(lineEditRect, _pBorderRadius, _pBorderRadius);
        painter->setPen(Qt::NoPen);
        if (frame.state & QStyle::State_HasFocus) { painter->setBrush(NXThemeColor(_themeMode, DialogBase)); }
        else if (frame.state & QStyle::State_MouseOver)
        {
          painter->setBrush(NXThemeColor(_themeMode, BasicHoverAlpha));
        }
        else
        {
          painter->setBrush(NXThemeColor(_themeMode, BasicBaseAlpha));
        }
        painter->drawRoundedRect(
            QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5, lineEditRect.width() - 3, lineEditRect.height() - 3),
            _pBorderRadius,
            _pBorderRadius);

        painter->setBrush(NXThemeColor(_themeMode, BasicHemline));
        QPainterPath path;
        path.moveTo(6, lineEditRect.height());
        path.lineTo(lineEditRect.width() - 6, lineEditRect.height());
        path.arcTo(QRectF(lineEditRect.width() - 12, lineEditRect.height() - 12, 12, 12), -90, 45);
        path.lineTo(6 - 3 * std::sqrt(2), lineEditRect.height() - (6 - 3 * std::sqrt(2)));
        path.arcTo(QRectF(0, lineEditRect.height() - 12, 12, 12), 225, 45);
        path.closeSubpath();
        painter->drawPath(path);
        painter->restore();
      }
      return;
    }
    default :
    {
      break;
    }
  }
  QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int NXMultiCellLineEditStyle::pixelMetric(PixelMetric metric,
                                 const QStyleOption *option /*= nullptr*/,
                                 const QWidget *widget /*= nullptr*/) const
{
  return QProxyStyle::pixelMetric(metric, option, widget);
}
