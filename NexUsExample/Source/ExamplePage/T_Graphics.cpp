#include "T_Graphics.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "NXGraphicsItem.h"
#include "NXGraphicsScene.h"
#include "NXGraphicsView.h"

T_Graphics::T_Graphics(QWidget *parent)
    : T_BasePage(parent)
{
  // 预览窗口标题
  setWindowTitle("NXGraphics");
  // 顶部元素
  createCustomWidget("图形视图框架被放置于此，可在此界面体验其效果，按住Ctrl进行缩放，按住Shitf进行连接");

  QWidget *centralWidget = new QWidget(this);
  centralWidget->setWindowTitle("NXGraphics");

  // GraphicsView
  NXGraphicsScene *scene = new NXGraphicsScene(this);
  scene->setSceneRect(0, 0, 1500, 1500);
  NXGraphicsItem *item1 = new NXGraphicsItem();
  item1->setWidth(100);
  item1->setHeight(100);
  item1->setMaxLinkPortCount(100);
  item1->setMaxLinkPortCount(1);
  NXGraphicsItem *item2 = new NXGraphicsItem();
  item2->setWidth(100);
  item2->setHeight(100);
  scene->addItem(item1);
  scene->addItem(item2);
  NXGraphicsView *view = new NXGraphicsView(scene);
  view->setScene(scene);
  view->setFixedHeight(600);
  QHBoxLayout *viewLayout = new QHBoxLayout();
  viewLayout->setContentsMargins(0, 0, 12, 0);
  viewLayout->addWidget(view);

  QVBoxLayout *centerVLayout = new QVBoxLayout(centralWidget);
  centerVLayout->setContentsMargins(0, 0, 0, 0);
  centerVLayout->addLayout(viewLayout);
  centerVLayout->addStretch();
  addCentralWidget(centralWidget, true, false, 0);
}

T_Graphics::~T_Graphics() { }
