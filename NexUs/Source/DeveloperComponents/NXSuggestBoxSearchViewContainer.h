#ifndef NXSUGGESTBOXSEARCHVIEWCONTAINER_H
#define NXSUGGESTBOXSEARCHVIEWCONTAINER_H

#include <QWidget>

#include "NXDef.h"

class NXSuggestBoxSearchViewContainer : public QWidget
{
  Q_OBJECT
public:
  explicit NXSuggestBoxSearchViewContainer(QWidget *parent = nullptr);
  ~NXSuggestBoxSearchViewContainer();

protected:
  virtual void paintEvent(QPaintEvent *event) override;

private:
  NXThemeType::ThemeMode _themeMode;
};

#endif // NXSUGGESTBOXSEARCHVIEWCONTAINER_H
