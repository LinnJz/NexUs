#ifndef NXPROGRESSBAR_H
#define NXPROGRESSBAR_H

#include <QProgressBar>

#include "NXProperty.h"

class NXProgressBarPrivate;

class NX_EXPORT NXProgressBar : public QProgressBar
{
  Q_OBJECT
  Q_Q_CREATE(NXProgressBar)
public:
  explicit NXProgressBar(QWidget *parent = nullptr);
  ~NXProgressBar() override;
  void setMinimum(int minimum);
  void setMaximum(int maximum);

protected:
  virtual void paintEvent(QPaintEvent *event) override;
  virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // NXPROGRESSBAR_H
