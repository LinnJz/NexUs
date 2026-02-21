#ifndef T_TABLEWIDGET_H
#define T_TABLEWIDGET_H

#include "T_BasePage.h"

class NXTableWidget;

class T_TableWidget : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_TableWidget(QWidget *parent = nullptr);
  ~T_TableWidget();

private:
  NXTableWidget *_tableWidget { nullptr };
};

#endif // T_TABLEWIDGET_H
