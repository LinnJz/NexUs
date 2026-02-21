#ifndef NXPLAINTEXTEDIT_H
#define NXPLAINTEXTEDIT_H

#include <QPlainTextEdit>

#include "NXProperty.h"

class NXPlainTextEditPrivate;

class NX_EXPORT NXPlainTextEdit : public QPlainTextEdit
{
  Q_OBJECT
  Q_Q_CREATE(NXPlainTextEdit)
public:
  explicit NXPlainTextEdit(QWidget *parent = nullptr);
  explicit NXPlainTextEdit(const QString& text, QWidget *parent = nullptr);
  ~NXPlainTextEdit() override;

protected:
  virtual void focusInEvent(QFocusEvent *event) override;
  virtual void focusOutEvent(QFocusEvent *event) override;
  virtual void contextMenuEvent(QContextMenuEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXPLAINTEXTEDIT_H
