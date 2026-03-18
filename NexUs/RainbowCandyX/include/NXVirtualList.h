#ifndef NXVIRTUALLIST_H
#define NXVIRTUALLIST_H

#include <QListView>

#include "NXProperty.h"

class NXVirtualListPrivate;
class NX_EXPORT NXVirtualList : public QListView
{
    Q_OBJECT
    Q_Q_CREATE(NXVirtualList)
    Q_PROPERTY_CREATE_H(int, ItemHeight)
    Q_PROPERTY_CREATE_H(bool, IsTransparent)
    Q_PROPERTY_CREATE_H(bool, IsAlternatingRowColors)
public:
    explicit NXVirtualList(QWidget* parent = nullptr);
    ~NXVirtualList() override;

    void setItemCount(int count) noexcept;
    int getItemCount() const noexcept;

Q_SIGNALS:
  void itemRequestData(int startIndex, int endIndex);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // NXVIRTUALLIST_H
