#ifndef NXAUTOCOMPLETE_H
#define NXAUTOCOMPLETE_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXAutoCompletePrivate;

class NX_EXPORT NXAutoComplete : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXAutoComplete)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, MaxVisibleItems)
  Q_PROPERTY_CREATE_H(Qt::CaseSensitivity, CaseSensitivity)

public:
  enum MatchMode
  {
    Contains = 0,
    StartsWith,
    EndsWith,
    RegExp,
  };
  Q_ENUM(MatchMode)

  explicit NXAutoComplete(QWidget *parent = nullptr);
  ~NXAutoComplete();

  void setPlaceholderText(const QString &placeholderText);
  QString placeholderText() const;

  void setText(const QString &text);
  QString text() const;

  void setCompletions(const QStringList &completions);
  QStringList completions() const;

  void setMatchMode(MatchMode mode);
  MatchMode matchMode() const;

  void setFixedHeight(int h);
  Q_SIGNAL void textChanged(const QString &text);
  Q_SIGNAL void textEdited(const QString &text);
  Q_SIGNAL void completionSelected(const QString &completion);
  Q_SIGNAL void returnPressed(const QString &text);
};

#endif // NXAUTOCOMPLETE_H
