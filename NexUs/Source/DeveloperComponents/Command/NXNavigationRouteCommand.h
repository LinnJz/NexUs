#ifndef NXFRAMEWORK_NXNAVIGATIONROUTECOMMAND_H
#define NXFRAMEWORK_NXNAVIGATIONROUTECOMMAND_H

#include "NXActionCommander.h"
class NXNavigationBar;

class NXNavigationRouteCommand : public NXActionCommand
{
  Q_OBJECT
  Q_PRIVATE_CREATE(NXNavigationBar *, NavigationBar);
  Q_PRIVATE_CREATE(const QString &, UndoPageKey);
  Q_PRIVATE_CREATE(const QString &, RedoPageKey);

public:
  explicit NXNavigationRouteCommand(QObject *parent = nullptr);
  ~NXNavigationRouteCommand() override;

  void undo() override;
  void redo() override;
};

#endif //NXFRAMEWORK_NXNAVIGATIONROUTECOMMAND_H
