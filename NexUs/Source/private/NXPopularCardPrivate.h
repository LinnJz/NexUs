#ifndef NXPOPULARCARDPRIVATE_H
#define NXPOPULARCARDPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "NXDef.h"
class NXPopularCard;
class NXPopularCardFloater;

class NXPopularCardPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXPopularCard)
  Q_PROPERTY_CREATE_D(QPixmap, CardPixmap)
  Q_PROPERTY_CREATE_D(QPixmap, CardFloatPixmap)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, SubTitle)
  Q_PROPERTY_CREATE_D(QString, InteractiveTips)
  Q_PROPERTY_CREATE_D(QString, DetailedText)
  Q_PROPERTY_CREATE_D(QString, CardButtonText)
  Q_PROPERTY_CREATE_D(QWidget *, CardFloatArea)
  Q_PROPERTY_CREATE(qreal, HoverYOffset)
  Q_PROPERTY_CREATE(qreal, HoverOpacity)
  Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
  explicit NXPopularCardPrivate(QObject *parent = nullptr);
  ~NXPopularCardPrivate();

private:
  friend class NXPopularCardFloater;
  bool _isFloating { false };
  NXThemeType::ThemeMode _themeMode;
  int _shadowBorderWidth { 6 };
  int _textHSpacing { 20 };
  int _textVSpacing { 5 };
  QRect _buttonTargetRect;
  QRectF _interactiveTipsBaseRect;
  QTimer *_floatTimer { nullptr };
  NXPopularCardFloater *_floater { nullptr };

  void _showFloater();
};

#endif // NXPOPULARCARDPRIVATE_H
