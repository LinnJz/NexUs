#ifndef NXSHEETPANELPRIVATE_H
#define NXSHEETPANELPRIVATE_H

#include <QObject>
#include <QPoint>

#include "NXDef.h"
class QVBoxLayout;
class QPropertyAnimation;
class NXMaskWidget;
class NXSheetPanel;

class NXSheetPanelPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXSheetPanel)
  Q_PROPERTY_CREATE_D(qreal, PeekRatio)
  Q_PROPERTY_CREATE_D(qreal, HalfRatio)
  Q_PROPERTY_CREATE_D(qreal, FullRatio)
  Q_PROPERTY_CREATE_D(qreal, OverlayOpacity)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(NXSheetPanelType::Direction, Direction)
  Q_PROPERTY_CREATE_D(bool, DragHandleVisible)
  Q_PROPERTY_CREATE_D(bool, CloseOnOverlayClick)

public:
  explicit NXSheetPanelPrivate(QObject *parent = nullptr);
  ~NXSheetPanelPrivate();

  void _doOpenAnimation(NXSheetPanelType::DetentLevel level);
  void _doCloseAnimation();
  void _updatePanelGeometry(int targetSize, bool animate = true);
  int _getTargetSize(NXSheetPanelType::DetentLevel level) const;
  NXSheetPanelType::DetentLevel _getNearestDetent(int currentSize) const;

private:
  friend class NXSheetPanel;
  bool _isOpened { false };
  bool _isDragging { false };
  NXThemeType::ThemeMode _themeMode;
  NXSheetPanelType::DetentLevel _currentDetent { NXSheetPanelType::Half };
  int _dragStartSize { 0 };
  QPoint _dragStartPos;
  NXMaskWidget *_maskWidget { nullptr };
  QWidget *_panelWidget { nullptr };
  QWidget *_centralWidget { nullptr };
  QWidget *_dragHandle { nullptr };
  QVBoxLayout *_panelLayout { nullptr };
};

#endif // NXSHEETPANELPRIVATE_H
