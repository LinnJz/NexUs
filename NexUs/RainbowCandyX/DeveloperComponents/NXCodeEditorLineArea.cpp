#include <QPaintEvent>
#include <QtGlobal>
#include "NXCodeEditorLineArea.h"

// NXCodePlainTextEdit
QTextBlock NXCodePlainTextEdit::getFirstVisibleBlock() const noexcept { return firstVisibleBlock(); }

QRectF NXCodePlainTextEdit::getBlockBoundingGeometry(const QTextBlock& block) const noexcept
{
  return blockBoundingGeometry(block);
}

QRectF NXCodePlainTextEdit::getBlockBoundingRect(const QTextBlock& block) const noexcept
{
  return blockBoundingRect(block);
}

QPointF NXCodePlainTextEdit::getContentOffset() const noexcept { return contentOffset(); }

// NXCodeEditorLineArea
NXCodeEditorLineArea::NXCodeEditorLineArea(NXCodePlainTextEdit *editor, NXCodeEditorPrivate *d, QWidget *parent)
    : QWidget(parent)
    , _editor(editor)
    , _d(d)
{
}

NXCodeEditorLineArea::~NXCodeEditorLineArea() { }

QSize NXCodeEditorLineArea::sizeHint() const { return QSize(lineAreaWidth(), 0); }

int NXCodeEditorLineArea::lineAreaWidth() const noexcept
{
  int digits   = 1;
  int maxBlock = qMax(1, _editor->blockCount());
  while (maxBlock >= 10)
  {
    maxBlock /= 10;
    ++digits;
  }
  int space = _editor->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + 20;
  return space;
}

void NXCodeEditorLineArea::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  NXThemeType::ThemeMode themeMode = _d->_themeMode;
  painter.fillRect(event->rect(), NXThemeColor(themeMode, BasicBaseDeep));

  QTextBlock block = _editor->getFirstVisibleBlock();
  int blockNumber  = block.blockNumber();
  int top          = qRound(_editor->getBlockBoundingGeometry(block).translated(_editor->getContentOffset()).top());
  int bottom       = top + qRound(_editor->getBlockBoundingRect(block).height());

  int currentBlockNumber = _editor->textCursor().blockNumber();

  while (block.isValid() && top <= event->rect().bottom())
  {
    if (block.isVisible() && bottom >= event->rect().top())
    {
      QString number = QString::number(blockNumber + 1);
      if (blockNumber == currentBlockNumber) { painter.setPen(NXThemeColor(themeMode, PrimaryNormal)); }
      else
      {
        painter.setPen(NXThemeColor(themeMode, BasicDetailsText));
      }
      painter.setFont(_editor->font());
      painter.drawText(0, top, width() - 8, _editor->fontMetrics().height(), Qt::AlignRight | Qt::AlignVCenter, number);
    }
    block  = block.next();
    top    = bottom;
    bottom = top + qRound(_editor->getBlockBoundingRect(block).height());
    ++blockNumber;
  }
}
