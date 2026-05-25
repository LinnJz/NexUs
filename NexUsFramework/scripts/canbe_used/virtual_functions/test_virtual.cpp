#include "test_virtual.h"

TestWidget::TestWidget(QWidget *parent)
    : QWidget(parent)
    , m_value(0)
{
}

TestWidget::~TestWidget()
{
}

void
TestWidget::wheelEvent(QWheelEvent *event)
{
  event->accept();
}

void
TestWidget::paintEvent(QPaintEvent *event)
{
}

int
TestWidget::getValue() const
{
  return m_value;
}

void
TestWidget::setValue(int val)
{
  m_value = val;
}

const QString &
TestWidget::name() const
{
  return m_name;
}

QList<int>
TestWidget::items(QWidget *parent)
{
  return {};
}

void
TestWidget::mousePressEvent(QMouseEvent *event)
{
}

void
TestWidget::mouseReleaseEvent(QMouseEvent *event)
{
}

void
TestWidget::keyPressEvent(QKeyEvent *event)
{
}
