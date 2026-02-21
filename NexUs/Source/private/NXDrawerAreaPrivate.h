#ifndef NXFRAMEWORK_NEXUS_PRIVATE_NXDRAWERAREAPRIVATE_H_
#define NXFRAMEWORK_NEXUS_PRIVATE_NXDRAWERAREAPRIVATE_H_

#include <QObject>
#include "DeveloperComponents/NXDrawerContainer.h"
#include "DeveloperComponents/NXDrawerHeader.h"
#include "NXDef.h"
class NXDrawerArea;

class NXDrawerAreaPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXDrawerArea)
public:
  explicit NXDrawerAreaPrivate(QObject *parent = nullptr);
  ~NXDrawerAreaPrivate() override;

  Q_SLOT void onDrawerHeaderClicked(bool isExpand);

private:
  NXThemeType::ThemeMode _themeMode;
  NXDrawerHeader *_drawerHeader { nullptr };
  NXDrawerContainer *_drawerContainer { nullptr };
};

#endif // NXFRAMEWORK_NEXUS_PRIVATE_NXDRAWERAREAPRIVATE_H_
