#ifndef NXLAZYSTACKEDLAYOUTPRIVATE_H
#define NXLAZYSTACKEDLAYOUTPRIVATE_H

#include <functional>

#include <QList>
#include <QObject>

#include "NXLazyStackedLayout.h"

class QLayoutItem;
class QWidget;

class NXLazyStackedLayoutPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXLazyStackedLayout)

public:
  explicit NXLazyStackedLayoutPrivate(QObject *parent = nullptr);
  ~NXLazyStackedLayoutPrivate() override;

  struct StackItemTag
  {
    QLayoutItem *Item { nullptr };
    std::function<QWidget *()> LazyFactory {};
  };

private:
  QList<StackItemTag> _itemList {};
  int _currentIndex { -1 };
  NXLazyStackedLayout::StackingMode _stackingMode { NXLazyStackedLayout::StackOne };
};

#endif // NXLAZYSTACKEDLAYOUTPRIVATE_H
