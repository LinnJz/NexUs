#include "NXMessageBarPrivate.h"

#include <QDateTime>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <QPropertyAnimation>
#include <QTimer>

#include "NXIconButton.h"
#include "NXMessageBar.h"

QMap<NXMessageBarType::PositionPolicy, QList<QPointer<NXMessageBar>> *> _messageBarActiveMap;

NXMessageBarManager::NXMessageBarManager(QObject *parent) { }

NXMessageBarManager::~NXMessageBarManager() { }

void NXMessageBarManager::requestMessageBarEvent(NXMessageBar *messageBar)
{
  if (!messageBar) { return; }
  if (_messageBarEventMap.contains(messageBar))
  {
    QList<QVariantMap> eventList = _messageBarEventMap.value(messageBar);
    QVariantMap eventData        = eventList.last();
    eventList.removeLast();
    if (eventList.isEmpty()) { _messageBarEventMap.remove(messageBar); }
    else
    {
      _messageBarEventMap[messageBar] = eventList;
    }
    // 触发事件
    QString functionName     = eventData.value("EventFunctionName").toString();
    QVariantMap functionData = eventData.value("EventFunctionData").toMap();
    QMetaObject::invokeMethod(messageBar->d_func(),
                              functionName.toLocal8Bit().constData(),
                              Qt::AutoConnection,
                              Q_ARG(QVariantMap, functionData));
  }
}

void NXMessageBarManager::postMessageBarCreateEvent(NXMessageBar *messageBar)
{
  if (!messageBar) { return; }
  updateActiveMap(messageBar, true); // 计算坐标前增加
  if (!_messageBarEventMap.contains(messageBar))
  {
    QList<QVariantMap> eventList;
    QVariantMap eventData;
    eventData.insert("EventFunctionName", "messageBarEnd");
    eventList.append(eventData);
    _messageBarEventMap.insert(messageBar, eventList);
  }
}

void NXMessageBarManager::postMessageBarEndEvent(NXMessageBar *messageBar)
{
  if (!messageBar) { return; }
  updateActiveMap(messageBar, false);
  // Other MessageBar事件入栈 记录同一策略事件
  NXMessageBarType::PositionPolicy policy = messageBar->d_ptr->_policy;
  foreach (auto otherMessageBar, *_messageBarActiveMap.value(policy))
  {
    if (!otherMessageBar) continue;
    if (otherMessageBar->d_ptr->_judgeCreateOrder(messageBar))
    {
      QList<QVariantMap> eventList = _messageBarEventMap[otherMessageBar];
      // 优先执行先触发的事件 End事件保持首位
      QVariantMap eventData;
      eventData.insert("EventFunctionName", "onOtherMessageBarEnd");
      QVariantMap functionData;
      functionData.insert("TargetPosY", otherMessageBar->d_ptr->_calculateTargetPosY());
      eventData.insert("EventFunctionData", functionData);
      // 若处于创建动画阶段  则合并事件动画
      if (otherMessageBar->d_ptr->getWorkMode() == WorkStatus::CreateAnimation)
      {
        while (eventList.count() > 1) { eventList.removeLast(); }
      }
      eventList.insert(1, eventData);
      _messageBarEventMap[otherMessageBar] = eventList;
      otherMessageBar->d_ptr->tryToRequestMessageBarEvent();
    }
  }
}

void NXMessageBarManager::forcePostMessageBarEndEvent(NXMessageBar *messageBar)
{
  if (!messageBar) { return; }
  // 清除事件堆栈记录
  _messageBarEventMap.remove(messageBar);
  // 发布终止事件
  postMessageBarEndEvent(messageBar);
}

int NXMessageBarManager::getMessageBarEventCount(NXMessageBar *messageBar)
{
  if (!messageBar) { return -1; }
  if (!_messageBarEventMap.contains(messageBar)) { return -1; }
  QList<QVariantMap> eventList = _messageBarEventMap[messageBar];
  return eventList.count();
}

void NXMessageBarManager::updateActiveMap(NXMessageBar *messageBar, bool isActive)
{
  if (!messageBar) { return; }
  NXMessageBarType::PositionPolicy policy = messageBar->d_ptr->_policy;
  if (isActive)
  {
    if (_messageBarActiveMap.contains(policy)) { _messageBarActiveMap[policy]->append(messageBar); }
    else
    {
      QList<QPointer<NXMessageBar>> *messageBarList = new QList<QPointer<NXMessageBar>>();
      messageBarList->append(messageBar);
      _messageBarActiveMap.insert(policy, messageBarList);
    }
  }
  else
  {
    if (_messageBarActiveMap.contains(policy))
    {
      if (_messageBarActiveMap[policy]->count() > 0) { _messageBarActiveMap[policy]->removeOne(messageBar); }
    }
  }
}

NXMessageBarPrivate::NXMessageBarPrivate(QObject *parent)
    : QObject { parent }
{
  setProperty("MessageBarClosedY", 0);
  setProperty("MessageBarFinishY", 0);
  _pTimePercent = 100;
  _createTime   = QDateTime::currentMSecsSinceEpoch();
}

NXMessageBarPrivate::~NXMessageBarPrivate() { }

void NXMessageBarPrivate::tryToRequestMessageBarEvent()
{
  Q_Q(NXMessageBar);
  if (!_isMessageBarCreateAnimationFinished || _isMessageBarEventAnimationStart) { return; }
  NXMessageBarManager::getInstance()->requestMessageBarEvent(q);
}

WorkStatus NXMessageBarPrivate::getWorkMode() const
{
  if (!_isMessageBarCreateAnimationFinished) { return WorkStatus::CreateAnimation; }
  if (_isMessageBarEventAnimationStart) { return WorkStatus::OtherEventAnimation; }
  return WorkStatus::Idle;
}

void NXMessageBarPrivate::onOtherMessageBarEnd(QVariantMap eventData)
{
  Q_Q(NXMessageBar);
  _isMessageBarEventAnimationStart      = true;
  qreal targetPosY                      = eventData.value("TargetPosY").toReal();
  QPropertyAnimation *closePosAnimation = new QPropertyAnimation(this, "MessageBarClosedY");
  connect(closePosAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
    q->move(q->pos().x(), value.toUInt());
  });
  connect(closePosAnimation,
          &QPropertyAnimation::finished,
          this,
          [=]()
  {
    _isMessageBarEventAnimationStart = false;
    if (NXMessageBarManager::getInstance()->getMessageBarEventCount(q) > 1)
    {
      NXMessageBarManager::getInstance()->requestMessageBarEvent(q);
    }
    if (_isReadyToEnd) { NXMessageBarManager::getInstance()->requestMessageBarEvent(q); }
  });
  closePosAnimation->setEasingCurve(QEasingCurve::OutCubic);
  closePosAnimation->setDuration(220);
  closePosAnimation->setStartValue(q->pos().y());
  closePosAnimation->setEndValue(targetPosY);
  closePosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NXMessageBarPrivate::messageBarEnd(QVariantMap eventData)
{
  Q_Q(NXMessageBar);
  NXMessageBarManager::getInstance()->postMessageBarEndEvent(q);
  QPropertyAnimation *barFinishedOpacityAnimation = new QPropertyAnimation(this, "pOpacity");
  connect(barFinishedOpacityAnimation,
          &QPropertyAnimation::valueChanged,
          this,
          [=]()
  {
    _closeButton->setOpacity(_pOpacity);
    q->update();
  });
  connect(barFinishedOpacityAnimation, &QPropertyAnimation::finished, this, [=]() { q->deleteLater(); });
  barFinishedOpacityAnimation->setDuration(300);
  barFinishedOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
  barFinishedOpacityAnimation->setStartValue(1);
  barFinishedOpacityAnimation->setEndValue(0);
  barFinishedOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NXMessageBarPrivate::onCloseButtonClicked()
{
  Q_Q(NXMessageBar);
  if (_isReadyToEnd) { return; }
  _isReadyToEnd    = true;
  _isNormalDisplay = false;
  NXMessageBarManager::getInstance()->forcePostMessageBarEndEvent(q);
  QPropertyAnimation *opacityAnimation = new QPropertyAnimation(this, "pOpacity");
  connect(opacityAnimation,
          &QPropertyAnimation::valueChanged,
          this,
          [=]()
  {
    _closeButton->setOpacity(_pOpacity);
    q->update();
  });
  connect(opacityAnimation, &QPropertyAnimation::finished, q, [=]() { q->deleteLater(); });
  opacityAnimation->setStartValue(_pOpacity);
  opacityAnimation->setEndValue(0);
  opacityAnimation->setDuration(220);
  opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
  opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NXMessageBarPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode)
{
  Q_Q(NXMessageBar);
  _themeMode = themeMode;
  q->update();
}

void NXMessageBarPrivate::_messageBarCreate(int displayMsec)
{
  Q_Q(NXMessageBar);
  q->show();
  QFont font = q->font();
  font.setPixelSize(16);
  font.setWeight(QFont::Bold);
  q->setFont(font);
  int titleWidth = q->fontMetrics().horizontalAdvance(_title);
  font.setPixelSize(14);
  font.setWeight(QFont::Medium);
  q->setFont(font);
  int textWidth  = q->fontMetrics().horizontalAdvance(_text);
  int fixedWidth = _closeButtonLeftRightMargin + _leftPadding + _titleLeftSpacing + _textLeftSpacing +
                   _closeButtonWidth + titleWidth + textWidth + 2 * _shadowBorderWidth;
  q->setFixedWidth(fixedWidth > 500 ? 500 : fixedWidth);
  NXMessageBarManager::getInstance()->postMessageBarCreateEvent(q);
  int startX = 0;
  int startY = 0;
  int endX   = 0;
  int endY   = 0;
  _calculateInitialPos(startX, startY, endX, endY);
  // 滑入动画
  QPropertyAnimation *barPosAnimation = new QPropertyAnimation(q, "pos");
  connect(barPosAnimation,
          &QPropertyAnimation::finished,
          q,
          [=]()
  {
    _isNormalDisplay                     = true;
    _isMessageBarCreateAnimationFinished = true;
    if (NXMessageBarManager::getInstance()->getMessageBarEventCount(q) > 1)
    {
      NXMessageBarManager::getInstance()->requestMessageBarEvent(q);
    }
    QTimer::singleShot(displayMsec,
                       q,
                       [=]()
    {
      _isReadyToEnd = true;
      NXMessageBarManager::getInstance()->requestMessageBarEvent(q);
    });
    QPropertyAnimation *timePercentAnimation = new QPropertyAnimation(this, "pTimePercent");
    connect(timePercentAnimation, &QPropertyAnimation::valueChanged, this, [=]() { q->update(); });
    timePercentAnimation->setStartValue(100);
    timePercentAnimation->setEndValue(0);
    timePercentAnimation->setEasingCurve(QEasingCurve::Linear);
    timePercentAnimation->setDuration(displayMsec);
    timePercentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  });
  switch (_policy)
  {
    case NXMessageBarType::Top :
    case NXMessageBarType::Bottom :
    {
      barPosAnimation->setDuration(250);
      break;
    }
    default :
    {
      barPosAnimation->setDuration(450);
      break;
    }
  }
  barPosAnimation->setStartValue(QPoint(startX, startY));
  barPosAnimation->setEndValue(QPoint(endX, endY));
  barPosAnimation->setEasingCurve(QEasingCurve::InOutSine);
  barPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NXMessageBarPrivate::_calculateInitialPos(int& startX, int& startY, int& endX, int& endY)
{
  Q_Q(NXMessageBar);
  QList<int> resultList  = _getOtherMessageBarTotalData();
  int minimumHeightTotal = resultList[0];
  int indexLessCount     = resultList[1];
  int lastEndY           = endY;
  switch (_policy)
  {
    case NXMessageBarType::Top :
    {
      // 25动画距离
      startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
      startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin - 25;
      endX   = startX;
      endY   = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
      break;
    }
    case NXMessageBarType::Left :
    {
      startX = -q->minimumWidth();
      startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
      endX   = _messageBarHorizontalMargin;
      endY   = startY;
      break;
    }
    case NXMessageBarType::Bottom :
    {
      startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
      startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
               _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin - 25;
      endX = startX;
      endY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
             _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
      break;
    }
    case NXMessageBarType::Right :
    {
      startX = q->parentWidget()->width();
      startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
      endX   = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
      endY   = startY;
      break;
    }
    case NXMessageBarType::TopRight :
    {
      startX = q->parentWidget()->width();
      startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
      endX   = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
      endY   = startY;
      break;
    }
    case NXMessageBarType::TopLeft :
    {
      startX = -q->minimumWidth();
      startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
      endX   = _messageBarHorizontalMargin;
      endY   = startY;
      break;
    }
    case NXMessageBarType::BottomRight :
    {
      startX = q->parentWidget()->width();
      startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
               _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
      endX = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
      endY = startY;
      break;
    }
    case NXMessageBarType::BottomLeft :
    {
      startX = -q->minimumWidth();
      startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
               _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
      endX = _messageBarHorizontalMargin;
      endY = startY;
      break;
    }
  }
  if (endY == lastEndY) { return; }
  if (endY < _messageBarVerticalTopMargin ||
      endY > q->parentWidget()->height() - _messageBarVerticalBottomMargin - q->minimumHeight())
  {
    if (_messageBarActiveMap[_policy] && !_messageBarActiveMap[_policy]->isEmpty())
    {
      auto firstMessageBar = (*_messageBarActiveMap[_policy])[0];
      if (firstMessageBar) { firstMessageBar->d_ptr->onCloseButtonClicked(); }
    }
    _calculateInitialPos(startX, startY, endX, endY);
  }
}

QList<int> NXMessageBarPrivate::_getOtherMessageBarTotalData(bool isJudgeCreateOrder)
{
  Q_Q(NXMessageBar);
  QList<int> resultList;
  int minimumHeightTotal                        = 0;
  int indexLessCount                            = 0;
  QList<QPointer<NXMessageBar>> *messageBarList = _messageBarActiveMap[_policy];
  for (auto messageBar : *messageBarList)
  {
    if (!messageBar || messageBar == q) { continue; }
    if (!isJudgeCreateOrder || (isJudgeCreateOrder && _judgeCreateOrder(messageBar.data())))
    {
      indexLessCount++;
      minimumHeightTotal += messageBar->minimumHeight();
    }
  }
  resultList.append(minimumHeightTotal);
  resultList.append(indexLessCount);
  return resultList;
}

qreal NXMessageBarPrivate::_calculateTargetPosY()
{
  Q_Q(NXMessageBar);
  QList<int> resultList  = _getOtherMessageBarTotalData(true);
  int minimumHeightTotal = resultList[0];
  int indexLessCount     = resultList[1];
  switch (_policy)
  {
    case NXMessageBarType::Top :
    case NXMessageBarType::TopRight :
    case NXMessageBarType::TopLeft :
    {
      return minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
    }
    case NXMessageBarType::Left :
    case NXMessageBarType::Right :
    {
      return minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
    }
    case NXMessageBarType::Bottom :
    case NXMessageBarType::BottomRight :
    case NXMessageBarType::BottomLeft :
    {
      return q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
             _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
    }
  }
  return 0;
}

bool NXMessageBarPrivate::_judgeCreateOrder(NXMessageBar *otherMessageBar)
{
  if (otherMessageBar->d_ptr->_createTime < _createTime)
  {
    // otherMessageBar先创建
    return true;
  }
  else
  {
    return false;
  }
}

void NXMessageBarPrivate::_drawSuccess(QPainter *painter)
{
  Q_Q(NXMessageBar);
  painter->save();
  painter->setBrush(_themeMode == NXThemeType::Light ? QColor(0xE0, 0xF6, 0xDD) : QColor(0x39, 0x4D, 0x37));
  QRect foregroundRect(_shadowBorderWidth,
                       _shadowBorderWidth,
                       q->width() - 2 * _shadowBorderWidth,
                       q->height() - 2 * _shadowBorderWidth);
  painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
  // 图标绘制
  painter->setPen(Qt::white);
  QPainterPath textPath;
  textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
  painter->setClipPath(textPath);
  painter->fillPath(textPath, _themeMode == NXThemeType::Light ? QColor(0x11, 0x77, 0x10) : QColor(0x4C, 0xAF, 0x50));
  QFont iconFont = QFont("NXAwesome");
  iconFont.setPixelSize(12);
  painter->setFont(iconFont);
  painter->drawText(
      _leftPadding, 0, q->width(), q->height(), Qt::AlignVCenter, QChar((unsigned short) NXIconType::Check));
  // 时间进度条绘制
  QPainterPath clipPath;
  clipPath.addRoundedRect(foregroundRect, _borderRadius, _borderRadius);
  painter->setClipPath(clipPath);
  painter->setPen(Qt::NoPen);
  painter->setBrush(QColor(0x3C, 0x96, 0x4B));
  painter->drawRoundedRect(QRectF(foregroundRect.x(),
                                  foregroundRect.bottom() - _timePercentHeight,
                                  foregroundRect.width() * _pTimePercent / 100.0,
                                  _timePercentHeight + 1),
                           2,
                           2);
  painter->restore();
  // 文字颜色
  painter->setPen(_themeMode == NXThemeType::Light ? QPen(Qt::black) : QPen(QColor(0xE0, 0xF6, 0xDD)));
}

void NXMessageBarPrivate::_drawWarning(QPainter *painter)
{
  Q_Q(NXMessageBar);
  painter->save();
  painter->setBrush(_themeMode == NXThemeType::Light ? QColor(0x6B, 0x56, 0x27) : QColor(0x5A, 0x4A, 0x1F));
  QRect foregroundRect(_shadowBorderWidth,
                       _shadowBorderWidth,
                       q->width() - 2 * _shadowBorderWidth,
                       q->height() - 2 * _shadowBorderWidth);
  painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
  // 图标绘制
  // exclamation
  painter->setPen(Qt::black);
  QPainterPath textPath;
  textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
  painter->setClipPath(textPath);
  painter->fillPath(textPath, QColor(0xF8, 0xE2, 0x23));
  painter->drawText(_leftPadding + 4, 0, q->width(), q->height(), Qt::AlignVCenter, "!");
  // 时间进度条绘制
  QPainterPath clipPath;
  clipPath.addRoundedRect(foregroundRect, _borderRadius, _borderRadius);
  painter->setClipPath(clipPath);
  painter->setPen(Qt::NoPen);
  painter->fillPath(textPath, _themeMode == NXThemeType::Light ? QColor(0xF8, 0xE2, 0x23) : QColor(0xFF, 0xEB, 0x3B));
  painter->drawRoundedRect(QRectF(foregroundRect.x(),
                                  foregroundRect.bottom() - _timePercentHeight,
                                  foregroundRect.width() * _pTimePercent / 100.0,
                                  _timePercentHeight + 1),
                           2,
                           2);
  painter->restore();
  // 文字颜色
  painter->setPen(_themeMode == NXThemeType::Light ? QColor(0xFA, 0xFA, 0xFA) : QColor(0xFF, 0xF3, 0xCD));
}

void NXMessageBarPrivate::_drawInformation(QPainter *painter)
{
  Q_Q(NXMessageBar);
  painter->save();
  painter->setBrush(_themeMode == NXThemeType::Light ? QColor(0xF4, 0xF4, 0xF4) : QColor(0x37, 0x47, 0x4F));
  QRect foregroundRect(_shadowBorderWidth,
                       _shadowBorderWidth,
                       q->width() - 2 * _shadowBorderWidth,
                       q->height() - 2 * _shadowBorderWidth);
  painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
  // 图标绘制
  painter->setPen(Qt::white);
  QPainterPath textPath;
  textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
  painter->setClipPath(textPath);
  painter->fillPath(textPath, _themeMode == NXThemeType::Light ? QColor(0x00, 0x66, 0xB4) : QColor(0x42, 0xA5, 0xF5));
  painter->drawText(_leftPadding + 4, 0, q->width(), q->height(), Qt::AlignVCenter, "i");
  // 时间进度条绘制
  QPainterPath clipPath;
  clipPath.addRoundedRect(foregroundRect, _borderRadius, _borderRadius);
  painter->setClipPath(clipPath);
  painter->setPen(Qt::NoPen);
  painter->setBrush(QColor(0x00, 0x66, 0xB4));
  painter->drawRoundedRect(QRectF(foregroundRect.x(),
                                  foregroundRect.bottom() - _timePercentHeight,
                                  foregroundRect.width() * _pTimePercent / 100.0,
                                  _timePercentHeight + 1),
                           2,
                           2);
  painter->restore();
  // 文字颜色
  painter->setPen(_themeMode == NXThemeType::Light ? Qt::black : QColor(0xE1, 0xF5, 0xFE));
}

void NXMessageBarPrivate::_drawError(QPainter *painter)
{
  Q_Q(NXMessageBar);
  painter->save();
  painter->setBrush(_themeMode == NXThemeType::Light ? QColor(0xFE, 0xE7, 0xEA) : QColor(0x4E, 0x34, 0x2E));
  QRect foregroundRect(_shadowBorderWidth,
                       _shadowBorderWidth,
                       q->width() - 2 * _shadowBorderWidth,
                       q->height() - 2 * _shadowBorderWidth);
  painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
  // 图标绘制
  painter->setPen(Qt::white);
  QPainterPath textPath;
  textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
  painter->setClipPath(textPath);
  painter->fillPath(textPath, _themeMode == NXThemeType::Light ? QColor(0xBA, 0x2D, 0x20) : QColor(0xEF, 0x53, 0x50));
  QFont iconFont = QFont("NXAwesome");
  iconFont.setPixelSize(13);
  painter->setFont(iconFont);
  painter->drawText(
      _leftPadding + 1, 0, q->width(), q->height(), Qt::AlignVCenter, QChar((unsigned short) NXIconType::Xmark));
  // 时间进度条绘制
  QPainterPath clipPath;
  clipPath.addRoundedRect(foregroundRect, _borderRadius, _borderRadius);
  painter->setClipPath(clipPath);
  painter->setPen(Qt::NoPen);
  painter->setBrush(QColor(0xCC, 0x5C, 0x65));
  painter->drawRoundedRect(QRectF(foregroundRect.x(),
                                  foregroundRect.bottom() - _timePercentHeight,
                                  foregroundRect.width() * _pTimePercent / 100.0,
                                  _timePercentHeight + 1),
                           2,
                           2);
  painter->restore();
  // 文字颜色
  painter->setPen(_themeMode == NXThemeType::Light ? Qt::black : QColor(0xFF, 0xCD, 0xD2));
}
