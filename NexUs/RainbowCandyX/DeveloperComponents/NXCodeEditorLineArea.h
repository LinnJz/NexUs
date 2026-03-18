#ifndef NXCODEEDITORLINEAREA_H
#define NXCODEEDITORLINEAREA_H

#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>
#include <QWidget>

#include "NXTheme.h"
#include "private/NXCodeEditorPrivate.h"

class NXCodePlainTextEdit : public QPlainTextEdit
{
public:
  using QPlainTextEdit::QPlainTextEdit;
  QTextBlock getFirstVisibleBlock() const noexcept;
  QRectF getBlockBoundingGeometry(const QTextBlock& block) const noexcept;
  QRectF getBlockBoundingRect(const QTextBlock& block) const noexcept;
  QPointF getContentOffset() const noexcept;
};

class NXCodeEditorLineArea : public QWidget
{
public:
  explicit NXCodeEditorLineArea(NXCodePlainTextEdit *editor, NXCodeEditorPrivate *d, QWidget *parent = nullptr);
  ~NXCodeEditorLineArea();
  QSize sizeHint() const override;
  int lineAreaWidth() const noexcept;

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  NXCodePlainTextEdit *_editor;
  NXCodeEditorPrivate *_d;
  friend class NXCodeEditorPrivate;
};

#endif // NXCODEEDITORLINEAREA_H
