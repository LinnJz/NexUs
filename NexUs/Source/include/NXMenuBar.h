#ifndef NXMENUBAR_H
#define NXMENUBAR_H

#include <QMenuBar>

#include "NXDef.h"
#include "NXProperty.h"

class NXMenu;

class NX_EXPORT NXMenuBar : public QMenuBar
{
  Q_OBJECT
public:
  explicit NXMenuBar(QWidget *parent = nullptr);
  ~NXMenuBar();

  QAction *addMenu(QMenu *menu);
  NXMenu *addMenu(const QString& title);
  NXMenu *addMenu(const QIcon& icon, const QString& title);
  NXMenu *addMenu(NXIconType::IconName, const QString& title);

  QAction *addNXIconAction(NXIconType::IconName icon, const QString& text);
  QAction *addNXIconAction(NXIconType::IconName icon, const QString& text, const QKeySequence& shortcut);
};

#endif // NXMENUBAR_H
