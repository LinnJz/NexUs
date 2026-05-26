#ifndef NXTERMINALWIDGET_H
#define NXTERMINALWIDGET_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXTerminalWidgetPrivate;

class NX_EXPORT NXTerminalWidget : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXTerminalWidget)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Prompt)
  Q_PROPERTY_CREATE_H(int, MaxHistorySize)
  Q_PROPERTY_CREATE_H(int, MaxLineCount)
  Q_PROPERTY_CREATE_H(int, FontPixelSize)

public:
  explicit NXTerminalWidget(QWidget *parent = nullptr);
  ~NXTerminalWidget();

  void appendOutput(const QString &text, const QColor &color = QColor());
  void appendHtml(const QString &html);
  void appendError(const QString &text);
  void appendSuccess(const QString &text);
  void clear();

  QStringList getCommandHistory() const;
  Q_SIGNAL void commandSubmitted(const QString &command);
  Q_SIGNAL void tabPressed(const QString &currentInput);

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // NXTERMINALWIDGET_H
