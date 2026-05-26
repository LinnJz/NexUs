#ifndef NXSHEETPANEL_H
#define NXSHEETPANEL_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXSheetPanelPrivate;

class NX_EXPORT NXSheetPanel : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXSheetPanel)
  Q_PROPERTY_CREATE_H(qreal, PeekRatio)
  Q_PROPERTY_CREATE_H(qreal, HalfRatio)
  Q_PROPERTY_CREATE_H(qreal, FullRatio)
  Q_PROPERTY_CREATE_H(qreal, OverlayOpacity)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(NXSheetPanelType::Direction, Direction)
  Q_PROPERTY_CREATE_H(bool, DragHandleVisible)
  Q_PROPERTY_CREATE_H(bool, CloseOnOverlayClick)

public:
  explicit NXSheetPanel(QWidget *parent = nullptr);
  ~NXSheetPanel();

  void setCentralWidget(QWidget *widget);

  void open(NXSheetPanelType::DetentLevel level = NXSheetPanelType::Half);
  void close();

  NXSheetPanelType::DetentLevel currentDetent() const;
  bool isOpened() const;
  Q_SIGNAL void opened();
  Q_SIGNAL void closed();
  Q_SIGNAL void detentChanged(NXSheetPanelType::DetentLevel level);

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // NXSHEETPANEL_H
