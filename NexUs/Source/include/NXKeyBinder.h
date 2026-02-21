#ifndef NEXUS_SRC_NXKEYBINDER_H_
#define NEXUS_SRC_NXKEYBINDER_H_

#include <QLabel>
#include "NXDef.h"

class NXKeyBinderPrivate;

class NX_EXPORT NXKeyBinder : public QLabel
{
  Q_OBJECT
  Q_Q_CREATE(NXKeyBinder)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(quint32, NativeVirtualBinderKey)
public:
  explicit NXKeyBinder(QWidget *parent = nullptr);
  ~NXKeyBinder();
  void setBinderKeyText(const QString& binderKeyText);
  QString getBinderKeyText() const;
Q_SIGNALS:
  Q_SIGNAL void binderKeyTextChanged(const QString& binderKeyText);
  Q_SIGNAL void nativeVirtualBinderKeyChanged(quint32 binderKey);
  Q_SIGNAL void pBinderKeyTextChanged();

protected:
  virtual bool event(QEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NEXUS_SRC_NXKEYBINDER_H_
