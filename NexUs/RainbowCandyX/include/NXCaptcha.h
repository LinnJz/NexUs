#ifndef NXCAPTCHA_H
#define NXCAPTCHA_H

#include <QWidget>

#include "NXDef.h"

class NXCaptchaPrivate;
class NX_EXPORT NXCaptcha : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXCaptcha)
    Q_PROPERTY_CREATE_H(int, CodeLength)
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, BoxSize)
    Q_PROPERTY_CREATE_H(int, BoxSpacing)
public:
    enum InputMode
    {
        DigitOnly = 0,
        AlphaNumeric
    };
    Q_ENUM(InputMode)

    explicit NXCaptcha(QWidget* parent = nullptr);
    ~NXCaptcha() override;

    void setInputMode(InputMode mode) noexcept;
    InputMode getInputMode() const noexcept;

    QString getCode() const noexcept;
    void clear() noexcept;

Q_SIGNALS:
    void codeCompleted(const QString& code);
    void codeChanged(const QString& code);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // NXCAPTCHA_H
