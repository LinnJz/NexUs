#ifndef NXFRAMEWORK_NXROLLERPICKERCONTAINER_H
#define NXFRAMEWORK_NXROLLERPICKERCONTAINER_H

#include <QPropertyAnimation>
#include <QWidget>
#include "NXDef.h"
class NXRoller;

class NXRollerPickerContainer : public QWidget
{
  friend class NXRollerPicker;
  friend class NXRollerPickerPrivate;
  Q_OBJECT
  Q_PROPERTY_CREATE(int, AnimationPixOffsetY)
  Q_PRIVATE_CREATE(int, ButtonAreaHeight)

public:
  explicit NXRollerPickerContainer(QWidget *parent = nullptr);
  ~NXRollerPickerContainer() override;

  void doPickerAnimation() noexcept;

Q_SIGNALS:
  void overButtonClicked();
  void cancelButtonClicked();

protected:
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void hideEvent(QHideEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  int _buttonMargin { 5 };
  int _buttonSpacing { 5 };
  bool _isOverButtonHover { false };
  bool _isCancelButtonHover { false };
  bool _isOverButtonClicked { false };
  NXThemeType::ThemeMode _themeMode;

  QPixmap _animationPix;
  QRect _overButtonRect;
  QRect _cancelButtonRect;
  QList<NXRoller *> _rollerList;
  QList<int> _historyIndexList;
  void _handleSaveOrReset(bool isSave);
};

#endif // NXFRAMEWORK_NXROLLERPICKERCONTAINER_H
