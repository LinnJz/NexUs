#ifndef NXCONTENTDIALOGPRIVATE_H
#define NXCONTENTDIALOGPRIVATE_H

#include <QObject>

#include "NXDef.h"
class QVBoxLayout;
class QHBoxLayout;
class NXPushButton;
class NXContentDialog;
class NXMaskWidget;
class NXAppBar;

class NXContentDialogPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXContentDialog)
public:
  explicit NXContentDialogPrivate(QObject *parent = nullptr);
  ~NXContentDialogPrivate() override;

private:
  NXThemeType::ThemeMode _themeMode;
  qint64 _currentWinID { 0 };

  int _leftButtonDoneCode { 0 /*Rejected*/ };
  int _middleButtonDoneCode { 0 /*Rejected*/ };
  int _rightButtonDoneCode { 1 /*Accepted*/ };
  NXAppBar *_appBar { nullptr };
  NXMaskWidget *_maskWidget { nullptr };
  QWidget *_centralWidget { nullptr };
  QWidget *_buttonWidget { nullptr };
  QVBoxLayout *_mainLayout { nullptr };
  QHBoxLayout *_buttonLayout { nullptr };
  NXPushButton *_leftButton { nullptr };
  NXPushButton *_middleButton { nullptr };
  NXPushButton *_rightButton { nullptr };
  void _doCloseAnimation(int result);
};

#endif // NXCONTENTDIALOGPRIVATE_H
