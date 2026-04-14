#include "NXScrollPagePrivate.h"

#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QTimer>

#include "NXBreadcrumbBar.h"
#include "NXScrollPage.h"

NXScrollPagePrivate::NXScrollPagePrivate(QObject *parent)
    : QObject { parent }
{
}

NXScrollPagePrivate::~NXScrollPagePrivate()
{
}

void
NXScrollPagePrivate::_switchCentralStackIndex(int targetIndex, int lastIndex) noexcept
{
  QWidget *currentWidget = _centralStackedWidget->widget(lastIndex);
  QWidget *targetWidget  = _centralStackedWidget->widget(targetIndex);
  targetWidget->resize(currentWidget->size());
  targetWidget->setVisible(true);

  QPropertyAnimation *currentWidgetAnimation = new QPropertyAnimation(currentWidget, "pos");
  currentWidgetAnimation->setEasingCurve(QEasingCurve::InExpo);
  currentWidgetAnimation->setDuration(300);

  QPropertyAnimation *targetWidgetAnimation = new QPropertyAnimation(targetWidget, "pos");
  connect(targetWidgetAnimation, &QPropertyAnimation::finished, this, [=]()
  {
    _centralStackedWidget->setCurrentIndex(targetIndex);
  });
  targetWidgetAnimation->setEasingCurve(QEasingCurve::InExpo);
  targetWidgetAnimation->setDuration(300);
  if (targetIndex > lastIndex)
  {
    // 左滑
    currentWidgetAnimation->setStartValue(currentWidget->pos());
    currentWidgetAnimation->setEndValue(QPoint(-_centralStackedWidget->width(), 0));
    targetWidgetAnimation->setStartValue(QPoint(_centralStackedWidget->width(), 0));
    targetWidgetAnimation->setEndValue(QPoint(0, 0));
  }
  else
  {
    // 右滑
    currentWidgetAnimation->setStartValue(currentWidget->pos());
    currentWidgetAnimation->setEndValue(QPoint(_centralStackedWidget->width(), 0));
    targetWidgetAnimation->setStartValue(QPoint(-_centralStackedWidget->width(), 0));
    targetWidgetAnimation->setEndValue(QPoint(0, 0));
  }
  currentWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  targetWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
