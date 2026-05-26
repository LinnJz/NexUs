#ifndef NXSTATCARD_H
#define NXSTATCARD_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXStatCardPrivate;

class NX_EXPORT NXStatCard : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXStatCard)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Title)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Value)
  Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Description)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_H(NXIconType::IconName, CardIcon)

public:
  enum TrendType
  {
    None = 0,
    Up,
    Down,
    Neutral
  };
  Q_ENUM(TrendType)

  explicit NXStatCard(QWidget *parent = nullptr);
  ~NXStatCard();

  void setTrend(TrendType trend);
  TrendType getTrend() const;

  void setTrendText(const QString &text);
  QString getTrendText() const;

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXSTATCARD_H
