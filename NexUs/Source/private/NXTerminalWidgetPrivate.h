#ifndef NXTERMINALWIDGETPRIVATE_H
#define NXTERMINALWIDGETPRIVATE_H

#include <QObject>
#include <QStringList>

#include "NXDef.h"
#include "NXProperty.h"

class QPlainTextEdit;
class QLineEdit;
class QLabel;
class NXTerminalWidget;

class NXTerminalWidgetPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTerminalWidget)
  Q_PROPERTY_CREATE_D(QString, Prompt)
  Q_PROPERTY_CREATE_D(int, MaxHistorySize)
  Q_PROPERTY_CREATE_D(int, MaxLineCount)
  Q_PROPERTY_CREATE_D(int, FontPixelSize)

public:
  explicit NXTerminalWidgetPrivate(QObject *parent = nullptr);
  ~NXTerminalWidgetPrivate();

  void submitCommand();
  void navigateHistory(int direction);
  void updateFont();
  void trimOutput();

private:
  NXThemeType::ThemeMode _themeMode;
  int _historyIndex { -1 };
  QStringList _commandHistory;
  QString _savedInput;
  QPlainTextEdit *_outputArea { nullptr };
  QLineEdit *_inputLine { nullptr };
  QLabel *_promptLabel { nullptr };
};

#endif // NXTERMINALWIDGETPRIVATE_H
