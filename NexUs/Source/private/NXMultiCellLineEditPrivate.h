#ifndef NXMULTICELLLINEEDITPRIVATE_H
#define NXMULTICELLLINEEDITPRIVATE_H

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>

#include "NXDef.h"
class QLabel;
class QHBoxLayout;
class QLineEdit;
class NXEvent;
class NXMultiCellLineEdit;
class NXMultiCellLineEditStyle;

class NXMultiCellLineEditPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXMultiCellLineEdit)
  Q_PROPERTY_CREATE_D(QString, CellSeparator)
  Q_PROPERTY_CREATE_D(QString, CellEmptyFillText)
  Q_PROPERTY_CREATE_D(int, CellCount)
  Q_PROPERTY_CREATE_D(int, CellSpacing)
  Q_PROPERTY_CREATE_D(int, ContentsHorizontalMargin)
  Q_PROPERTY_CREATE_D(bool, IsCellAutoAdvanceEnabled)

public:
  explicit NXMultiCellLineEditPrivate(QObject *parent = nullptr);
  ~NXMultiCellLineEditPrivate() override;

  Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);

  void rebuildCells(int cellCount) noexcept;
  void rebuildLayout() noexcept;
  void updateFocusCell(int cellIndex) noexcept;

  void pushGlobalRoute() noexcept;
  bool canGlobalUndo() const noexcept;
  bool canGlobalRedo() const noexcept;
  void globalUndo() noexcept;
  void globalRedo() noexcept;

  QLineEdit *createCell() noexcept;
  void setupCell(QLineEdit *cell, bool preserveClearAllowed) noexcept;
  void resetGlobalRoute() noexcept;
  bool isAllNormalEcho() const noexcept;
  bool isAnyEditable() const noexcept;
  void copyAllCells() const noexcept;
  void clearAllEditableCells() noexcept;
  void selectAllCells() const noexcept;
  void updateClearButtonState(QLineEdit *focusCell) noexcept;
  void updateCellObjectNames() noexcept;

  void animateCellMarkIn(int cellIndex) noexcept;
  void animateCellMarkOut(int cellIndex) noexcept;

private:
  struct GlobalTextRouteState
  {
    QStringList texts;
    QList<int> cursorPositions;
    QList<int> selectionStarts;
    QList<int> selectionLengths;
  };

  bool _isApplyingGlobalRoute { false };
  bool _isOverallOperationEnabled { false };
  int _globalRouteCurrentIndex { -1 };
  int _globalRedoRouteIndex { -1 };
  int _globalRouteMaxCount { 200 };
  int _currentFocusCellIndex { -1 };
  int _hoverCellIndex { -1 };
  NXThemeType::ThemeMode _themeMode;

  NXMultiCellLineEditStyle *_style { nullptr };
  NXEvent *_focusEvent { nullptr };

  QHBoxLayout *_cellLayout { nullptr };
  QList<QLineEdit *> _cellEdits;
  QList<QLabel *> _cellSeparators;
  QList<qreal> _cellExpandMarkWidths;
  QList<bool> _cellValidStates;
  QList<GlobalTextRouteState> _globalRouteList;

  QLineEdit *_cellAt(int cellIndex) const noexcept;
  int _cellIndex(QObject *watched) const noexcept;
  void _connectCellSignals(int cellIndex, QLineEdit *cell) noexcept;
  void _updateCellValidState(int cellIndex) noexcept;

  GlobalTextRouteState _currentGlobalTextRouteState() const noexcept;
  void _applyGlobalTextRouteState(const GlobalTextRouteState& state) noexcept;
};

#endif // NXMULTICELLLINEEDITPRIVATE_H
