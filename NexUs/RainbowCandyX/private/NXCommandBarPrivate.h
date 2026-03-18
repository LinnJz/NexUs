#ifndef NXCOMMANDBARPRIVATE_H
#define NXCOMMANDBARPRIVATE_H

#include <QObject>

#include "NXProperty.h"
#include "NXCommandBar.h"

class NXMenu;
class NXCommandBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXCommandBar)
    Q_PROPERTY_CREATE_D(int, ButtonSize)
public:
    explicit NXCommandBarPrivate(QObject* parent = nullptr);
    ~NXCommandBarPrivate();

    QList<QRect> _getVisibleItemRects() const;
    int _getItemWidth(const NXCommandBar::CommandItem& item) const;

private:
    NXThemeType::ThemeMode _themeMode;
    QList<NXCommandBar::CommandItem> _items;
    int _hoveredIndex{-1};
    int _pressedIndex{-1};
    int _visibleCount{0};
    NXMenu* _overflowMenu{nullptr};
};

#endif // NXCOMMANDBARPRIVATE_H
