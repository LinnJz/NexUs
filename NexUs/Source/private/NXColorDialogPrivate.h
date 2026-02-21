#ifndef NXCOLORDIALOGPRIVATE_H
#define NXCOLORDIALOGPRIVATE_H

#include <QModelIndex>
#include <QObject>
#include <QPixmap>

#include "NXDef.h"
class NXAppBar;
class NXColorDialog;
class NXColorPicker;
class NXBaseListView;
class NXColorDisplayModel;
class NXColorDisplayDelegate;
class NXColorPreview;
class NXLineEdit;
class NXComboBox;
class NXText;
class NXPushButton;
class NXToolButton;
class QSlider;
class QColor;
class NXColorValueSliderStyle;

class NXColorDialogPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXColorDialog)
  Q_PROPERTY_CREATE_D(QColor, CurrentColor)
  Q_PROPERTY_CREATE_D(NXColorSchemeType::ColorSchemeType, ColorSchemeType)
public:
  explicit NXColorDialogPrivate(QObject *parent = nullptr);
  ~NXColorDialogPrivate();

  Q_SLOT void onColorPickerColorChanged(QColor selectedColor);
  Q_SLOT void onColorValueSliderChanged(int value);
  Q_SLOT void onTransparencyValueSliderChanged(int value);

  Q_SLOT void onColorModeChanged(int index);
  Q_SLOT void onHtmlEditFocusOut(const QString& text);
  Q_SLOT void onHtmlEditChanged(const QString& text);
  Q_SLOT void onColorEditChanged(const QString& text);

  Q_SLOT void onBasicColorViewClicked(const QModelIndex& index);
  Q_SLOT void onCustomColorViewClicked(const QModelIndex& index);

  Q_SLOT void onAddCustomColorButtonClicked();
  Q_SLOT void onRemoveCustomColorButtonClicked();

private:
  NXThemeType::ThemeMode _themeMode;
  NXAppBar *_appBar { nullptr };
  NXColorPicker *_colorPicker { nullptr };
  NXColorPreview *_colorPreview { nullptr };
  QSlider *_colorValueSlider { nullptr };
  QSlider *_transparencyValueSlider { nullptr };
  NXColorValueSliderStyle *_colorValueSliderStyle { nullptr };
  NXColorValueSliderStyle *_transparencyValueSliderStyle { nullptr };
  NXLineEdit *_htmlEdit { nullptr };
  NXComboBox *_modeComboBox { nullptr };
  NXLineEdit *_firstEdit { nullptr };
  NXLineEdit *_secondEdit { nullptr };
  NXLineEdit *_thridEdit { nullptr };
  NXLineEdit *_fourthEdit { nullptr };
  NXText *_firstText { nullptr };
  NXText *_secondText { nullptr };
  NXText *_thridText { nullptr };
  NXText *_fourthText { nullptr };

  NXBaseListView *_basicColorView { nullptr };
  NXColorDisplayModel *_basicColorModel { nullptr };
  NXColorDisplayDelegate *_basicColorDelegate { nullptr };

  NXToolButton *_addCustomColorButton { nullptr };
  NXToolButton *_removeCustomColorButton { nullptr };
  NXBaseListView *_customColorView { nullptr };
  NXColorDisplayModel *_customColorModel { nullptr };
  NXColorDisplayDelegate *_customColorDelegate { nullptr };

  NXPushButton *_overButton { nullptr };
  NXPushButton *_cancelButton { nullptr };

  void _initBasicColor();
  void _initCustomColor();
  void _updateHtmlEditValue();
  void _updateEditValue();
  void _updateColorPreview();
  void _updateColorValueSlider();
  void _updateTransparencyValueSlider();
  QString _completeColorText(QString text) const;
  QString _getHex4ChanelValue() const;
  QColor _getColorFromEdit() const;
};

#endif // NXCOLORDIALOGPRIVATE_H
