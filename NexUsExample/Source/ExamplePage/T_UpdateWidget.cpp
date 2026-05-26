#include "T_UpdateWidget.h"

#include <QVBoxLayout>

#include "NXText.h"

T_UpdateWidget::T_UpdateWidget(QWidget *parent)
    : QWidget { parent }
{
  setMinimumSize(200, 260);
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
  mainLayout->setContentsMargins(5, 10, 5, 5);
  mainLayout->setSpacing(4);
  NXText *updateTitle = new NXText(QStringLiteral("2026-4-30更新"), 15, this);
  NXText *update1 =
      new NXText(QStringLiteral("1、NXAppBar无边框优化, 修正了Close窗口后再次打开失去原生动画的问题"), 13, this);
  NXText *update2 = new NXText(QStringLiteral("2、为NXWindow添加了Dock的便利API"), 13, this);
  NXText *update3 = new NXText(QStringLiteral("3、组件整体绘制风格和效果优化"), 13, this);
  NXText *update4 = new NXText(QStringLiteral("4、为部分组件添加了Disable等状态效果"), 13, this);
  NXText *update5 = new NXText(QStringLiteral("5、QQ交流群: 850243692"), 13, this);
  update1->setIsWrapAnywhere(true);
  update2->setIsWrapAnywhere(true);
  update3->setIsWrapAnywhere(true);
  update4->setIsWrapAnywhere(true);

  mainLayout->addWidget(updateTitle);
  mainLayout->addWidget(update1);
  mainLayout->addWidget(update2);
  mainLayout->addWidget(update3);
  mainLayout->addWidget(update4);
  mainLayout->addWidget(update5);
  mainLayout->addStretch();
}

T_UpdateWidget::~T_UpdateWidget()
{
}
