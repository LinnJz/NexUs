#ifndef NXCOMMANDBAR_H
#define NXCOMMANDBAR_H

#include <QWidget>

#include "NXDef.h"

class NXMenu;
class NXCommandBarPrivate;
class NX_EXPORT NXCommandBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXCommandBar)
    Q_PROPERTY_CREATE_H(int, ButtonSize)
public:
    struct CommandItem
    {
        NXIconType::IconName icon = NXIconType::None;
        QString text;
        bool isSeparator = false;
    };

    explicit NXCommandBar(QWidget* parent = nullptr);
    ~NXCommandBar();

    void addItem(const CommandItem& item) noexcept;
    void addSeparator() noexcept;
    void clearItems() noexcept;

Q_SIGNALS:
  void itemClicked(int index);

protected:
  void paintEvent(QPaintEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void leaveEvent(QEvent* event) override;
  QSize sizeHint() const override;
};

#endif // NXCOMMANDBAR_H
