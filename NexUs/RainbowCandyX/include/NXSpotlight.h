#ifndef NXSPOTLIGHT_H
#define NXSPOTLIGHT_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXSpotlightPrivate;
class NX_EXPORT NXSpotlight : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXSpotlight)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, Padding)
    Q_PROPERTY_CREATE_H(int, OverlayAlpha)
    Q_PROPERTY_CREATE_H(bool, IsCircle)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)
    Q_PROPERTY_CREATE_2_H(const QString&, QString, Content)
public:
    explicit NXSpotlight(QWidget* parent = nullptr);
    ~NXSpotlight() override;

    void showSpotlight(QWidget* target, const QString& buttonText = QStringLiteral("知道了"));

    struct SpotlightStep {
        QWidget* target{nullptr};
        QString title;
        QString content;
        bool isCircle{false};
    };

    void setSteps(const QList<SpotlightStep>& steps) noexcept;
    void start() noexcept;
    void next() noexcept;
    void previous() noexcept;
    void finish() noexcept;

    int currentStep() const noexcept;
    int stepCount() const noexcept;

Q_SIGNALS:
  void stepChanged(int step);
  void finished();

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // NXSPOTLIGHT_H
