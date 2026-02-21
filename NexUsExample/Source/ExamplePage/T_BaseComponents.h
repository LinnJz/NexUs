#ifndef T_BASECOMPONENTS_H
#define T_BASECOMPONENTS_H

#include "T_BasePage.h"

class NXToggleSwitch;
class NXToggleButton;
class NXComboBox;
class NXMultiSelectComboBox;
class NXMessageButton;
class NXCheckBox;
class NXSpinBox;
class NXSlider;
class NXRadioButton;
class NXProgressBar;
class NXProgressRing;
class NXGroupBox;

class T_BaseComponents : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_BaseComponents(QWidget *parent = nullptr);
  ~T_BaseComponents() override;

protected:
  virtual void mouseReleaseEvent(QMouseEvent *event);

private:
  NXToggleSwitch *_toggleSwitch { nullptr };
  NXToggleButton *_toggleButton { nullptr };
  NXComboBox *_comboBox { nullptr };
  NXMultiSelectComboBox *_multiSelectComboBox { nullptr };
  NXMessageButton *_messageButton { nullptr };
  NXMessageButton *_infoMessageButton { nullptr };
  NXMessageButton *_warningMessageButton { nullptr };
  NXMessageButton *_errorMessageButton { nullptr };
  NXCheckBox *_checkBox { nullptr };
  NXSpinBox *_spinBox { nullptr };
  NXSlider *_slider { nullptr };
  NXRadioButton *_radioButton { nullptr };
  NXProgressBar *_progressBar { nullptr };
  NXProgressRing *_progressRing { nullptr };
  NXProgressRing *_progressPercentRing { nullptr };
  NXProgressRing *_progressBusyRing { nullptr };
  NXProgressRing *_progressBusyTransparentRing { nullptr };
  NXGroupBox *_groupBox { nullptr };
};

#endif // T_BASECOMPONENTS_H
