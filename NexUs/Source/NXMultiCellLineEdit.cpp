#include "NXMultiCellLineEdit.h"

#include <algorithm>

#include <QAction>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QDebug>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>
#include <QStyleOption>

#include "DeveloperComponents/NXMultiCellLineEditStyle.h"
#include "NXEventBus.h"
#include "NXMenu.h"
#include "NXTheme.h"
#include "private/NXMultiCellLineEditPrivate.h"

namespace
{
  constexpr const char *CLEAR_ALLOWED_PROPERTY = "NXMultiCellClearAllowed";

  int NormalizeCellIndex(const QList<QLineEdit *>& cells, int cellIndex, const char *actionName)
  {
    if (cells.isEmpty())
    {
      qWarning() << "NXMultiCellLineEdit" << actionName << "cell list empty";
      return -1;
    }
    if (cellIndex >= 0 && cellIndex < cells.size()) { return cellIndex; }
    qWarning() << "NXMultiCellLineEdit" << actionName << "cellIndex out of range:" << cellIndex
               << "max:" << cells.size() - 1;
    return cells.size() - 1;
  }

  int NormalizeListCount(const char *actionName, int cellCount, int listSize)
  {
    if (listSize > cellCount)
    {
      qWarning() << "NXMultiCellLineEdit" << actionName << "list size exceeds cell count:" << listSize << ">"
                 << cellCount;
    }
    return std::min(cellCount, listSize);
  }

  bool IsCtrlAltTab(const QKeyEvent& event)
  {
    return event.key() == Qt::Key_Tab && event.modifiers().testFlag(Qt::ControlModifier) &&
           event.modifiers().testFlag(Qt::AltModifier);
  }

  bool IsCtrlShift(const QKeyEvent& event, Qt::Key key)
  {
    return event.key() == key && event.modifiers().testFlag(Qt::ControlModifier) &&
           event.modifiers().testFlag(Qt::ShiftModifier);
  }

  bool IsCtrlOnly(const QKeyEvent& event, Qt::Key key)
  {
    return event.key() == key && event.modifiers().testFlag(Qt::ControlModifier) &&
           !event.modifiers().testFlag(Qt::ShiftModifier) && !event.modifiers().testFlag(Qt::AltModifier);
  }
}

NXMultiCellLineEdit::NXMultiCellLineEdit(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new NXMultiCellLineEditPrivate())
{
  Q_D(NXMultiCellLineEdit);
  d->q_ptr                      = this;
  d->_pCellSeparator            = QStringLiteral(".");
  d->_pCellCount                = 4;
  d->_pCellSpacing              = 6;
  d->_pContentsHorizontalMargin = 10;
  d->_pIsCellAutoAdvanceEnabled = true;
  d->_themeMode                 = nxTheme->getThemeMode();
  setObjectName("NXMultiCellLineEdit");
  //setStyleSheet("#NXMultiCellLineEdit{background-color:transparent;}");
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover, true);
  setFocusPolicy(Qt::StrongFocus);

  d->_focusEvent = new NXEvent(QStringLiteral("WMWindowClicked"), QStringLiteral("onWMWindowClickedEvent"), d);
  d->_focusEvent->registerAndInit();

  if (!d->_cellLayout)
  {
    d->_cellLayout = new QHBoxLayout(this);
    d->_cellLayout->setContentsMargins(d->_pContentsHorizontalMargin, 0, d->_pContentsHorizontalMargin, 0);
  }
  d->_cellLayout->setSpacing(d->_pCellSpacing);
  d->_style = new NXMultiCellLineEditStyle(this->style());
  setStyle(d->_style);

  auto applyTheme = [this](NXThemeType::ThemeMode themeMode)
  {
    Q_D(NXMultiCellLineEdit);
    d->_themeMode = themeMode;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Text, NXThemeColor(d->_themeMode, BasicText));
    palette.setColor(QPalette::PlaceholderText,
                     d->_themeMode == NXThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
    this->setPalette(palette);
    for (auto *cell : d->_cellEdits)
    {
      if (!cell) { continue; }
      cell->setPalette(palette);
    }
    this->update();
  };

  applyTheme(nxTheme->getThemeMode());
  connect(nxTheme, &NXTheme::themeModeChanged, this, applyTheme);
  d->rebuildCells(d->_pCellCount);
}

NXMultiCellLineEdit::NXMultiCellLineEdit(int cellCount, const QString& separator, QWidget *parent)
    : NXMultiCellLineEdit(parent)
{
  setCellSeparator(separator);
  setCellCount(cellCount);
}

NXMultiCellLineEdit::~NXMultiCellLineEdit()
{
  delete this->style();
}

void NXMultiCellLineEdit::setBorderRadius(int borderRadius)
{
  Q_D(NXMultiCellLineEdit);
  d->_style->setBorderRadius(borderRadius);
  update();
}

int NXMultiCellLineEdit::getBorderRadius() const
{
  return d_func()->_style->getBorderRadius();
}

void NXMultiCellLineEdit::setIsCellAutoAdvanceEnabled(bool isCellAutoAdvanceEnabled)
{
  Q_D(NXMultiCellLineEdit);
  if (d->_pIsCellAutoAdvanceEnabled == isCellAutoAdvanceEnabled) { return; }
  d->_pIsCellAutoAdvanceEnabled = isCellAutoAdvanceEnabled;
  Q_EMIT pIsCellAutoAdvanceEnabledChanged();
}

bool NXMultiCellLineEdit::getIsCellAutoAdvanceEnabled() const
{
  Q_D(const NXMultiCellLineEdit);
  return d->_pIsCellAutoAdvanceEnabled;
}

void NXMultiCellLineEdit::setCellCount(int cellCount)
{
  Q_D(NXMultiCellLineEdit);
  if (cellCount < 1) { cellCount = 1; }
  const int oldCount = d->_cellEdits.size();
  if (oldCount == cellCount) { return; }

  if (cellCount > oldCount)
  {
    for (int i = oldCount; i < cellCount; ++i)
    {
      auto *cell = d->createCell();
      d->_cellEdits.append(cell);
      d->_cellExpandMarkWidths.append(0);
      d->_cellValidStates.append(true);
      d->_connectCellSignals(i, cell);
    }
  }
  else
  {
    for (int i = oldCount - 1; i >= cellCount; --i)
    {
      if (QLineEdit *cell = d->_cellEdits.value(i))
      {
        cell->removeEventFilter(this);
        cell->deleteLater();
      }
      d->_cellEdits.removeAt(i);
      d->_cellExpandMarkWidths.removeAt(i);
      d->_cellValidStates.removeAt(i);
    }
  }

  d->_pCellCount = cellCount;
  d->rebuildLayout();
  d->resetGlobalRoute();
  Q_EMIT pCellCountChanged();
  update();
}

int NXMultiCellLineEdit::getCellCount() const
{
  Q_D(const NXMultiCellLineEdit);
  return d->_pCellCount;
}

void NXMultiCellLineEdit::setCellSpacing(int cellSpacing)
{
  Q_D(NXMultiCellLineEdit);
  if (d->_pCellSpacing == cellSpacing) { return; }
  d->_pCellSpacing = cellSpacing;
  if (d->_cellLayout) { d->_cellLayout->setSpacing(cellSpacing); }
  Q_EMIT pCellSpacingChanged();
}

int NXMultiCellLineEdit::getCellSpacing() const
{
  Q_D(const NXMultiCellLineEdit);
  return d->_pCellSpacing;
}

void NXMultiCellLineEdit::setContentsHorizontalMargin(int contentsHorizontalMargin)
{
  Q_D(NXMultiCellLineEdit);
  if (d->_pContentsHorizontalMargin == contentsHorizontalMargin) { return; }
  d->_pContentsHorizontalMargin = contentsHorizontalMargin;
  if (d->_cellLayout) { d->_cellLayout->setContentsMargins(contentsHorizontalMargin, 0, contentsHorizontalMargin, 0); }
  Q_EMIT pContentsHorizontalMarginChanged();
}

int NXMultiCellLineEdit::getContentsHorizontalMargin() const
{
  Q_D(const NXMultiCellLineEdit);
  return d->_pContentsHorizontalMargin;
}

void NXMultiCellLineEdit::setCellSeparator(const QString& cellSeparator)
{
  Q_D(NXMultiCellLineEdit);
  if (d->_pCellSeparator == cellSeparator) { return; }
  d->_pCellSeparator = cellSeparator;
  for (auto *label : d->_cellSeparators)
  {
    if (label) { label->setText(cellSeparator); }
  }
  Q_EMIT pCellSeparatorChanged();
}

QString NXMultiCellLineEdit::getCellSeparator() const
{
  Q_D(const NXMultiCellLineEdit);
  return d->_pCellSeparator;
}

void NXMultiCellLineEdit::setCellEmptyFillText(const QString& cellEmptyFillText)
{
  Q_D(NXMultiCellLineEdit);
  if (d->_pCellEmptyFillText == cellEmptyFillText) { return; }
  d->_pCellEmptyFillText = cellEmptyFillText;
  Q_EMIT pCellEmptyFillTextChanged();
}

QString NXMultiCellLineEdit::getCellEmptyFillText() const
{
  Q_D(const NXMultiCellLineEdit);
  return d->_pCellEmptyFillText;
}

void NXMultiCellLineEdit::setCombinedText(const QString& combinedText)
{
  Q_D(NXMultiCellLineEdit);
  if (d->_pCellSeparator.isEmpty())
  {
    if (!d->_cellEdits.isEmpty())
    {
      d->_cellEdits.first()->setText(combinedText);
      d->pushGlobalRoute();
    }
    return;
  }

  const QStringList parts = combinedText.split(d->_pCellSeparator, Qt::KeepEmptyParts);
  setCellTexts(parts);
}

QString NXMultiCellLineEdit::getCombinedText() const
{
  Q_D(const NXMultiCellLineEdit);
  QStringList texts;
  texts.reserve(d->_cellEdits.size());
  for (auto *cell : d->_cellEdits)
  {
    texts.append(cell ? cell->text() : QString());
  }
  return texts.join(d->_pCellSeparator);
}

void NXMultiCellLineEdit::setCellTexts(const QStringList& cellTexts)
{
  Q_D(NXMultiCellLineEdit);
  const int count = NormalizeListCount("setCellTexts", d->_cellEdits.size(), cellTexts.size());
  for (int i = 0; i < count; ++i)
  {
    if (QLineEdit *cell = d->_cellEdits[i]) { cell->setText(cellTexts[i]); }
  }
  for (int i = count; i < d->_cellEdits.size(); ++i)
  {
    if (QLineEdit *cell = d->_cellEdits[i]) { cell->clear(); }
  }
  d->pushGlobalRoute();
}

QStringList NXMultiCellLineEdit::getCellTexts() const
{
  Q_D(const NXMultiCellLineEdit);
  QStringList texts;
  texts.reserve(d->_cellEdits.size());
  for (auto *cell : d->_cellEdits)
  {
    texts.append(cell ? cell->text() : QString());
  }
  return texts;
}

void NXMultiCellLineEdit::setCellRegularExpressionPatterns(const QStringList& cellRegularExpressionPatterns)
{
  const int count = NormalizeListCount("setCellRegularExpressionPatterns",
                                       getCellCount(),
                                       (int) cellRegularExpressionPatterns.size());
  for (int i = 0; i < count; ++i)
  {
    setCellRegularExpression(i, QRegularExpression(cellRegularExpressionPatterns[i]));
  }
}

QStringList NXMultiCellLineEdit::getCellRegularExpressionPatterns() const
{
  QStringList patterns;
  patterns.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    patterns.append(getCellRegularExpression(i).pattern());
  }
  return patterns;
}

void NXMultiCellLineEdit::setIsCellClearButtonEnabledList(const QList<bool>& cellClearButtonEnabledList)
{
  const int count = NormalizeListCount("setIsCellClearButtonEnabledList",
                                       getCellCount(),
                                       (int) cellClearButtonEnabledList.size());
  for (int i = 0; i < count; ++i)
  {
    setIsClearButtonEnabled(i, cellClearButtonEnabledList[i]);
  }
}

QList<bool> NXMultiCellLineEdit::getIsCellClearButtonEnabledList() const
{
  QList<bool> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getIsClearButtonEnabled(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCellMaxLengths(const QList<int>& cellMaxLengths)
{
  const int count = NormalizeListCount("setCellMaxLengths", getCellCount(), (int) cellMaxLengths.size());
  for (int i = 0; i < count; ++i)
  {
    setCellMaxLength(i, cellMaxLengths[i]);
  }
}

QList<int> NXMultiCellLineEdit::getCellMaxLengths() const
{
  QList<int> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getCellMaxLength(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCellContentsMarginsList(const QList<QMargins>& cellContentsMarginsList)
{
  const int count = NormalizeListCount("setCellContentsMarginsList",
                                       getCellCount(),
                                       (int) cellContentsMarginsList.size());
  for (int i = 0; i < count; ++i)
  {
    setCellMargins(i, cellContentsMarginsList[i]);
  }
}

QList<QMargins> NXMultiCellLineEdit::getCellContentsMarginsList() const
{
  QList<QMargins> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getCellMargins(i));
  }
  return values;
}

void NXMultiCellLineEdit::setIsCellReadOnlyList(const QList<bool>& cellReadOnlyList)
{
  const int count = NormalizeListCount("setIsCellReadOnlyList", getCellCount(), (int) cellReadOnlyList.size());
  for (int i = 0; i < count; ++i)
  { setIsCellReadOnly(i, cellReadOnlyList[i]);
  }
}

QList<bool> NXMultiCellLineEdit::getIsCellReadOnlyList() const
{
  QList<bool> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getIsCellReadOnly(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCellPlaceholderTexts(const QList<QString>& cellPlaceholderTexts)
{
  const int count = NormalizeListCount("setCellPlaceholderTexts", getCellCount(), (int) cellPlaceholderTexts.size());
  for (int i = 0; i < count; ++i)
  {
    setCellPlaceholderText(i, cellPlaceholderTexts[i]);
  }
}

QList<QString> NXMultiCellLineEdit::getCellPlaceholderTexts() const
{
  QList<QString> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getCellPlaceholderText(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCellAlignments(const QList<Qt::Alignment>& cellAlignments)
{
  const int count = NormalizeListCount("setCellAlignments", getCellCount(), (int) cellAlignments.size());
  for (int i = 0; i < count; ++i)
  {
    setCellAlignment(i, cellAlignments[i]);
  }
}

QList<Qt::Alignment> NXMultiCellLineEdit::getCellAlignments() const
{
  QList<Qt::Alignment> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getCellAlignment(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCellFonts(const QList<QFont>& cellFonts)
{
  const int count = NormalizeListCount("setCellFonts", getCellCount(), (int) cellFonts.size());
  for (int i = 0; i < count; ++i)
  {
    setCellFont(i, cellFonts[i]);
  }
}

QList<QFont> NXMultiCellLineEdit::getCellFonts() const
{
  QList<QFont> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getCellFont(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCellInputMasks(const QList<QString>& cellInputMasks)
{
  const int count = NormalizeListCount("setCellInputMasks", getCellCount(), (int) cellInputMasks.size());
  for (int i = 0; i < count; ++i)
  {
    setCellInputMask(i, cellInputMasks[i]);
  }
}

QList<QString> NXMultiCellLineEdit::getCellInputMasks() const
{
  QList<QString> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getCellInputMask(i));
  }
  return values;
}

void NXMultiCellLineEdit::setIsCellEnabledList(const QList<bool>& cellEnabledList)
{
  const int count = NormalizeListCount("setIsCellEnabledList", getCellCount(), (int) cellEnabledList.size());
  for (int i = 0; i < count; ++i)
  {
    setIsCellEnabled(i, cellEnabledList[i]);
  }
}

QList<bool> NXMultiCellLineEdit::getIsCellEnabledList() const
{
  QList<bool> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getIsCellEnabled(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCellInputMethodHintsList(const QList<Qt::InputMethodHints>& cellInputMethodHintsList)
{
  const int count = NormalizeListCount("setCellInputMethodHintsList",
                                       getCellCount(),
                                       (int) cellInputMethodHintsList.size());
  for (int i = 0; i < count; ++i)
  {
    setCellInputMethodHints(i, cellInputMethodHintsList[i]);
  }
}

QList<Qt::InputMethodHints> NXMultiCellLineEdit::getCellInputMethodHintsList() const
{
  QList<Qt::InputMethodHints> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getCellInputMethodHints(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCellEchoModes(const QList<QLineEdit::EchoMode>& cellEchoModes)
{
  const int count = NormalizeListCount("setCellEchoModes", getCellCount(), (int) cellEchoModes.size());
  for (int i = 0; i < count; ++i)
  {
    setCellEchoMode(i, cellEchoModes[i]);
  }
}

QList<QLineEdit::EchoMode> NXMultiCellLineEdit::getCellEchoModes() const
{
  QList<QLineEdit::EchoMode> values;
  values.reserve(getCellCount());
  for (int i = 0; i < getCellCount(); ++i)
  {
    values.append(getCellEchoMode(i));
  }
  return values;
}

void NXMultiCellLineEdit::setCustomLineEdit(int cellIndex, QLineEdit *lineEdit)
{
  Q_D(NXMultiCellLineEdit);
  if (!lineEdit) { return; }
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCustomLineEdit");
  if (normalizedIndex < 0) { return; }
  if (d->_cellEdits[normalizedIndex] == lineEdit) { return; }

  QLineEdit *old = d->_cellEdits[normalizedIndex];
  if (old) { old->removeEventFilter(this); }

  d->setupCell(lineEdit, true);

  d->_cellEdits[normalizedIndex] = lineEdit;
  d->_connectCellSignals(normalizedIndex, lineEdit);

  if (old) { old->deleteLater(); }
  d->rebuildLayout();
  d->resetGlobalRoute();
}

QLineEdit *NXMultiCellLineEdit::getCustomLineEdit(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCustomLineEdit");
  if (normalizedIndex < 0) { return nullptr; }
  return d->_cellEdits.value(normalizedIndex, nullptr);
}

QLineEdit *NXMultiCellLineEdit::getCellLineEdit(int cellIndex) const
{
  return getCustomLineEdit(cellIndex);
}

int NXMultiCellLineEdit::getCellIndex(const QLineEdit *lineEdit) const
{
  Q_D(const NXMultiCellLineEdit);
  if (!lineEdit) { return -1; }
  return d->_cellEdits.indexOf(const_cast<QLineEdit *>(lineEdit));
}

QList<QLineEdit *> NXMultiCellLineEdit::getCellLineEdits() const
{
  Q_D(const NXMultiCellLineEdit);
  return d->_cellEdits;
}

void NXMultiCellLineEdit::setCellRegularExpression(int cellIndex, const QRegularExpression& regularExpression)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellRegularExpression");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex))
  {
    cell->setValidator(new QRegularExpressionValidator(regularExpression, cell));
    d->_updateCellValidState(normalizedIndex);
  }
}

QRegularExpression NXMultiCellLineEdit::getCellRegularExpression(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellRegularExpression");
  if (normalizedIndex < 0) { return {}; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex))
  {
    if (const auto *validator = qobject_cast<const QRegularExpressionValidator *>(cell->validator()))
    {
      return validator->regularExpression();
    }
  }
  return {};
}

void NXMultiCellLineEdit::setCellMaxLength(int cellIndex, int maxLength)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellMaxLength");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setMaxLength(maxLength); }
}

int NXMultiCellLineEdit::getCellMaxLength(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellMaxLength");
  if (normalizedIndex < 0) { return 0; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->maxLength(); }
  return 0;
}

void NXMultiCellLineEdit::setIsClearButtonEnabled(int cellIndex, bool isEnabled)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setIsClearButtonEnabled");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex))
  {
    cell->setProperty(CLEAR_ALLOWED_PROPERTY, isEnabled);
    cell->setClearButtonEnabled(isEnabled && cell->hasFocus());
  }
}

bool NXMultiCellLineEdit::getIsClearButtonEnabled(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getIsClearButtonEnabled");
  if (normalizedIndex < 0) { return false; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->property(CLEAR_ALLOWED_PROPERTY).toBool(); }
  return false;
}

void NXMultiCellLineEdit::setCellMargins(int cellIndex, const QMargins& contentsMargins)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellMargins");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex))
  {
    cell->setTextMargins(contentsMargins.left(),
                         contentsMargins.top(),
                         contentsMargins.right(),
                         contentsMargins.bottom());
  }
}

QMargins NXMultiCellLineEdit::getCellMargins(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellMargins");
  if (normalizedIndex < 0) { return {}; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->textMargins(); }
  return {};
}

void NXMultiCellLineEdit::setIsCellReadOnly(int cellIndex, bool readOnly)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setIsCellReadOnly");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setReadOnly(readOnly); }
}

bool NXMultiCellLineEdit::getIsCellReadOnly(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getIsCellReadOnly");
  if (normalizedIndex < 0) { return true; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->isReadOnly(); }
  return true;
}

void NXMultiCellLineEdit::setCellText(int cellIndex, const QString& text)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellText");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setText(text); }
  d->pushGlobalRoute();
}

QString NXMultiCellLineEdit::getCellText(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellText");
  if (normalizedIndex < 0) { return {}; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->text(); }
  return {};
}

void NXMultiCellLineEdit::setCellPlaceholderText(int cellIndex, const QString& placeholder)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellPlaceholderText");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setPlaceholderText(placeholder); }
}

QString NXMultiCellLineEdit::getCellPlaceholderText(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellPlaceholderText");
  if (normalizedIndex < 0) { return {}; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->placeholderText(); }
  return {};
}

void NXMultiCellLineEdit::setCellAlignment(int cellIndex, Qt::Alignment alignment)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellAlignment");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setAlignment(alignment); }
}

Qt::Alignment NXMultiCellLineEdit::getCellAlignment(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellAlignment");
  if (normalizedIndex < 0) { return Qt::AlignLeft; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->alignment(); }
  return Qt::AlignLeft;
}

void NXMultiCellLineEdit::setCellFont(int cellIndex, const QFont& font)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellFont");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setFont(font); }
}

QFont NXMultiCellLineEdit::getCellFont(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellFont");
  if (normalizedIndex < 0) { return {}; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->font(); }
  return {};
}

void NXMultiCellLineEdit::setCellInputMask(int cellIndex, const QString& inputMask)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellInputMask");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setInputMask(inputMask); }
}

QString NXMultiCellLineEdit::getCellInputMask(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellInputMask");
  if (normalizedIndex < 0) { return {}; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->inputMask(); }
  return {};
}

void NXMultiCellLineEdit::setIsCellEnabled(int cellIndex, bool enabled)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setIsCellEnabled");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setEnabled(enabled); }
}

bool NXMultiCellLineEdit::getIsCellEnabled(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getIsCellEnabled");
  if (normalizedIndex < 0) { return false; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->isEnabled(); }
  return false;
}

void NXMultiCellLineEdit::setCellValidator(int cellIndex, QValidator *validator)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellValidator");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex))
  {
    if (validator && !validator->parent()) { validator->setParent(cell); }
    cell->setValidator(validator);
    d->_updateCellValidState(normalizedIndex);
  }
}

const QValidator *NXMultiCellLineEdit::getCellValidator(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellValidator");
  if (normalizedIndex < 0) { return nullptr; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->validator(); }
  return nullptr;
}

void NXMultiCellLineEdit::setCellInputMethodHints(int cellIndex, Qt::InputMethodHints hints)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellInputMethodHints");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setInputMethodHints(hints); }
}

Qt::InputMethodHints NXMultiCellLineEdit::getCellInputMethodHints(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellInputMethodHints");
  if (normalizedIndex < 0) { return {}; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->inputMethodHints(); }
  return {};
}

void NXMultiCellLineEdit::setCellEchoMode(int cellIndex, QLineEdit::EchoMode mode)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setCellEchoMode");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { cell->setEchoMode(mode); }
}

QLineEdit::EchoMode NXMultiCellLineEdit::getCellEchoMode(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getCellEchoMode");
  if (normalizedIndex < 0) { return QLineEdit::Normal; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex)) { return cell->echoMode(); }
  return QLineEdit::Normal;
}

bool NXMultiCellLineEdit::getIsCellValid(int cellIndex) const
{
  Q_D(const NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "getIsCellValid");
  if (normalizedIndex < 0) { return false; }
  return d->_cellValidStates.value(normalizedIndex, false);
}

QList<bool> NXMultiCellLineEdit::getCellValidStates() const
{
  Q_D(const NXMultiCellLineEdit);
  return d->_cellValidStates;
}

void NXMultiCellLineEdit::setIsAllCellReadOnly(bool readOnly)
{
  Q_D(NXMultiCellLineEdit);
  for (auto *cell : d->_cellEdits)
  {
    if (cell) { cell->setReadOnly(readOnly); }
  }
}

void NXMultiCellLineEdit::setIsAllCellEnabled(bool enabled)
{
  Q_D(NXMultiCellLineEdit);
  for (auto *cell : d->_cellEdits)
  {
    if (cell) { cell->setEnabled(enabled); }
  }
}

void NXMultiCellLineEdit::clear(int cellIndex)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "clear");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex))
  {
    if (!cell->isReadOnly()) { cell->clear(); }
  }
  d->pushGlobalRoute();
}

void NXMultiCellLineEdit::clearAll()
{
  Q_D(NXMultiCellLineEdit);
  for (auto *cell : d->_cellEdits)
  {
    if (!cell || cell->isReadOnly()) { continue; }
    QSignalBlocker blocker(cell);
    cell->clear();
  }
  d->pushGlobalRoute();
}

void NXMultiCellLineEdit::setFocusToCell(int cellIndex)
{
  Q_D(NXMultiCellLineEdit);
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "setFocusToCell");
  if (normalizedIndex < 0) { return; }
  if (QLineEdit *cell = d->_cellEdits.value(normalizedIndex))
  {
    if (!cell->isEnabled()) { return; }
    cell->setFocus();
  }
}

int NXMultiCellLineEdit::currentFocusCellIndex() const
{
  Q_D(const NXMultiCellLineEdit);
  if (d->_currentFocusCellIndex >= 0) { return d->_currentFocusCellIndex; }
  for (int i = 0; i < d->_cellEdits.size(); ++i)
  {
    if (d->_cellEdits[i] && d->_cellEdits[i]->hasFocus()) { return i; }
  }
  return -1;
}

void NXMultiCellLineEdit::insertCell(int cellIndex, const QString& initialText)
{
  Q_D(NXMultiCellLineEdit);
  if (cellIndex < 0 || cellIndex > d->_cellEdits.size())
  {
    qWarning() << "NXMultiCellLineEdit insertCell cellIndex out of range:" << cellIndex
               << "max:" << d->_cellEdits.size();
    cellIndex = std::clamp(cellIndex, 0, (int)d->_cellEdits.size());
  }

  auto *cell = d->createCell();
  cell->setText(initialText);

  d->_cellEdits.insert(cellIndex, cell);
  d->_cellExpandMarkWidths.insert(cellIndex, 0);
  d->_cellValidStates.insert(cellIndex, true);
  d->_connectCellSignals(cellIndex, cell);

  d->_pCellCount = d->_cellEdits.size();
  d->rebuildLayout();
  d->resetGlobalRoute();

  Q_EMIT pCellCountChanged();
  cell->setFocus();
}

void NXMultiCellLineEdit::removeCell(int cellIndex)
{
  Q_D(NXMultiCellLineEdit);
  if (d->_cellEdits.size() <= 1) { return; }
  const int normalizedIndex = NormalizeCellIndex(d->_cellEdits, cellIndex, "removeCell");
  if (normalizedIndex < 0) { return; }

  const bool hadFocus = d->_cellEdits[normalizedIndex] && d->_cellEdits[normalizedIndex]->hasFocus();
  QLineEdit *removed  = d->_cellEdits.takeAt(normalizedIndex);
  d->_cellExpandMarkWidths.removeAt(normalizedIndex);
  d->_cellValidStates.removeAt(normalizedIndex);

  if (removed)
  {
    removed->removeEventFilter(this);
    removed->deleteLater();
  }

  d->_pCellCount = d->_cellEdits.size();
  d->rebuildLayout();
  d->resetGlobalRoute();

  Q_EMIT pCellCountChanged();

  if (hadFocus)
  {
    const int nextIndex = std::min(normalizedIndex, (int) d->_cellEdits.size() - 1);
    if (QLineEdit *cell = d->_cellEdits.value(nextIndex)) { cell->setFocus(); }
  }
}

void NXMultiCellLineEdit::focusInEvent(QFocusEvent *event)
{
  Q_D(NXMultiCellLineEdit);
  QWidget::focusInEvent(event);
  const int targetIndex = std::clamp(d->_currentFocusCellIndex, 0, (int) d->_cellEdits.size() - 1);
  if (QLineEdit *cell = d->_cellEdits.value(targetIndex)) { cell->setFocus(); }
}

void NXMultiCellLineEdit::focusOutEvent(QFocusEvent *event)
{
  QWidget::focusOutEvent(event);
}

void NXMultiCellLineEdit::paintEvent(QPaintEvent *event)
{
  Q_D(NXMultiCellLineEdit);
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.save();
  QStyleOptionFrame option;
  option.initFrom(this);
  option.rect = rect();
  bool hasCellFocus = hasFocus();
  if (!hasCellFocus)
  {
    for (auto *cell : d->_cellEdits)
    {
      if (cell && cell->hasFocus())
      {
        hasCellFocus = true;
        break;
      }
    }
  }
  if (hasCellFocus) { option.state |= QStyle::State_HasFocus; }
  if (d->_hoverCellIndex >= 0 || underMouse()) { option.state |= QStyle::State_MouseOver; }
  style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
  painter.restore();

  painter.save();
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  painter.setPen(Qt::NoPen);

  if (d->_hoverCellIndex >= 0 && d->_hoverCellIndex < d->_cellEdits.size())
  {
    QLineEdit *cell = d->_cellEdits[d->_hoverCellIndex];
    if (cell && !cell->hasFocus())
    {
      painter.setBrush(NXThemeColor(d->_themeMode, BasicHoverAlpha));
      QRectF hoverRect = cell->geometry();
      painter.drawRoundedRect(hoverRect.adjusted(0.5, 1.5, -0.5, -3.5), 4, 4);
    }
  }

  painter.setBrush(NXThemeColor(d->_themeMode, PrimaryNormal));
  constexpr qreal markHeight = 2.5;
  const qreal markY          = height() - markHeight;

  for (int i = 0; i < d->_cellEdits.size(); ++i)
  {
    QLineEdit *cell = d->_cellEdits[i];
    if (!cell) { continue; }

    const QRect geo = cell->geometry();
    const qreal expandWidth = d->_cellExpandMarkWidths.value(i, 0);
    if (expandWidth <= 0) { continue; }
    painter.drawRoundedRect(QRectF(geo.center().x() - expandWidth, markY, expandWidth * 2, markHeight), 2, 2);
  }

  painter.restore();
}

void NXMultiCellLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
  Q_D(NXMultiCellLineEdit);
  const int cellIndex = currentFocusCellIndex();
  if (QLineEdit *cell = d->_cellEdits.value(cellIndex))
  {
    const bool isOverallMode   = d->_isOverallOperationEnabled;
    const bool isAllEchoNormal = d->isAllNormalEcho();

    NXMenu *menu = new NXMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    QAction *action = nullptr;

    if (!cell->isReadOnly())
    {
      action = menu->addNXIconAction(NXIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
      action->setEnabled(!isOverallMode && cell->isUndoAvailable());
      connect(action, &QAction::triggered, this, [cell]() { cell->undo(); });

      action = menu->addNXIconAction(NXIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
      action->setEnabled(!isOverallMode && cell->isRedoAvailable());
      connect(action, &QAction::triggered, this, [cell]() { cell->redo(); });

      action = menu->addNXIconAction(NXIconType::ArrowRotateLeft,
                                     tr("整体撤销"),
                                     QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Z));
      action->setEnabled(d->canGlobalUndo());
      connect(action, &QAction::triggered, this, [d]() { d->globalUndo(); });

      action = menu->addNXIconAction(NXIconType::ArrowRotateRight,
                                     tr("整体恢复"),
                                     QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Y));
      action->setEnabled(d->canGlobalRedo());
      connect(action, &QAction::triggered, this, [d]() { d->globalRedo(); });

      menu->addSeparator();
    }

#ifndef QT_NO_CLIPBOARD
    if (!cell->isReadOnly())
    {
      action = menu->addNXIconAction(NXIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
      if (isOverallMode)
      {
        action->setEnabled(isAllEchoNormal && d->isAnyEditable());
        connect(action, &QAction::triggered, this, [d]()
        {
          d->copyAllCells();
          d->clearAllEditableCells();
        });
      }
      else
      {
        action->setEnabled(cell->hasSelectedText() && cell->echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, this, [cell]() { cell->cut(); });
      }
    }

    action = menu->addNXIconAction(NXIconType::Copy, tr("复制"), QKeySequence::Copy);
    if (isOverallMode)
    {
      action->setEnabled(isAllEchoNormal);
      connect(action, &QAction::triggered, this, [d]() { d->copyAllCells(); });
    }
    else
    {
      action->setEnabled(cell->hasSelectedText() && cell->echoMode() == QLineEdit::Normal);
      connect(action, &QAction::triggered, this, [cell]() { cell->copy(); });
    }

    if (!cell->isReadOnly())
    {
      action = menu->addNXIconAction(NXIconType::Paste, tr("粘贴"), QKeySequence::Paste);
      if (isOverallMode)
      {
        action->setEnabled(false);
      }
      else
      {
        action->setEnabled(!QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, [cell]() { cell->paste(); });
      }
    }
#endif

    if (!cell->isReadOnly())
    {
      action = menu->addNXIconAction(NXIconType::DeleteLeft, tr("删除"));
      if (isOverallMode)
      {
        action->setEnabled(d->isAnyEditable());
        connect(action, &QAction::triggered, this, [d]() { d->clearAllEditableCells(); });
      }
      else
      {
        action->setEnabled(!cell->text().isEmpty() && cell->hasSelectedText());
        connect(action,
                &QAction::triggered,
                this,
                [d, cell]()
        {
          if (!cell->hasSelectedText()) { return; }
          const int startIndex = cell->selectionStart();
          const int endIndex   = cell->selectionEnd();
          cell->setText(cell->text().remove(startIndex, endIndex - startIndex));
          d->pushGlobalRoute();
        });
      }
    }

    if (!menu->isEmpty()) { menu->addSeparator(); }
    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    if (isOverallMode)
    {
      action->setEnabled(isAllEchoNormal);
      connect(action, &QAction::triggered, this, [d]() { d->selectAllCells(); });
    }
    else
    {
      action->setEnabled(!cell->text().isEmpty() && !(cell->selectedText() == cell->text()));
      connect(action, &QAction::triggered, this, [cell]() { cell->selectAll(); });
    }

    menu->popup(event->globalPos());
    cell->setFocus();
    return;
  }
  QWidget::contextMenuEvent(event);
}

bool NXMultiCellLineEdit::eventFilter(QObject *watched, QEvent *event)
{
  Q_D(NXMultiCellLineEdit);
  QLineEdit *cell = qobject_cast<QLineEdit *>(watched);
  if (!cell) { return QWidget::eventFilter(watched, event); }
  const int cellIndex = d->_cellEdits.indexOf(cell);
  if (cellIndex < 0) { return QWidget::eventFilter(watched, event); }
  switch (event->type())
  {
    case QEvent::FocusIn :
    {
      d->updateFocusCell(cellIndex);
      Q_EMIT cellFocusIn(cellIndex);
      d->updateClearButtonState(cell);
      d->animateCellMarkIn(cellIndex);
      update();
      break;
    }
    case QEvent::FocusOut :
    {
      auto *focusEvent = static_cast<QFocusEvent *>(event);
      Q_EMIT cellFocusOut(cellIndex);
      if (focusEvent->reason() != Qt::PopupFocusReason)
      {
        cell->setClearButtonEnabled(false);
        d->animateCellMarkOut(cellIndex);
      }
      update();
      break;
    }
    case QEvent::Enter :
    {
      d->_hoverCellIndex = cellIndex;
      update();
      break;
    }
    case QEvent::Leave :
    {
      if (d->_hoverCellIndex == cellIndex) { d->_hoverCellIndex = -1; }
      update();
      break;
    }
    case QEvent::ContextMenu :
    {
      cell->setFocus();
      contextMenuEvent(static_cast<QContextMenuEvent *>(event));
      return true;
    }
    case QEvent::KeyPress :
    {
      auto *keyEvent = static_cast<QKeyEvent *>(event);

      if (IsCtrlAltTab(*keyEvent))
      {
        d->_isOverallOperationEnabled = !d->_isOverallOperationEnabled;
        keyEvent->accept();
        return true;
      }

      if (IsCtrlShift(*keyEvent, Qt::Key_Z))
      {
        d->globalUndo();
        keyEvent->accept();
        return true;
      }

      if (IsCtrlShift(*keyEvent, Qt::Key_Y))
      {
        d->globalRedo();
        keyEvent->accept();
        return true;
      }

      if (d->_isOverallOperationEnabled)
      {
        if (IsCtrlOnly(*keyEvent, Qt::Key_Z))
        {
          d->globalUndo();
          keyEvent->accept();
          return true;
        }
        if (IsCtrlOnly(*keyEvent, Qt::Key_Y))
        {
          d->globalRedo();
          keyEvent->accept();
          return true;
        }
        if (IsCtrlOnly(*keyEvent, Qt::Key_A))
        {
          for (auto *item : d->_cellEdits)
          {
            if (item) { item->selectAll(); }
          }
          keyEvent->accept();
          return true;
        }
#ifndef QT_NO_CLIPBOARD
        if (IsCtrlOnly(*keyEvent, Qt::Key_C) && d->isAllNormalEcho())
        {
          d->copyAllCells();
          keyEvent->accept();
          return true;
        }
        if (IsCtrlOnly(*keyEvent, Qt::Key_X) && d->isAllNormalEcho())
        {
          d->copyAllCells();
          d->clearAllEditableCells();
          keyEvent->accept();
          return true;
        }
        if (IsCtrlOnly(*keyEvent, Qt::Key_V))
        {
          keyEvent->accept();
          return true;
        }
#endif
        if (keyEvent->key() == Qt::Key_Delete)
        {
          clearAll();
          keyEvent->accept();
          return true;
        }
      }

      if (keyEvent->key() == Qt::Key_Backspace && cell->text().isEmpty() && cell->cursorPosition() == 0 &&
          cell->selectionStart() < 0 && cellIndex > 0)
      {
        if (QLineEdit *prev = d->_cellEdits.value(cellIndex - 1))
        {
          prev->setFocus();
          prev->setCursorPosition(prev->text().size());
          keyEvent->accept();
          return true;
        }
      }

      if (keyEvent->key() == Qt::Key_Left && cell->cursorPosition() == 0 && cell->selectionStart() < 0 && cellIndex > 0)
      {
        if (QLineEdit *prev = d->_cellEdits.value(cellIndex - 1))
        {
          prev->setFocus();
          prev->setCursorPosition(prev->text().size());
          keyEvent->accept();
          return true;
        }
      }

      if (keyEvent->key() == Qt::Key_Right && cell->cursorPosition() == cell->text().size() && cell->selectionStart() < 0 &&
          cellIndex + 1 < d->_cellEdits.size())
      {
        if (QLineEdit *next = d->_cellEdits.value(cellIndex + 1))
        {
          next->setFocus();
          next->setCursorPosition(0);
          keyEvent->accept();
          return true;
        }
      }

      if (keyEvent->key() == Qt::Key_Tab && !keyEvent->modifiers().testFlag(Qt::ControlModifier) &&
          !keyEvent->modifiers().testFlag(Qt::AltModifier))
      {
        if (cellIndex + 1 < d->_cellEdits.size())
        {
          if (QLineEdit *next = d->_cellEdits.value(cellIndex + 1))
          {
            next->setFocus();
            keyEvent->accept();
            return true;
          }
        }
      }

      if (keyEvent->key() == Qt::Key_Backtab)
      {
        if (cellIndex > 0)
        {
          if (QLineEdit *prev = d->_cellEdits.value(cellIndex - 1))
          {
            prev->setFocus();
            keyEvent->accept();
            return true;
          }
        }
      }

      break;
    }
    default :
    {
      break;
    }
  }

  return QWidget::eventFilter(watched, event);
}
