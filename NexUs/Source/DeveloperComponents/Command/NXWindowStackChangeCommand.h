#ifndef NXFRAMEWORK_NXWINDOWSTACKCHANGECOMMAND_H
#define NXFRAMEWORK_NXWINDOWSTACKCHANGECOMMAND_H

#include "NXActionCommander.h"
#include "NXWindowPrivate.h"

class NXWindowStackChangeCommand : public NXActionCommand
{
  Q_OBJECT
  Q_PRIVATE_CREATE(int, UndoStackIndex);
  Q_PRIVATE_CREATE(int, RedoStackIndex);
  Q_PRIVATE_CREATE(NXWindowPrivate *, WindowPrivate)

public:
  explicit NXWindowStackChangeCommand(QObject *parent = nullptr);
  ~NXWindowStackChangeCommand() override;

  void undo() override;
  void redo() override;
};

#endif //NXFRAMEWORK_NXWINDOWSTACKCHANGECOMMAND_H
