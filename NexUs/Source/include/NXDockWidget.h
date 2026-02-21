#ifndef NXDOCKWIDGET_H
#define NXDOCKWIDGET_H

#include <QDockWidget>

#include "NXProperty.h"
class NXDockWidgetPrivate;

class NX_EXPORT NXDockWidget : public QDockWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXDockWidget)
public:
  explicit NXDockWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
  explicit NXDockWidget(const QString& title, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
  ~NXDockWidget() override;
  Q_SIGNAL void dockClosed();
  Q_SIGNAL void dockResized(const QSize& size);

protected:
  virtual void resizeEvent(QResizeEvent *event) override;
  virtual void closeEvent(QCloseEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
#ifdef Q_OS_WIN
  virtual bool event(QEvent *event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  virtual bool nativeEvent(const QByteArray& eventType, void *message, qintptr *result) override;
#else
  virtual bool nativeEvent(const QByteArray& eventType, void *message, long *result) override;
#endif
#endif
};

#endif // NXDOCKWIDGET_H
