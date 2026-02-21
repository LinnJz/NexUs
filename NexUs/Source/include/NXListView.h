#ifndef NXLISTVIEW_H
#define NXLISTVIEW_H

#include <QListView>

#include "NXProperty.h"

class NXListViewPrivate;

class NX_EXPORT NXListView : public QListView
{
  Q_OBJECT
  Q_Q_CREATE(NXListView)
  Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
  Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
public:
  explicit NXListView(QWidget *parent = nullptr);
  ~NXListView();
};

#endif // NXLISTVIEW_H
