#ifndef NXAPPLICATION_H
#define NXAPPLICATION_H

#include <QIcon>
#include <QObject>

#include "NXDef.h"
#include "singleton.h"
#define nxApp NXApplication::getInstance()
class NXApplicationPrivate;

class NX_EXPORT NXApplication : public QObject
{
#pragma push_macro("DISABLE_COPY")
#undef DISABLE_COPY
#define DISABLE_COPY(Class)
  SINGLETON_CREATE_H(NXApplication)
#pragma pop_macro("DISABLE_COPY")
  Q_OBJECT
  Q_Q_CREATE(NXApplication)
  Q_PROPERTY_CREATE_Q_H(NXApplicationType::WindowDisplayMode, WindowDisplayMode)

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

#endif // NXAPPLICATION_H
