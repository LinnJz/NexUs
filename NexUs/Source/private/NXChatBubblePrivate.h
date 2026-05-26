#ifndef NXCHATBUBBLEPRIVATE_H
#define NXCHATBUBBLEPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QRect>

#include "NXDef.h"
#include "NXProperty.h"

class NXChatBubble;

class NXChatBubblePrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXChatBubble)
  Q_PROPERTY_CREATE_D(QString, MessageText)
  Q_PROPERTY_CREATE_D(QString, SenderName)
  Q_PROPERTY_CREATE_D(QString, Timestamp)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, AvatarSize)
  Q_PROPERTY_CREATE_D(int, MaxBubbleWidth)

public:
  explicit NXChatBubblePrivate(QObject *parent = nullptr);
  ~NXChatBubblePrivate();

private:
  NXThemeType::ThemeMode _themeMode;
  NXChatBubble::BubbleDirection _direction { NXChatBubble::Left };
  NXChatBubble::MessageStatus _status { NXChatBubble::None };
  int _imageMaxWidth { 280 };
  QRect _imageRect;
  QColor _bubbleColor;
  QPixmap _avatar;
  QPixmap _messageImage;
  QPixmap _scaledImage;
};

#endif // NXCHATBUBBLEPRIVATE_H
