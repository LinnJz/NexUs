#ifndef NXPOPULARCARDPRIVATE_H
#define NXPOPULARCARDPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "NXDef.h"
class NXPopularCard;
class NXPopularCardFloater;

class NXPopularCardPrivate : public QObject
{
  friend class NXPopularCardFloater;
  Q_OBJECT
  Q_D_CREATE(NXPopularCard)
  Q_PROPERTY_CREATE_D(QWidget *, CardFloatArea)
  Q_PROPERTY_CREATE_D(QPixmap, CardPixmap)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, SubTitle)
  Q_PROPERTY_CREATE_D(QString, InteractiveTips)
  Q_PROPERTY_CREATE_D(QString, DetailedText)
  Q_PROPERTY_CREATE_D(QString, CardButtonText)
  Q_PROPERTY_CREATE_D(QPixmap, CardFloatPixmap)
  Q_PROPERTY_CREATE(qreal, HoverYOffset);
  Q_PROPERTY_CREATE(qreal, HoverOpacity);
  Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
  explicit NXPopularCardPrivate(QObject *parent = nullptr);
  ~NXPopularCardPrivate();

private:
  int _shadowBorderWidth { 6 };
  int _textHSpacing { 20 };
  int _textVSpacing { 5 };
  NXThemeType::ThemeMode _themeMode;
  bool _isFloating { false };

  QTimer *_floatTimer { nullptr };
  NXPopularCardFloater *_floater { nullptr };
  QRectF _interactiveTipsBaseRect;
  QRect _buttonTargetRect;

  void _showFloater();
};

#endif // NXPOPULARCARDPRIVATE_H
