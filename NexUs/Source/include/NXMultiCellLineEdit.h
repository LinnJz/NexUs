#ifndef NXMULTICELLLINEEDIT_H
#define NXMULTICELLLINEEDIT_H

#include <QLineEdit>
#include <QWidget>

#include "NXProperty.h"

class QRegularExpression;
class QValidator;
class NXMultiCellLineEditPrivate;

class NX_EXPORT NXMultiCellLineEdit : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXMultiCellLineEdit)

  Q_PRIVATE_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(bool, IsCellAutoAdvanceEnabled)
  Q_PROPERTY_CREATE_H(int, CellCount)
  Q_PROPERTY_CREATE_H(int, CellSpacing)
  Q_PROPERTY_CREATE_H(int, ContentsHorizontalMargin)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, CellSeparator)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, CellEmptyFillText)

  Q_PRIVATE_CREATE_2_H(const QString&, QString, CombinedText)
  Q_PRIVATE_CREATE_2_H(const QStringList&, QStringList, CellTexts)
  Q_PRIVATE_CREATE_2_H(const QStringList&, QStringList, CellRegularExpressionPatterns)
  Q_PRIVATE_CREATE_2_H(const QList<bool>&, QList<bool>, IsCellClearButtonEnabledList)
  Q_PRIVATE_CREATE_2_H(const QList<int>&, QList<int>, CellMaxLengths)
  Q_PRIVATE_CREATE_2_H(const QList<QMargins>&, QList<QMargins>, CellContentsMarginsList)
  Q_PRIVATE_CREATE_2_H(const QList<bool>&, QList<bool>, IsCellReadOnlyList)
  Q_PRIVATE_CREATE_2_H(const QList<QString>&, QList<QString>, CellPlaceholderTexts)
  Q_PRIVATE_CREATE_2_H(const QList<Qt::Alignment>&, QList<Qt::Alignment>, CellAlignments)
  Q_PRIVATE_CREATE_2_H(const QList<QFont>&, QList<QFont>, CellFonts)
  Q_PRIVATE_CREATE_2_H(const QList<QString>&, QList<QString>, CellInputMasks)
  Q_PRIVATE_CREATE_2_H(const QList<bool>&, QList<bool>, IsCellEnabledList)
  Q_PRIVATE_CREATE_2_H(const QList<Qt::InputMethodHints>&, QList<Qt::InputMethodHints>, CellInputMethodHintsList)
  Q_PRIVATE_CREATE_2_H(const QList<QLineEdit::EchoMode>&, QList<QLineEdit::EchoMode>, CellEchoModes)

public:
  explicit NXMultiCellLineEdit(QWidget *parent = nullptr);
  explicit NXMultiCellLineEdit(int cellCount, const QString& separator, QWidget *parent = nullptr);
  ~NXMultiCellLineEdit() override;

  void setCustomLineEdit(int cellIndex, QLineEdit *lineEdit) noexcept;
  QLineEdit *getCustomLineEdit(int cellIndex) const noexcept;
  QLineEdit *getCellLineEdit(int cellIndex) const noexcept;
  int getCellIndex(const QLineEdit *lineEdit) const noexcept;
  QList<QLineEdit *> getCellLineEdits() const noexcept;

  void setCellRegularExpression(int cellIndex, const QRegularExpression& regularExpression) noexcept;
  QRegularExpression getCellRegularExpression(int cellIndex) const noexcept;

  void setCellMaxLength(int cellIndex, int maxLength) noexcept;
  int getCellMaxLength(int cellIndex) const noexcept;

  void setIsClearButtonEnabled(int cellIndex, bool isEnabled) noexcept;
  bool getIsClearButtonEnabled(int cellIndex) const noexcept;

  void setCellMargins(int cellIndex, const QMargins& margins) noexcept;
  QMargins getCellMargins(int cellIndex) const noexcept;

  void setIsCellReadOnly(int cellIndex, bool readOnly) noexcept;
  bool getIsCellReadOnly(int cellIndex) const noexcept;

  void setCellText(int cellIndex, const QString& text) noexcept;
  QString getCellText(int cellIndex) const noexcept;

  void setCellPlaceholderText(int cellIndex, const QString& placeholder) noexcept;
  QString getCellPlaceholderText(int cellIndex) const noexcept;

  void setCellAlignment(int cellIndex, Qt::Alignment alignment) noexcept;
  Qt::Alignment getCellAlignment(int cellIndex) const noexcept;

  void setCellFont(int cellIndex, const QFont& font) noexcept;
  QFont getCellFont(int cellIndex) const noexcept;

  void setCellInputMask(int cellIndex, const QString& inputMask) noexcept;
  QString getCellInputMask(int cellIndex) const noexcept;

  void setIsCellEnabled(int cellIndex, bool enabled) noexcept;
  bool getIsCellEnabled(int cellIndex) const noexcept;

  void setCellValidator(int cellIndex, QValidator *validator) noexcept;
  const QValidator *getCellValidator(int cellIndex) const noexcept;

  void setCellInputMethodHints(int cellIndex, Qt::InputMethodHints hints) noexcept;
  Qt::InputMethodHints getCellInputMethodHints(int cellIndex) const noexcept;

  void setCellEchoMode(int cellIndex, QLineEdit::EchoMode mode) noexcept;
  QLineEdit::EchoMode getCellEchoMode(int cellIndex) const noexcept;
  bool getIsCellValid(int cellIndex) const noexcept;
  QList<bool> getCellValidStates() const noexcept;
  void setIsAllCellReadOnly(bool readOnly) noexcept;
  void setIsAllCellEnabled(bool enabled) noexcept;

  void clear(int cellIndex) noexcept;
  void clearAll() noexcept;

  void setFocusToCell(int cellIndex) noexcept;
  int currentFocusCellIndex() const noexcept;

  void insertCell(int cellIndex, const QString& initialText = QString()) noexcept;
  void removeCell(int cellIndex) noexcept;

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
