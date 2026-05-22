#ifndef TEST_SIGNALS3_H
#define TEST_SIGNALS3_H

#include <QObject>

class TestSignals3 : public QObject
{
  Q_OBJECT

public:
  void init();
  Q_SIGNAL void dataChanged(int value);
  Q_SIGNAL void finished();

private:
  void internalCleanup();
};

#endif
