#ifndef NXPASSWORDBOX_H
#define NXPASSWORDBOX_H

#include <QLineEdit>

#include "NXProperty.h"

class NXPasswordBoxPrivate;
class NX_EXPORT NXPasswordBox : public QLineEdit
{
    Q_OBJECT
    Q_Q_CREATE(NXPasswordBox)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(bool, IsPasswordVisible)
public:
    explicit NXPasswordBox(QWidget* parent = nullptr);
    ~NXPasswordBox() override;

Q_SIGNALS:
  void focusIn(const QString& text);
  void focusOut(const QString& text);
  void wmFocusOut(const QString& text);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // NXPASSWORDBOX_H
