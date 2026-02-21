#ifndef NXFRAMEWORK_NXWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_RECVSCREEN_H_
#define NXFRAMEWORK_NXWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_RECVSCREEN_H_

#include <QWidget>

class T_RecvScreen : public QWidget
{
  Q_OBJECT
public:
  explicit T_RecvScreen(QWidget *parent = nullptr);
  ~T_RecvScreen();

  Q_SLOT void onSendHandleResult(QPixmap pix);

protected:
  virtual void paintEvent(QPaintEvent *event) override;

private:
  QPixmap _pix;
};

#endif // NXFRAMEWORK_NXWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_RECVSCREEN_H_
