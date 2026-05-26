#ifndef NXRIBBONGROUP_H
#define NXRIBBONGROUP_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class QHBoxLayout;
class NXToolButton;
class NXRibbonGroupPrivate;

class NX_EXPORT NXRibbonGroup : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXRibbonGroup)

public:
  enum ButtonSize
  {
    Large = 0x00'01,
    Small = 0x00'02,
  };

  explicit NXRibbonGroup(const QString &title, QWidget *parent = nullptr);
  ~NXRibbonGroup();

  void setTitle(const QString &title);
  QString getTitle() const;

  NXToolButton *addToolButton(NXIconType::IconName icon, const QString &text, ButtonSize size = Large);
  void addWidget(QWidget *widget);

protected:
  void paintEvent(QPaintEvent *event) override;
  QSize sizeHint() const override;
};

#endif // NXRIBBONGROUP_H
