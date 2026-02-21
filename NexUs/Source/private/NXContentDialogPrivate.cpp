#include "NXContentDialogPrivate.h"

#include "DeveloperComponents/NXMaskWidget.h"
#include "NXContentDialog.h"

NXContentDialogPrivate::NXContentDialogPrivate(QObject *parent)
    : QObject { parent }
{
}

NXContentDialogPrivate::~NXContentDialogPrivate() { }

void NXContentDialogPrivate::_doCloseAnimation(int result)
{
  Q_Q(NXContentDialog);
  _maskWidget->doMaskAnimation(0);
  q->done(result);
}
