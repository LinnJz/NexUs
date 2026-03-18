#ifndef NXLINEEDIT_H
#define NXLINEEDIT_H

#include <QLineEdit>

#include "NXProperty.h"

class NXLineEditPrivate;

class NX_EXPORT NXLineEdit : public QLineEdit
{
  Q_OBJECT
  Q_Q_CREATE(NXLineEdit)
  Q_PROPERTY_CREATE_H(bool, IsClearButtonEnabled)

public:
  explicit NXLineEdit(QWidget *parent = nullptr);
  explicit NXLineEdit(const QString& text, QWidget *parent = nullptr);
  ~NXLineEdit() override;
  void setBorderRadius(int borderRadius) noexcept;
  int getBorderRadius() const noexcept;

  void setContentsMargins(const QMargins& margins) noexcept;
  QMargins getContentsMargins() const noexcept;

  void setLineEditIconMargin(int margin) noexcept;
  int getLineEditIconMargin() const noexcept;
Q_SIGNALS:
  void focusIn(const QString& text);
  void focusOut(const QString& text);
  void wmFocusOut(const QString& text);

protected:
  void focusInEvent(QFocusEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
};

#endif // NXLINEEDIT_H
