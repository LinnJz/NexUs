#ifndef NXEXPANDERPRIVATE_H
#define NXEXPANDERPRIVATE_H

#include <QObject>

#include "NXDef.h"
#include "NXExpander.h"
class QVariantAnimation;
class QVBoxLayout;
class NXExpanderPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXExpander)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle)
    Q_PROPERTY_CREATE_D(NXIconType::IconName, HeaderIcon)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
public:
    explicit NXExpanderPrivate(QObject* parent = nullptr);
    ~NXExpanderPrivate() override;

private:
    bool _isExpanded{false};
    bool _isPressed{false};
    NXExpander::ExpandDirection _expandDirection{NXExpander::Down};
    QWidget* _contentWidget{nullptr};
    QWidget* _headerCustomWidget{nullptr};
    QWidget* _contentContainer{nullptr};
    QVBoxLayout* _contentLayout{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QWidget* _headerWidget{nullptr};
    QVariantAnimation* _expandAnimation{nullptr};
    int _headerHeight{48};
    int _contentHeight{0};
    NXThemeType::ThemeMode _themeMode;
    void _toggleExpand();
    void _startExpandAnimation(bool expand);
};

#endif // NXEXPANDERPRIVATE_H
