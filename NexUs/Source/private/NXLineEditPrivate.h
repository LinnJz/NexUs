#ifndef NXLINEEDITPRIVATE_H
#define NXLINEEDITPRIVATE_H

#include <QMargins>
#include <QObject>
#include <QVariantMap>

#include "NXDef.h"
class NXEvent;
class NXLineEdit;
class NXLineEditStyle;

class NXLineEditPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXLineEdit)
  Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
  Q_PROPERTY_CREATE_D(bool, IsClearButtonEnabled)
  Q_PROPERTY_CREATE_D(QMargins, ContentMargins)
public:
  explicit NXLineEditPrivate(QObject *parent = nullptr);
  ~NXLineEditPrivate() override;
  Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);
  void resetTextRoute();
  void pushTextRoute();
  bool canTextRouteBack() const;
  bool canTextRouteForward() const;
  void textRouteBack();
  void textRouteForward();
  bool canOverallUndo() const;
  bool canOverallRedo() const;
  void overallUndo();
  void overallRedo();

private:
  struct TextRouteState
  {
    QString text;
    int cursorPosition { 0 };
    int selectionStart { -1 };
    int selectionLength { 0 };
  };

  int _overallRedoRouteIndex { -1 };

  int _textRouteCurrentIndex { -1 };
  int _textRouteMaxCount { 200 };

  NXThemeType::ThemeMode _themeMode;
  qreal _textSpacing { 0.5 };
  NXEvent *_focusEvent { nullptr };
  NXLineEditStyle *_lineEditStyle { nullptr };

  QList<TextRouteState> _textRouteList;

  TextRouteState _currentTextRouteState() const;
  void _applyTextRouteState(const TextRouteState& state);
};

#endif // NXLINEEDITPRIVATE_H
