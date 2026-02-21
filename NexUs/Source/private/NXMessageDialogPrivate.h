#ifndef NXMESSAGEDIALOGPRIVATE_H
#define NXMESSAGEDIALOGPRIVATE_H

#include <QObject>
#include <QWidget>

#include "NXDef.h"

class NXMessageDialog;

class NXMessageDialogButton : public QWidget
{
  Q_OBJECT

public:
  enum ButtonType
  {
    Confirm,
    Cancel
  };

  explicit NXMessageDialogButton(ButtonType type, QWidget *parent = nullptr);
  ~NXMessageDialogButton() override;

Q_SIGNALS:
  void clicked();

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;

private:
  ButtonType _buttonType;
  bool _isPressed { false };
  bool _isHovered { false };
  NXThemeType::ThemeMode _themeMode;
};

class NXMessageDialogPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXMessageDialog)

  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, Content)
  Q_PROPERTY_CREATE_D(int, TitlePixelSize)
  Q_PROPERTY_CREATE_D(int, ContentPixelSize)

public:
  explicit NXMessageDialogPrivate(QObject *parent = nullptr);

  ~NXMessageDialogPrivate();

  NXThemeType::ThemeMode _themeMode;
  NXMessageDialogButton *_confirmButton { nullptr };
  NXMessageDialogButton *_cancelButton { nullptr };
};

#endif // NXMESSAGEDIALOGPRIVATE_H
