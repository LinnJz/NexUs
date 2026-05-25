#ifndef TEST_SIGNALS2_H
#define TEST_SIGNALS2_H

#include <QObject>

class TestSignals2 : public QObject
{
  Q_OBJECT

public:
  void doSomething();
  Q_SIGNAL void clicked();
  Q_SIGNAL void toggled(bool checked);

protected:
  void helperMethod();

private:
  int m_data;
};

#endif
