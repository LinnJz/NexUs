#ifndef NXSTATUSBAR_H
#define NXSTATUSBAR_H

#include <QStatusBar>

#include "NXProperty.h"

class NX_EXPORT NXStatusBar : public QStatusBar
{
  Q_OBJECT
public:
  explicit NXStatusBar(QWidget *parent = nullptr);
  ~NXStatusBar() override;
};

#endif // NXSTATUSBAR_H
