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
    explicit NXWizardPrivate(QObject* parent = nullptr);
    ~NXWizardPrivate() override;

    QStringList _stepTitles;
    QList<QWidget*> _stepPages;
    QStackedWidget* _stackedWidget{nullptr};
    NXThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{10};

    QRect _cancelButtonRect;
    QRect _prevButtonRect;
    QRect _nextButtonRect;
};

#endif // NXWIZARDPRIVATE_H
