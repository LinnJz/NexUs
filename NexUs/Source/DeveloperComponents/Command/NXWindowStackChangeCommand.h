#ifndef NXFRAMEWORK_NXWINDOWSTACKCHANGECOMMAND_H
#define NXFRAMEWORK_NXWINDOWSTACKCHANGECOMMAND_H

#include "NXActionCommander.h"
#include "private/NXWindowPrivate.h"

class NXWindowStackChangeCommand : public NXActionCommand
{
  Q_OBJECT
  Q_PRIVATE_CREATE(NXWindowPrivate *, WindowPrivate)
  Q_PRIVATE_CREATE(int, UndoStackIndex)
  Q_PRIVATE_CREATE(int, RedoStackIndex)

public:
  explicit NXWindowStackChangeCommand(QObject *parent = nullptr);
  ~NXWindowStackChangeCommand();

  void undo() override;
  void redo() override;
};

#endif //NXFRAMEWORK_NXWINDOWSTACKCHANGECOMMAND_H
