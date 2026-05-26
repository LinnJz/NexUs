#ifndef NXCHATBUBBLE_H
#define NXCHATBUBBLE_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXChatBubblePrivate;

class NX_EXPORT NXChatBubble : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXChatBubble)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), MessageText)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), SenderName)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Timestamp)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(int, AvatarSize)
  Q_PROPERTY_CREATE_H(int, MaxBubbleWidth)

public:
  enum BubbleDirection
  {
    Left = 0,
    Right
  };
  Q_ENUM(BubbleDirection)

  enum MessageStatus
  {
    None = 0,
    Sending,
    Sent,
    Read,
    Failed
  };
  Q_ENUM(MessageStatus)

  explicit NXChatBubble(QWidget *parent = nullptr);
  ~NXChatBubble();

  void setDirection(BubbleDirection direction);
  BubbleDirection getDirection() const;

  void setStatus(MessageStatus status);
  MessageStatus getStatus() const;

  void setAvatar(const QPixmap &avatar);
  QPixmap getAvatar() const;

  void setBubbleColor(const QColor &color);
  QColor getBubbleColor() const;

  void setMessageImage(const QPixmap &image);
  QPixmap getMessageImage() const;

  void setImageMaxWidth(int width);
  int getImageMaxWidth() const;

  QSize sizeHint() const override;
  Q_SIGNAL void imageDoubleClicked(const QPixmap &image);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // NXCHATBUBBLE_H
