#ifndef NXSPLITINPUTPRIVATE_H
#define NXSPLITINPUTPRIVATE_H

#include <QObject>
#include <QVector>

#include "NXDef.h"

class QHBoxLayout;
class QLabel;
class QLineEdit;
class QVariantAnimation;
class NXSplitInput;

class NXSplitInputPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSplitInput)
  Q_PROPERTY_CREATE_D(qreal, SegmentSpacing)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(bool, IsClearButtonEnable)
  Q_PROPERTY_CREATE_D(bool, IsKeepMarkOnCompleted)

public:
  explicit NXSplitInputPrivate(QObject *parent = nullptr);
  ~NXSplitInputPrivate();
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
  bool _allCompleted { false };
  NXThemeType::ThemeMode _themeMode;
  int _segmentCount { 1 };
  int _defaultMaxLength { 999999 };
  int _hoveredIndex { -1 };
  QString _separatorText;
  QHBoxLayout *_boxLayout { nullptr };
  QVector<QLineEdit *> _lineEdits;
  QVector<QLabel *> _separators;
  QVector<int> _maxLengths;
  QVector<qreal> _segmentMarks;
  QVector<bool> _segmentCompleted;
  QVector<QVariantAnimation *> _markAnimations;

  void _buildWidgets();
  void _updateCompletedState();
  void _updateSeparatorColors();
  void _onChildTextChanged(int index);
  void _onChildEditingFinished(int index);
  void _animateMarkIn(int index);
  void _animateMarkOut(int index);
};

#endif // NXSPLITINPUTPRIVATE_H
