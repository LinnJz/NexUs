#ifndef NXLAZYSTACKEDWIDGET_H
#define NXLAZYSTACKEDWIDGET_H

#include <functional>

#include <QFrame>

#include "NXProperty.h"

class NXLazyStackedWidgetPrivate;

class NX_EXPORT NXLazyStackedWidget : public QFrame
{
  Q_OBJECT
  Q_Q_CREATE(NXLazyStackedWidget)
  Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
  Q_PROPERTY(int count READ count)

public:
  explicit NXLazyStackedWidget(QWidget *parent = nullptr);
  ~NXLazyStackedWidget() override;

  int addWidget(QWidget *widget);
  int insertWidget(int index, QWidget *widget);
  void removeWidget(QWidget *widget);

  int addLazyWidget(std::function<QWidget *()> &&lazyFactory);
  int insertLazyWidget(int index, std::function<QWidget *()> &&lazyFactory);

  QWidget *ensureWidget(int index) noexcept;
  bool isWidgetCreated(int index) const noexcept;

  QWidget *currentWidget() const noexcept;
  int currentIndex() const noexcept;

  int indexOf(const QWidget *widget) const noexcept;
  QWidget *widget(int index) const noexcept;
  int count() const noexcept;

public Q_SLOTS:
  void setCurrentIndex(int index);
  void setCurrentWidget(QWidget *widget);

Q_SIGNALS:
  void currentChanged(int index);
  void widgetRemoved(int index);
  void widgetCreated(int index, QWidget *widget);

protected:
  bool event(QEvent *event) override;
};

#endif // NXLAZYSTACKEDWIDGET_H
