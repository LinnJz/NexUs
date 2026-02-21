#include "NXContentDialog.h"

#include <NXPushButton.h>

#include <QApplication>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QScreen>
#include <QVBoxLayout>
#include "DeveloperComponents/NXMaskWidget.h"
#include "DeveloperComponents/NXWinShadowHelper.h"
#include "NXText.h"
#include "NXTheme.h"
#include "private/NXContentDialogPrivate.h"

Q_TAKEOVER_NATIVEEVENT_CPP(NXContentDialog, d_func()->_appBar);

NXContentDialog::NXContentDialog(QWidget *parent)
    : QDialog { parent }
    , d_ptr(new NXContentDialogPrivate())
{
  Q_D(NXContentDialog);
  d->q_ptr = this;

  d->_maskWidget = new NXMaskWidget(parent);
  d->_maskWidget->move(0, 0);
  d->_maskWidget->setFixedSize(parent->size());
  d->_maskWidget->setVisible(false);

  resize(400, height());
  setWindowModality(Qt::ApplicationModal);
  d->_appBar = new NXAppBar(this);
  d->_appBar->setWindowButtonFlags(NXAppBarType::NoneButtonHint);
  d->_appBar->setIsFixedSize(true);
  d->_appBar->setAppBarHeight(0);
#ifdef Q_OS_WIN
  // 防止意外拉伸
  createWinId();
#endif
  d->_leftButton = new NXPushButton("cancel", this);
  connect(d->_leftButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    Q_EMIT buttonClicked(ButtonType::LeftButton);
    doneWithAnimation(getButtonDoneCode(ButtonType::LeftButton));
  });
  d->_leftButton->setMinimumSize(0, 0);
  d->_leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
  d->_leftButton->setFixedHeight(38);
  d->_leftButton->setBorderRadius(6);
  d->_middleButton = new NXPushButton("minimum", this);
  connect(d->_middleButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    Q_EMIT buttonClicked(ButtonType::MiddleButton);
    doneWithAnimation(getButtonDoneCode(ButtonType::MiddleButton));
  });
  d->_middleButton->setMinimumSize(0, 0);
  d->_middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
  d->_middleButton->setFixedHeight(38);
  d->_middleButton->setBorderRadius(6);
  d->_rightButton = new NXPushButton("exit", this);
  connect(d->_rightButton,
          &NXPushButton::clicked,
          this,
          [=]()
  {
    Q_EMIT buttonClicked(ButtonType::RightButton);
    doneWithAnimation(getButtonDoneCode(ButtonType::RightButton));
  });
  d->_rightButton->setLightDefaultColor(NXThemeColor(NXThemeType::Light, PrimaryNormal));
  d->_rightButton->setLightHoverColor(NXThemeColor(NXThemeType::Light, PrimaryHover));
  d->_rightButton->setLightPressColor(NXThemeColor(NXThemeType::Light, PrimaryPress));
  d->_rightButton->setLightTextColor(Qt::white);
  d->_rightButton->setDarkDefaultColor(NXThemeColor(NXThemeType::Dark, PrimaryNormal));
  d->_rightButton->setDarkHoverColor(NXThemeColor(NXThemeType::Dark, PrimaryHover));
  d->_rightButton->setDarkPressColor(NXThemeColor(NXThemeType::Dark, PrimaryPress));
  d->_rightButton->setDarkTextColor(Qt::black);
  d->_rightButton->setMinimumSize(0, 0);
  d->_rightButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
  d->_rightButton->setFixedHeight(38);
  d->_rightButton->setBorderRadius(6);

  d->_centralWidget           = new QWidget(this);
  QVBoxLayout *centralVLayout = new QVBoxLayout(d->_centralWidget);
  centralVLayout->setContentsMargins(15, 25, 15, 10);
  NXText *title = new NXText("退出", this);
  title->setTextStyle(NXTextType::Title);
  NXText *subTitle = new NXText("确定要退出程序吗", this);
  subTitle->setTextStyle(NXTextType::Body);
  centralVLayout->addWidget(title);
  centralVLayout->addSpacing(2);
  centralVLayout->addWidget(subTitle);
  centralVLayout->addStretch();

  d->_mainLayout = new QVBoxLayout(this);
  d->_mainLayout->setContentsMargins(0, 0, 0, 0);
  d->_buttonWidget = new QWidget(this);
  d->_buttonWidget->setFixedHeight(60);
  d->_buttonLayout = new QHBoxLayout(d->_buttonWidget);
  d->_buttonLayout->addWidget(d->_leftButton);
  d->_buttonLayout->addWidget(d->_middleButton);
  d->_buttonLayout->addWidget(d->_rightButton);
  d->_mainLayout->addWidget(d->_centralWidget);
  d->_mainLayout->addWidget(d->_buttonWidget);

  d->_themeMode = nxTheme->getThemeMode();
  connect(
      nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

NXContentDialog::~NXContentDialog()
{
  Q_D(NXContentDialog);
  d->_maskWidget->deleteLater();
}

void NXContentDialog::setCentralWidget(QWidget *centralWidget)
{
  Q_D(NXContentDialog);
  if (!centralWidget || centralWidget == d->_centralWidget) { return; }

  centralWidget->setParent(this);

  if (d->_centralWidget)
  {
    d->_mainLayout->removeWidget(d->_centralWidget);
    delete d->_centralWidget;
    d->_centralWidget = nullptr;
  }

  d->_centralWidget = centralWidget;
  d->_mainLayout->insertWidget(0, d->_centralWidget);
}

NXPushButton *NXContentDialog::leftButton() const { return button(ButtonType::LeftButton); }

NXPushButton *NXContentDialog::middleButton() const { return button(ButtonType::MiddleButton); }

NXPushButton *NXContentDialog::rightButton() const { return button(ButtonType::RightButton); }

NXPushButton *NXContentDialog::button(ButtonType button) const
{
  Q_D(const NXContentDialog);
  switch (button)
  {
    case ButtonType::LeftButton   : return d->_leftButton;
    case ButtonType::MiddleButton : return d->_middleButton;
    case ButtonType::RightButton  : return d->_rightButton;
  }
  return nullptr;
}

void NXContentDialog::setButtonText(ButtonType buttonType, const QString& text)
{
  auto *buttonWidget = button(buttonType);
  if (!buttonWidget) { return; }
  buttonWidget->setText(text);
}

QString NXContentDialog::getButtonText(ButtonType buttonType) const
{
  auto *buttonWidget = button(buttonType);
  return buttonWidget ? buttonWidget->text() : QString {};
}

void NXContentDialog::setIsButtonVisible(ButtonType buttonType, bool visible)
{
  auto *buttonWidget = button(buttonType);
  if (!buttonWidget) { return; }
  buttonWidget->setVisible(visible);
}

bool NXContentDialog::getIsButtonVisible(ButtonType buttonType) const
{
  auto *buttonWidget = button(buttonType);
  return buttonWidget ? buttonWidget->isVisible() : false;
}

void NXContentDialog::setButtonDoneCode(ButtonType buttonType, int doneCode)
{
  Q_D(NXContentDialog);
  switch (buttonType)
  {
    case ButtonType::LeftButton   : d->_leftButtonDoneCode = doneCode; break;
    case ButtonType::MiddleButton : d->_middleButtonDoneCode = doneCode; break;
    case ButtonType::RightButton  : d->_rightButtonDoneCode = doneCode; break;
    default :
#if _WIN32
      __assume(false);
#elif __linux__ || __unix__ || __APPLE__
      __builtin_unreachable();
#else
      break
#endif
  }
}

int NXContentDialog::getButtonDoneCode(ButtonType buttonType) const
{
  Q_D(const NXContentDialog);
  switch (buttonType)
  {
    case ButtonType::LeftButton   : return d->_leftButtonDoneCode;
    case ButtonType::MiddleButton : return d->_middleButtonDoneCode;
    case ButtonType::RightButton  : return d->_rightButtonDoneCode;
    default                       : return DialogCode::Rejected;
  }
}

void NXContentDialog::doneWithAnimation(int code)
{
  Q_D(NXContentDialog);
  d->_doCloseAnimation(code);
}

NXAppBar *NXContentDialog::appBar() const
{
  Q_D(const NXContentDialog);
  return d->_appBar;
}

void NXContentDialog::showEvent(QShowEvent *event)
{
  Q_D(NXContentDialog);
  d->_maskWidget->setVisible(true);
  d->_maskWidget->raise();
  d->_maskWidget->setFixedSize(parentWidget()->size());
  d->_maskWidget->doMaskAnimation(90);
  QDialog::showEvent(event);
}

void NXContentDialog::paintEvent(QPaintEvent *event)
{
  Q_D(NXContentDialog);
  QPainter painter(this);
  painter.save();
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(NXThemeColor(d->_themeMode, DialogBase));
  // 背景绘制
  painter.drawRect(rect());
  // 按钮栏背景绘制
  painter.setBrush(NXThemeColor(d->_themeMode, DialogLayoutArea));
  painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
  painter.restore();
}

void NXContentDialog::keyPressEvent(QKeyEvent *event) { event->accept(); }
