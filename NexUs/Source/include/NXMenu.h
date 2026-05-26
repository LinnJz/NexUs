#ifndef NXMENU_H
#define NXMENU_H

#include <QMenu>
#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"
class NXMenuPrivate;

class NX_EXPORT NXMenu : public QMenu
{
  Q_OBJECT
  Q_Q_CREATE(NXMenu)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, MenuItemHeight)

public:
  explicit NXMenu(QWidget *parent = nullptr);
  explicit NXMenu(const QString &title, QWidget *parent = nullptr);
  ~NXMenu();

  QAction *addMenu(QMenu *menu);
  NXMenu *addMenu(const QString &title);
  NXMenu *addMenu(const QIcon &icon, const QString &title);
  NXMenu *addMenu(NXIconType::IconName icon, const QString &title);

  QAction *addNXIconAction(NXIconType::IconName icon, const QString &text);
  QAction *addNXIconAction(NXIconType::IconName icon, const QString &text, const QKeySequence &shortcut);

  bool isHasParentMenu() const;
  bool isHasChildMenu() const;
  bool isHasIcon() const;
  Q_SIGNAL void menuShow();

protected:
  void showEvent(QShowEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXMENU_H
