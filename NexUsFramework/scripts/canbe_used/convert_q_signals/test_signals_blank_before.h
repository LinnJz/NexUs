#ifndef TEST_SIGNALS_BLANK_H
#define TEST_SIGNALS_BLANK_H

#include <QObject>

class BlankTest : public QObject
{
  Q_OBJECT

public:
  void setup();
  Q_SIGNAL void valueChanged(int newValue);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif
