# 前言

撰写CLI py脚本，将ElaWidgetTools、ElaWidgetToolsExample的头文件和源文件内容进行替换，使用git 进行比较替换前后是否正确，完成功能后进行审视是否可以落地使用SIMD、循环展开、并行、内存池复用、0拷贝技术减少IO读取等常见的性能优化技术进一步优化py脚本功能，完成性能优化之后再次进行回归测试验证功能是否正确。

# 需求

输入指定文件夹，递归文件夹执行扫描C++头文件`".h", ".hpp", ".hxx", ".hh"`，C++源文件`".cpp", ".cxx", ".cc", ".c"`，完成C++词法文本调整、增删操作。

# 提示

```
1、只会在头文件出现的宏
Q_PROPERTY_CREATE_Q_H
Q_PRIVATE_CREATE_Q_H
Q_PROPERTY_REF_CREATE_Q_H
Q_PRIVATE_REF_CREATE_Q_H

Q_PROPERTY_CREATE
Q_PROPERTY_REF_CREATE
Q_PRIVATE_CREATE
Q_PRIVATE_REF_CREATE

Q_PROPERTY_CREATE_D
Q_PRIVATE_CREATE_D

2、只会出现在源文件的宏
Q_PROPERTY_CREATE_Q_CPP
Q_PROPERTY_REF_CREATE_Q_CPP
Q_PRIVATE_CREATE_Q_CPP
Q_PRIVATE_REF_CREATE_Q_CPP
```

> 定义详细查看./ElaWidgetTools/ElaProperty.h

# 功能1

定位到

```
Q_PROPERTY_CREATE_Q_H
Q_PRIVATE_CREATE_Q_H
Q_PROPERTY_REF_CREATE_Q_H
Q_PRIVATE_REF_CREATE_Q_H

Q_PROPERTY_REF_CREATE
Q_PRIVATE_REF_CREATE

Q_PROPERTY_CREATE_Q_CPP
Q_PROPERTY_REF_CREATE_Q_CPP
Q_PRIVATE_CREATE_Q_CPP
Q_PRIVATE_REF_CREATE_Q_CPP
```

将`_REF_`、`_Q_`替换为`_`，

```
Q_PROPERTY_CREATE_Q_H -> Q_PROPERTY_CREATE_H
Q_PRIVATE_CREATE_Q_H -> Q_PRIVATE_CREATE_H
Q_PROPERTY_REF_CREATE_Q_H -> Q_PROPERTY_CREATE_H
Q_PRIVATE_REF_CREATE_Q_H -> Q_PRIVATE_CREATE_H

Q_PROPERTY_REF_CREATE -> Q_PROPERTY_CREATE
Q_PRIVATE_REF_CREATE -> Q_PRIVATE_CREATE

Q_PROPERTY_CREATE_Q_CPP -> Q_PROPERTY_CREATE_CPP
Q_PROPERTY_REF_CREATE_Q_CPP -> Q_PRIVATE_CREATE_CPP
Q_PRIVATE_CREATE_Q_CPP -> Q_PROPERTY_CREATE_CPP
Q_PRIVATE_REF_CREATE_Q_CPP -> Q_PRIVATE_CREATE_CPP
```

可能的实现：进行全词匹配然后进行替换

# 功能2

删除提示的所有宏

```
Q_PROPERTY_CREATE_Q_H
Q_PRIVATE_CREATE_Q_H
Q_PROPERTY_REF_CREATE_Q_H
Q_PRIVATE_REF_CREATE_Q_H

Q_PROPERTY_CREATE
Q_PROPERTY_REF_CREATE
Q_PRIVATE_CREATE
Q_PRIVATE_REF_CREATE

Q_PROPERTY_CREATE_D
Q_PRIVATE_CREATE_D

Q_PROPERTY_CREATE_Q_CPP
Q_PROPERTY_REF_CREATE_Q_CPP
Q_PRIVATE_CREATE_Q_CPP
Q_PRIVATE_REF_CREATE_Q_CPP
```

末尾的分号，前提是如果有

```
  Q_PROPERTY_CREATE_H(bool, IsLogFileNameWithTime);
  Q_PROPERTY_CREATE_H(QString, LogFileName);
  Q_PROPERTY_CREATE_H(QString, LogSavePath)
```

删除

```
  Q_PROPERTY_CREATE_H(bool, IsLogFileNameWithTime)
  Q_PROPERTY_CREATE_H(QString, LogFileName)
  Q_PROPERTY_CREATE_H(QString, LogSavePath)
```

可能的实现：在功能1的基础上向后查找到)，然后再向后一位看看是否有分号然后进行删除

# 功能3

定义一个set集合，默认内容如下

```
    "QBitArray", "QBitmap", "QBrush", "QByteArray",
    "QCache", "QCollator", "QCollatorSortKey", "QContiguousCache", "QColor",
    "QCryptographicHash", "QCursor", "QEasingCurve", "QFont",
    "QGlyphRun", "QHash", "QIcon", "QImage",
    "QItemSelection", "QJsonArray", "QJsonDocument", "QJsonObject",
    "QJsonValue", "QLinkedList", "QList", "QLocale",
    "QMap", "QModelIndex", "QMultiHash", "QMultiMap", "QOpenGLVersionProfile",
    "QPageLayout", "QPageSize", "QPainterPath", "QPalette",
    "QPen", "QPersistentModelIndex", "QPicture", "QPixmap",
    "QPolygon", "QPolygonF", "QQueue", "QRawFont",
    "QRegion", "QRegularExpression", "QSet",
    "QStack", "QStaticText", "QString", "QStringList",
    "QStyleHintReturn", "QStyleOption", "QSurfaceFormat",
    "QTextBlock", "QTextBlockFormat", "QTextBoundaryFinder",
    "QTextCharFormat", "QTextDocument", "QTextFormat",
    "QTextFrame", "QTextFrameFormat", "QTextImageFormat",
    "QTextList", "QTextListFormat", "QTextTable",
    "QTextTableFormat", "QTransform", "QUrl", "QVarLengthArray",
    "QVariant", "QVariantMap", "QVector",
```

将以下宏

```
Q_PROPERTY_CREATE_Q_H
Q_PRIVATE_CREATE_Q_H
Q_PROPERTY_REF_CREATE_Q_H
Q_PRIVATE_REF_CREATE_Q_H

Q_PROPERTY_CREATE
Q_PROPERTY_REF_CREATE
Q_PRIVATE_CREATE
Q_PRIVATE_REF_CREATE

Q_PROPERTY_CREATE_Q_CPP
Q_PROPERTY_REF_CREATE_Q_CPP
Q_PRIVATE_CREATE_Q_CPP
Q_PRIVATE_REF_CREATE_Q_CPP
```

包裹的类型，进行再包裹一层QS_SET_CREF

示例

```
Q_PROPERTY_CREATE_Q_H(QString, LogFileName) -> Q_PROPERTY_CREATE_Q_H(QS_SET_CREF(QString), LogFileName)

Q_PRIVATE_CREATE_Q_H(QString, LogFileName) -> Q_PRIVATE_CREATE_Q_H(QS_SET_CREF(QString), LogFileName)

Q_PROPERTY_REF_CREATE_Q_H(QString, LogFileName) -> Q_PROPERTY_REF_CREATE_Q_H(QS_SET_CREF(QString), LogFileName)

Q_PRIVATE_REF_CREATE_Q_H(QString, LogFileName) -> Q_PRIVATE_REF_CREATE_Q_H(QS_SET_CREF(QString), LogFileName)

Q_PROPERTY_CREATE(QString, LogFileName)-> Q_PROPERTY_CREATE(QS_SET_CREF(QString), LogFileName)

Q_PROPERTY_REF_CREATE(QString, LogFileName)-> Q_PROPERTY_REF_CREATE(QS_SET_CREF(QString), LogFileName)

Q_PRIVATE_CREATE(QString, LogFileName)-> Q_PRIVATE_CREATE(QS_SET_CREF(QString), LogFileName)

Q_PRIVATE_REF_CREATE(QString, LogFileName)-> Q_PRIVATE_REF_CREATE(QS_SET_CREF(QString), LogFileName)

Q_PROPERTY_CREATE_D(QString, LogFileName) 不需要做
Q_PRIVATE_CREATE_D(QString, LogFileName) 不需要做

Q_PROPERTY_CREATE_Q_CPP(Class, QString, LogFileName)-> Q_PROPERTY_CREATE_Q_CPP(Class, QS_SET_CREF(QString), LogFileName)

Q_PROPERTY_REF_CREATE_Q_CPP(Class, QString, LogFileName)-> Q_PROPERTY_REF_CREATE_Q_CPP(Class, QS_SET_CREF(QString), LogFileName)

Q_PRIVATE_CREATE_Q_CPP(Class, QString, LogFileName)-> Q_PRIVATE_CREATE_Q_CPP(Class, QS_SET_CREF(QString), LogFileName)

Q_PRIVATE_REF_CREATE_Q_CPP(Class, QString, LogFileName)-> Q_PRIVATE_REF_CREATE_Q_CPP(Class, QS_SET_CREF(QString), LogFileName)

注意模板类型要特别注意，需要处理<>尖括号
Q_PROPERTY_CREATE_Q_H(QMap<int, int>, LogFileName) -> Q_PROPERTY_CREATE_Q_H(QS_SET_CREF(QMap<int, int>), LogFileName)
```

# 功能4

将后缀带`_H`、`_CPP`的宏

```
Q_PROPERTY_CREATE_Q_H
Q_PRIVATE_CREATE_Q_H
Q_PROPERTY_REF_CREATE_Q_H
Q_PRIVATE_REF_CREATE_Q_H

Q_PROPERTY_CREATE_Q_CPP
Q_PROPERTY_REF_CREATE_Q_CPP
Q_PRIVATE_CREATE_Q_CPP
Q_PRIVATE_REF_CREATE_Q_CPP
```

，定义类型按照字节大小从小到大排序，不在列表内的类型始终在末尾，特别处理**枚举类型和指针类型**，指针带* 8字节，枚举带:: 4字节

```
TYPE_SIZES = {
    'bool': 1, 'char': 1, 'signed char': 1, 'unsigned char': 1,
    'int8_t': 1, 'uint8_t': 1, 'qint8': 1, 'quint8': 1,
    'short': 2, 'unsigned short': 2, 'short int': 2,
    'int16_t': 2, 'uint16_t': 2, 'qint16': 2, 'quint16': 2,
    'int': 4, 'unsigned int': 4, 'unsigned': 4,
    'long': 4, 'unsigned long': 4, 'long int': 4,
    'uint32_t': 4, 'float': 4, 'qint32': 4, 'quint32': 4, 'int32_t': 4,
    'long long': 8, 'unsigned long long': 8, 'double': 8, 'long double': 8,
    'qreal': 8, 'qint64': 8, 'quint64': 8, 'int64_t': 8, 'uint64_t': 8,
    'size_t': 8, 'ptrdiff_t': 8,
    'QLocale': 8, 'QEasingCurve': 8, 'QPersistentModelIndex': 8,
    'QCryptographicHash': 8, 'QCollator': 8, 'QCollatorSortKey': 8,
    'QStaticText': 8, 'QTextBoundaryFinder': 72,
    'QGlyphRun': 8, 'QRawFont': 8,
    'QSurfaceFormat': 8, 'QOpenGLVersionProfile': 8,
    'QBrush': 8, 'QPen': 8, 'QIcon': 8, 'QCursor': 8,
    'QRegion': 8, 'QPainterPath': 8, 'QRegularExpression': 8,
    'QPageSize': 8, 'QPageLayout': 8, 'QStyleHintReturn': 8,
    'QUrl': 8, 'QJsonObject': 8, 'QJsonArray': 8, 'QJsonDocument': 8,
    'QDate': 8, 'QVariantMap': 8, 'QSize': 8, 'QPoint': 8,
    'QRect': 16, 'QSizeF': 16, 'QPointF': 16, 'QColor': 16,
    'QTextImageFormat': 16, 'QTextBlock': 16, 'QFont': 16,
    'QTextLength': 16, 'QPalette': 16,
    'QTextDocument': 16, 'QTextList': 16, 'QTextFrame': 16,
    'QTextTable': 16, 'QTextFormat': 16, 'QTextCharFormat': 16,
    'QTextBlockFormat': 16, 'QTextListFormat': 16, 'QTextFrameFormat': 16,
    'QTextTableFormat': 16,
    'QStringList': 24, 'QModelIndex': 24,
    'QPolygon': 24, 'QPolygonF': 24, 'QString': 24, 'QPicture': 24,
    'QJsonValue': 24, 'QPixmap': 24, 'QItemSelection': 24,
    'QByteArray': 24, 'QImage': 24, 'QBitmap': 24, 'QBitArray': 24,
    'QVariant': 32, 'QRectF': 32, 'QStyleOption': 64, 'QTransform': 80,
}

TEMPLATE_SIZES = {
    'QMap': 8, 'QHash': 8, 'QMultiMap': 8, 'QMultiHash': 16,
    'QSet': 8, 'QContiguousCache': 8,
    'QList': 24, 'QVector': 24, 'QStack': 24, 'QQueue': 24,
    'QLinkedList': 24,
    'QVarLengthArray': 56, 'QCache': 72,
}
```

注意模板类型适用于所有参数，不光光是int，例如`QList<QString>`和`QList<int>`大小一样

示例

```
  Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, BrushAlpha)
  Q_PROPERTY_CREATE_Q_H(int, CardPixmapBorderRadius)
  Q_PROPERTY_CREATE_Q_H(int, SubTitlePixelSize)
  Q_PROPERTY_CREATE_Q_H(int, SubTitleSpacing)
  Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize)
  Q_PROPERTY_CREATE_Q_H(int, TitleSpacing)
  Q_PROPERTY_CREATE_Q_H(NXCardPixType::PixMode, CardPixMode) // 枚举
  Q_PROPERTY_CREATE_Q_H(NXWidget *, Widget) // 指针
  Q_PROPERTY_CREATE_Q_H(qreal, MainOpacity)
  Q_PROPERTY_CREATE_Q_H(qreal, NoiseOpacity)
  Q_PROPERTY_CREATE_Q_H(QPixmap, CardPixmap)
  Q_PROPERTY_CREATE_Q_H(QSize, CardPixmapSize)
  Q_PROPERTY_CREATE_Q_H(QString, SubTitle)
  Q_PROPERTY_CREATE_Q_H(QString, Title)
  Q_PROPERTY_CREATE_Q_H(QString, Url)
```

可能实现的算法：在功能1操作解析定位完宏的第一行和最后一行，解析类型，根据大小排序

# 功能5

将后缀非`_H`、`_CPP`的宏这些宏

```
1、只会在头文件出现的宏
Q_PROPERTY_CREATE
Q_PROPERTY_REF_CREATE
Q_PRIVATE_CREATE
Q_PRIVATE_REF_CREATE

Q_PROPERTY_CREATE_D
Q_PRIVATE_CREATE_D
```

宏区域成员执行大小降序排序，非宏区域需要将成员变量大小执行升序排序

示例

```
class NXAppBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXAppBar)
  // 宏区域，注意这个注释是提示实际代码没有
  Q_PROPERTY_CREATE_D(bool, IsStayTop)
  Q_PROPERTY_CREATE_D(bool, IsFixedSize)
  Q_PROPERTY_CREATE_D(bool, IsDefaultClosed)
  Q_PROPERTY_CREATE_D(bool, IsOnlyAllowMinAndClose)
  Q_PROPERTY_CREATE_D(int, AppBarHeight)
  Q_PRIVATE_CREATE_D(QMenu *, CustomMenu)

public:
  explicit NXAppBarPrivate(QObject *parent = nullptr);
  ~NXAppBarPrivate();
  Q_SLOT void onMinButtonClicked();
  Q_SLOT void onMaxButtonClicked();
  Q_SLOT void onCloseButtonClicked();
  Q_SLOT void onStayTopButtonClicked();

private:
// 非宏区域，注意这个注释是提示实际代码没有
  NXThemeType::ThemeMode _themeMode;
  QHBoxLayout *_mainLayout { nullptr };
  QVBoxLayout *_iconLabelLayout { nullptr };
  QVBoxLayout *_titleLabelLayout { nullptr };
  NXAppBarType::ButtonFlags _buttonFlags;
  NXToolButton *_routeBackButton { nullptr };
  NXToolButton *_routeForwardButton { nullptr };
  NXToolButton *_navigationButton { nullptr };
  NXToolButton *_themeChangeButton { nullptr };
  NXToolButton *_stayTopButton { nullptr };
  NXToolButton *_minButton { nullptr };
  NXToolButton *_maxButton { nullptr };
  NXIconButton *_closeButton { nullptr };
  QList<QWidget *> _customAreaWidgetList { nullptr, nullptr, nullptr };
  QList<QObject *> _customAreaHitTestObjectList { nullptr, nullptr, nullptr };
  QStringList _customAreaHitTestFunctionNameList { QStringLiteral(""), QStringLiteral(""), QStringLiteral("") };
  QList<QWidget *> _clientWidgetList;
  QScreen *_lastScreen { nullptr };
  NXText *_titleLabel { nullptr };
  QLabel *_iconLabel { nullptr };
  qint64 _currentWinID { 0 };
  int _lastMinTrackWidth { 0 };
  int _edges { 0 };
  quint64 _clickTimer { 0 };
  int _margins { 8 };
#ifndef Q_OS_WIN
  bool _isDragging { false };
  QPoint _dragStartPos;
#endif
  bool _isHoverMaxButton { false };
  int _win7Margins { 0 };
  void _changeMaxButtonAwesome(bool isMaximized);
  void _showAppBarMenu(QPoint point);
  void _updateCursor(int edges);
  bool _containsCursorToItem(QWidget *item);
  void _onThemeModeChange(NXThemeType::ThemeMode themeMode);
  int _calculateMinimumWidth();
  QVBoxLayout *_createVLayout(QWidget *widget);
};
```

执行结果

```
class NXAppBarPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXAppBar)
  // 降序排序，注意这个注释是提示实际代码没有
  Q_PRIVATE_CREATE_D(QMenu *, CustomMenu)
  Q_PROPERTY_CREATE_D(int, AppBarHeight)
  Q_PROPERTY_CREATE_D(bool, IsStayTop)
  Q_PROPERTY_CREATE_D(bool, IsFixedSize)
  Q_PROPERTY_CREATE_D(bool, IsDefaultClosed)
  Q_PROPERTY_CREATE_D(bool, IsOnlyAllowMinAndClose)

public:
  explicit NXAppBarPrivate(QObject *parent = nullptr);
  ~NXAppBarPrivate();
  Q_SLOT void onMinButtonClicked();
  Q_SLOT void onMaxButtonClicked();
  Q_SLOT void onCloseButtonClicked();
  Q_SLOT void onStayTopButtonClicked();

private:
// 升序排序，注意这个注释是提示实际代码没有
#ifndef Q_OS_WIN
  bool _isDragging { false };
#endif
  bool _isHoverMaxButton { false };
  int _win7Margins { 0 };
  int _lastMinTrackWidth { 0 };
  int _edges { 0 };
  int _margins { 8 };
  NXThemeType::ThemeMode _themeMode;
  NXAppBarType::ButtonFlags _buttonFlags;
#ifndef Q_OS_WIN
  QPoint _dragStartPos;
#endif
  quint64 _clickTimer { 0 };
  qint64 _currentWinID { 0 };
  QHBoxLayout *_mainLayout { nullptr };
  QVBoxLayout *_iconLabelLayout { nullptr };
  QVBoxLayout *_titleLabelLayout { nullptr };
  NXToolButton *_routeBackButton { nullptr };
  NXToolButton *_routeForwardButton { nullptr };
  NXToolButton *_navigationButton { nullptr };
  NXToolButton *_themeChangeButton { nullptr };
  NXToolButton *_stayTopButton { nullptr };
  NXToolButton *_minButton { nullptr };
  NXToolButton *_maxButton { nullptr };
  NXIconButton *_closeButton { nullptr }; 
  QScreen *_lastScreen { nullptr };
  NXText *_titleLabel { nullptr };
  QLabel *_iconLabel { nullptr };
  QList<QWidget *> _customAreaWidgetList { nullptr, nullptr, nullptr };
  QList<QObject *> _customAreaHitTestObjectList { nullptr, nullptr, nullptr };
  QStringList _customAreaHitTestFunctionNameList { QStringLiteral(""), QStringLiteral(""), QStringLiteral("") };
  QList<QWidget *> _clientWidgetList;
 
  void _changeMaxButtonAwesome(bool isMaximized);
  void _showAppBarMenu(QPoint point);
  void _updateCursor(int edges);
  bool _containsCursorToItem(QWidget *item);
  void _onThemeModeChange(NXThemeType::ThemeMode themeMode);
  int _calculateMinimumWidth();
  QVBoxLayout *_createVLayout(QWidget *widget);
};
```

可能执行的算法：类型排序和功能4一样，难点在于
1、如何处理有预处理命令包裹的类型

```
#ifndef Q_OS_WIN
  bool _isDragging { false };
  QPoint _dragStartPos;
#endif
```

2、如何确定非宏成员变量区域

宏区域始终在类开始时定义很好做到，

但是非宏区域，它可能在public（概率小），也可能在protected（概率小），也可能在private（概率大），且可能有多个private、public、protected

3、前向声明问题

```
class A
{
private:

int a;
struct B
{
};

QList<B> b;
int c;
}
```

执行不能

```
class A
{
private:

int a;
int c;
QList<B> b; // 无法感知B的存在
struct B
{
};
}
```

必须确保前向声明的类结构体必须在之前

```
class A
{
private:

struct B
{
};

int a;
int c;
QList<B> b; // 正确
}
```



> 注：仅需要处理头文件，源文件不需要处理，因为这些宏不会出现在.cpp等源文件中

# 功能6

扫描所有头文件和源文件，识别函数（`返回值 函数名(参数列表)`），然后将参数为`QS_TYPES`的类型加上`const &`，`TYPE_DIFF_LIST`的类型（除override的函数以外）移除`const &`，函数返回值移除const &

`const &`有两种写法，`const Type &`, `Type const &`要注意

示例：

```
void
NXCalendar::setMaximumDate(const QDate &maximumDate)
{
}
移除 const &
void
NXCalendar::setMaximumDate(QDate maximumDate)
{
}

void clicked(QDate const &date);
移除
void clicked(QDate date);

 NXNavigationType::NodeResult addExpanderNode(QString expanderTitle,
                                              QString &expanderKey,
                                              NXIconType::IconName awesome = NXIconType::None) const;

添加 const &
 NXNavigationType::NodeResult addExpanderNode(const QString &expanderTitle,
                                              QString &expanderKey, // 是 QString & 不用修改
                                              NXIconType::IconName awesome = NXIconType::None) const;
                                              
NXNavigationType::NodeResult addExpanderNode(const QString &expanderTitle, // 已经是const& 不修改
                                             QString &expanderKey,
                                             const QString &targetExpanderKey,  // 已经是const& 不修改
                                             NXIconType::IconName awesome = NXIconType::None) const;
                                             
  NXNavigationType::NodeResult addCategoryNode(QString *categoryTitle, QString &categoryKey); // 是QString *不修改
    
标准分离写法
// .h等头文件
  QSize sizeFromContents(ContentsType type,
                         const QStyleOption *option,
                         const QSize &size,
                         const QWidget *widget) const override; //声明，什么也不做因为有 override
    // .cpp等源文件                     
 QSize sizeFromContents(ContentsType type,
                         const QStyleOption *option,
                         const QSize &size,
                         const QWidget *widget) const // 定义什么也不做，因为声明有override，定义不写override，但实际上我们不应该移除const QSize &size为 QSize size
{
}
非标准分离写法
// 仅在.h头文件直接定义
 QSize sizeFromContents(ContentsType type,
                         const QStyleOption *option,
                         const QSize &size, // 有override不需要移除
                         const QWidget *widget) const override
{
}

const QString &
NXChatBubble::getMessageText() const
{
  return d_ptr->_pMessageText;
}
移除返回值的const QString &
QString
NXChatBubble::getMessageText() const
{
  return d_ptr->_pMessageText;
}
```

可能实现的算法，解析`返回值 函数名(参数列表)`注意可能存在多行代码才能得到一个完成的函数定义，如示例所示可能不是一行，函数可以分为声明和定义，声明末尾是分号，定义末尾是{左花括号，然后处理()括号内的参数列表，找到非const、volatile、&、*等修饰的`QS_TYPES`类型，除override的函数（注意标准分离头文件和源文件的写法定义不需要写override，但是声明需要写override，非标准分离文件则定义有override）以外将其添加`const &` 以 `const Type &`的形式

# 总结

进行合适的编排将功能1到6同时必须同时执行
