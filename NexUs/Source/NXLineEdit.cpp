#include "NXLineEdit.h"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "DeveloperComponents/NXLineEditStyle.h"
#include "NXEventBus.h"
#include "NXMenu.h"
#include "NXTheme.h"
#include "private/NXLineEditPrivate.h"

NXLineEdit::NXLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , d_ptr(new NXLineEditPrivate())
{
  Q_D(NXLineEdit);
  d->q_ptr = this;
  setObjectName("NXLineEdit");
  setFixedHeight(35);
  d->_themeMode             = nxTheme->getThemeMode();
  d->_pExpandMarkWidth      = 0;
  d->_pIsClearButtonEnabled = true;
  setFocusPolicy(Qt::StrongFocus);
  // 事件总线
  d->_focusEvent = new NXEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
  d->_focusEvent->registerAndInit();
  setMouseTracking(true);
  QFont textFont = font();
  textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
  setFont(textFont);
  d->_lineEditStyle = new NXLineEditStyle(style());
  setStyle(d->_lineEditStyle);
  setStyleSheet("#NXLineEdit{background-color:transparent;padding-left: 10px;}");
  d->onThemeChanged(nxTheme->getThemeMode());
  connect(nxTheme, &NXTheme::themeModeChanged, d, &NXLineEditPrivate::onThemeChanged);
  setVisible(true);
}

NXLineEdit::NXLineEdit(const QString& text, QWidget *parent)
    : NXLineEdit(parent)
{
  setText(text);
}

NXLineEdit::~NXLineEdit() { delete this->style(); }

void NXLineEdit::setIsClearButtonEnabled(bool isClearButtonEnabled)
{
  Q_D(NXLineEdit);
  d->_pIsClearButtonEnabled = isClearButtonEnabled;
  setClearButtonEnabled(isClearButtonEnabled);
  Q_EMIT pIsClearButtonEnabledChanged();
}

bool NXLineEdit::getIsClearButtonEnabled() const
{
  Q_D(const NXLineEdit);
  return d->_pIsClearButtonEnabled;
}

void NXLineEdit::setBorderRadius(int borderRadius)
{
  Q_D(const NXLineEdit);
  d->_lineEditStyle->setLineEditBorderRadius(borderRadius);
}

int NXLineEdit::getBorderRadius() const
{
  Q_D(const NXLineEdit);
  return d->_lineEditStyle->getLineEditBorderRadius();
}

void NXLineEdit::setContentsPaddings(int left, int top, int right, int bottom)
{
  Q_D(NXLineEdit);
  d->_paddings       = { left, top, right, bottom };
  QString styleSheet = QString(QStringLiteral("#NXLineEdit { "
                                              "padding-left: %1px; "
                                              "padding-top: %2px; "
                                              "padding-right: %3px; "
                                              "padding-bottom: %4px; "
                                              "}"))
                           .arg(left)
                           .arg(top)
                           .arg(right)
                           .arg(bottom);

  setStyleSheet(styleSheet);
}

QMargins NXLineEdit::getContentsPaddings() const
{
  Q_D(const NXLineEdit);
  return d->_paddings;
}

void NXLineEdit::setLineEditIconMargin(int margin)
{
  Q_D(NXLineEdit);
  d->_lineEditStyle->setLineEditIconMargin(margin);
}

int NXLineEdit::getLineEditIconMargin() const
{
  Q_D(const NXLineEdit);
  return d->_lineEditStyle->getLineEditIconMargin();
}

void NXLineEdit::focusInEvent(QFocusEvent *event)
{
  Q_D(NXLineEdit);
  Q_EMIT focusIn(this->text());
  if (event->reason() == Qt::MouseFocusReason)
  {
    if (d->_pIsClearButtonEnabled) { setClearButtonEnabled(true); }
    QPropertyAnimation *markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
    connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { update(); });
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(d->_pExpandMarkWidth);
    markAnimation->setEndValue(width() / 2 - d->_lineEditStyle->getLineEditBorderRadius() / 2);
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  }
  QLineEdit::focusInEvent(event);
}

void NXLineEdit::focusOutEvent(QFocusEvent *event)
{
  Q_D(NXLineEdit);
  Q_EMIT focusOut(this->text());
  if (event->reason() != Qt::PopupFocusReason)
  {
    if (d->_pIsClearButtonEnabled) { setClearButtonEnabled(false); }
    QPropertyAnimation *markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
    connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { update(); });
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(d->_pExpandMarkWidth);
    markAnimation->setEndValue(0);
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    Q_EMIT wmFocusOut(text());
  }
  QLineEdit::focusOutEvent(event);
}

void NXLineEdit::paintEvent(QPaintEvent *event)
{
  Q_D(NXLineEdit);
  if (palette().color(QPalette::Text) != NXThemeColor(d->_themeMode, BasicText)) { d->onThemeChanged(d->_themeMode); }
  QLineEdit::paintEvent(event);
  QPainter painter(this);
  painter.save();
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(NXThemeColor(d->_themeMode, PrimaryNormal));
  painter.drawRoundedRect(
      QRectF(width() / 2 - d->_pExpandMarkWidth, height() - 2.5, d->_pExpandMarkWidth * 2, 2.5), 2, 2);
  painter.restore();
}

void NXLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
  NXMenu *menu = new NXMenu(this);
  menu->setMenuItemHeight(27);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  QAction *action = nullptr;
  if (!isReadOnly())
  {
    action = menu->addNXIconAction(NXIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
    action->setEnabled(isUndoAvailable());
    connect(action, &QAction::triggered, this, &NXLineEdit::undo);

    action = menu->addNXIconAction(NXIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
    action->setEnabled(isRedoAvailable());
    connect(action, &QAction::triggered, this, &NXLineEdit::redo);
    menu->addSeparator();
  }
#ifndef QT_NO_CLIPBOARD
  if (!isReadOnly())
  {
    action = menu->addNXIconAction(NXIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
    action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, this, &NXLineEdit::cut);
  }

  action = menu->addNXIconAction(NXIconType::Copy, tr("复制"), QKeySequence::Copy);
  action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
  connect(action, &QAction::triggered, this, &NXLineEdit::copy);

  if (!isReadOnly())
  {
    action = menu->addNXIconAction(NXIconType::Paste, tr("粘贴"), QKeySequence::Paste);
    action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
    connect(action, &QAction::triggered, this, &NXLineEdit::paste);
  }
#endif
  if (!isReadOnly())
  {
    action = menu->addNXIconAction(NXIconType::DeleteLeft, tr("删除"));
    action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
    connect(action,
            &QAction::triggered,
            this,
            [=](bool checked)
    {
      if (hasSelectedText())
      {
        int startIndex = selectionStart();
        int endIndex   = selectionEnd();
        setText(text().remove(startIndex, endIndex - startIndex));
      }
    });
  }
  if (!menu->isEmpty()) { menu->addSeparator(); }
  action = menu->addAction(tr("全选"));
  action->setShortcut(QKeySequence::SelectAll);
  action->setEnabled(!text().isEmpty() && !(selectedText() == text()));
  connect(action, &QAction::triggered, this, &NXLineEdit::selectAll);
  menu->popup(event->globalPos());
}
