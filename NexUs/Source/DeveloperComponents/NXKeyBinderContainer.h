#ifndef NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_
#define NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_

#include <QWidget>

#include "NXDef.h"
class NXKeyBinder;

class NXKeyBinderContainer : public QWidget
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QS_SET_CREF(QString), BinderKeyText)
  Q_PRIVATE_CREATE(quint32, NativeVirtualBinderKey)

public:
  explicit NXKeyBinderContainer(QWidget *parent = nullptr);
  ~NXKeyBinderContainer();
  void logOrResetHistoryData(bool isLog);
  void saveBinderChanged();

protected:
  bool event(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  quint32 _historyNativeVirtualBinderKey { 0 };
  NXThemeType::ThemeMode _themeMode;
  QString _historyBinderKeyText;
  NXKeyBinder *_keyBinder { nullptr };
};

#endif //NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_
