#ifndef NXFRAMEWORK_NXDIALOGPRIVATE_H
#define NXFRAMEWORK_NXDIALOGPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXAppBar;
class NXDialog;

class NXDialogPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXDialog)
public:
  explicit NXDialogPrivate(QObject *parent = nullptr);
  ~NXDialogPrivate() override;

private:
  NXThemeType::ThemeMode _themeMode;
  NXApplicationType::WindowDisplayMode _windowDisplayMode;
  NXAppBar *_appBar { nullptr };
};

#endif // NXFRAMEWORK_NXDIALOGPRIVATE_H
