#ifndef NXTEXTPRIVATE_H
#define NXTEXTPRIVATE_H

#include <QColor>
#include <QObject>
#include <memory>
#include "NXDef.h"

class QPainter;
class QTextDocument;
class QRectF;
class QString;
class NXText;

class NXTextPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXText)
  Q_PROPERTY_CREATE_D(bool, IsAllowClick)
  Q_PROPERTY_CREATE_D(bool, IsWrapAnywhere)
  Q_PROPERTY_CREATE_D(NXTextType::TextStyle, TextStyle)
  Q_PROPERTY_CREATE_D(NXIconType::IconName, NXIcon)
  Q_PROPERTY_CREATE_D(NXTextType::DisplayMode, DisplayMode)

public:
  explicit NXTextPrivate(QObject *parent = nullptr);
  ~NXTextPrivate() override;
  Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);
  bool drawByDisplayMode(QPainter& painter) const;

private:
  qreal _textSpacing { 0.5 };
  NXThemeType::ThemeMode _themeMode;

  std::unique_ptr<QTextDocument>
  _createDocument(const QString& htmlText, qreal maxWidth, Qt::Alignment alignment) const;
  void _drawNXIcon(QPainter& painter, const QRectF& targetRect) const;
  void _drawTextCentered(QPainter& painter, const QRectF& targetRect, const QString& htmlText) const;
  void _drawTextDocumentHorizontal(QPainter& painter,
                                   const QRectF& targetRect,
                                   const QString& htmlText,
                                   Qt::Alignment alignment) const;
};

#endif // NXTEXTPRIVATE_H
