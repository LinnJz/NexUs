#ifndef NXNAVIGATIONSTYLE_H
#define NXNAVIGATIONSTYLE_H
#include <QModelIndex>
#include <QProxyStyle>

#include "NXDef.h"
class NXNavigationNode;
class NXNavigationView;
class QPropertyAnimation;

class NXNavigationStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PROPERTY_CREATE(qreal, Opacity)
  Q_PROPERTY_CREATE(qreal, Rotate)
  Q_PROPERTY_CREATE(qreal, LastSelectMarkTop)
  Q_PROPERTY_CREATE(qreal, LastSelectMarkBottom)
  Q_PROPERTY_CREATE(qreal, SelectMarkTop)
  Q_PROPERTY_CREATE(qreal, SelectMarkBottom)
  Q_PRIVATE_CREATE(QModelIndex, PressIndex)
  Q_PRIVATE_CREATE(NXNavigationView *, NavigationView)
  Q_PROPERTY_CREATE(int, ItemHeight)
public:
  explicit NXNavigationStyle(QStyle *style = nullptr);
  ~NXNavigationStyle() override;
  void drawPrimitive(PrimitiveElement element,
                     const QStyleOption *option,
                     QPainter *painter,
                     const QWidget *widget = nullptr) const override;
  void drawControl(ControlElement element,
                   const QStyleOption *option,
                   QPainter *painter,
                   const QWidget *widget = nullptr) const override;
  QSize sizeFromContents(ContentsType type,
                         const QStyleOption *option,
                         const QSize& size,
                         const QWidget *widget) const override;

  void navigationNodeStateChange(QVariantMap data);

private:
  NXThemeType::ThemeMode _themeMode;
  int _leftPadding { 11 };
  int _iconAreaWidth { 40 };
  int _textRightSpacing { 3 };
  int _indicatorIconAreaWidth { 34 };
  bool _isSelectMarkDisplay { true };

  NXNavigationNode *_opacityAnimationTargetNode { nullptr };
  NXNavigationNode *_expandAnimationTargetNode { nullptr };
  NXNavigationNode *_lastSelectedNode { nullptr };
  QPropertyAnimation *_lastSelectMarkTopAnimation { nullptr };
  QPropertyAnimation *_lastSelectMarkBottomAnimation { nullptr };
  QPropertyAnimation *_selectMarkTopAnimation { nullptr };
  QPropertyAnimation *_selectMarkBottomAnimation { nullptr };
  bool _compareItemY(NXNavigationNode *node1, NXNavigationNode *node2);
};

#endif // NXNAVIGATIONSTYLE_H
