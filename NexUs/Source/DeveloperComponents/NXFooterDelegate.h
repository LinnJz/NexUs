#ifndef NXFOOTERDELEGATE_H
#define NXFOOTERDELEGATE_H

#include <QModelIndex>
#include <QStyledItemDelegate>

#include "NXDef.h"
class NXNavigationNode;
class QPropertyAnimation;
class NXBaseListView;

class NXFooterDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  Q_PRIVATE_CREATE(QS_SET_CREF(QModelIndex), PressIndex)
  Q_PRIVATE_CREATE(NXBaseListView *, NXListView)

public:
  explicit NXFooterDelegate(QObject *parent = nullptr);
  ~NXFooterDelegate();
  void navigationNodeStateChange(const QVariantMap &data);

protected:
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
  bool _isSelectMarkDisplay { true };
  int _leftPadding { 11 };
  int _iconAreaWidth { 40 };
  int _textRightSpacing { 3 };
  int _indicatorIconAreaWidth { 24 };
  NXThemeType::ThemeMode _themeMode;
  qreal _lastSelectMarkTop { 10 };
  qreal _lastSelectMarkBottom { 10 };
  qreal _selectMarkTop { 10 };
  qreal _selectMarkBottom { 10 };
  NXNavigationNode *_lastSelectedNode { nullptr };
  QPropertyAnimation *_lastSelectMarkTopAnimation { nullptr };
  QPropertyAnimation *_lastSelectMarkBottomAnimation { nullptr };
  QPropertyAnimation *_selectMarkTopAnimation { nullptr };
  QPropertyAnimation *_selectMarkBottomAnimation { nullptr };
  bool _compareItemY(NXNavigationNode *node1, NXNavigationNode *node2);
};

#endif // NXFOOTERDELEGATE_H
