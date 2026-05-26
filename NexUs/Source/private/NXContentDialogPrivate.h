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
  ~NXContentDialogPrivate();

  void startCloseAnimation(NXPushButton *triggeredButton);

private:
  bool _isClosing { false };
  NXThemeType::ThemeMode _themeMode;
  qint64 _currentWinID { 0 };
  NXAppBar *_appBar { nullptr };
  NXMaskWidget *_maskWidget { nullptr };
  QWidget *_centralWidget { nullptr };
  QWidget *_buttonWidget { nullptr };
  QVBoxLayout *_mainLayout { nullptr };
  QHBoxLayout *_buttonLayout { nullptr };
  NXPushButton *_activeButton { nullptr };
  QList<NXPushButton *> _buttons;

  void _handleCloseAnimation();
  void _moveToCenter();
};

#endif // NXCONTENTDIALOGPRIVATE_H
