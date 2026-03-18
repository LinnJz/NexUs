#ifndef NXCOUNTDOWN_H
#define NXCOUNTDOWN_H

#include <QWidget>

#include "NXDef.h"
#include "NXProperty.h"

class NXCountdownPrivate;
class NX_EXPORT NXCountdown : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXCountdown)
    Q_PROPERTY_CREATE_H(bool, IsShowDays)
    Q_PROPERTY_CREATE_H(bool, IsShowHours)
    Q_PROPERTY_CREATE_H(bool, IsShowMinutes)
    Q_PROPERTY_CREATE_H(bool, IsShowSeconds)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, DigitWidth)
    Q_PROPERTY_CREATE_H(int, DigitHeight)
    Q_PROPERTY_CREATE_H(int, DigitSpacing)
    Q_PROPERTY_CREATE_H(int, FontPixelSize)
public:
    explicit NXCountdown(QWidget* parent = nullptr);
    ~NXCountdown() override;

    void setTargetDateTime(const QDateTime& dateTime) noexcept;
    QDateTime getTargetDateTime() const noexcept;

    void setRemainingSeconds(qint64 seconds) noexcept;
    qint64 getRemainingSeconds() const noexcept;

    void start() noexcept;
    void pause() noexcept;
    void resume() noexcept;
    void stop() noexcept;

    bool isRunning() const noexcept;

Q_SIGNALS:
  void timeout();
  void tick(qint64 remainingSeconds);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
    void _updateSize();
};

#endif // NXCOUNTDOWN_H
