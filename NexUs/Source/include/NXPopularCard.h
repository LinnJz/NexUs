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
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QPixmap), CardPixmap)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QPixmap), CardFloatPixmap)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Title)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), SubTitle)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), InteractiveTips)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), DetailedText)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), CardButtonText)
  Q_PROPERTY_CREATE_H(QWidget *, CardFloatArea)
  Q_PROPERTY_CREATE_H(int, BorderRadius)

public:
  explicit NXPopularCard(QWidget *parent = nullptr);
  ~NXPopularCard();
  Q_SIGNAL void popularCardClicked();
  Q_SIGNAL void popularCardButtonClicked();

protected:
  bool event(QEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXPOPULARCARD_H
