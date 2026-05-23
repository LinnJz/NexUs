#include "NXContentDialogPrivate.h"

#include "NXContentDialog.h"
#include "NXMaskWidget.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>
#include <QWindow>

NXContentDialogPrivate::NXContentDialogPrivate(QObject *parent)
    : QObject { parent }
{
}

NXContentDialogPrivate::~NXContentDialogPrivate()
{
}

void
NXContentDialogPrivate::_doCloseAnimation(bool isAccept)
{
  Q_Q(NXContentDialog);
  _maskWidget->doMaskAnimation(0);
  isAccept ? q->accept() : q->reject();
}

void
NXContentDialogPrivate::_moveToCenter() noexcept
{
  Q_Q(NXContentDialog);
  int width      = q->width();
  int height     = q->height();
  auto globalPos = _maskWidget->mapToGlobal(QPoint{ 0, 0 });
  q->setGeometry(globalPos.x() + (_maskWidget->width() - width) / 2,
                 globalPos.y() + (_maskWidget->height() - height) / 2, width, height);
}
