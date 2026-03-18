#ifndef NXWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERFNMONITOR_H_
#define NXWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERFNMONITOR_H_

#include <functional>
#include <qglobal.h>

#ifdef Q_OS_MACOS
void elaStartFnKeyMonitor(std::function<void()> onFnKeyPressed);
void elaStopFnKeyMonitor();
#else
inline void elaStartFnKeyMonitor(std::function<void()>) {}
inline void elaStopFnKeyMonitor() {}
#endif

#endif // NXWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERFNMONITOR_H_
