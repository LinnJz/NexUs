#ifndef NXACTIONCOMMANDERVIEW_H
#define NXACTIONCOMMANDERVIEW_H

#include <QTreeView>

#include "NXProperty.h"

class NXActionCommanderViewStyle;

class NXActionCommanderView : public QTreeView
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QS_SET_CREF(QString), DomainName)

public:
  explicit NXActionCommanderView(QWidget *parent = nullptr);
  ~NXActionCommanderView() override;

  Q_SLOT void onCustomContextMenuRequested(const QPoint &pos);

Q_SIGNALS:
  Q_SIGNAL void runToCommand(const QString &domainName, int commandIndex);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  NXActionCommanderViewStyle *_style { nullptr };
};

#endif //NXACTIONCOMMANDERVIEW_H
