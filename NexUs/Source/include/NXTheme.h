#ifndef NXTHEME_H
#define NXTHEME_H

#include <QObject>
#include <QColor>
#include "LinnSingleton.h"
#include "NXDef.h"

#define nxTheme                             NXTheme::getInstance()
#define NXThemeColor(themeMode, themeColor) nxTheme->getThemeColor(themeMode, NXThemeType::themeColor)
class QPainter;
class NXThemePrivate;

#pragma push_macro("Q_DISABLE_COPY")
#undef Q_DISABLE_COPY
#define Q_DISABLE_COPY(Class)

class NX_EXPORT NXTheme : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXTheme)
  LINN_SINGLETON_CREATE(LINN_SINGLETON_UNIQUE(NXTheme))

private:
  explicit NXTheme(QObject *parent = nullptr);
  ~NXTheme();

public:
  void setThemeMode(NXThemeType::ThemeMode themeMode);
  NXThemeType::ThemeMode getThemeMode() const;

  void drawEffectShadow(QPainter *painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);
  void drawEffectShadow(QPainter *painter,
                        QRect widgetRect,
                        int shadowBorderWidth,
                        int borderRadius,
                        int maxAlpha,
                        int extendPixels = 1,
                        const QColor& lightColor = QColor(0xC0, 0xC0, 0xC0),
                        const QColor& darkColor  = QColor(0x80, 0x80, 0x80));

  void setThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor, QColor newColor);
  const QColor& getThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor);
Q_SIGNALS:
  Q_SIGNAL void themeModeChanged(NXThemeType::ThemeMode themeMode);
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXTHEME_H
