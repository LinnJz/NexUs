#ifndef NXTOOLBAR_H
#define NXTOOLBAR_H

#include <QToolBar>

#include "NXDef.h"
class NXToolBarPrivate;

class NX_EXPORT NXToolBar : public QToolBar
{
  Q_OBJECT
  Q_Q_CREATE(NXToolBar)

public:
  explicit NXToolBar(QWidget *parent = nullptr);
  explicit NXToolBar(const QString& title, QWidget *parent = nullptr);
  ~NXToolBar();

  void setToolBarSpacing(int spacing) noexcept;
  int getToolBarSpacing() const noexcept;

  QAction *addNXIconAction(NXIconType::IconName icon, const QString& text) noexcept;
  QAction *addNXIconAction(NXIconType::IconName icon, const QString& text, const QKeySequence& shortcut) noexcept;

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXTOOLBAR_H
