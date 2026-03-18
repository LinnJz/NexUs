#include "NXTeachingTipPrivate.h"

NXTeachingTipPrivate::NXTeachingTipPrivate(QObject *parent)
    : QObject(parent)
{
}

NXTeachingTipPrivate::~NXTeachingTipPrivate() { }

QPoint NXTeachingTipPrivate::_calculatePosition() const
{
  Q_Q(const NXTeachingTip);
  if (!_target) { return QPoint(0, 0); }
  QPoint targetCenter             = _target->mapToGlobal(QPoint(_target->width() / 2, _target->height() / 2));
  NXTeachingTip::TailPosition pos = _tailPosition;
  if (pos == NXTeachingTip::Auto) { pos = _calculateAutoTailPosition(); }

  int gap = _tailSize + 4;
  QPoint result;
  switch (pos)
  {
  case NXTeachingTip::Bottom :
    // Tip appears above target, tail points down
    result = QPoint(targetCenter.x() - q->width() / 2, _target->mapToGlobal(QPoint(0, 0)).y() - q->height() - gap);
    break;
  case NXTeachingTip::Top :
    // Tip appears below target, tail points up
    result = QPoint(targetCenter.x() - q->width() / 2, _target->mapToGlobal(QPoint(0, _target->height())).y() + gap);
    break;
  case NXTeachingTip::Right :
    // Tip appears left of target, tail points right
    result = QPoint(_target->mapToGlobal(QPoint(0, 0)).x() - q->width() - gap, targetCenter.y() - q->height() / 2);
    break;
  case NXTeachingTip::Left :
    // Tip appears right of target, tail points left
    result = QPoint(_target->mapToGlobal(QPoint(_target->width(), 0)).x() + gap, targetCenter.y() - q->height() / 2);
    break;
  default :
    result = QPoint(targetCenter.x() - q->width() / 2, _target->mapToGlobal(QPoint(0, 0)).y() - q->height() - gap);
    break;
  }

  // Screen boundary clamping
  QScreen *screen = QApplication::screenAt(targetCenter);
  if (screen)
  {
    QRect screenGeo = screen->availableGeometry();
    result.setX(qBound(screenGeo.left() + 4, result.x(), screenGeo.right() - q->width() - 4));
    result.setY(qBound(screenGeo.top() + 4, result.y(), screenGeo.bottom() - q->height() - 4));
  }
  return result;
}

NXTeachingTip::TailPosition NXTeachingTipPrivate::_calculateAutoTailPosition() const
{
  Q_Q(const NXTeachingTip);
  if (!_target) { return NXTeachingTip::Bottom; }
  QPoint targetGlobal = _target->mapToGlobal(QPoint(0, 0));
  QScreen *screen     = QApplication::screenAt(targetGlobal);
  if (!screen) { return NXTeachingTip::Bottom; }
  QRect screenGeo = screen->availableGeometry();
  int spaceAbove  = targetGlobal.y() - screenGeo.top();
  int spaceBelow  = screenGeo.bottom() - targetGlobal.y() - _target->height();

  // Prefer showing above (tail at bottom)
  if (spaceAbove >= q->sizeHint().height() + _tailSize + 4) { return NXTeachingTip::Bottom; }
  if (spaceBelow >= q->sizeHint().height() + _tailSize + 4) { return NXTeachingTip::Top; }
  return NXTeachingTip::Bottom;
}

void NXTeachingTipPrivate::_doShowAnimation()
{
  Q_Q(NXTeachingTip);
  QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(q);
  q->setGraphicsEffect(effect);
  _pOpacity = 0.0;
  effect->setOpacity(0);

  QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity", q);
  animation->setDuration(200);
  animation->setStartValue(0.0);
  animation->setEndValue(1.0);
  animation->setEasingCurve(QEasingCurve::OutCubic);
  QObject::connect(animation, &QPropertyAnimation::finished, q, [=]() { q->setGraphicsEffect(nullptr); });
  animation->start(QAbstractAnimation::DeleteWhenStopped);
}
