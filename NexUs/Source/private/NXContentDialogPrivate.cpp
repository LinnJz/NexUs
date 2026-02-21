#include "NXContentDialogPrivate.h"

#include "DeveloperComponents/NXMaskWidget.h"
#include "NXContentDialog.h"

NXContentDialogPrivate::NXContentDialogPrivate(QObject *parent)
    : QObject { parent }
{
}

NXContentDialogPrivate::~NXContentDialogPrivate() { }

void NXContentDialogPrivate::_doCloseAnimation(bool isAccept)
{
  Q_Q(NXContentDialog);
  _maskWidget->doMaskAnimation(0);
  isAccept ? q->accept() : q->reject();
}
