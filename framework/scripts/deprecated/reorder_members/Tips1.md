# 一

```
选择当前项目合适的skills，使用py脚本写一个算法。
功能：扫描所有C++/Qt的.h，.hpp，.hxx等与header相关的头文件，定位到成员变量，执行成员顺序编排使得类的大小减少。
测试文件： `Sample\private` `Sample\DeveloperComponents` 文件夹下的.h文件
提示：
Q_PROPERTY_CREATE

Q_PROPERTY_REF_CREATE 与 Q_PROPERTY_CREATE_2等价但是写法不一样需要进行替换

Q_PROPERTY_CREATE_D

Q_PRIVATE_CREATE_D

Q_PRIVATE_CREATE

Q_PRIVATE_REF_CREATE
等是一个宏，它们展开均会定义一个成员变量
例如
#define Q_PRIVATE_CREATE_D(TYPE, M) \
private:                            \
    TYPE _p##M;

Q_PRIVATE_CREATE_D(int, X)
展开
private:
int _pX;
所以你可以等价于只要是上述宏定义的代码均代表一个类型的成员变量。

注意：
Q_PROPERTY_REF_CREATE(QModelIndex, ModelIndex)和
Q_PROPERTY_CREATE_2(const QModelIndex &, QModelIndex, ModelIndex)生成的内容等价，在转换的时候你需要进行处理这种情况
即将Q_PROPERTY_REF_CREATE代码转换到Q_PROPERTY_CREATE_2的写法。
功能要求示例：
NXAcrylicUrlCardPrivate 以8字节对齐那么当前的大小考虑到虚表，是sizeof(NXAcrylicUrlCardPrivate) 224
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
执行成员变量编排，此时sizeof(NXAcrylicUrlCardPrivate) 是216大小，注意我们需要删除宏定义额外多余的;
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
换句话说我们需要处理小于8字节的类型，如bool、int、char、float、枚举、quint32等类型的编排，核心在于宏声明最后总是小于8字节的类型，非宏声明之前是小于8字节的类型，它们需要“紧挨着”，相同类型尽量紧挨着。如果有必要可以将显示非宏的变量与宏变量声明并排，例如
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
可以看到
  bool _isAlphaAnimationFinished { true }; // 我们将这个变量和宏定义声明并排
  Q_PROPERTY_CREATE_D(bool, IsSelected)
  Q_PROPERTY_CREATE_D(ELAIconType::IconName, Awesome)
  Q_PROPERTY_CREATE(int, HoverAlpha)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
...
  ELAThemeType::ThemeMode _themeMode;
宏定义小于8字节在最后，非宏定义小于8字节在最前。
示例：
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
可能的输出情况 `Output` （注意这个Output仅是参考，有些变量 `Sample` 没有，不代表 `Sample` 转换后得到的内容和 `Output` 一模一样，我们需要以 `Sam
```

# 二

```
我稍微看了一下代码虽然正确，但是格式稍微不符合预期。我拿output两个文件说明 `Output\private\ElaCountdownPrivate.h` `Output\private\ElaExpanderPrivate.h` 
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
期望输出
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
示例2：
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
期望输出
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
通过这两个例子，我想向你表达的是，非必要不需要让非宏成员以宏成员并排，除非迫不得已的情况，比如我先前提到的哪种情况非常少实际上也可以写为
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
和我第一次提问解释的大小是一模一样的，所以我需要你重新调整算法，以“拼凑”的模式构建以“8字节”为基准的代码，非必要不要让非宏变量与宏变量并排，其他要求和第一次一样，使用skills思考调整py代码逻辑设计新算法实现，注意完成之后需要测试验收审计
```

# 三

```
我检查了private文件夹，对于 `Output\private\ElaCommandBarPrivate.h` `Output\private\ElaNotificationCenterPrivate.h` `Output\private\ElaPromotionViewPrivate.h` `Output\private\ElaRollerPrivate.h` `Output\private\ElaSheetPanelPrivate.h` `Output\private\ElaSplashScreenPrivate.h` `Output\private\ElaSpotlightPrivate.h` `Output\private\ElaTeachingTipPrivate.h` `Output\private\ElaToastPrivate.h` `Output\private\ElaToolBarPrivate.h` `Output\private\ElaUploadAreaPrivate.h` `Output\private\ElaWizardPrivate.h` 这些文件不符合要求，
示例：
class ElaMenu;
class ElaCommandBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCommandBar)
    Q_PROPERTY_CREATE_D(int, ButtonSize)
...

private:
    ElaThemeType::ThemeMode _themeMode;
    QList<ElaCommandBar::CommandItem> _items;
    int _hoveredIndex{-1};
    int _pressedIndex{-1};
    int _visibleCount{0};
    ElaMenu* _overflowMenu{nullptr};
};
期望为
    ElaThemeType::ThemeMode _themeMode;
    int _hoveredIndex{-1};
    int _pressedIndex{-1};
    int _visibleCount{0};
    QList<ElaCommandBar::CommandItem> _items;
枚举和int尽量紧贴
示例：
class ElaNotificationCenterPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNotificationCenter)

    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, PanelWidth)
...

private:
    bool _isVisible{false};
    QList<ElaNotificationCenter::NotificationItem> _items;
    ElaThemeType::ThemeMode _themeMode;
    int _scrollOffset{0};
    QPropertyAnimation* _slideAnimation{nullptr};
    QWidget* _anchorWidget{nullptr};
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    int _shadowBorderWidth{0};
#else
    int _shadowBorderWidth{8};
#endif

    int _hitTestCard(const QPoint& pos) const;
    QRect _clearAllButtonRect() const;
    bool _hitTestClearAll(const QPoint& pos) const;
};

期望为
    bool _isVisible{false};
    ElaThemeType::ThemeMode _themeMode;
    int _scrollOffset{0};
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    int _shadowBorderWidth{0};
#else
    int _shadowBorderWidth{8};
#endif
    QList<ElaNotificationCenter::NotificationItem> _items;
    QPropertyAnimation* _slideAnimation{nullptr};
    QWidget* _anchorWidget{nullptr};
类型int和枚举紧贴
示例：
class ElaPromotionViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPromotionView)
    Q_PROPERTY_CREATE_D(bool, IsAutoScroll)
    Q_PROPERTY_CREATE_D(int, CardExpandWidth)
    Q_PROPERTY_CREATE_D(int, CardCollapseWidth)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
    Q_PROPERTY_CREATE_D(int, AutoScrollInterval)
...

private:
    bool _isAllowSwitch{true};
    ElaThemeType::ThemeMode _themeMode;
    int _leftPadding{40};
    int _cardSpacing{5};
    int _bottomMargin{30};
    int _indicatorSpacing{12};
    QTimer* _autoScrollTimer{nullptr};
    QList<ElaPromotionCard*> _promotionCardList;
};
期望为
    Q_PROPERTY_CREATE_D(int, CardExpandWidth)
    Q_PROPERTY_CREATE_D(int, CardCollapseWidth)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
    Q_PROPERTY_CREATE_D(int, AutoScrollInterval)
    Q_PROPERTY_CREATE_D(bool, IsAutoScroll)
...

private:
    bool _isAllowSwitch{true};
    ElaThemeType::ThemeMode _themeMode;
    int _leftPadding{40};
    int _cardSpacing{5};
    int _bottomMargin{30};
    int _indicatorSpacing{12};
解释四个宏int恰好16字节，两个bool和一个枚举6字节视为8字节，后面剩余4个int恰好16字节，和原来代码相比少了8字节占用
示例：
class ElaRollerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaRoller)
    Q_PROPERTY_CREATE(qreal, ScrollOffset)
    Q_PROPERTY_CREATE_D(QStringList, ItemList)
    Q_PROPERTY_CREATE_D(bool, IsContainer)
    Q_PROPERTY_CREATE_D(bool, IsEnableLoop)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
...

private:
    bool _isUpArrowPress{false};
    bool _isDownArrowPress{false};
    ElaThemeType::ThemeMode _themeMode;
    int _penBorderWidth{1};
    int _indicatorMargin{3};
    QPropertyAnimation* _scrollAnimation{nullptr};
    QTimer* _pressSustainTimer{nullptr};
    QTimer* _repeatScrollTimer{nullptr};
    qreal _targetScrollOffset{0};
    QPoint _mousePoint;
    QRect _upArrowRect;
    QRect _downArrowRect;
    void _scroll(int delta);
};期望为
    Q_PROPERTY_CREATE_D(QStringList, ItemList)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
    Q_PROPERTY_CREATE_D(bool, IsContainer)
    Q_PROPERTY_CREATE_D(bool, IsEnableLoop)
...
   bool _isUpArrowPress{false};
    bool _isDownArrowPress{false};
    ElaThemeType::ThemeMode _themeMode;
    int _penBorderWidth{1};
    int _indicatorMargin{3};
理由和第三示例一样，宏四个int是16字节，宏两个bool + 非宏两个bool + 一个枚举恰好8字节，后面的两个非宏int8字节，总体减少了8字节。

总之算法存在缺陷，上述引用的文件都存在两个问题的一种，1 int和枚举类型不紧贴（即大小相同的类型需要紧贴，枚举和int都是4字节，char 和bool都是一字节，像这样大小相同的类型需要紧贴），2算法设计有缺陷，没有达到最优空间减小，示例3和示例4很好的说明了这一点，当宏存在bool类型的时候需要额外小心，有可能宏bool在末尾定义才可能是最优解，而不是在宏int之前，修改算法，将提供的这几个文件修正，我需要让他们通过，注意其他文件需要回归测试需要在修改算法之后仍旧保持最优解（可能调整算法后定义顺序和目前不一样，但是没关系，我们始终做的是空间最小，同类型紧贴，非必要非宏成员不紧贴宏成员，至于定义顺序如bool在int前，还是int在bool前没关系）
```

# 四

```
当前虽然能够执行但是存在缺陷。存在以下问题的代码太多，我就举出三个例子 `Output\DeveloperComponents\ElaWinShadowHelper.h` `Output\DeveloperComponents\ElaCodeHighlighter.h` `Output\private\ElaEmojiPickerPrivate.h` ， 
 class ElaEmojiPickerPrivate : public QObject 
 { 
   Q_OBJECT 
   Q_D_CREATE(ElaEmojiPicker) 
   Q_PROPERTY_CREATE_D(int, EmojiSize) 
   Q_PROPERTY_CREATE_D(int, Columns) 
 
 public: 
   int _currentCategory { 0 }; 
   ElaThemeType::ThemeMode _themeMode; 
   QVector<EmojiCategory> _categories; 
   QStringList _recentEmojis; 
   QGridLayout *_gridLayout { nullptr }; 
   ElaLineEdit *_searchEdit { nullptr }; 
   QWidget *_gridContainer { nullptr }; 
   explicit ElaEmojiPickerPrivate(QObject *parent = nullptr); 
   ~ElaEmojiPickerPrivate(); 
 
   struct EmojiCategory 
   { 
     QString name; 
     QString icon; 
     QStringList emojis; 
   }; 
 
   void initEmojis(); 
   void buildGrid(int categoryIndex); 
   void filterEmojis(const QString &text); 
 }; 
 QVector<EmojiCategory> _categories;没有前向声明 
   struct EmojiCategory 
   { 
     QString name; 
     QString icon; 
     QStringList emojis; 
   }; 
 编译错误。 
 `Output\DeveloperComponents\ElaWinShadowHelper.h#L19-69` 两个问题，拷贝了两次内容 
   DwmExtendFrameIntoClientAreaFunc _dwmExtendFrameIntoClientArea { nullptr }; 
   DwmSetWindowAttributeFunc _dwmSetWindowAttribute { nullptr }; 
   DwmIsCompositionEnabledFunc _dwmIsCompositionEnabled { nullptr }; 
   DwmEnableBlurBehindWindowFunc _dwmEnableBlurBehindWindow { nullptr }; 
   SetWindowCompositionAttributeFunc _setWindowCompositionAttribute { nullptr }; 
   GetDpiForWindowFunc _getDpiForWindow { nullptr }; 
   GetDpiForMonitorFunc _getDpiForMonitor { nullptr }; 
   GetSystemMetricsForDpiFunc _getSystemMetricsForDpi { nullptr }; 
   RTL_OSVERSIONINFOW _windowsVersion {}; 
 且也没有前向声明， `Output\DeveloperComponents\ElaCodeHighlighter.h#L77-86` 两个问题，没有前向声明HighlightRule，且为什么return _language <= 1 || _language == 7;出现在这个位置？算法存在逻辑问题。请纠正py代码
```

# 五

```
`Tips.md` 根据历史对话，我说了要将Q_PROPERTY_REF_CREATE替换为Q_PROPERTY_CREATE_2，
示例：
Q_PROPERTY_REF_CREATE(QModelIndex, ModelIndex)替换为
Q_PROPERTY_CREATE_2(const QModelIndex &, QModelIndex, ModelIndex)，现在我希望你进行修改，将这个功能去除不要，其他功能和原来一样。
然后新增一个功能，将Q_PROPERTY_CREATE_Q_H、Q_PROPERTY_CREATE_Q_CPP、Q_PRIVATE_CREATE_Q_H、Q_PRIVATE_CREATE_Q_CPP的 _Q_去掉，例如Q_PROPERTY_CREATE_Q_H去掉_Q_得到Q_PROPERTY_CREATE_H，注意也要去掉分号
将Q_PROPERTY_CREATE_Q_H、Q_PROPERTY_CREATE_Q_CPP、Q_PRIVATE_CREATE_Q_H、Q_PRIVATE_CREATE_Q_CPP的类型进行同类型紧贴，按类型大小从小到达排序，不需要考虑大小，因为这几个宏不定义成员变量。
示例
 `Sample\ElaAcrylicUrlCard.h#L14-28` `Sample\ElaAcrylicUrlCard.cpp#L10-24` 
执行新增功能，int4字节，枚举4字节，qreal、QSize8字节，QPixmap、QString24字节
.h文件将被修改为
    Q_PROPERTY_CREATE_H(int, BorderRadius)
    Q_PROPERTY_CREATE_H(int, BrushAlpha)
    Q_PROPERTY_CREATE_H(int, TitlePixelSize)
    Q_PROPERTY_CREATE_H(int, SubTitlePixelSize)
    Q_PROPERTY_CREATE_H(int, TitleSpacing)
    Q_PROPERTY_CREATE_H(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_H(int, SubTitleSpacing)
    Q_PROPERTY_CREATE_H(ElaCardPixType::PixMode, CardPixMode)
    Q_PROPERTY_CREATE_H(qreal, MainOpacity)
    Q_PROPERTY_CREATE_H(qreal, NoiseOpacity)
    Q_PROPERTY_CREATE_H(QSize, CardPixmapSize)
    Q_PROPERTY_CREATE_H(QString, Title)
    Q_PROPERTY_CREATE_H(QString, SubTitle)
    Q_PROPERTY_CREATE_H(QString, Url)
    Q_PROPERTY_CREATE_H(QPixmap, CardPixmap)
可以看到同类型紧贴，分号删除，_Q_删除，类型大到小排序
.cpp一样
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, BorderRadius)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, BrushAlpha)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, TitlePixelSize)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, SubTitlePixelSize)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, TitleSpacing)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, SubTitleSpacing)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, int, CardPixmapBorderRadius)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, ElaCardPixType::PixMode, CardPixMode)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, qreal, MainOpacity)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, qreal, NoiseOpacity)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QSize, CardPixmapSize)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QString, Title)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QString, SubTitle)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QString, Url)
Q_PROPERTY_CREATE_CPP(ElaAcrylicUrlCard, QPixmap, CardPixmap)
上述操作对于Q_PRIVATE_CREATE_Q_H、Q_PRIVATE_CREATE_Q_CPP也适用
同样的，完成py代码后别忘了回归测试，测试文件在 `Sample` ，输出到 `Output`
```

