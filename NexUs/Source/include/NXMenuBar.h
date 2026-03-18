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

  QAction *addMenu(QMenu *menu) noexcept;
  NXMenu *addMenu(const QString& title) noexcept;
  NXMenu *addMenu(const QIcon& icon, const QString& title) noexcept;
  NXMenu *addMenu(NXIconType::IconName, const QString& title) noexcept;

  QAction *addNXIconAction(NXIconType::IconName icon, const QString& text) noexcept;
  QAction *addNXIconAction(NXIconType::IconName icon, const QString& text, const QKeySequence& shortcut) noexcept;
};

#endif // NXMENUBAR_H
