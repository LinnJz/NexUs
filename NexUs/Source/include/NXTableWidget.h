#ifndef NXTABLEWIDGET_H
#define NXTABLEWIDGET_H

#include <QTableWidget>

#include "NXProperty.h"

class NXTableWidgetPrivate;

class NX_EXPORT NXTableWidget : public QTableWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXTableWidget)
  Q_PROPERTY_CREATE_H(int, ItemHeight)
  Q_PROPERTY_CREATE_H(int, HeaderMargin)
  Q_PROPERTY_CREATE_H(bool, IsTransparent)

public:
  explicit NXTableWidget(QWidget *parent = nullptr);
  ~NXTableWidget();

  void insertRows(int row, int count) noexcept;
  void removeRows(int row, int count) noexcept;
  void insertColumns(int column, int count) noexcept;
  void removeColumns(int column, int count) noexcept;

  void setItemText(int row, int column, const QString& text) noexcept;
  QString getItemText(int row, int column) const noexcept;
  void setRowData(int row, const QStringList& data) noexcept;
  QStringList getRowData(int row) const noexcept;

Q_SIGNALS:
  void tableWidgetShow();
  void tableWidgetHide();
  void hoverIndexChanged(const QModelIndex& index);

protected:
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;
};

#endif // NXTABLEWIDGET_H
