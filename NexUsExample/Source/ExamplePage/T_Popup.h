#ifndef T_POPUP_H
#define T_POPUP_H

#include "T_BasePage.h"

class NXToolButton;
class NXColorDialog;
class NXInputDialog;
class NXCalendar;
class NXCalendarPicker;
class NXKeyBinder;
class NXRoller;
class NXRollerPicker;
class NXDrawerArea;
class NXMessageCard;
class NXMessageDialog;

class T_Popup : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_Popup(QWidget *parent = nullptr);
  ~T_Popup() override;

private:
  NXToolButton *_toolButton { nullptr };
  NXColorDialog *_colorDialog { nullptr };
  NXCalendar *_calendar { nullptr };
  NXCalendarPicker *_calendarPicker { nullptr };
  NXKeyBinder *_keyBinder { nullptr };
  NXRoller *_roller { nullptr };
  NXRollerPicker *_timeRollerPicker { nullptr };
  NXRollerPicker *_clockRollerPicker { nullptr };
  NXDrawerArea *_drawer { nullptr };
  NXMessageCard *_messageCard { nullptr };
  NXMessageDialog *_messageDialog { nullptr };
};

#endif // T_POPUP_H
