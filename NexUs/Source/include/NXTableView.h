#ifndef NXTABLEVIEW_H
#define NXTABLEVIEW_H

#include <QTableView>

#include "NXProperty.h"

class NXTableView;
class NXModelIndexWidgetPrivate;

class NX_EXPORT NXModelIndexWidget : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXModelIndexWidget)
  Q_PROPERTY_CREATE_Q_H(const QModelIndex&, Index)
public:
  explicit NXModelIndexWidget(NXTableView *parent = nullptr);
  explicit NXModelIndexWidget(const QModelIndex& index, NXTableView *parent = nullptr);
  ~NXModelIndexWidget();

  bool isValid() const;
  int row() const;
  int column() const;
Q_SIGNALS:
  void entered();
  void leaved();
  void clicked();
  void doubleClicked();
  void indexChanged(const QModelIndex& newIndex);

protected:
  virtual void enterEvent(QEnterEvent *event) override;
  virtual void leaveEvent(QEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
  virtual bool event(QEvent *event) override;
};
class NXTableViewPrivate;

class NX_EXPORT NXTableView : public QTableView
{
  Q_OBJECT
  Q_Q_CREATE(NXTableView)
  Q_PROPERTY_CREATE_Q_H(int, HeaderMargin)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(bool, IsDrawAlternateRowsEnabled)
  Q_PROPERTY_CREATE_Q_H(bool, IsSelectionEffectsEnabled)
  Q_PROPERTY_CREATE_Q_H(bool, IsHoverEffectsEnabled)

public:
  using coords = std::tuple<int, int, int, int>;
  explicit NXTableView(QWidget *parent = nullptr);
  ~NXTableView();
  void setIndexWidget(const QModelIndex& index, QWidget *widget);
  QWidget *indexWidget(const QModelIndex& index) const;

  void setHorizontalHeaderFontSize(int pixelSize);
  void setVerticalHeaderFontSize(int pixelSize);
  void setFontSize(int pixelSize);

  void adjustHeaderColumnIconRect(const QHash<int, coords>& adjusts);
  void adjustColummTextRect(const QHash<int, coords>& adjusts);

  QList<NXModelIndexWidget *> getIndexWidgets() const;
  NXModelIndexWidget *getIndexWidget(const QModelIndex& index) const;
  // void updateIndexWidgets();

Q_SIGNALS:
  Q_SIGNAL void tableViewShow();
  Q_SIGNAL void tableViewHide();
  Q_SIGNAL void currentHoverIndexChanged(const QModelIndex& index);
  void indexWidgetAdded(NXModelIndexWidget *widget);
  void indexWidgetRemoved(NXModelIndexWidget *widget);

protected:
  virtual void showEvent(QShowEvent *event) override;
  virtual void hideEvent(QHideEvent *event) override;
  virtual void leaveEvent(QEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  /*
  virtual void rowsInserted(const QModelIndex& parent, int start, int end) override;
  virtual void rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) override;
  virtual void reset() override;*/
private:
  friend class NXModelIndexWidget;
};

#endif // NXTABLEVIEW_H
