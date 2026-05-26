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
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), SourceTitle)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), TargetTitle)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, ItemHeight)
  Q_PROPERTY_CREATE_H(bool, IsSearchVisible)

public:
  explicit NXTransfer(QWidget *parent = nullptr);
  ~NXTransfer();

  void setSourceItems(const QStringList &items);
  void addSourceItem(const QString &text);
  void addSourceItems(const QStringList &items);

  QStringList getSourceItems() const;
  QStringList getTargetItems() const;

  void moveToTarget();
  void moveToSource();
  void moveAllToTarget();
  void moveAllToSource();
  Q_SIGNAL void transferChanged(const QStringList &sourceItems, const QStringList &targetItems);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXTRANSFER_H
