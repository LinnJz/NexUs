#ifndef NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_
#define NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_

#include <QWidget>
#include "NXDef.h"
class NXKeyBinder;

class NXKeyBinderContainer : public QWidget
{
  Q_OBJECT
  Q_PRIVATE_CREATE_2(const QString&, QString, BinderKeyText)
  Q_PRIVATE_CREATE(quint32, NativeVirtualBinderKey)

public:
  explicit NXKeyBinderContainer(QWidget *parent = nullptr);
  ~NXKeyBinderContainer() override;
  void logOrResetHistoryData(bool isLog) noexcept;
  void saveBinderChanged() noexcept;

protected:
  bool event(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  QString _historyBinderKeyText;
  quint32 _historyNativeVirtualBinderKey { 0 };
  NXThemeType::ThemeMode _themeMode;
  NXKeyBinder *_keyBinder { nullptr };
};

#endif // NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_
