#ifndef NXSTEPSPRIVATE_H
#define NXSTEPSPRIVATE_H

#include <QObject>
#include <QStringList>

#include "NXDef.h"

class NXSteps;
class NXStepsPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXSteps)
    Q_PROPERTY_CREATE_D(int, CurrentStep)
    Q_PROPERTY_CREATE_D(int, StepCount)

public:
    explicit NXStepsPrivate(QObject* parent = nullptr);
    ~NXStepsPrivate() override;

    QStringList _stepTitles;
    NXThemeType::ThemeMode _themeMode;
};

#endif // NXSTEPSPRIVATE_H
