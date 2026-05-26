#ifndef NXAUTOCOMPLETECONTAINER_H
#define NXAUTOCOMPLETECONTAINER_H

#include <QWidget>

#include "NXDef.h"

class NXLineEdit;

class NXAutoCompleteContainer : public QWidget
{
  Q_OBJECT

public:
  explicit NXAutoCompleteContainer(QWidget *parent = nullptr);
  ~NXAutoCompleteContainer();

  void setForwardTarget(NXLineEdit *target);
  Q_SIGNAL void popupClosed();

protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void hideEvent(QHideEvent *event) override;

private:
  NXThemeType::ThemeMode _themeMode;
  NXLineEdit *_forwardTarget { nullptr };
};

#endif // NXAUTOCOMPLETECONTAINER_H
