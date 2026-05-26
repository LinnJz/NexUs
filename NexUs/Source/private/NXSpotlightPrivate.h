#ifndef NXSPOTLIGHTPRIVATE_H
#define NXSPOTLIGHTPRIVATE_H

#include <QObject>
#include <QRectF>

#include "NXDef.h"
#include "NXSpotlight.h"
class QPropertyAnimation;
class NXText;
class NXPushButton;
class NXSpotlight;

class NXSpotlightPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSpotlight)
  Q_PROPERTY_CREATE(QRectF, SpotlightRect)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, Content)
  Q_PROPERTY_CREATE(qreal, Opacity)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, Padding)
  Q_PROPERTY_CREATE_D(int, OverlayAlpha)
  Q_PROPERTY_CREATE_D(bool, IsCircle)

public:
  explicit NXSpotlightPrivate(QObject *parent = nullptr);
  ~NXSpotlightPrivate();

  void _showStep(int index, bool animate = true);
  void _updateOverlay();
  void _updateTipPosition();
  QRectF _getTargetRect(QWidget *target) const;

private:
  friend class NXSpotlight;
  bool _isActive { false };
  NXThemeType::ThemeMode _themeMode;
  int _currentStep { -1 };
  int _maskAlpha { 0 };
  QList<NXSpotlight::SpotlightStep> _steps;
  QWidget *_currentTarget { nullptr };
  QWidget *_tipWidget { nullptr };
  NXText *_tipTitle { nullptr };
  NXText *_tipContent { nullptr };
  NXPushButton *_prevButton { nullptr };
  NXPushButton *_nextButton { nullptr };
  NXText *_stepIndicator { nullptr };
};

#endif // NXSPOTLIGHTPRIVATE_H
