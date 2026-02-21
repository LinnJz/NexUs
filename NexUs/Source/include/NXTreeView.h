#ifndef NXTREEVIEW_H
#define NXTREEVIEW_H

#include <QTreeView>

#include "NXDef.h"

class NXTreeViewPrivate;

class NX_EXPORT NXTreeView : public QTreeView
{
  Q_OBJECT
  Q_Q_CREATE(NXTreeView)
  Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
  Q_PROPERTY_CREATE_Q_H(int, HeaderMargin)
public:
  explicit NXTreeView(QWidget *parent = nullptr);
  ~NXTreeView();
  void setNXIcon(NXIconType::IconName icon);
};

#endif // NXTREEVIEW_H
