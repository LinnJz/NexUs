#ifndef NXWIZARDPRIVATE_H
#define NXWIZARDPRIVATE_H

#include <QObject>
#include <QStringList>

#include "NXDef.h"

class NXWizard;
class QStackedWidget;

class NXWizardPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXWizard)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, CurrentStep)

public:
  NXThemeType::ThemeMode _themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
  int _shadowBorderWidth { 0 };
#else
  int _shadowBorderWidth { 10 };
#endif
  QRect _cancelButtonRect;
  QRect _prevButtonRect;
  QRect _nextButtonRect;
  QStringList _stepTitles;
  QList<QWidget *> _stepPages;
  QStackedWidget *_stackedWidget { nullptr };
  explicit NXWizardPrivate(QObject *parent = nullptr);
  ~NXWizardPrivate();
};

#endif // NXWIZARDPRIVATE_H
