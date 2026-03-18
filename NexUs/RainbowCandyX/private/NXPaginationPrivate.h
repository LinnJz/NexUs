#ifndef NXPAGINATIONPRIVATE_H
#define NXPAGINATIONPRIVATE_H

#include <QList>
#include <QObject>
#include <QPair>
#include <QRect>

#include "NXDef.h"
class NXLineEdit;
class NXPagination;
class NXPaginationPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXPagination)
    Q_PROPERTY_CREATE_D(int, CurrentPage)
    Q_PROPERTY_CREATE_D(int, TotalPages)
    Q_PROPERTY_CREATE_D(int, ButtonSize)
    Q_PROPERTY_CREATE_D(int, PagerCount)
    Q_PROPERTY_CREATE_D(bool, JumperVisible)
public:
    explicit NXPaginationPrivate(QObject* parent = nullptr);
    ~NXPaginationPrivate() override;

    QList<QPair<QRect, int>> _getButtonRects() const;
    QList<int> _getVisiblePages() const;
    void _updateJumperPosition();

    NXLineEdit* _jumperEdit{nullptr};

private:
    NXThemeType::ThemeMode _themeMode;
    int _hoverIndex{-1};
};

#endif // NXPAGINATIONPRIVATE_H
