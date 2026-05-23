#include "NXContentDialog.h"

#include <NXPushButton.h>

#include <QApplication>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QScreen>
#include <QTimer>
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
  d->_leftButton = new NXPushButton(QStringLiteral("cancel"), this);
  connect(d->_leftButton, &NXPushButton::clicked, this, [=]()
  {
    d->_doCloseAnimation(false);
    Q_EMIT leftButtonClicked();
  });
  d->_leftButton->setMinimumSize(0, 0);
  d->_leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
  d->_leftButton->setFixedHeight(38);
  d->_leftButton->setBorderRadius(6);
  d->_middleButton = new NXPushButton(QStringLiteral("minimum"), this);
  connect(d->_middleButton, &NXPushButton::clicked, this, [=]()
  {
    Q_EMIT middleButtonClicked();
  });
  d->_middleButton->setMinimumSize(0, 0);
  d->_middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
  d->_middleButton->setFixedHeight(38);
  d->_middleButton->setBorderRadius(6);
  d->_rightButton = new NXPushButton(QStringLiteral("exit"), this);
  connect(d->_rightButton, &NXPushButton::clicked, this, [=]()
  {
    d->_doCloseAnimation(true);
    Q_EMIT rightButtonClicked();
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
  NXText *title = new NXText(QStringLiteral("退出"), this);
  title->setTextStyle(NXTextType::Title);
  NXText *subTitle = new NXText(QStringLiteral("确定要退出程序吗"), this);
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
  connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode)
  {
    d->_themeMode = themeMode;
  });
}

NXContentDialog::~NXContentDialog()
{
  Q_D(NXContentDialog);
  d->_maskWidget->deleteLater();
}

void
NXContentDialog::setCentralWidget(QWidget *centralWidget)
{
  Q_D(NXContentDialog);
  d->_mainLayout->takeAt(0);
  d->_mainLayout->takeAt(0);
  delete d->_centralWidget;
  d->_centralWidget = centralWidget;
  d->_mainLayout->addWidget(centralWidget);
  d->_mainLayout->addWidget(d->_buttonWidget);
}

NXAppBar *
NXContentDialog::appBar() const noexcept
{
  return d_func()->_appBar;
}

void
NXContentDialog::setLeftButtonText(const QString &text) noexcept
{
  Q_D(NXContentDialog);
  d->_leftButton->setText(text);
}

void
NXContentDialog::setMiddleButtonText(const QString &text) noexcept
{
  Q_D(NXContentDialog);
  d->_middleButton->setText(text);
}

void
NXContentDialog::setRightButtonText(const QString &text) noexcept
{
  Q_D(NXContentDialog);
  d->_rightButton->setText(text);
}

void
NXContentDialog::setLeftButtonVisible(bool visible) noexcept
{
  Q_D(NXContentDialog);
  d->_leftButton->setVisible(visible);
}

void
NXContentDialog::setMiddleButtonVisible(bool visible) noexcept
{
  Q_D(NXContentDialog);
  d->_middleButton->setVisible(visible);
}

void
NXContentDialog::setRightButtonVisible(bool visible) noexcept
{
  Q_D(NXContentDialog);
  d->_rightButton->setVisible(visible);
}

void
NXContentDialog::showEvent(QShowEvent *event)
{
  Q_D(NXContentDialog);
  d->_maskWidget->setVisible(true);
  d->_maskWidget->raise();
  d->_maskWidget->setFixedSize(parentWidget()->size());
  d->_maskWidget->doMaskAnimation(90);
  d->_moveToCenter();
  QDialog::showEvent(event);
}

void
NXContentDialog::paintEvent(QPaintEvent *event)
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

void
NXContentDialog::keyPressEvent(QKeyEvent *event)
{
  Q_D(NXContentDialog);
  switch (event->key())
  {
  case Qt::Key_Escape :
  {
    d->_doCloseAnimation(false);
    break;
  }
  default :
  {
    break;
  }
  }
  event->accept();
}
