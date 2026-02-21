#ifndef NXMICABASEINITOBJECT_H
#define NXMICABASEINITOBJECT_H

#include <QImage>
#include <QObject>
class NXApplicationPrivate;

class NXMicaBaseInitObject : public QObject
{
  Q_OBJECT
public:
  explicit NXMicaBaseInitObject(NXApplicationPrivate *appPrivate, QObject *parent = nullptr);
  ~NXMicaBaseInitObject();
  Q_SLOT void onInitMicaBase(const QImage& img);
Q_SIGNALS:
  Q_SIGNAL void initFinished();

private:
  NXApplicationPrivate *_appPrivate { nullptr };
};

#endif // NXMICABASEINITOBJECT_H
