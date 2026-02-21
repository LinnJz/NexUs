#ifndef T_LISTVIEW_H
#define T_LISTVIEW_H

#include "T_BasePage.h"

class NXListView;

class T_ListView : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_ListView(QWidget *parent = nullptr);
  ~T_ListView();

private:
  NXListView *_listView { nullptr };
};

#endif // T_LISTVIEW_H
