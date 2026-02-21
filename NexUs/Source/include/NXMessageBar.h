#ifndef NXMESSAGEBAR_H
#define NXMESSAGEBAR_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXMessageBarPrivate;

class NX_EXPORT NXMessageBar : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXMessageBar)

public:
  static void success(
      NXMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget *parent = nullptr);
  static void warning(
      NXMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget *parent = nullptr);
  static void information(
      NXMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget *parent = nullptr);
  static void error(
      NXMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget *parent = nullptr);

protected:
  virtual void paintEvent(QPaintEvent *event) override;
  virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
  friend class NXMessageBarManager;
  explicit NXMessageBar(NXMessageBarType::PositionPolicy policy,
                        NXMessageBarType::MessageMode messageMode,
                        QString& title,
                        QString& text,
                        int displayMsec,
                        QWidget *parent = nullptr);
  ~NXMessageBar();
};

#endif // NXMESSAGEBAR_H
