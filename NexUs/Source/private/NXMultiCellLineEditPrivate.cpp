#include "NXMultiCellLineEditPrivate.h"

#include <algorithm>

#include <QClipboard>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QSignalBlocker>
#include <QValidator>
#include <QVariantAnimation>

#include "NXApplication.h"
#include "NXEventBus.h"
#include "NXMultiCellLineEdit.h"
#include "NXTheme.h"

namespace
{
  constexpr const char *CLEAR_ALLOWED_PROPERTY = "NXMultiCellClearAllowed";
}

NXMultiCellLineEditPrivate::NXMultiCellLineEditPrivate(QObject *parent)
    : QObject { parent }
{
}

NXMultiCellLineEditPrivate::~NXMultiCellLineEditPrivate()
{
}

void NXMultiCellLineEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
  Q_Q(NXMultiCellLineEdit);
  if (!q) { return; }

  QLineEdit *focusCell = nullptr;
  for (auto *cell : _cellEdits)
  {
    if (cell && cell->hasFocus())
    {
      focusCell = cell;
      break;
    }
  }

  NXAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<NXAppBarType::WMMouseActionType>();
  if (actionType == NXAppBarType::WMLBUTTONDOWN)
  {
    if (focusCell && focusCell->hasSelectedText()) { focusCell->clearFocus(); }
    return;
  }

  if (actionType == NXAppBarType::WMLBUTTONUP || actionType == NXAppBarType::WMNCLBUTTONDOWN)
  {
    if (NXApplication::containsCursorToItem(q) || (actionType == NXAppBarType::WMLBUTTONUP && focusCell && focusCell->hasSelectedText()))
    {
      return;
    }
    if (focusCell) { focusCell->clearFocus(); }
    if (q->hasFocus()) { q->clearFocus(); }
  }
}

void NXMultiCellLineEditPrivate::rebuildCells(int cellCount)
{
  Q_Q(NXMultiCellLineEdit);

  if (!_cellLayout) { return; }
  if (cellCount < 1) { cellCount = 1; }
  _pCellCount = cellCount;

  while (QLayoutItem *item = _cellLayout->takeAt(0))
  {
    if (QWidget *w = item->widget()) { w->deleteLater(); }
    delete item;
  }

  _cellEdits.clear();
  _cellSeparators.clear();
  _cellExpandMarkWidths.clear();
  _cellValidStates.clear();

  for (int i = 0; i < cellCount; ++i)
  {
    auto *cell = createCell();
    _cellLayout->addWidget(cell, 1);
    _cellEdits.append(cell);
    _cellExpandMarkWidths.append(0);
    _cellValidStates.append(true);
    _connectCellSignals(i, cell);

    if (i < cellCount - 1)
    {
      auto *separator = new QLabel(_pCellSeparator, q);
      separator->setAlignment(Qt::AlignCenter);
      separator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
      separator->setAttribute(Qt::WA_TransparentForMouseEvents);
      _cellLayout->addWidget(separator);
      _cellSeparators.append(separator);
    }
  }

  updateCellObjectNames();
  resetGlobalRoute();
  q->update();
}

void NXMultiCellLineEditPrivate::rebuildLayout()
{
  Q_Q(NXMultiCellLineEdit);
  if (!_cellLayout) { return; }

  while (QLayoutItem *item = _cellLayout->takeAt(0))
  {
    delete item;
  }
  for (auto *label : _cellSeparators)
  {
    if (label) { label->deleteLater(); }
  }
  _cellSeparators.clear();

  for (int i = 0; i < _cellEdits.size(); ++i)
  {
    QLineEdit *cell = _cellEdits[i];
    if (!cell) { continue; }
    _cellLayout->addWidget(cell, 1);
    if (i < _cellEdits.size() - 1)
    {
      auto *separator = new QLabel(_pCellSeparator, q);
      separator->setAlignment(Qt::AlignCenter);
      separator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
      separator->setAttribute(Qt::WA_TransparentForMouseEvents);
      _cellLayout->addWidget(separator);
      _cellSeparators.append(separator);
    }
  }

  updateCellObjectNames();
}

void NXMultiCellLineEditPrivate::updateFocusCell(int cellIndex)
{
  _currentFocusCellIndex = cellIndex;
}

void NXMultiCellLineEditPrivate::pushGlobalRoute()
{
  if (_isApplyingGlobalRoute) { return; }

  GlobalTextRouteState state = _currentGlobalTextRouteState();
  if (_globalRouteList.isEmpty())
  {
    _globalRouteList.append(state);
    _globalRouteCurrentIndex = 0;
    _globalRedoRouteIndex    = -1;
    return;
  }

  _globalRedoRouteIndex  = -1;
  _globalRouteCurrentIndex = std::clamp(_globalRouteCurrentIndex, 0, (int) _globalRouteList.size() - 1);

  const GlobalTextRouteState& currentState = _globalRouteList[_globalRouteCurrentIndex];
  if (currentState.texts == state.texts && currentState.cursorPositions == state.cursorPositions &&
      currentState.selectionStarts == state.selectionStarts && currentState.selectionLengths == state.selectionLengths)
  {
    return;
  }

  if (_globalRouteCurrentIndex < _globalRouteList.size() - 1)
  {
    _globalRouteList.resize(_globalRouteCurrentIndex + 1);
  }

  _globalRouteList.append(state);
  _globalRouteCurrentIndex = _globalRouteList.size() - 1;

  const int removableCount = _globalRouteList.size() - _globalRouteMaxCount;
  if (removableCount <= 0) { return; }

  const int removeCount = std::min(removableCount, (int) _globalRouteList.size() - 1);
  _globalRouteList.remove(1, removeCount);
  _globalRouteCurrentIndex = std::max(0, _globalRouteCurrentIndex - removeCount);
  if (_globalRedoRouteIndex >= 0) { _globalRedoRouteIndex = std::max(-1, _globalRedoRouteIndex - removeCount); }
}

bool NXMultiCellLineEditPrivate::canGlobalUndo() const
{
  if (_globalRedoRouteIndex >= 0) { return false; }
  if (_globalRouteList.isEmpty()) { return false; }
  return _currentGlobalTextRouteState().texts != _globalRouteList.first().texts;
}

bool NXMultiCellLineEditPrivate::canGlobalRedo() const
{
  if (_globalRouteList.isEmpty()) { return false; }
  return _globalRedoRouteIndex >= 0 && _globalRedoRouteIndex < _globalRouteList.size();
}

void NXMultiCellLineEditPrivate::globalUndo()
{
  if (!canGlobalUndo()) { return; }
  _globalRedoRouteIndex    = _globalRouteCurrentIndex;
  _globalRouteCurrentIndex = 0;
  _applyGlobalTextRouteState(_globalRouteList[_globalRouteCurrentIndex]);
}

void NXMultiCellLineEditPrivate::globalRedo()
{
  if (!canGlobalRedo()) { return; }
  _globalRouteCurrentIndex = std::clamp(_globalRedoRouteIndex, 0, (int) _globalRouteList.size() - 1);
  _globalRedoRouteIndex    = -1;
  _applyGlobalTextRouteState(_globalRouteList[_globalRouteCurrentIndex]);
}

QLineEdit *NXMultiCellLineEditPrivate::createCell()
{
  Q_Q(NXMultiCellLineEdit);
  auto *cell = new QLineEdit(q);
  setupCell(cell, false);
  return cell;
}

void NXMultiCellLineEditPrivate::setupCell(QLineEdit *cell, bool preserveClearAllowed)
{
  Q_Q(NXMultiCellLineEdit);
  if (!cell) { return; }
  cell->setParent(q);
  cell->setFrame(false);
  cell->setMouseTracking(true);
  cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  cell->setStyleSheet("QLineEdit{background-color:transparent;border:none;}");
  cell->setPalette(q->palette());

  if (preserveClearAllowed)
  {
    const bool clearAllowed = cell->isClearButtonEnabled();
    cell->setProperty(CLEAR_ALLOWED_PROPERTY, clearAllowed);
  }
  else
  {
    cell->setProperty(CLEAR_ALLOWED_PROPERTY, true);
  }
  cell->setClearButtonEnabled(false);
  cell->installEventFilter(q);
}

void NXMultiCellLineEditPrivate::resetGlobalRoute()
{
  _globalRouteList.clear();
  _globalRouteCurrentIndex = -1;
  _globalRedoRouteIndex    = -1;
  pushGlobalRoute();
}

bool NXMultiCellLineEditPrivate::isAllNormalEcho() const
{
  for (auto *item : _cellEdits)
  {
    if (item && item->echoMode() != QLineEdit::Normal) { return false; }
  }
  return true;
}

bool NXMultiCellLineEditPrivate::isAnyEditable() const
{
  for (auto *item : _cellEdits)
  {
    if (item && !item->isReadOnly()) { return true; }
  }
  return false;
}

void NXMultiCellLineEditPrivate::copyAllCells() const
{
  QStringList texts;
  texts.reserve(_cellEdits.size());
  for (auto *item : _cellEdits)
  {
    texts.append(item ? item->text() : QString());
  }
  QGuiApplication::clipboard()->setText(texts.join(_pCellSeparator));
}

void NXMultiCellLineEditPrivate::clearAllEditableCells()
{
  for (auto *item : _cellEdits)
  {
    if (!item || item->isReadOnly()) { continue; }
    QSignalBlocker blocker(item);
    item->clear();
  }
  pushGlobalRoute();
}

void NXMultiCellLineEditPrivate::selectAllCells() const
{
  for (auto *item : _cellEdits)
  {
    if (item) { item->selectAll(); }
  }
}

void NXMultiCellLineEditPrivate::updateClearButtonState(QLineEdit *focusCell)
{
  for (auto *item : _cellEdits)
  {
    if (!item) { continue; }
    item->setClearButtonEnabled(false);
  }
  if (focusCell && focusCell->property(CLEAR_ALLOWED_PROPERTY).toBool())
  {
    focusCell->setClearButtonEnabled(true);
  }
}

void NXMultiCellLineEditPrivate::updateCellObjectNames()
{
  const QString prefix = QStringLiteral("NXMultiCellLineEditCell_");
  for (int i = 0; i < _cellEdits.size(); ++i)
  {
    QLineEdit *cell = _cellEdits[i];
    if (!cell) { continue; }
    const QString name = prefix + QString::number(i);
    if (cell->objectName().isEmpty() || cell->objectName().startsWith(prefix))
    {
      cell->setObjectName(name);
    }
  }
}

void NXMultiCellLineEditPrivate::animateCellMarkIn(int cellIndex)
{
  Q_Q(NXMultiCellLineEdit);
  QLineEdit *cell = _cellAt(cellIndex);
  if (!cell) { return; }

  const qreal startValue = _cellExpandMarkWidths.value(cellIndex, 0);
  const qreal endValue   = std::max<qreal>(0, cell->width() / 2.0 - 3);

  auto *markAnimation = new QVariantAnimation(q);
  connect(markAnimation,
          &QVariantAnimation::valueChanged,
          q,
          [this, q, cellIndex](const QVariant& value)
  {
    if (cellIndex < 0 || cellIndex >= _cellExpandMarkWidths.size()) { return; }
    _cellExpandMarkWidths[cellIndex] = value.toReal();
    q->update();
  });
  markAnimation->setDuration(300);
  markAnimation->setEasingCurve(QEasingCurve::InOutSine);
  markAnimation->setStartValue(startValue);
  markAnimation->setEndValue(endValue);
  markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NXMultiCellLineEditPrivate::animateCellMarkOut(int cellIndex)
{
  Q_Q(NXMultiCellLineEdit);
  if (cellIndex < 0 || cellIndex >= _cellExpandMarkWidths.size()) { return; }

  const qreal startValue = _cellExpandMarkWidths[cellIndex];
  const qreal endValue   = 0;

  auto *markAnimation = new QVariantAnimation(q);
  connect(markAnimation,
          &QVariantAnimation::valueChanged,
          q,
          [this, q, cellIndex](const QVariant& value)
  {
    if (cellIndex < 0 || cellIndex >= _cellExpandMarkWidths.size()) { return; }
    _cellExpandMarkWidths[cellIndex] = value.toReal();
    q->update();
  });
  markAnimation->setDuration(300);
  markAnimation->setEasingCurve(QEasingCurve::InOutSine);
  markAnimation->setStartValue(startValue);
  markAnimation->setEndValue(endValue);
  markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

QLineEdit *NXMultiCellLineEditPrivate::_cellAt(int cellIndex) const
{
  if (cellIndex < 0 || cellIndex >= _cellEdits.size()) { return nullptr; }
  return _cellEdits[cellIndex];
}

int NXMultiCellLineEditPrivate::_cellIndex(QObject *watched) const
{
  if (!watched) { return -1; }
  return _cellEdits.indexOf(qobject_cast<QLineEdit *>(watched));
}

void NXMultiCellLineEditPrivate::_connectCellSignals(int, QLineEdit *cell)
{
  Q_Q(NXMultiCellLineEdit);
  if (!cell) { return; }

  connect(cell,
          &QLineEdit::textChanged,
          q,
          [this, q, cell](const QString& text)
  {
    const int cellIndex = _cellEdits.indexOf(cell);
    if (cellIndex < 0) { return; }
    Q_EMIT q->cellTextChanged(cellIndex, text);
    _updateCellValidState(cellIndex);
    q->update();
  });

  connect(cell,
          &QLineEdit::textEdited,
          q,
          [this, q, cell](const QString& text)
  {
    const int cellIndex = _cellEdits.indexOf(cell);
    if (cellIndex < 0) { return; }
    Q_EMIT q->cellTextEdited(cellIndex, text);
    pushGlobalRoute();

    if (_pIsCellAutoAdvanceEnabled && cell->maxLength() > 0 && text.size() >= cell->maxLength())
    {
      if (cellIndex + 1 < _cellEdits.size())
      {
        _cellEdits[cellIndex + 1]->setFocus();
        _cellEdits[cellIndex + 1]->selectAll();
      }
    }
  });

  connect(cell,
          &QLineEdit::editingFinished,
          q,
          [this, q, cell]()
  {
    const int cellIndex = _cellEdits.indexOf(cell);
    if (cellIndex < 0) { return; }
    Q_EMIT q->cellEditingFinished(cellIndex);
  });

  connect(cell,
          &QLineEdit::returnPressed,
          q,
          [this, q, cell]()
  {
    const int cellIndex = _cellEdits.indexOf(cell);
    if (cellIndex < 0) { return; }
    Q_EMIT q->cellReturnPressed(cellIndex);
  });
}

void NXMultiCellLineEditPrivate::_updateCellValidState(int cellIndex)
{
  Q_Q(NXMultiCellLineEdit);
  QLineEdit *cell = _cellAt(cellIndex);
  if (!cell) { return; }

  bool isValid = true;
  if (const QValidator *validator = cell->validator())
  {
    QString text = cell->text();
    int pos      = 0;
    QValidator::State state = validator->validate(text, pos);
    isValid                 = text.isEmpty() || state == QValidator::Acceptable;
  }

  if (cellIndex < 0 || cellIndex >= _cellValidStates.size()) { return; }
  if (_cellValidStates[cellIndex] == isValid) { return; }
  _cellValidStates[cellIndex] = isValid;
  Q_EMIT q->cellValidatorStateChanged(cellIndex, isValid);
}

NXMultiCellLineEditPrivate::GlobalTextRouteState NXMultiCellLineEditPrivate::_currentGlobalTextRouteState() const
{
  GlobalTextRouteState state;
  state.texts.reserve(_cellEdits.size());
  state.cursorPositions.reserve(_cellEdits.size());
  state.selectionStarts.reserve(_cellEdits.size());
  state.selectionLengths.reserve(_cellEdits.size());

  for (auto *cell : _cellEdits)
  {
    if (!cell)
    {
      state.texts.append(QString());
      state.cursorPositions.append(0);
      state.selectionStarts.append(-1);
      state.selectionLengths.append(0);
      continue;
    }

    state.texts.append(cell->text());
    state.cursorPositions.append(cell->cursorPosition());
    const int selectionStart = cell->selectionStart();
    state.selectionStarts.append(selectionStart);
    state.selectionLengths.append(selectionStart >= 0 ? cell->selectedText().size() : 0);
  }
  return state;
}

void NXMultiCellLineEditPrivate::_applyGlobalTextRouteState(const GlobalTextRouteState& state)
{
  Q_Q(NXMultiCellLineEdit);
  _isApplyingGlobalRoute = true;

  const int count = std::min(_cellEdits.size(), state.texts.size());
  for (int i = 0; i < count; ++i)
  {
    QLineEdit *cell = _cellEdits[i];
    if (!cell) { continue; }
    QSignalBlocker blocker(cell);

    const QString& text = state.texts[i];
    cell->setText(text);
    cell->setCursorPosition(std::min(state.cursorPositions.value(i, 0), (int) text.size()));

    const int selectionStart  = state.selectionStarts.value(i, -1);
    const int selectionLength = state.selectionLengths.value(i, 0);
    if (selectionStart >= 0 && selectionLength > 0 && selectionStart <= text.size())
    {
      cell->setSelection(selectionStart, std::min(selectionLength, (int) text.size() - selectionStart));
    }
  }

  _isApplyingGlobalRoute = false;
  q->update();
}
