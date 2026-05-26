#ifndef NXVIRTUALLISTPRIVATE_H
#define NXVIRTUALLISTPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXVirtualList;
class NXVirtualListStyle;

class NXVirtualListPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXVirtualList)
  Q_PROPERTY_CREATE_D(int, ItemHeight)
  Q_PROPERTY_CREATE_D(bool, IsTransparent)
  Q_PROPERTY_CREATE_D(bool, IsAlternatingRowColors)

public:
  explicit NXVirtualListPrivate(QObject *parent = nullptr);
  ~NXVirtualListPrivate();

private:
  NXThemeType::ThemeMode _themeMode;
  int _itemCount { 0 };
  NXVirtualListStyle *_listViewStyle { nullptr };
};

#endif // NXVIRTUALLISTPRIVATE_H
