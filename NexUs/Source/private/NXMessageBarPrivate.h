#ifndef NXMESSAGEBARPRIVATE_H
#define NXMESSAGEBARPRIVATE_H

#include <QMap>
#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "LinnSingleton.h"
#include "NXDef.h"

enum WorkStatus
{
  Idle                = 0x00'00,
  CreateAnimation     = 0x00'01,
  OtherEventAnimation = 0x00'02,
};

class NXMessageBar;

class NXMessageBarManager : public QObject
{
  Q_OBJECT
  Q_SINGLETON_CREATE(QS_S_UNIQUE(NXMessageBarManager))

private:
  explicit NXMessageBarManager(QObject *parent = nullptr);
  ~NXMessageBarManager() override;

public:
  // 请求事件堆栈调用
  void requestMessageBarEvent(NXMessageBar *messageBar) noexcept;
  // 发布创建事件
  void postMessageBarCreateEvent(NXMessageBar *messageBar) noexcept;
  // 发布终止事件
  void postMessageBarEndEvent(NXMessageBar *messageBar) noexcept;
  // 强制发布终止事件
  void forcePostMessageBarEndEvent(NXMessageBar *messageBar) noexcept;
  // 获取当前事件数量
  int getMessageBarEventCount(NXMessageBar *messageBar) noexcept;
  // 更新活动序列
  void updateActiveMap(NXMessageBar *messageBar, bool isActive) noexcept;

private:
  QMap<NXMessageBar *, QList<QVariantMap>> _messageBarEventMap;
};

class NXIconButton;
class QPainter;

class NXMessageBarPrivate : public QObject
{
  friend class NXMessageBarManager;
  Q_OBJECT
  Q_D_CREATE(NXMessageBar)
  Q_PROPERTY_CREATE(qreal, Opacity)
  Q_PROPERTY_CREATE(qreal, TimePercent)

public:
  explicit NXMessageBarPrivate(QObject *parent = nullptr);
  ~NXMessageBarPrivate() override;
  void tryToRequestMessageBarEvent() noexcept;
  WorkStatus getWorkMode() const noexcept;
  Q_INVOKABLE void onOtherMessageBarEnd(const QVariantMap& eventData);
  Q_INVOKABLE void messageBarEnd(const QVariantMap& eventData);
  Q_SLOT void onCloseButtonClicked() noexcept;
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode) noexcept;

private:
  qreal _createTime { 0 };
  qreal _timePercentHeight { 2 };

  QString _title { };
  QString _text { };
  NXThemeType::ThemeMode _themeMode;
  int _borderRadius { 6 };
  NXMessageBarType::PositionPolicy _policy;
  NXMessageBarType::MessageMode _messageMode;


  // 位置数据
  int _leftPadding { 20 };                // 左边框到图标中心
  int _titleLeftSpacing { 30 };           // 图标中心到Title左侧
  int _textLeftSpacing { 15 };            // Title右侧到Text左侧
  int _closeButtonLeftRightMargin { 20 }; // closeButton左右总Margin
  int _closeButtonWidth { 30 };
  int _messageBarHorizontalMargin { 20 };
  int _messageBarVerticalBottomMargin { 20 };
  int _messageBarVerticalTopMargin { 50 };
  int _messageBarSpacing { 15 };
  int _shadowBorderWidth { 6 };

  // 逻辑数据
  bool _isMessageBarCreateAnimationFinished { false };
  bool _isReadyToEnd { false };
  bool _isNormalDisplay { false };
  bool _isMessageBarEventAnimationStart { false };
  NXIconButton *_closeButton { nullptr };

  Q_INVOKABLE void _messageBarCreate(int displayMsec) noexcept;

  // 初始坐标计算
  void _calculateInitialPos(int& startX, int& startY, int& endX, int& endY) noexcept;
  // 获取总高度和次序信息
  QList<int> _getOtherMessageBarTotalData(bool isJudgeCreateOrder = false) noexcept;
  // 计算目标坐标
  qreal _calculateTargetPosY() noexcept;

  // 创建次序判断
  bool _judgeCreateOrder(NXMessageBar *otherMessageBar) noexcept;

  // 绘制函数
  void _drawSuccess(QPainter *painter) noexcept;
  void _drawWarning(QPainter *painter) noexcept;
  void _drawInformation(QPainter *painter) noexcept;
  void _drawError(QPainter *painter) noexcept;
};

#endif // NXMESSAGEBARPRIVATE_H
