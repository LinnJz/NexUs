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
  friend class NXPopularCard;
  Q_OBJECT
  Q_PROPERTY_CREATE(qreal, HoverYOffset);
  Q_PROPERTY_CREATE(qreal, HoverOpacity);

public:
  explicit NXPopularCardFloater(NXPopularCard *card, NXPopularCardPrivate *cardPrivate, QWidget *parent = nullptr);
  ~NXPopularCardFloater();
  void showFloater();
  void hideFloater();

protected:
  virtual bool event(QEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;

private:
  int _floatGeometryOffset { 25 };
  NXThemeType::ThemeMode _themeMode;
  bool _isHideAnimationFinished { true };

  NXPushButton *_overButton { nullptr };
  QGraphicsOpacityEffect *_opacityEffect { nullptr };
  NXPopularCard *_card { nullptr };
  NXPopularCardPrivate *_cardPrivate { nullptr };

  QRect _calculateTargetGeometry(QRect cardGeometry);
};

#endif // NXPOPULARCARDFLOATER_H
