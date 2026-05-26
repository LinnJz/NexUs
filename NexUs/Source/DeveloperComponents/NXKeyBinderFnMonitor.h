#ifndef NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERFNMONITOR_H_
#define NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERFNMONITOR_H_

#include <functional>
#include <qglobal.h>

#ifdef Q_OS_MACOS
void startFnKeyMonitor(std::function<void()> onFnKeyPressed);
void stopFnKeyMonitor();
#else
inline void
startFnKeyMonitor(std::function<void()>)
{
}

inline void
stopFnKeyMonitor()
{
}
#endif

#endif // NEXUS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERFNMONITOR_H_
