#ifndef NXTHEME_H
#define NXTHEME_H

#include <QColor>
#include <QObject>
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
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXTheme))

private:
  explicit NXTheme(QObject *parent = nullptr);
  ~NXTheme();

public:
  void setThemeMode(NXThemeType::ThemeMode themeMode) noexcept;
  NXThemeType::ThemeMode getThemeMode() const noexcept;

  void
  setThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor, const QColor& newColor) noexcept;
  const QColor& getThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor) const noexcept;

  void drawEffectShadow(QPainter *painter,
                        QRect widgetRect,
                        int shadowBorderWidth,
                        int borderRadius,
                        int maxAlpha             = 32,
                        int extendPixels         = 1,
                        const QColor& lightColor = QColor(0xC0, 0xC0, 0xC0),
                        const QColor& darkColor  = QColor(0x80, 0x80, 0x80)) noexcept;

Q_SIGNALS:
  void themeModeChanged(NXThemeType::ThemeMode themeMode);
};

#pragma pop_macro("Q_DISABLE_COPY")
#endif // NXTHEME_H
