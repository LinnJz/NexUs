#ifndef NXINPUTDIALOGPRIVATE_H
#define NXINPUTDIALOGPRIVATE_H

#include <QObject>

#include "NXDef.h"

class NXInputDialog;
class NXMaskWidget;
class NXAppBar;
class NXLineEdit;
class NXSpinBox;
class NXDoubleSpinBox;
class NXText;
class NXPushButton;
class QWidget;
class QTextEdit;
class QVBoxLayout;

class NXInputDialogPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXInputDialog)
  Q_PROPERTY_CREATE_D(QString, TitleText)
  Q_PROPERTY_CREATE_D(QString, SubTitleText)
  Q_PROPERTY_CREATE_D(QString, LabelText)
  Q_PROPERTY_CREATE_D(QString, TextValue)
  Q_PROPERTY_CREATE_D(int, IntValue)
  Q_PROPERTY_CREATE_D(double, DoubleValue)
  Q_PROPERTY_CREATE_D(QString, OkButtonText)
  Q_PROPERTY_CREATE_D(QString, CancelButtonText)
  Q_PROPERTY_CREATE_D(QString, PlaceholderText)
  Q_PROPERTY_CREATE_D(int, InputMinimumWidth)
  Q_PROPERTY_CREATE_D(int, InputMaximumWidth)

public:
  explicit NXInputDialogPrivate(QObject *parent = nullptr);
  ~NXInputDialogPrivate() override;

private:
  qint64 _currentWinID { 0 };
  NXAppBar *_appBar { nullptr };
  NXThemeType::ThemeMode _themeMode;
  NXMaskWidget *_maskWidget { nullptr };
  NXText *_titleLabel { nullptr };
  NXText *_subTitleLabel { nullptr };
  NXText *_inputLabel { nullptr };
  NXLineEdit *_lineEdit { nullptr };
  NXSpinBox *_spinBox { nullptr };
  NXDoubleSpinBox *_doubleSpinBox { nullptr };
  QTextEdit *_textEdit { nullptr };
  QVBoxLayout *_contentLayout { nullptr };
  QWidget *_buttonWidget { nullptr };
  NXPushButton *_okButton { nullptr };
  NXPushButton *_cancelButton { nullptr };
  bool _isMultiLine { false };
  bool _isIntMode { false };
  bool _isDoubleMode { false };
  void _doCloseAnimation(bool isAccept);
  friend class NXInputDialog;
};

#endif // NXINPUTDIALOGPRIVATE_H
