#ifndef T_NEWCOMPONENTS2_H
#define T_NEWCOMPONENTS2_H

#include "T_BasePage.h"

class NXChatBubble;
class NXDashboardGauge;
class NXTerminalWidget;

class T_NewComponents2 : public T_BasePage
{
  Q_OBJECT

public:
  Q_INVOKABLE explicit T_NewComponents2(QWidget *parent = nullptr);
  ~T_NewComponents2();

private:
  // ChatBubble
  NXChatBubble *_chatBubbleLeft { nullptr };
  NXChatBubble *_chatBubbleRight { nullptr };
  NXChatBubble *_chatBubbleSystem { nullptr };
  NXChatBubble *_chatBubbleImage { nullptr };
  // DashboardGauge
  NXDashboardGauge *_gauge { nullptr };
  NXDashboardGauge *_gaugeSmall { nullptr };
  // TerminalWidget
  NXTerminalWidget *_terminal { nullptr };
};

#endif // T_NEWCOMPONENTS2_H
