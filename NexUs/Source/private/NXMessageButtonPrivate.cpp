#include "NXMessageButtonPrivate.h"

#include <QPalette>

#include "NXMessageBar.h"
#include "NXMessageButton.h"

NXMessageButtonPrivate::NXMessageButtonPrivate(QObject *parent)
    : QObject { parent }
{
}

NXMessageButtonPrivate::~NXMessageButtonPrivate() { }

void NXMessageButtonPrivate::_showMessage()
{
  switch (_pMessageMode)
  {
    case NXMessageBarType::Success :
    {
      NXMessageBar::success(_pPositionPolicy, _pBarTitle, _pBarText, _pDisplayMsec, _pMessageTargetWidget);
      break;
    }
    case NXMessageBarType::Warning :
    {
      NXMessageBar::warning(_pPositionPolicy, _pBarTitle, _pBarText, _pDisplayMsec, _pMessageTargetWidget);
      break;
    }
    case NXMessageBarType::Information :
    {
      NXMessageBar::information(_pPositionPolicy, _pBarTitle, _pBarText, _pDisplayMsec, _pMessageTargetWidget);
      break;
    }
    case NXMessageBarType::Error :
    {
      NXMessageBar::error(_pPositionPolicy, _pBarTitle, _pBarText, _pDisplayMsec, _pMessageTargetWidget);
      break;
    }
    default :
    {
      break;
    }
  }
}
