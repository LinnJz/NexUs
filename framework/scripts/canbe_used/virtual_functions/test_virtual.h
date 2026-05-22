#ifndef TEST_WIDGET_H
#define TEST_WIDGET_H

#include <QWheelEvent>
#include <QWidget>

class TestWidget : public QWidget
{
public:
  TestWidget(QWidget *parent = nullptr);
  virtual ~TestWidget();

  void wheelEvent(QWheelEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

  virtual int getValue() const = 0;
  virtual void setValue(int val);

  virtual const QString &name() const;
  virtual QList<int> items(QWidget *parent = nullptr) final;

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  int m_value;
  QString m_name;
};

#endif
