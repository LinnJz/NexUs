#ifndef NXAPPLICATION_H
#define NXAPPLICATION_H

#include <QIcon>
#include <QObject>

#include "LinnSingleton.h"
#include "NXDef.h"
#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(CLASS)
#define nxApp NXApplication::getInstance()
class NXApplicationPrivate;

class NX_EXPORT NXApplication : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXApplication)
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXApplication))
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), NXMicaImagePath)
  Q_PROPERTY_CREATE_H(NXApplicationType::WindowDisplayMode, WindowDisplayMode)

private:
  explicit NXApplication(QObject *parent = nullptr);
  ~NXApplication();

public:
  void init();
  void syncWindowDisplayMode(QWidget *widget, bool isSync = true);
  static bool containsCursorToItem(QWidget *item);
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXAPPLICATION_H
