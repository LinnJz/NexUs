#ifndef T_SETTING_H
#define T_SETTING_H

#include "T_BasePage.h"
class NXRadioButton;
class NXToggleSwitch;
class NXComboBox;

class T_Setting : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_Setting(QWidget *parent = nullptr);
  ~T_Setting() override;

private:
  NXComboBox *_themeComboBox { nullptr };
  NXRadioButton *_normalButton { nullptr };

  NXRadioButton *_windowNormalButton { nullptr };
  NXRadioButton *_windowPixmapButton { nullptr };
  NXRadioButton *_windowMovieButton { nullptr };

  NXRadioButton *_elaMicaButton { nullptr };
#ifdef Q_OS_WIN
  NXRadioButton *_micaButton { nullptr };
  NXRadioButton *_micaAltButton { nullptr };
  NXRadioButton *_acrylicButton { nullptr };
  NXRadioButton *_dwmBlurnormalButton { nullptr };
#endif
  NXToggleSwitch *_logSwitchButton { nullptr };
  NXToggleSwitch *_userCardSwitchButton { nullptr };
  NXRadioButton *_minimumButton { nullptr };
  NXRadioButton *_compactButton { nullptr };
  NXRadioButton *_maximumButton { nullptr };
  NXRadioButton *_autoButton { nullptr };

  NXRadioButton *_noneButton { nullptr };
  NXRadioButton *_popupButton { nullptr };
  NXRadioButton *_scaleButton { nullptr };
  NXRadioButton *_flipButton { nullptr };
  NXRadioButton *_blurButton { nullptr };
};

#endif // T_SETTING_H
