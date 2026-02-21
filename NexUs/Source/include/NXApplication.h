#ifndef NXAPPLICATION_H
#define NXAPPLICATION_H

#include <QIcon>
#include <QObject>

#include "LinnSingleton.h"
#include "NXDef.h"

#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(Class)

#define nxApp NXApplication::getInstance()
class NXApplicationPrivate;

class NX_EXPORT NXApplication : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXApplication)
  Q_PROPERTY_CREATE_Q_H(NXApplicationType::WindowDisplayMode, WindowDisplayMode)
  LINN_SINGLETON_CREATE(LINN_SINGLETON_UNIQUE(NXApplication))

private:
  explicit NXApplication(QObject *parent = nullptr);
  ~NXApplication() override;

public:
  void init();
  void syncWindowDisplayMode(QWidget *widget, bool isSync = true);
  static bool containsCursorToItem(QWidget *item);

  void setMicaImagePath(const QString& micaImagePath);
  QString getMicaImagePath() const;
  Q_SIGNAL void pMicaImagePathChanged();
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXAPPLICATION_H
