#ifndef NXFRAMEWORK_NXROLLERPICKERPRIVATE_H
#define NXFRAMEWORK_NXROLLERPICKERPRIVATE_H
#include <QObject>
#include "NXDef.h"
#include "NXRoller.h"
#include "NXRollerPickerContainer.h"
class QHBoxLayout;
class NXRollerPicker;

class NXRollerPickerPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXRollerPicker)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
  explicit NXRollerPickerPrivate(QObject *parent = nullptr);
  ~NXRollerPickerPrivate() override;
  Q_SLOT void onRollerPickerClicked();
  Q_SLOT void onOverButtonClicked();
  Q_SLOT void onCancelButtonClicked();

private:
  int _pickerRollerHeight { 245 };
  NXThemeType::ThemeMode _themeMode;
  QHBoxLayout *_containerLayout { nullptr };
  NXRollerPickerContainer *_rollerPickerContainer { nullptr };
  int _getRollerTotalWidth() const;
};

#endif // NXFRAMEWORK_NXROLLERPICKERPRIVATE_H
