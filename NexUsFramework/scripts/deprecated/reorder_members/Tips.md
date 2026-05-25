**需求：**

使用py脚本写一个算法，CLI输入文件路径，扫描路径（需要递归子文件夹）所有C++/Qt的.h，.hpp，.hxx等与header相关的头文件，定位到成员变量，执行成员顺序编排使得类的大小减少。

**提示：**
`Q_PROPERTY_CREATE`

`Q_PROPERTY_REF_CREATE`

`Q_PROPERTY_CREATE_D`

`Q_PRIVATE_CREATE_D`

`Q_PRIVATE_CREATE`

`Q_PRIVATE_REF_CREATE`

等是一个宏，它们展开均会定义一个成员变量
例如

```
Q_PRIVATE_CREATE_D定义为
#define Q_PRIVATE_CREATE_D(TYPE, M) \
private:                            \
    TYPE _p##M;

使用
Q_PRIVATE_CREATE_D(int, X)
```

展开

```
private:
int _pX;
```

所以你可以等价于只要是上述宏定义的代码均代表一个类型的成员变量。

**示例**

1、NXAcrylicUrlCardPrivate 以8字节对齐那么当前的大小考虑到虚表，是sizeof(NXAcrylicUrlCardPrivate) 224

```
class ElaAcrylicUrlCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaAcrylicUrlCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(qreal, MainOpacity)
    Q_PROPERTY_CREATE_D(qreal, NoiseOpacity)
    Q_PROPERTY_CREATE_D(int, BrushAlpha)
    Q_PROPERTY_CREATE_D(QString, Title);
    Q_PROPERTY_CREATE_D(QString, SubTitle);
    Q_PROPERTY_CREATE_D(int, TitlePixelSize);
    Q_PROPERTY_CREATE_D(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_D(int, TitleSpacing);
    Q_PROPERTY_CREATE_D(int, SubTitleSpacing);
    Q_PROPERTY_CREATE_D(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_D(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_D(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_D(ElaCardPixType::PixMode, CardPixMode);
    Q_PROPERTY_CREATE_D(QString, Url);

public:
    explicit ElaAcrylicUrlCardPrivate(QObject* parent = nullptr);
    ~ElaAcrylicUrlCardPrivate();

private:
    QPixmap _noisePix;
    ElaThemeType::ThemeMode _themeMode;
    qreal _borderWidth{1.5};
};
```

执行成员变量编排，此时sizeof(NXAcrylicUrlCardPrivate) 是216大小，注意我们需要删除宏定义额外多余的;

```

class ElaAcrylicUrlCardPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(ElaAcrylicUrlCard)
  Q_PROPERTY_CREATE_D(qreal, MainOpacity)
  Q_PROPERTY_CREATE_D(qreal, NoiseOpacity)
  Q_PROPERTY_CREATE_D(QString, Title)
  Q_PROPERTY_CREATE_D(QString, SubTitle)
  Q_PROPERTY_CREATE_D(QString, Url)
  Q_PROPERTY_CREATE_D(QPixmap, CardPixmap)
  Q_PROPERTY_CREATE_D(QSize, CardPixmapSize)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(int, BrushAlpha)
  Q_PROPERTY_CREATE_D(int, TitlePixelSize)
  Q_PROPERTY_CREATE_D(int, SubTitlePixelSize)
  Q_PROPERTY_CREATE_D(int, TitleSpacing)
  Q_PROPERTY_CREATE_D(int, SubTitleSpacing)
  Q_PROPERTY_CREATE_D(int, CardPixmapBorderRadius)
  Q_PROPERTY_CREATE_D(ElaCardPixType::PixMode, CardPixMode)

public:
  explicit ElaAcrylicUrlCardPrivate(QObject *parent = nullptr);
  ~ElaAcrylicUrlCardPrivate();

private:
  ElaThemeType::ThemeMode _themeMode;
  qreal _borderWidth { 1.5 };
  QPixmap _noisePix;
};
```

换句话说我们需要处理小于8字节的类型，如bool、int、char、float、枚举、quint32等类型的编排，核心在于宏声明最后总是小于8字节的类型，非宏声明之前是小于8字节的类型，它们需要“紧挨着”，相同类型尽量紧挨着。如果有必要可以将显示非宏的变量与宏变量声明并排，例如

```
class ELAIconButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(ELAIconButton)
  Q_PROPERTY_CREATE_D(qreal, Opacity)
  Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
  Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
  Q_PROPERTY_CREATE_D(QColor, LightIconColor)
  Q_PROPERTY_CREATE_D(QColor, DarkIconColor)
  Q_PROPERTY_CREATE_D(QColor, LightHoverIconColor)
  Q_PROPERTY_CREATE_D(QColor, DarkHoverIconColor)
  bool _isAlphaAnimationFinished { true }; // 我们将这个变量和宏定义声明并排才能使得大小减小
  Q_PROPERTY_CREATE_D(bool, IsSelected)
  Q_PROPERTY_CREATE_D(ELAIconType::IconName, Awesome)
  Q_PROPERTY_CREATE(int, HoverAlpha)
  Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
  explicit ELAIconButtonPrivate(QObject *parent = nullptr);
  ~ELAIconButtonPrivate();

private:
  ELAThemeType::ThemeMode _themeMode;
  QPixmap _iconPix;
};
```

可以看到

```
  bool _isAlphaAnimationFinished { true }; // 我们将这个变量和宏定义声明并排
  Q_PROPERTY_CREATE_D(bool, IsSelected)
  Q_PROPERTY_CREATE_D(ELAIconType::IconName, Awesome)
  Q_PROPERTY_CREATE(int, HoverAlpha)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
...
  ELAThemeType::ThemeMode _themeMode;
```

宏定义小于8字节在最后，非宏定义小于8字节在最前。
示例：

```
class ELAWindowPrivate : public QObject
{
  friend class ELAWindowStackChangeCommand;
  Q_OBJECT
  Q_D_CREATE(ELAWindow)
  Q_PROPERTY_CREATE_D(int, ThemeChangeTime)
  Q_PROPERTY_CREATE_D(ELAWindowType::StackSwitchMode, StackSwitchMode)
  Q_PROPERTY_CREATE_D(ELANavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
  Q_PROPERTY_CREATE_D(ELAWindowType::PaintMode, WindowPaintMode)
...
private:
  bool _isNavigationBarFloat { false };
  bool _isNavigationDisplayModeChanged { false };
  bool _isNavigationEnable { true };
  bool _isNavigationBarExpanded { false };
  bool _isInitFinished { false };
  int _contentsMargins { 5 };
  int _navigationTargetIndex { 0 };
  ELANavigationType::NavigationDisplayMode _currentNavigationBarDisplayMode { ELANavigationType::Maximal };
  ELAThemeType::ThemeMode _themeMode;
  ELAApplicationType::WindowDisplayMode _windowDisplayMode;
  QString _lightWindowMoviePath {};
  QString _darkWindowMoviePath {};
  QPixmap *_lightWindowPix;
  QPixmap *_darkWindowPix;
  QMovie *_windowPaintMovie { nullptr };
  ELAEvent *_focusEvent { nullptr };
  ELANavigationBar *_navigationBar { nullptr };
  ELACentralStackedWidget *_centerStackedWidget { nullptr };
  ELACentralStackedWidget *_navigationCenterStackedWidget { nullptr };
  ELAAppBar *_appBar { nullptr };
  QHBoxLayout *_centerLayout { nullptr };
  ELAThemeAnimationWidget *_animationWidget { nullptr };

  QMap<QString, const QMetaObject *> _pageMetaMap;
  QMap<QString, QWidget *> _routeMap;} // 相同类型尽量紧挨着
```

但是不意味着这样写

```
class ElaCountdownPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCountdown)
    Q_PROPERTY_CREATE(qreal, FlipAngle)
    bool _isRunning{false};
    bool _useTargetMode{false};
    Q_PROPERTY_CREATE_D(bool, IsShowDays)
    Q_PROPERTY_CREATE_D(bool, IsShowHours)
    Q_PROPERTY_CREATE_D(bool, IsShowMinutes)
    Q_PROPERTY_CREATE_D(bool, IsShowSeconds)
    int _prevSeconds{-1};
    int _prevMinutes{-1};
    int _prevHours{-1};
    int _prevDays{-1};
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, DigitWidth)
    Q_PROPERTY_CREATE_D(int, DigitHeight)
    Q_PROPERTY_CREATE_D(int, DigitSpacing)
    Q_PROPERTY_CREATE_D(int, FontPixelSize)
public:
    explicit ElaCountdownPrivate(QObject* parent = nullptr);
    ~ElaCountdownPrivate() override;

    Q_SLOT void onTimerTick();
    void _startFlipAnimation();

private:
    ElaThemeType::ThemeMode _themeMode;
    QTimer* _timer{nullptr};
    QDateTime _targetDateTime;
    qint64 _remainingSeconds{0};


    friend class ElaCountdown;
};
```

我们的期望是

```
class ElaCountdownPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCountdown)
    Q_PROPERTY_CREATE(qreal, FlipAngle)
    Q_PROPERTY_CREATE_D(bool, IsShowDays)
    Q_PROPERTY_CREATE_D(bool, IsShowHours)
    Q_PROPERTY_CREATE_D(bool, IsShowMinutes)
    Q_PROPERTY_CREATE_D(bool, IsShowSeconds)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, DigitWidth)
    Q_PROPERTY_CREATE_D(int, DigitHeight)
    Q_PROPERTY_CREATE_D(int, DigitSpacing)
    Q_PROPERTY_CREATE_D(int, FontPixelSize)
public:
    explicit ElaCountdownPrivate(QObject* parent = nullptr);
    ~ElaCountdownPrivate() override;

    Q_SLOT void onTimerTick();
    void _startFlipAnimation();

private:
    bool _isRunning{false};
    bool _useTargetMode{false};
    ElaThemeType::ThemeMode _themeMode;
    int _prevSeconds{-1};
    int _prevMinutes{-1};
    int _prevHours{-1};
    int _prevDays{-1};
    QTimer* _timer{nullptr};
    QDateTime _targetDateTime;
    qint64 _remainingSeconds{0};

    friend class ElaCountdown;
};
```

向你解释为什么，因为
    Q_PROPERTY_CREATE_D(bool, IsShowDays)
    Q_PROPERTY_CREATE_D(bool, IsShowHours)
    Q_PROPERTY_CREATE_D(bool, IsShowMinutes)
    Q_PROPERTY_CREATE_D(bool, IsShowSeconds)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
恰好8字节，
    Q_PROPERTY_CREATE_D(int, DigitWidth)
    Q_PROPERTY_CREATE_D(int, DigitHeight)
    Q_PROPERTY_CREATE_D(int, DigitSpacing)
    Q_PROPERTY_CREATE_D(int, FontPixelSize)
恰好16字节
    bool _isRunning{false};
    bool _useTargetMode{false};
    ElaThemeType::ThemeMode _themeMode;
虽然不足8字节，但实际上6字节以8字节看
    int _prevSeconds{-1};
    int _prevMinutes{-1};
    int _prevHours{-1};
    int _prevDays{-1};
恰好16字节

又例如：

```
class ElaExpanderPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaExpander)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle)
    int _headerHeight{48};
    int _contentHeight{0};
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, HeaderIcon)
public:
    explicit ElaExpanderPrivate(QObject* parent = nullptr);
    ~ElaExpanderPrivate() override;

private:
    bool _isExpanded{false};
    bool _isPressed{false};
    ElaExpander::ExpandDirection _expandDirection{ElaExpander::Down};
    ElaThemeType::ThemeMode _themeMode;
    QWidget* _contentWidget{nullptr};
    QWidget* _headerCustomWidget{nullptr};
    QWidget* _contentContainer{nullptr};
    QWidget* _headerWidget{nullptr};
    QVBoxLayout* _contentLayout{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QVariantAnimation* _expandAnimation{nullptr};
    void _toggleExpand();
    void _startExpandAnimation(bool expand);
};

Q_PROPERTY_CREATE_D(QColor, DarkHoverIconColor)
Q_PROPERTY_CREATE_D(bool, IsSelected)
Q_PROPERTY_CREATE_D(ELAIconType::IconName, Awesome)
5字节视为8字节
Q_PROPERTY_CREATE(int, HoverAlpha)
Q_PROPERTY_CREATE_D(int, BorderRadius)
8字节
bool _isAlphaAnimationFinished { true }; 
ELAThemeType::ThemeMode _themeMode;
5字节视为8字节
QPixmap _iconPix;
```

期望输出

```

class ElaExpanderPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaExpander)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, HeaderIcon)
public:
    explicit ElaExpanderPrivate(QObject* parent = nullptr);
    ~ElaExpanderPrivate() override;

private:
    bool _isExpanded{false};
    bool _isPressed{false};
    ElaExpander::ExpandDirection _expandDirection{ElaExpander::Down};
    ElaThemeType::ThemeMode _themeMode;
    int _headerHeight{48};
    int _contentHeight{0};
    QWidget* _contentWidget{nullptr};
    QWidget* _headerCustomWidget{nullptr};
    QWidget* _contentContainer{nullptr};
    QWidget* _headerWidget{nullptr};
    QVBoxLayout* _contentLayout{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QVariantAnimation* _expandAnimation{nullptr};
    void _toggleExpand();
    void _startExpandAnimation(bool expand);
};
```

解释   
 Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
恰好8字节，
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, HeaderIcon)
    bool _isExpanded{false};
    bool _isPressed{false};
虽然是6字节，但是视为8字节
 ElaExpander::ExpandDirection _expandDirection{ElaExpander::Down};
    ElaThemeType::ThemeMode _themeMode;
    int _headerHeight{48};
    int _contentHeight{0};
恰好16字节
通过这两个例子，我想向你表达的是，非必要不需要让非宏成员以宏成员并排，除非迫不得已的情况，比如我先前提到的

```
class ELAIconButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(ELAIconButton)
  Q_PROPERTY_CREATE_D(qreal, Opacity)
  Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
  Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
  Q_PROPERTY_CREATE_D(QColor, LightIconColor)
  Q_PROPERTY_CREATE_D(QColor, DarkIconColor)
  Q_PROPERTY_CREATE_D(QColor, LightHoverIconColor)
  Q_PROPERTY_CREATE_D(QColor, DarkHoverIconColor)
  bool _isAlphaAnimationFinished { true }; // 我们将这个变量和宏定义声明并排才能使得大小减小
  Q_PROPERTY_CREATE_D(bool, IsSelected)
  Q_PROPERTY_CREATE_D(ELAIconType::IconName, Awesome)
  Q_PROPERTY_CREATE(int, HoverAlpha)
  Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
  explicit ELAIconButtonPrivate(QObject *parent = nullptr);
  ~ELAIconButtonPrivate();

private:
  ELAThemeType::ThemeMode _themeMode;
  QPixmap _iconPix;
};
```

实际上也可以写为

```
class ELAIconButtonPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(ELAIconButton)
  Q_PROPERTY_CREATE_D(qreal, Opacity)
  Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
  Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
  Q_PROPERTY_CREATE_D(QColor, LightIconColor)
  Q_PROPERTY_CREATE_D(QColor, DarkIconColor)
  Q_PROPERTY_CREATE_D(QColor, LightHoverIconColor)
  Q_PROPERTY_CREATE_D(QColor, DarkHoverIconColor)
  Q_PROPERTY_CREATE_D(bool, IsSelected)
  Q_PROPERTY_CREATE_D(ELAIconType::IconName, Awesome)
  Q_PROPERTY_CREATE(int, HoverAlpha)
  Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
  explicit ELAIconButtonPrivate(QObject *parent = nullptr);
  ~ELAIconButtonPrivate();

private:
  bool _isAlphaAnimationFinished { true };
  ELAThemeType::ThemeMode _themeMode;
  QPixmap _iconPix;
};
```

解释
Q_PROPERTY_CREATE_D(bool, IsSelected)
Q_PROPERTY_CREATE_D(ELAIconType::IconName, Awesome)
5字节视为8字节
Q_PROPERTY_CREATE(int, HoverAlpha)
Q_PROPERTY_CREATE_D(int, BorderRadius)
8字节
bool _isAlphaAnimationFinished { true }; 
ELAThemeType::ThemeMode _themeMode;
5字节视为8字节
所以即使不紧挨着它的大小也是最小的，所以一定要认真审视，以“拼凑”的模式构建以“8字节”为基准的代码，非必要不要让非宏变量与宏变量并排。
**算法可能的逻辑**：收集类型，宏成员变量定义最后都是小于8字节的类型，非宏成员变量定义开始都是小于8字节的类型

```
  Q_PROPERTY_CREATE_D(QColor, DarkHoverIconColor)
  Q_PROPERTY_CREATE_D(bool, IsSelected) // 最后
  Q_PROPERTY_CREATE_D(ELAIconType::IconName, Awesome)

  bool _isAlphaAnimationFinished { true }; // 开始
  ELAThemeType::ThemeMode _themeMode;

```

为了使得大小最小，那么相同类型需要紧挨着，但这个不是充分条件

```
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle) // QString紧挨
    Q_PROPERTY_CREATE_D(int, BorderRadius) // int紧挨
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, HeaderIcon)
public:
    explicit ElaExpanderPrivate(QObject* parent = nullptr);
    ~ElaExpanderPrivate() override;

private:
    bool _isExpanded{false}; // bool紧挨
    bool _isPressed{false};
    ElaExpander::ExpandDirection _expandDirection{ElaExpander::Down}; // 枚举紧挨
    ElaThemeType::ThemeMode _themeMode;
    int _headerHeight{48}; // int紧挨
    int _contentHeight{0};
    QWidget* _contentWidget{nullptr}; //指针紧挨
    QWidget* _headerCustomWidget{nullptr};
    QWidget* _contentContainer{nullptr};
    QWidget* _headerWidget{nullptr};
```

必要条件将小于8字节的类型以8字节拼凑紧挨着

**测试文件**：Sample文件夹下的文件

TDD开发，输出测试到Output，最后需要验收审计，如果有不清楚的问题向我提问。

