#ifndef T_TABLEVIEW_H
#define T_TABLEVIEW_H

#include "T_BasePage.h"

class NXTableView;

class T_TableView : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_TableView(QWidget *parent = nullptr);
  ~T_TableView();

private:
  NXTableView *_tableView { nullptr };
};

#endif // T_TABLEVIEW_H
