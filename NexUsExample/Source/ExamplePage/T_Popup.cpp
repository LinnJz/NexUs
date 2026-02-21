#include "T_Popup.h"

#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "NXCalendar.h"
#include "NXCalendarPicker.h"
#include "NXCheckBox.h"
#include "NXColorDialog.h"
#include "NXDrawerArea.h"
#include "NXInputDialog.h"
#include "NXKeyBinder.h"
#include "NXMenu.h"
#include "NXMessageCard.h"
#include "NXMessageDialog.h"
#include "NXPushButton.h"
#include "NXRoller.h"
#include "NXRollerPicker.h"
#include "NXScrollPageArea.h"
#include "NXText.h"
#include "NXToggleSwitch.h"
#include "NXToolButton.h"

T_Popup::T_Popup(QWidget *parent)
    : T_BasePage(parent)
{
  // 预览窗口标题
  setWindowTitle("NXPopup");

  // 顶部元素
  createCustomWidget("一些常用的弹出组件被放置于此，可在此界面体验其效果并按需添加进项目中");

  QWidget *centralWidget = new QWidget(this);
  centralWidget->setWindowTitle("NXPopup");

  _toolButton = new NXToolButton(this);
  _toolButton->setIsTransparent(false);
  _toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
  _toolButton->setText("NXToolButton");

  NXMenu *menu = new NXMenu(this);
  menu->addNXIconAction(NXIconType::JackOLantern, "JackOLantern");
  menu->addNXIconAction(NXIconType::LacrosseStick, "LacrosseStick");
  _toolButton->setMenu(menu);
  _toolButton->setNXIcon(NXIconType::Broom);

  NXScrollPageArea *toolButtonArea = new NXScrollPageArea(this);
  QHBoxLayout *toolButtonLayout    = new QHBoxLayout(toolButtonArea);
  NXText *toolButtonText           = new NXText("NXToolButton", this);
  toolButtonText->setTextPixelSize(15);
  toolButtonLayout->addWidget(toolButtonText);
  toolButtonLayout->addWidget(_toolButton);
  toolButtonLayout->addStretch();

  _colorDialog      = new NXColorDialog(this);
  NXText *colorText = new NXText(_colorDialog->getCurrent4ChannelColor(), this);
  colorText->setTextPixelSize(15);
  NXPushButton *colorDialogButton = new NXPushButton(this);
  colorDialogButton->setFixedSize(35, 35);
  colorDialogButton->setLightDefaultColor(_colorDialog->getCurrentColor());
  colorDialogButton->setLightHoverColor(_colorDialog->getCurrentColor());
  colorDialogButton->setLightPressColor(_colorDialog->getCurrentColor());
  colorDialogButton->setDarkDefaultColor(_colorDialog->getCurrentColor());
  colorDialogButton->setDarkHoverColor(_colorDialog->getCurrentColor());
  colorDialogButton->setDarkPressColor(_colorDialog->getCurrentColor());
  connect(colorDialogButton, &NXPushButton::clicked, this, [=]() { _colorDialog->exec(); });
  connect(_colorDialog,
          &NXColorDialog::colorSelected,
          this,
          [=](const QColor& color)
  {
    colorDialogButton->setLightDefaultColor(color);
    colorDialogButton->setLightHoverColor(color);
    colorDialogButton->setLightPressColor(color);
    colorDialogButton->setDarkDefaultColor(color);
    colorDialogButton->setDarkHoverColor(color);
    colorDialogButton->setDarkPressColor(color);
    colorText->setText(_colorDialog->getCurrent4ChannelColor());
  });

  NXScrollPageArea *colorDialogArea = new NXScrollPageArea(this);
  QHBoxLayout *colorDialogLayout    = new QHBoxLayout(colorDialogArea);
  NXText *colorDialogText           = new NXText("NXColorDialog", this);
  colorDialogText->setTextPixelSize(15);
  colorDialogLayout->addWidget(colorDialogText);
  colorDialogLayout->addWidget(colorDialogButton);
  colorDialogLayout->addWidget(colorText);
  colorDialogLayout->addStretch();

  NXScrollPageArea *inputDialogArea = new NXScrollPageArea(this);
  QHBoxLayout *inputDialogLayout    = new QHBoxLayout(inputDialogArea);
  NXText *inputDialogText           = new NXText("NXInputDialog", this);
  inputDialogText->setTextPixelSize(15);
  inputDialogLayout->addWidget(inputDialogText);

  NXPushButton *textInputButton = new NXPushButton("文本", this);
  textInputButton->setFixedSize(80, 38);
  connect(textInputButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    bool ok;
    QString text = NXInputDialog::getText(this, "输入对话框", "请输入您的信息", "名称:", "", &ok);
    if (ok && !text.isEmpty()) { qDebug() << "文本输入:" << text; }
  });
  inputDialogLayout->addWidget(textInputButton);

  NXPushButton *intInputButton = new NXPushButton("整数", this);
  intInputButton->setFixedSize(80, 38);
  connect(intInputButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    bool ok;
    int value = NXInputDialog::getInt(this, "输入年龄", "请提供您的个人信息", "年龄:", 18, 0, 150, 1, &ok);
    if (ok) { qDebug() << "整数输入:" << value; }
  });
  inputDialogLayout->addWidget(intInputButton);

  NXPushButton *doubleInputButton = new NXPushButton("小数", this);
  doubleInputButton->setFixedSize(80, 38);
  connect(doubleInputButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    bool ok;
    double value = NXInputDialog::getDouble(this, "输入价格", "商品定价系统", "价格:", 99.99, 0.0, 9999.99, 2, &ok);
    if (ok) { qDebug() << "小数输入:" << value; }
  });
  inputDialogLayout->addWidget(doubleInputButton);

  NXPushButton *multiLineInputButton = new NXPushButton("多行", this);
  multiLineInputButton->setFixedSize(80, 38);
  connect(multiLineInputButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    bool ok;
    QString text = NXInputDialog::getMultiLineText(this, "输入备注", "请详细描述您的需求", "详细描述:", "", &ok);
    if (ok && !text.isEmpty()) { qDebug() << "多行输入:" << text; }
  });
  inputDialogLayout->addWidget(multiLineInputButton);
  inputDialogLayout->addStretch();

  _calendar = new NXCalendar(this);

  _calendarPicker                      = new NXCalendarPicker(this);
  NXScrollPageArea *calendarPickerArea = new NXScrollPageArea(this);
  QHBoxLayout *calendarPickerLayout    = new QHBoxLayout(calendarPickerArea);
  NXText *calendarPickerText           = new NXText("NXCalendarPicker", this);
  calendarPickerText->setTextPixelSize(15);
  calendarPickerLayout->addWidget(calendarPickerText);
  calendarPickerLayout->addWidget(_calendarPicker);
  calendarPickerLayout->addStretch();

  _keyBinder                      = new NXKeyBinder(this);
  NXScrollPageArea *keyBinderArea = new NXScrollPageArea(this);
  QHBoxLayout *keyBinderLayout    = new QHBoxLayout(keyBinderArea);
  NXText *keyBinderText           = new NXText("NXKeyBinder", this);
  keyBinderText->setTextPixelSize(15);
  keyBinderLayout->addWidget(keyBinderText);
  keyBinderLayout->addWidget(_keyBinder);
  keyBinderLayout->addStretch();

  _roller = new NXRoller(this);
  QStringList rollerItemList;
  for (int i = 0; i < 100; i++) { rollerItemList.append(QString::number(i + 1)); }
  _roller->setItemList(rollerItemList);
  NXScrollPageArea *rollerArea = new NXScrollPageArea(this);
  rollerArea->setFixedHeight(220);
  QHBoxLayout *rollerLayout = new QHBoxLayout(rollerArea);
  NXText *rollerText        = new NXText("NXRoller", this);
  rollerText->setTextPixelSize(15);
  rollerLayout->addWidget(rollerText);
  rollerLayout->addWidget(_roller);
  rollerLayout->addSpacing(30);

  NXText *rollerPickerText = new NXText("NXRollerPicker", this);
  rollerPickerText->setTextPixelSize(15);
  rollerLayout->addWidget(rollerPickerText);

  QTime currentTime     = QTime::currentTime();
  QString currentHour   = QString("%1").arg(currentTime.hour(), 2, 10, QChar('0'));
  QString currentMinute = QString("%1").arg(currentTime.minute(), 2, 10, QChar('0'));
  _timeRollerPicker     = new NXRollerPicker(this);
  QStringList hourItemList;
  for (int i = 0; i < 24; i++) { hourItemList.append(QString("%1").arg(i, 2, 10, QChar('0'))); }
  QStringList minuteList;
  for (int i = 0; i < 61; i++) { minuteList.append(QString("%1").arg(i, 2, 10, QChar('0'))); }
  _timeRollerPicker->addRoller(hourItemList);
  _timeRollerPicker->addRoller(minuteList);
  _timeRollerPicker->addRoller({ "AM", "PM" }, false);
  _timeRollerPicker->setCurrentData({ currentHour, currentMinute, currentTime.hour() >= 12 ? "PM" : "AM" });

  _clockRollerPicker = new NXRollerPicker(this);
  _clockRollerPicker->addRoller(hourItemList);
  _clockRollerPicker->addRoller(minuteList);
  _clockRollerPicker->setRollerWidth(0, 135);
  _clockRollerPicker->setRollerWidth(1, 135);
  _clockRollerPicker->setCurrentData({ currentHour, currentMinute });

  QVBoxLayout *rollerPickerLayout = new QVBoxLayout();
  rollerPickerLayout->addWidget(_timeRollerPicker);
  rollerPickerLayout->addWidget(_clockRollerPicker);
  rollerLayout->addLayout(rollerPickerLayout);
  rollerLayout->addStretch();

  _drawer                         = new NXDrawerArea(this);
  QWidget *drawerHeader           = new QWidget(this);
  QHBoxLayout *drawerHeaderLayout = new QHBoxLayout(drawerHeader);
  NXText *drawerIcon              = new NXText(this);
  drawerIcon->setTextPixelSize(15);
  drawerIcon->setNXIcon(NXIconType::MessageArrowDown);
  drawerIcon->setFixedSize(25, 25);
  NXText *drawerText = new NXText("NXDrawer", this);
  drawerText->setTextPixelSize(15);

  NXToggleSwitch *drawerSwitch = new NXToggleSwitch(this);
  NXText *drawerSwitchText     = new NXText("关", this);
  drawerSwitchText->setTextPixelSize(15);
  connect(drawerSwitch,
          &NXToggleSwitch::toggled,
          this,
          [=](bool toggled)
  {
    if (toggled)
    {
      drawerSwitchText->setText("开");
      _drawer->expand();
    }
    else
    {
      drawerSwitchText->setText("关");
      _drawer->collapse();
    }
  });
  connect(
      _drawer, &NXDrawerArea::expandStateChanged, this, [=](bool isExpand) { drawerSwitch->setIsToggled(isExpand); });

  drawerHeaderLayout->addWidget(drawerIcon);
  drawerHeaderLayout->addWidget(drawerText);
  drawerHeaderLayout->addStretch();
  drawerHeaderLayout->addWidget(drawerSwitchText);
  drawerHeaderLayout->addWidget(drawerSwitch);

  _drawer->setDrawerHeader(drawerHeader);
  QWidget *drawerWidget1 = new QWidget(this);
  drawerWidget1->setFixedHeight(75);
  QHBoxLayout *drawerWidget1Layout = new QHBoxLayout(drawerWidget1);
  NXCheckBox *drawerCheckBox1      = new NXCheckBox("测试窗口1", this);
  drawerWidget1Layout->addSpacing(60);
  drawerWidget1Layout->addWidget(drawerCheckBox1);

  QWidget *drawerWidget2 = new QWidget(this);
  drawerWidget2->setFixedHeight(75);
  QHBoxLayout *drawerWidget2Layout = new QHBoxLayout(drawerWidget2);
  NXCheckBox *drawerCheckBox2      = new NXCheckBox("测试窗口2", this);
  drawerWidget2Layout->addSpacing(60);
  drawerWidget2Layout->addWidget(drawerCheckBox2);

  QWidget *drawerWidget3 = new QWidget(this);
  drawerWidget3->setFixedHeight(75);
  QHBoxLayout *drawerWidget3Layout = new QHBoxLayout(drawerWidget3);
  NXCheckBox *drawerCheckBox3      = new NXCheckBox("测试窗口3", this);
  drawerWidget3Layout->addSpacing(60);
  drawerWidget3Layout->addWidget(drawerCheckBox3);

  _drawer->addDrawer(drawerWidget1);
  _drawer->addDrawer(drawerWidget2);
  _drawer->addDrawer(drawerWidget3);

  // NXMessageCard 示例
  _messageCard = new NXMessageCard(this);
  _messageCard->setIsCompleted(true);
  _messageCard->setTitle("Test");
  _messageCard->setContent("相信回旋吧，只管相信就是了!");
  _messageCard->setFixedSize(250, 75);
  _messageCard->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
  _messageCard->setAttribute(Qt::WA_TranslucentBackground);

  NXScrollPageArea *messageCardArea = new NXScrollPageArea(this);
  QHBoxLayout *messageCardLayout    = new QHBoxLayout(messageCardArea);
  NXText *messageCardText           = new NXText("NXMessageCard", this);
  messageCardText->setTextPixelSize(15);
  messageCardLayout->addWidget(messageCardText);

  NXPushButton *showMessageCardButton = new NXPushButton("显示浮出控件", this);
  showMessageCardButton->setFixedSize(120, 38);
  connect(showMessageCardButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    QPoint globalPos = showMessageCardButton->mapToGlobal(QPoint(0, showMessageCardButton->height() + 5));
    _messageCard->move(globalPos);
    _messageCard->show();
  });
  messageCardLayout->addWidget(showMessageCardButton);
  messageCardLayout->addStretch();

  // NXMessageDialog 示例
  _messageDialog = new NXMessageDialog(this);
  _messageDialog->setTitle("标题");
  _messageDialog->setContent("左眼用来忘记你、右眼用来记忆你。");
  _messageDialog->setFixedSize(280, 150);
  _messageDialog->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
  _messageDialog->setAttribute(Qt::WA_TranslucentBackground);

  // 连接信号以判断点击了哪个按钮
  connect(_messageDialog, &NXMessageDialog::confirmed, this, [=]() { qDebug() << "确认按钮被点击"; });
  connect(_messageDialog, &NXMessageDialog::cancelled, this, [=]() { qDebug() << "取消按钮被点击"; });

  NXScrollPageArea *messageDialogArea = new NXScrollPageArea(this);
  QHBoxLayout *messageDialogLayout    = new QHBoxLayout(messageDialogArea);
  NXText *messageDialogText           = new NXText("NXMessageDialog", this);
  messageDialogText->setTextPixelSize(15);
  messageDialogLayout->addWidget(messageDialogText);

  NXPushButton *showMessageDialogButton = new NXPushButton("显示对话框", this);
  showMessageDialogButton->setFixedSize(120, 38);
  connect(showMessageDialogButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    QPoint globalPos = showMessageDialogButton->mapToGlobal(QPoint(0, showMessageDialogButton->height() + 5));
    _messageDialog->move(globalPos);
    _messageDialog->show();
  });
  messageDialogLayout->addWidget(showMessageDialogButton);
  messageDialogLayout->addStretch();

  QVBoxLayout *centerVLayout = new QVBoxLayout(centralWidget);
  centerVLayout->setContentsMargins(0, 0, 0, 0);
  centerVLayout->addWidget(toolButtonArea);
  centerVLayout->addWidget(colorDialogArea);
  centerVLayout->addWidget(inputDialogArea);
  centerVLayout->addWidget(calendarPickerArea);
  centerVLayout->addWidget(_calendar);
  centerVLayout->addWidget(keyBinderArea);
  centerVLayout->addWidget(_drawer);
  centerVLayout->addWidget(messageCardArea);
  centerVLayout->addWidget(messageDialogArea);
  centerVLayout->addWidget(rollerArea);
  centerVLayout->addStretch();
  addCentralWidget(centralWidget, true, false, 0);
}

T_Popup::~T_Popup() { }
