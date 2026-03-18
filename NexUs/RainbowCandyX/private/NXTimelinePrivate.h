#ifndef NXTIMELINEPRIVATE_H
#define NXTIMELINEPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXTimeline.h"

class NXTimelinePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXTimeline)
public:
    explicit NXTimelinePrivate(QObject* parent = nullptr);
    ~NXTimelinePrivate() override;

private:
    NXThemeType::ThemeMode _themeMode;
    QList<NXTimeline::TimelineItem> _items;
};

#endif // NXTIMELINEPRIVATE_H
