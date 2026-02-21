#ifndef NXPOPULARCARD_H
#define NXPOPULARCARD_H

#include <QPixmap>
#include <QWidget>

#include "NXProperty.h"
class NXPopularCardPrivate;

class NX_EXPORT NXPopularCard : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXPopularCard)
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap)
  Q_PROPERTY_CREATE_Q_H(QPixmap, CardFloatPixmap)
  Q_PROPERTY_CREATE_Q_H(QString, Title)
  Q_PROPERTY_CREATE_Q_H(QString, SubTitle)
  Q_PROPERTY_CREATE_Q_H(QString, InteractiveTips)
  Q_PROPERTY_CREATE_Q_H(QString, DetailedText)
  Q_PROPERTY_CREATE_Q_H(QString, CardButtonText)
  Q_PROPERTY_CREATE_Q_H(QWidget *, CardFloatArea)

public:
  explicit NXPopularCard(QWidget *parent = nullptr);
  ~NXPopularCard() override;
Q_SIGNALS:
  Q_SIGNAL void popularCardClicked();
  Q_SIGNAL void popularCardButtonClicked();

protected:
  virtual bool event(QEvent *event) override;
  virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NXPOPULARCARD_H
