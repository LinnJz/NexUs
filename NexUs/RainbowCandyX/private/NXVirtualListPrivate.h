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
    Q_PROPERTY_CREATE_D(bool, IsTransparent)
    Q_PROPERTY_CREATE_D(bool, IsAlternatingRowColors)
    Q_PROPERTY_CREATE_D(int, ItemHeight)

  public:
    explicit NXVirtualListPrivate(QObject* parent = nullptr);
    ~NXVirtualListPrivate() override;

private:
    int _itemCount { 0 };
    NXThemeType::ThemeMode _themeMode;
    NXVirtualListStyle* _listViewStyle{nullptr};
};

#endif // NXVIRTUALLISTPRIVATE_H
