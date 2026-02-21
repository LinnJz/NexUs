#include "NXLineEditPrivate.h"

#include <algorithm>

#include "NXApplication.h"
#include "NXLineEdit.h"
#include "NXTheme.h"

NXLineEditPrivate::NXLineEditPrivate(QObject *parent)
    : QObject { parent }
{
}

NXLineEditPrivate::~NXLineEditPrivate() { }

void NXLineEditPrivate::resetTextRoute()
{
  _overallRedoRouteIndex   = -1;
  _textRouteCurrentIndex   = 0;
  _textRouteList.clear();
  _textRouteList.append(_currentTextRouteState());
}

void NXLineEditPrivate::pushTextRoute()
{
  _overallRedoRouteIndex = -1;

  TextRouteState state = _currentTextRouteState();
  if (_textRouteList.isEmpty())
  {
    _textRouteList.append(state);
    _textRouteCurrentIndex = 0;
    return;
  }

  _textRouteCurrentIndex = std::clamp(_textRouteCurrentIndex, 0, (int)_textRouteList.size() - 1);

  const TextRouteState& currentState = _textRouteList[_textRouteCurrentIndex];
  if (currentState.text == state.text && currentState.cursorPosition == state.cursorPosition &&
      currentState.selectionStart == state.selectionStart && currentState.selectionLength == state.selectionLength)
  {
    return;
  }

  if (_textRouteCurrentIndex < _textRouteList.size() - 1)
  {
    _textRouteList.resize(_textRouteCurrentIndex + 1);
  }

  _textRouteList.append(state);
  _textRouteCurrentIndex = _textRouteList.size() - 1;

  const int removableCount = _textRouteList.size() - _textRouteMaxCount;
  if (removableCount <= 0) { return; }

  const int removeCount = std::min(removableCount, (int) _textRouteList.size() - 1);
  _textRouteList.remove(1, removeCount);
  _textRouteCurrentIndex = std::max(0, _textRouteCurrentIndex - removeCount);
  if (_overallRedoRouteIndex >= 0) { _overallRedoRouteIndex = std::max(-1, _overallRedoRouteIndex - removeCount); }
}

bool NXLineEditPrivate::canTextRouteBack() const
{
  return _textRouteCurrentIndex > 0;
}

bool NXLineEditPrivate::canTextRouteForward() const
{
  return _textRouteCurrentIndex + 1 < _textRouteList.size();
}

void NXLineEditPrivate::textRouteBack()
{
  if (!canTextRouteBack()) { return; }
  _textRouteCurrentIndex -= 1;
  _applyTextRouteState(_textRouteList[_textRouteCurrentIndex]);
}

void NXLineEditPrivate::textRouteForward()
{
  if (!canTextRouteForward()) { return; }
  _textRouteCurrentIndex += 1;
  _applyTextRouteState(_textRouteList[_textRouteCurrentIndex]);
}

bool NXLineEditPrivate::canOverallUndo() const
{
  Q_Q(const NXLineEdit);
  if (_overallRedoRouteIndex >= 0) { return false; }
  if (_textRouteList.isEmpty()) { return false; }
  return q->text() != _textRouteList.first().text;
}

bool NXLineEditPrivate::canOverallRedo() const
{
  if (_textRouteList.isEmpty()) { return false; }
  return _overallRedoRouteIndex >= 0 && _overallRedoRouteIndex < _textRouteList.size();
}

void NXLineEditPrivate::overallUndo()
{
  if (!canOverallUndo()) { return; }
  _overallRedoRouteIndex = _textRouteCurrentIndex;

  _textRouteCurrentIndex = 0;
  _applyTextRouteState(_textRouteList[_textRouteCurrentIndex]);
}

void NXLineEditPrivate::overallRedo()
{
  if (!canOverallRedo()) { return; }
  _textRouteCurrentIndex = std::clamp(_overallRedoRouteIndex, 0, (int) _textRouteList.size() - 1);
  _overallRedoRouteIndex = -1;
  _applyTextRouteState(_textRouteList[_textRouteCurrentIndex]);
}

void NXLineEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
  Q_Q(NXLineEdit);
  NXAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<NXAppBarType::WMMouseActionType>();
  if (actionType == NXAppBarType::WMLBUTTONDOWN)
  {
    if (q->hasSelectedText() && q->hasFocus()) { q->clearFocus(); }
  }
  else if (actionType == NXAppBarType::WMLBUTTONUP || actionType == NXAppBarType::WMNCLBUTTONDOWN)
  {
    if (NXApplication::containsCursorToItem(q) || (actionType == NXAppBarType::WMLBUTTONUP && q->hasSelectedText()))
    {
      return;
    }
    if (q->hasFocus()) { q->clearFocus(); }
  }
}

void NXLineEditPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode)
{
  Q_Q(NXLineEdit);
  _themeMode       = themeMode;
  QPalette palette = q->palette();
  palette.setColor(QPalette::Text, NXThemeColor(_themeMode, BasicText));
  palette.setColor(QPalette::PlaceholderText,
                   _themeMode == NXThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
  q->setPalette(palette);
}

NXLineEditPrivate::TextRouteState NXLineEditPrivate::_currentTextRouteState() const
{
  Q_Q(const NXLineEdit);
  TextRouteState state;
  state.text           = q->text();
  state.cursorPosition = q->cursorPosition();
  state.selectionStart = q->selectionStart();
  if (state.selectionStart >= 0) { state.selectionLength = q->selectedText().size(); }
  return state;
}

void NXLineEditPrivate::_applyTextRouteState(const TextRouteState& state)
{
  Q_Q(NXLineEdit);
  q->setText(state.text);
  q->setCursorPosition(std::min(state.cursorPosition, (int)state.text.size()));
  if (state.selectionStart >= 0 && state.selectionLength > 0 && state.selectionStart <= state.text.size())
  {
    q->setSelection(state.selectionStart,
                    std::min(state.selectionLength, (int) state.text.size() - state.selectionStart));
  }
}
