#ifndef TEST_SIGNALS_H
#define TEST_SIGNALS_H

#include <QObject>

class TestSignals : public QObject
{
  Q_OBJECT

public:
  TestSignals(QObject *parent = nullptr);

  void normalFunction();
  Q_SIGNAL void clicked();
};

#endif
