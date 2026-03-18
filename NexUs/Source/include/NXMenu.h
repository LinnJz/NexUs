#ifndef NXMENU_H
#define NXMENU_H

#include <QMenu>
#include <QWidget>

#include "NXDef.h"
class NXMenuPrivate;

class NX_EXPORT NXMenu : public QMenu
{
  Q_OBJECT
  Q_Q_CREATE(NXMenu)

public:
  explicit NXMenu(QWidget *parent = nullptr);
  explicit NXMenu(const QString& title, QWidget *parent = nullptr);
  ~NXMenu();

  void setBorderRadius(int borderRadius) noexcept;
  int getBorderRadius() const noexcept;

  void setMenuItemHeight(int menuItemHeight) noexcept;
  int getMenuItemHeight() const noexcept;

  void setAlignParentMenuHeight(bool alignParentMenuHeight) noexcept;
  bool getAlignParentMenuHeight() const noexcept;

  QAction *addMenu(QMenu *menu) noexcept;
  NXMenu *addMenu(const QString& title) noexcept;
  NXMenu *addMenu(const QIcon& icon, const QString& title) noexcept;
  NXMenu *addMenu(NXIconType::IconName icon, const QString& title) noexcept;

  QAction *addNXIconAction(NXIconType::IconName icon, const QString& text) noexcept;
  QAction *addNXIconAction(NXIconType::IconName icon, const QString& text, const QKeySequence& shortcut) noexcept;

  bool isHasParentMenu() const noexcept;
  bool isHasChildMenu() const noexcept;
  bool isHasIcon() const noexcept;

Q_SIGNALS:
  void menuShow();

protected:
  void showEvent(QShowEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXMENU_H
