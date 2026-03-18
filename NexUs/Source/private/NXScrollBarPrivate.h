#ifndef NXSCROLLBARPRIVATE_H
#define NXSCROLLBARPRIVATE_H

#include <QAbstractScrollArea>
#include <QObject>
#include <QScrollBar>

#include "NXProperty.h"
class QTimer;
class QPropertyAnimation;
class NXScrollBar;

class NXScrollBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXScrollBar)
  Q_PROPERTY_CREATE_D(qreal, SpeedLimit)
  Q_PROPERTY_CREATE(int, TargetMaximum)
  Q_PROPERTY_CREATE_D(bool, IsAnimation)

public:
  explicit NXScrollBarPrivate(QObject *parent = nullptr);
  ~NXScrollBarPrivate();
  Q_SLOT void onRangeChanged(int min, int max) noexcept;

private:
  bool _isExpand { false };
  int _scrollValue { -1 };
  QScrollBar *_originScrollBar { nullptr };
  QAbstractScrollArea *_originScrollArea { nullptr };
  QTimer *_expandTimer { nullptr };
  QPropertyAnimation *_slideSmoothAnimation { nullptr };

  void _scroll(Qt::KeyboardModifiers modifiers, int value) noexcept;
  int _pixelPosToRangeValue(int pos) const noexcept;

  // 映射处理函数
  void _initAllConfig() noexcept;
  void _handleScrollBarValueChanged(QScrollBar *scrollBar, int value) noexcept;
  void _handleScrollBarRangeChanged(int min, int max) noexcept;
  void _handleScrollBarGeometry() noexcept;
};

#endif // NXSCROLLBARPRIVATE_H
