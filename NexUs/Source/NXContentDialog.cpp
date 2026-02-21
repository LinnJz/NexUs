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
    Q_EMIT leftButtonClicked();
    onLeftButtonClicked();
    d->_doCloseAnimation(false);
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
    Q_EMIT middleButtonClicked();
    onMiddleButtonClicked();
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
    Q_EMIT rightButtonClicked();
    onRightButtonClicked();
    d->_doCloseAnimation(true);
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
  QHBoxLayout *buttonLayout = new QHBoxLayout(d->_buttonWidget);
  buttonLayout->addWidget(d->_leftButton);
  buttonLayout->addWidget(d->_middleButton);
  buttonLayout->addWidget(d->_rightButton);
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

void NXContentDialog::onLeftButtonClicked() { }

void NXContentDialog::onMiddleButtonClicked() { }

void NXContentDialog::onRightButtonClicked() { }

void NXContentDialog::setCentralWidget(QWidget *centralWidget)
{
  Q_D(NXContentDialog);
  d->_mainLayout->takeAt(0);
  d->_mainLayout->takeAt(0);
  delete d->_centralWidget;
  d->_centralWidget = centralWidget;
  d->_mainLayout->addWidget(centralWidget);
  d->_mainLayout->addWidget(d->_buttonWidget);
}

void NXContentDialog::setLeftButtonText(const QString& text)
{
  Q_D(NXContentDialog);
  d->_leftButton->setText(text);
}

void NXContentDialog::setMiddleButtonText(const QString& text)
{
  Q_D(NXContentDialog);
  d->_middleButton->setText(text);
}

void NXContentDialog::setRightButtonText(const QString& text)
{
  Q_D(NXContentDialog);
  d->_rightButton->setText(text);
}

void NXContentDialog::setLeftButtonVisible(bool visible)
{
  Q_D(NXContentDialog);
  if (d->_isLeftButtonVisible == visible) { return; }
  d->_isLeftButtonVisible = visible;
  if (visible)
  {
    d->_buttonLayout->insertWidget(0, d->_leftButton);
    d->_leftButton->show();
  }
  else
  {
    d->_buttonLayout->removeWidget(d->_leftButton);
    d->_leftButton->hide();
  }
}

void NXContentDialog::setMiddleButtonVisible(bool visible)
{
  Q_D(NXContentDialog);
  if (d->_isMiddleButtonVisible == visible) { return; }
  d->_isMiddleButtonVisible = visible;
  if (visible)
  {
    d->_buttonLayout->insertWidget(1, d->_middleButton);
    d->_middleButton->show();
  }
  else
  {
    d->_buttonLayout->removeWidget(d->_middleButton);
    d->_middleButton->hide();
  }
}

void NXContentDialog::setRightButtonVisible(bool visible)
{
  Q_D(NXContentDialog);
  if (d->_isRightButtonVisible == visible) { return; }
  d->_isRightButtonVisible = visible;
  if (visible)
  {
    d->_buttonLayout->addWidget(d->_rightButton);
    d->_rightButton->show();
  }
  else
  {
    d->_buttonLayout->removeWidget(d->_rightButton);
    d->_rightButton->hide();
  }
}

void NXContentDialog::close()
{
  Q_D(NXContentDialog);
  d->_doCloseAnimation(false);
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
