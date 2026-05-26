#include "T_Card.h"

#include <QVBoxLayout>

#include "NXLCDNumber.h"
#include "NXPromotionCard.h"
#include "NXPromotionView.h"

T_Card::T_Card(QWidget *parent)
    : T_BasePage(parent)
{
  // 预览窗口标题
  setWindowTitle(QStringLiteral("NXCard"));

  // 顶部元素
  createCustomWidget(QStringLiteral("一些常用的卡片组件被放置于此，可在此界面体验其效果并按需添加进项目中"));

  _lcdNumber = new NXLCDNumber(this);
  _lcdNumber->setIsUseAutoClock(true);
  _lcdNumber->setIsTransparent(false);
  //    _lcdNumber->setAutoClockFormat("hh:mm:ss");
  _lcdNumber->setFixedHeight(100);

  _promotionCard = new NXPromotionCard(this);
  _promotionCard->setFixedSize(600, 300);
  _promotionCard->setCardPixmap(QPixmap(QStringLiteral(":/Resource/Image/Card/miku.png")));
  _promotionCard->setCardTitle(QStringLiteral("MiKu"));
  _promotionCard->setPromotionTitle(QStringLiteral("SONG~"));
  _promotionCard->setTitle(QStringLiteral("STYX HELIX"));
  _promotionCard->setSubTitle(QStringLiteral("Never close your eyes, Searching for a true fate"));

  _promotionView = new NXPromotionView(this);

  NXPromotionCard *exampleCard1 = new NXPromotionCard(this);
  exampleCard1->setCardPixmap(QPixmap(QStringLiteral(":/Resource/Image/Card/miku.png")));
  exampleCard1->setCardTitle(QStringLiteral("MiKu"));
  exampleCard1->setPromotionTitle(QStringLiteral("SONG~"));
  exampleCard1->setTitle(QStringLiteral("STYX HELIX"));
  exampleCard1->setSubTitle(QStringLiteral("Never close your eyes, Searching for a true fate"));

  NXPromotionCard *exampleCard2 = new NXPromotionCard(this);
  exampleCard2->setCardPixmap(QPixmap(QStringLiteral(":/Resource/Image/Card/beach.png")));
  exampleCard2->setCardTitle(QStringLiteral("Beach"));
  exampleCard2->setPromotionTitle(QStringLiteral("SONG~"));
  exampleCard2->setTitle(QStringLiteral("STYX HELIX"));
  exampleCard2->setSubTitle(QStringLiteral("Never close your eyes, Searching for a true fate"));

  NXPromotionCard *exampleCard3 = new NXPromotionCard(this);
  exampleCard3->setCardPixmap(QPixmap(QStringLiteral(":/Resource/Image/Card/dream.png")));
  exampleCard3->setCardTitle(QStringLiteral("Dream"));
  exampleCard3->setPromotionTitle(QStringLiteral("SONG~"));
  exampleCard3->setTitle(QStringLiteral("STYX HELIX"));
  exampleCard3->setSubTitle(QStringLiteral("Never close your eyes, Searching for a true fate"));

  NXPromotionCard *exampleCard4 = new NXPromotionCard(this);
  exampleCard4->setCardPixmap(QPixmap(QStringLiteral(":/Resource/Image/Card/classroom.png")));
  exampleCard4->setCardTitle(QStringLiteral("Classroom"));
  exampleCard4->setPromotionTitle(QStringLiteral("SONG~"));
  exampleCard4->setTitle(QStringLiteral("STYX HELIX"));
  exampleCard4->setSubTitle(QStringLiteral("Never close your eyes, Searching for a true fate"));

  _promotionView->appendPromotionCard(exampleCard1);
  _promotionView->appendPromotionCard(exampleCard2);
  _promotionView->appendPromotionCard(exampleCard3);
  _promotionView->appendPromotionCard(exampleCard4);
  _promotionView->setIsAutoScroll(true);

  QWidget *centralWidget = new QWidget(this);
  centralWidget->setWindowTitle(QStringLiteral("NXCard"));
  QVBoxLayout *centerLayout = new QVBoxLayout(centralWidget);
  centerLayout->setContentsMargins(0, 0, 0, 0);
  centerLayout->addWidget(_lcdNumber);
  centerLayout->addSpacing(20);
  centerLayout->addWidget(_promotionCard);
  centerLayout->addSpacing(20);
  centerLayout->addWidget(_promotionView);
  centerLayout->addSpacing(100);
  centerLayout->addStretch();
  addCentralWidget(centralWidget, true, true, 0);
}

T_Card::~T_Card()
{
}
