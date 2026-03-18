#ifndef NXTRANSFER_H
#define NXTRANSFER_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXTransferPrivate;
class NX_EXPORT NXTransfer : public QWidget
{
    Q_OBJECT
  Q_Q_CREATE(NXTransfer)
  Q_PROPERTY_CREATE_H(bool, IsSearchVisible)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, ItemHeight)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, SourceTitle)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, TargetTitle)

  public:
    explicit NXTransfer(QWidget* parent = nullptr);
    ~NXTransfer() override;

    void setSourceItems(const QStringList& items) noexcept;
    void addSourceItem(const QString& text) noexcept;
    void addSourceItems(const QStringList& items) noexcept;

    QStringList getSourceItems() const noexcept;
    QStringList getTargetItems() const noexcept;

    void moveToTarget() noexcept;
    void moveToSource() noexcept;
    void moveAllToTarget() noexcept;
    void moveAllToSource() noexcept;

Q_SIGNALS:
  void transferChanged(const QStringList& sourceItems, const QStringList& targetItems);

protected:
  void paintEvent(QPaintEvent* event) override;
};

#endif // NXTRANSFER_H
