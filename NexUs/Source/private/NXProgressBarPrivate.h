#ifndef NXPROGRESSBARPRIVATE_H
#define NXPROGRESSBARPRIVATE_H

#include <QObject>

#include "NXProperty.h"

class NXProgressBarStyle;
class QPropertyAnimation;
class NXProgressBar;

class NXProgressBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXProgressBar)
public:
  explicit NXProgressBarPrivate(QObject *parent = nullptr);
  ~NXProgressBarPrivate();

private:
  bool _isBusyAnimation { false };
  NXProgressBarStyle *_style { nullptr };
  QPropertyAnimation *_busyAnimation { nullptr };
};

#endif // NXPROGRESSBARPRIVATE_H
