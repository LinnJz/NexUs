#ifndef TEST_MULTI_SIGNALS_H
#define TEST_MULTI_SIGNALS_H

#include <QObject>

class MyWidget : public QObject
{
  Q_OBJECT

public:
  MyWidget(QObject *parent = nullptr);
  void animate();
  Q_SIGNAL void started();
  Q_SIGNAL void progressUpdated(int percent);
  Q_SIGNAL void finished();

public slots:
  void onStart();
  void onStop();
  Q_SIGNAL void errorOccurred(const QString &message);

protected:
  void paintEvent();
};

#endif
