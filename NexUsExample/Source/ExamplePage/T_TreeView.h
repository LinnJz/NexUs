#ifndef T_TREEVIEW_H
#define T_TREEVIEW_H

#include "T_BasePage.h"

class NXTreeView;

class T_TreeView : public T_BasePage
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit T_TreeView(QWidget *parent = nullptr);
  ~T_TreeView();

private:
  NXTreeView *_treeView { nullptr };
};

#endif // T_TREEVIEW_H
