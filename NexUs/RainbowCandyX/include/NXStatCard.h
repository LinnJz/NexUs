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
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(NXIconType::IconName, CardIcon)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Value)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Description)

  public:
    enum TrendType
    {
        None = 0,
        Up,
        Down,
        Neutral
    };
    Q_ENUM(TrendType)

    explicit NXStatCard(QWidget* parent = nullptr);
    ~NXStatCard() override;

    void setTrend(TrendType trend) noexcept;
    TrendType getTrend() const noexcept;

    void setTrendText(const QString& text) noexcept;
    QString getTrendText() const noexcept;

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // NXSTATCARD_H
