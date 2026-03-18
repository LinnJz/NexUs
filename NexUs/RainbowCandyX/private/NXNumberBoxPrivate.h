#ifndef NXNUMBERBOXPRIVATE_H
#define NXNUMBERBOXPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXNumberBox;
class QLineEdit;
class NXNumberBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXNumberBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(double, Value)
    Q_PROPERTY_CREATE_D(double, Minimum)
    Q_PROPERTY_CREATE_D(double, Maximum)
    Q_PROPERTY_CREATE_D(double, Step)
    Q_PROPERTY_CREATE_D(int, Decimals)
    Q_PROPERTY_CREATE_D(bool, IsWrapping)
public:
    explicit NXNumberBoxPrivate(QObject* parent = nullptr);
    ~NXNumberBoxPrivate() override;

private:
    static const int _buttonWidth = 32;
    bool _isMinusHovered{false};
    bool _isPlusHovered{false};
    bool _isMinusPressed{false};
    bool _isPlusPressed{false};
    bool _isEditing{false};
    QLineEdit* _lineEdit{nullptr};
    NXThemeType::ThemeMode _themeMode;
    QRect _minusRect() const;
    QRect _plusRect() const;
    QRect _valueRect() const;
    QString _formatValue() const;
    void _clampValue();
    void _startEditing();
    void _finishEditing();
};

#endif // NXNUMBERBOXPRIVATE_H
