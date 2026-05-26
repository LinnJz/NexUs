#ifndef NXTREESELECTPRIVATE_H
#define NXTREESELECTPRIVATE_H

#include <QModelIndex>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QWidget>

#include "NXDef.h"

class QStandardItemModel;
class NXTreeSelect;
class NXTreeView;
class NXLineEdit;
class QSortFilterProxyModel;

class NXTreeSelectPopupWidget : public QWidget
{
  Q_OBJECT

public:
  NXTreeSelectPopupWidget(QWidget *parent = nullptr);
  NXThemeType::ThemeMode _themeMode;
  int _borderRadius { 6 };

protected:
  void paintEvent(QPaintEvent *event) override;
};

class NXTreeSelectPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXTreeSelect)
  Q_PROPERTY_CREATE_D(QString, PlaceholderText)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, ItemHeight)
  Q_PROPERTY_CREATE_D(int, MaxVisibleItems)
  Q_PROPERTY_CREATE_D(bool, IsSearchVisible)
  Q_PROPERTY_CREATE_D(bool, IsEditable)

public:
  explicit NXTreeSelectPrivate(QObject *parent = nullptr);
  ~NXTreeSelectPrivate();

  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);
  Q_SLOT void onItemClicked(const QModelIndex &index);
  Q_SLOT void onSearchTextChanged(const QString &text);
  Q_SLOT void onTreeExpanded(const QModelIndex &index);
  Q_SLOT void onTreeCollapsed(const QModelIndex &index);

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  friend class NXTreeSelect;
  bool _isHover { false };
  bool _isPopupVisible { false };
  bool _isPressed { false };
  NXThemeType::ThemeMode _themeMode;
  qreal _expandIconRotate { 0 };
  QString _currentText;
  QModelIndex _currentIndex;
  QStandardItemModel *_model { nullptr };
  QSortFilterProxyModel *_proxyModel { nullptr };
  NXTreeView *_treeView { nullptr };
  NXLineEdit *_searchEdit { nullptr };
  NXTreeSelectPopupWidget *_popupContainer { nullptr };
  void _showPopup();
  void _hidePopup();
  void _updateDisplayText();
  void _updatePopupHeight();
  int _calculatePopupHeight() const;
  int _countVisibleItems(const QModelIndex &parent = QModelIndex()) const;
};

#endif // NXTREESELECTPRIVATE_H
