#ifndef NXMULTICELLLINEEDITPRIVATE_H
#define NXMULTICELLLINEEDITPRIVATE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
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

  void rebuildCells(int cellCount);
  void rebuildLayout();
  void updateFocusCell(int cellIndex);

  void pushGlobalRoute();
  bool canGlobalUndo() const;
  bool canGlobalRedo() const;
  void globalUndo();
  void globalRedo();

  QLineEdit *createCell();
  void setupCell(QLineEdit *cell, bool preserveClearAllowed);
  void resetGlobalRoute();
  bool isAllNormalEcho() const;
  bool isAnyEditable() const;
  void copyAllCells() const;
  void clearAllEditableCells();
  void selectAllCells() const;
  void updateClearButtonState(QLineEdit *focusCell);
  void updateCellObjectNames();

  void animateCellMarkIn(int cellIndex);
  void animateCellMarkOut(int cellIndex);

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

  QLineEdit *_cellAt(int cellIndex) const;
  int _cellIndex(QObject *watched) const;
  void _connectCellSignals(int cellIndex, QLineEdit *cell);
  void _updateCellValidState(int cellIndex);

  GlobalTextRouteState _currentGlobalTextRouteState() const;
  void _applyGlobalTextRouteState(const GlobalTextRouteState& state);
};

#endif // NXMULTICELLLINEEDITPRIVATE_H
