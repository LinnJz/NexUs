#ifndef NXPOPULARCARDFLOATER_H
#define NXPOPULARCARDFLOATER_H

#include <QWidget>

#include "NXDef.h"
class NXPushButton;
class NXPopularCard;
class NXPopularCardPrivate;
class QGraphicsOpacityEffect;

class NXPopularCardFloater : public QWidget
{
  Q_OBJECT
  Q_PROPERTY_CREATE(qreal, HoverYOffset)
  Q_PROPERTY_CREATE(qreal, HoverOpacity)

public:
  explicit NXPopularCardFloater(NXPopularCard *card, NXPopularCardPrivate *cardPrivate, QWidget *parent = nullptr);
  ~NXPopularCardFloater();
  void showFloater();
  void hideFloater();

protected:
  bool event(QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  friend class NXPopularCard;
  bool _isHideAnimationFinished { true };
  NXThemeType::ThemeMode _themeMode;
  int _floatGeometryOffset { 25 };
  NXPushButton *_overButton { nullptr };
  QGraphicsOpacityEffect *_opacityEffect { nullptr };
  NXPopularCard *_card { nullptr };
  NXPopularCardPrivate *_cardPrivate { nullptr };
  QRect _calculateTargetGeometry(QRect cardGeometry);
};

#endif // NXPOPULARCARDFLOATER_H
