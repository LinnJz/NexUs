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
  Q_PROPERTY_CREATE_H(bool, DragHandleVisible)
  Q_PROPERTY_CREATE_H(bool, CloseOnOverlayClick)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(NXSheetPanelType::Direction, Direction)
    Q_PROPERTY_CREATE_H(qreal, OverlayOpacity)
    Q_PROPERTY_CREATE_H(qreal, PeekRatio)
    Q_PROPERTY_CREATE_H(qreal, HalfRatio)
    Q_PROPERTY_CREATE_H(qreal, FullRatio)

  public:
    explicit NXSheetPanel(QWidget* parent = nullptr);
    ~NXSheetPanel() override;

    void setCentralWidget(QWidget* widget) noexcept;

    void open(NXSheetPanelType::DetentLevel level = NXSheetPanelType::Half) noexcept;
    void close();

    NXSheetPanelType::DetentLevel currentDetent() const noexcept;
    bool isOpened() const noexcept;

Q_SIGNALS:
  void opened();
  void closed();
  void detentChanged(NXSheetPanelType::DetentLevel level);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // NXSHEETPANEL_H
