#ifndef NXMULTICELLLINEEDIT_H
#define NXMULTICELLLINEEDIT_H

#include <QWidget>
#include <QLineEdit>

#include "NXProperty.h"

class QRegularExpression;
class QValidator;
class NXMultiCellLineEditPrivate;

class NX_EXPORT NXMultiCellLineEdit : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXMultiCellLineEdit)

  Q_PRIVATE_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(bool, IsCellAutoAdvanceEnabled)
  Q_PROPERTY_CREATE_Q_H(int, CellCount)
  Q_PROPERTY_CREATE_Q_H(int, CellSpacing)
  Q_PROPERTY_CREATE_Q_H(int, ContentsHorizontalMargin)
  Q_PROPERTY_CREATE_Q_EX_H(const QString&, QString, CellSeparator)
  Q_PROPERTY_CREATE_Q_EX_H(const QString&, QString, CellEmptyFillText)

  Q_PRIVATE_CREATE_Q_EX_H(const QString&, QString, CombinedText)
  Q_PRIVATE_CREATE_Q_EX_H(const QStringList&, QStringList, CellTexts)
  Q_PRIVATE_CREATE_Q_EX_H(const QStringList&, QStringList, CellRegularExpressionPatterns)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<bool>&, QList<bool>, IsCellClearButtonEnabledList)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<int>&, QList<int>, CellMaxLengths)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<QMargins>&, QList<QMargins>, CellContentsMarginsList)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<bool>&, QList<bool>, IsCellReadOnlyList)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<QString>&, QList<QString>, CellPlaceholderTexts)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<Qt::Alignment>&, QList<Qt::Alignment>, CellAlignments)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<QFont>&, QList<QFont>, CellFonts)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<QString>&, QList<QString>, CellInputMasks)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<bool>&, QList<bool>, IsCellEnabledList)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<Qt::InputMethodHints>&, QList<Qt::InputMethodHints>, CellInputMethodHintsList)
  Q_PRIVATE_CREATE_Q_EX_H(const QList<QLineEdit::EchoMode>&, QList<QLineEdit::EchoMode>, CellEchoModes)

public:
  explicit NXMultiCellLineEdit(QWidget *parent = nullptr);
  explicit NXMultiCellLineEdit(int cellCount, const QString& separator, QWidget *parent = nullptr);
  ~NXMultiCellLineEdit() override;

  void setCustomLineEdit(int cellIndex, QLineEdit *lineEdit);
  QLineEdit *getCustomLineEdit(int cellIndex) const;
  QLineEdit *getCellLineEdit(int cellIndex) const;
  int getCellIndex(const QLineEdit *lineEdit) const;
  QList<QLineEdit *> getCellLineEdits() const;

  void setCellRegularExpression(int cellIndex, const QRegularExpression& regularExpression);
  QRegularExpression getCellRegularExpression(int cellIndex) const;

  void setCellMaxLength(int cellIndex, int maxLength);
  int getCellMaxLength(int cellIndex) const;

  void setIsClearButtonEnabled(int cellIndex, bool isEnabled);
  bool getIsClearButtonEnabled(int cellIndex) const;

  void setCellMargins(int cellIndex, const QMargins& margins);
  QMargins getCellMargins(int cellIndex) const;

  void setIsCellReadOnly(int cellIndex, bool readOnly);
  bool getIsCellReadOnly(int cellIndex) const;

  void setCellText(int cellIndex, const QString& text);
  QString getCellText(int cellIndex) const;

  void setCellPlaceholderText(int cellIndex, const QString& placeholder);
  QString getCellPlaceholderText(int cellIndex) const;

  void setCellAlignment(int cellIndex, Qt::Alignment alignment);
  Qt::Alignment getCellAlignment(int cellIndex) const;

  void setCellFont(int cellIndex, const QFont& font);
  QFont getCellFont(int cellIndex) const;

  void setCellInputMask(int cellIndex, const QString& inputMask);
  QString getCellInputMask(int cellIndex) const;

  void setIsCellEnabled(int cellIndex, bool enabled);
  bool getIsCellEnabled(int cellIndex) const;

  void setCellValidator(int cellIndex, QValidator *validator);
  const QValidator *getCellValidator(int cellIndex) const;

  void setCellInputMethodHints(int cellIndex, Qt::InputMethodHints hints);
  Qt::InputMethodHints getCellInputMethodHints(int cellIndex) const;

  void setCellEchoMode(int cellIndex, QLineEdit::EchoMode mode);
  QLineEdit::EchoMode getCellEchoMode(int cellIndex) const;
  bool getIsCellValid(int cellIndex) const;
  QList<bool> getCellValidStates() const;
  void setIsAllCellReadOnly(bool readOnly);
  void setIsAllCellEnabled(bool enabled);

  void clear(int cellIndex);
  void clearAll();

  void setFocusToCell(int cellIndex);
  int currentFocusCellIndex() const;

  void insertCell(int cellIndex, const QString& initialText = QString());
  void removeCell(int cellIndex);

Q_SIGNALS:
  void cellFocusIn(int cellIndex);
  void cellFocusOut(int cellIndex);
  void cellTextChanged(int cellIndex, const QString& text);
  void cellTextEdited(int cellIndex, const QString& text);
  void cellEditingFinished(int cellIndex);
  void cellReturnPressed(int cellIndex);
  void cellValidatorStateChanged(int cellIndex, bool valid);
  void cellClearButtonClicked(int cellIndex);

protected:
  void focusInEvent(QFocusEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif
