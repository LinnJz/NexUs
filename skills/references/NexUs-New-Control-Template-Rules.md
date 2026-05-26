# NexUs 新增控件模板规则

## 1. 文档目的

本文档不是解释 NexUs 的总体架构，而是提供一套可直接执行的“新增控件模板规则”。

目标是让 AI 大模型在收到“新增一个 NexUs 控件”的任务后，能够：

1. 先判断控件类型与实现层级。
2. 按 NexUs 既有架构创建正确的文件集合。
3. 按统一骨架生成头文件、实现文件、private 文件、style/model/delegate 文件。
4. 避免把功能写错层、漏主题、漏属性、漏动画、漏析构、漏协议。
5. 输出结果具备可维护性，能自然融入 NexUs，而不是生成一个表面可用但架构失配的控件。

本文档应与现有总规则文档配合使用：

- 总规则：负责解释 NexUs 是什么、各层职责是什么、修改现有控件应该改哪一层。
- 本模板规则：负责告诉大模型“新增控件时具体该怎么落地写代码”。

## 2. AI 新增控件时的总原则

大模型在新增控件时，必须先做结构判断，再写文件。

禁止直接进入“先写一个 QWidget 再慢慢补”。

必须按以下顺序执行：

1. 判断控件定位。
2. 判断继承基类。
3. 判断实现策略。
4. 判断是否需要 Private。
5. 判断是否需要 DeveloperComponents 支撑类。
6. 判断是否需要主题接入。
7. 判断是否需要动画属性。
8. 判断是否需要接入 `NXWindow` 页面体系。
9. 最后再生成文件骨架与代码。

## 3. 新增控件前的必答问题

AI 在开始写控件前，必须回答以下问题。

### 3.1 控件是什么类型

从以下类型中选择最接近的一类：

1. 纯展示控件
2. 输入控件
3. 按钮/选择控件
4. 卡片控件
5. 弹出控件
6. 容器控件
7. 视图控件
8. 顶层窗口控件
9. Graphics 图元/画布控件
10. 辅助组件或支撑组件

### 3.2 控件应继承哪个 Qt 基类

必须优先继承最接近语义的 Qt 基类，而不是无脑继承 `QWidget`。

错误示例：

- 复选类控件却继承 `QWidget`
- 工具按钮却继承 `QPushButton`
- 具备标准输入语义的控件却绕开原生输入基类

正确原则：

- 如果是按钮语义，优先 `QPushButton` / `QToolButton`
- 如果是文本输入，优先 `QLineEdit` / `QPlainTextEdit`
- 如果是勾选语义，优先 `QCheckBox` / `QRadioButton`
- 如果是容器或卡片，才考虑 `QWidget`
- 如果是顶层主窗口，优先 `QMainWindow`
- 如果是对话框，优先 `QDialog`
- 如果是图元，优先 `QGraphicsObject` / `QGraphicsItem` 体系

### 3.3 控件属于哪种实现策略

必须四选一，必要时组合：

1. 自绘型
   典型：卡片、圆环、开关、自定义按钮

2. 样式代理型
   典型：基于 Qt 原生控件换皮，使用 `QProxyStyle`

3. Model/View/Delegate 型
   典型：导航、建议框、日历、列表型复合控件

4. 容器组合型
   典型：对话框、滚动页、选择器、复合面板

如果判断不清，优先参考现有 NexUs 相近控件，而不是自由发挥。

### 3.4  编写控件是否符合基础代码编写要求

1. NexUs库大量方法都是使用了noexcept修饰方法，新控件撰写的方法是否需要包含noexcept，需要分析判断

2. NexUs库对于QString字符串统一使用了QStringLiteral包裹，注意的是`setProperty`、`property`、  `setObjectName`、`QPropertyAnimation构造`等因为不是QString的参数所以不需要包裹，编写代码含有字符串字面量时，需要分析判断QStringLiteral的使用

3. 遵循NexUs库编码风格，私有方法`下划线开头+驼峰命名`、全局方法`帕斯卡命名`、私有属性`下划线开头+驼峰命名`、由属性宏注册的私有属性`_p+帕斯卡命名`

4. 遵循NexUs库对于setter和getter的宏设置要求，大对象应该使用属性宏2，setter传入引用设置，如Q_PROPERTY_CREATE_2_H(const QString&, QString, Name)进行设置，pod/枚举类型则使用普通宏Q_PROPERTY_CREATE(int, Name)

5. 遵循NexUs库方法传参，对于大对象应该使用指针或者const TYPE&传递，避免拷贝

6. 遵循NexUs库的成员变量布局，成员变量结构布局以统一8字节布局，为了减小不必要的内存开销，bool、int和枚举小于8字节的类型都应该紧挨着

   ```C++
   // 错误写法
   int a;
   QString b;
   int c;
   
   // NexUs库写法
   int a;
   int c;
   QString b;
   ```

   

## 4. AI 的控件落层决策表

### 4.1 何时只创建 public 类

仅当控件非常简单，且满足以下条件时可以只写：

- `Source/include/NXFoo.h`
- `Source/NXFoo.cpp`

适用条件：

1. 状态极少。
2. 不需要复杂动画。
3. 不需要大量内部槽。
4. 不需要隐藏实现细节。
5. 不需要 model/style/delegate/container 等支撑类。

### 4.2 何时必须创建 private 类

满足以下任一条件，必须创建 private：

1. 公开属性较多。
2. 有多个内部状态字段。
3. 有动画对象。
4. 有多个子控件组合。
5. 有主题切换逻辑。
6. 有复杂事件逻辑。
7. 需要减少头文件暴露。

标准文件：

- `Source/include/NXFoo.h`
- `Source/NXFoo.cpp`
- `Source/private/NXFooPrivate.h`
- `Source/private/NXFooPrivate.cpp`

### 4.3 何时必须创建 DeveloperComponents 支撑类

满足以下场景之一，必须拆分到 `Source/DeveloperComponents`：

1. 需要自定义 `QProxyStyle`
2. 需要自定义 `QStyledItemDelegate`
3. 需要自定义 `QAbstractItemModel` / `QAbstractListModel`
4. 需要容器 widget 作为弹出层或内部承载层
5. 需要单独的辅助类承载动画视图或平台行为

## 5. 标准文件模板

下面给出 AI 新增控件时应优先遵循的文件模板。

## 5.1 标准模板 A：普通复杂控件

适合：

- 卡片
- 自绘输入壳层
- 复合控件
- 主题敏感控件

应创建：

1. `Source/include/NXFoo.h`
2. `Source/NXFoo.cpp`
3. `Source/private/NXFooPrivate.h`
4. `Source/private/NXFooPrivate.cpp`

### 5.1.1 头文件骨架

```cpp
#ifndef NXFOO_H
#define NXFOO_H

#include <QWidget>
#include "NXDef.h"

class NXFooPrivate;

class NX_EXPORT NXFoo : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXFoo)
  Q_PROPERTY_CREATE_H(bool, IsTransparent)
  Q_PROPERTY_CREATE_H(int, BorderRadius)
  Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)

public:
  explicit NXFoo(QWidget *parent = nullptr);
  ~NXFoo() override;

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // NXFOO_H
```

### 5.1.2 public cpp 骨架

```cpp
#include "NXFoo.h"

#include <QPainter>
#include "NXTheme.h"
#include "private/NXFooPrivate.h"

Q_PROPERTY_CREATE_CPP(NXFoo, bool, IsTransparent)
Q_PROPERTY_CREATE_CPP(NXFoo, int, BorderRadius)
Q_PROPERTY_CREATE_2_CPP(NXFoo, const QString&, QString, Title)

NXFoo::NXFoo(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new NXFooPrivate())
{
  Q_D(NXFoo);
  d->q_ptr = this;

  d->_pIsTransparent = false;
  d->_pBorderRadius  = 8;
  d->_pTitle         = {};

  setObjectName("NXFoo");
  setStyleSheet(QStringLiteral("#NXFoo{background-color:transparent;}"));

  d->_themeMode = nxTheme->getThemeMode();
  connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode)
  {
    d->_themeMode = themeMode;
    update();
  });
}

NXFoo::~NXFoo() { }

void NXFoo::paintEvent(QPaintEvent *event)
{
  Q_D(NXFoo);
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

  painter.setPen(Qt::NoPen);
  painter.setBrush(d->_pIsTransparent ? Qt::transparent : NXThemeColor(d->_themeMode, BasicBase));
  painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);

  QWidget::paintEvent(event);
}
```

### 5.1.3 private 头文件骨架

```cpp
#ifndef NXFOOPRIVATE_H
#define NXFOOPRIVATE_H

#include <QObject>
#include "NXDef.h"

class NXFoo;

class NXFooPrivate : public QObject
{
  Q_OBJECT
  Q_D_CREATE(NXFoo)
  Q_PROPERTY_CREATE_D(bool, IsTransparent)
  Q_PROPERTY_CREATE_D(int, BorderRadius)
  Q_PROPERTY_CREATE_D(QString, Title)

public:
  explicit NXFooPrivate(QObject *parent = nullptr);
  ~NXFooPrivate() override;

private:
  NXThemeType::ThemeMode _themeMode { NXThemeType::Light };
};

#endif // NXFOOPRIVATE_H
```

### 5.1.4 private cpp 骨架

```cpp
#include "NXFooPrivate.h"

NXFooPrivate::NXFooPrivate(QObject *parent)
    : QObject { parent }
{
}

NXFooPrivate::~NXFooPrivate() { }
```

## 5.2 标准模板 B：原生控件换皮型

适合：

- 新的按钮、列表、视图、输入框变体
- 主要改绘制和交互细节

应创建：

1. `Source/include/NXFoo.h`
2. `Source/NXFoo.cpp`
3. `Source/private/NXFooPrivate.h`
4. `Source/private/NXFooPrivate.cpp`
5. `Source/DeveloperComponents/NXFooStyle.h`
6. `Source/DeveloperComponents/NXFooStyle.cpp`

### 5.2.1 public 头文件骨架

```cpp
#ifndef NXFOO_H
#define NXFOO_H

#include <QPushButton>
#include "NXDef.h"

class NXFooPrivate;

class NX_EXPORT NXFoo : public QPushButton
{
  Q_OBJECT
  Q_Q_CREATE(NXFoo)
  Q_PROPERTY_CREATE_H(int, BorderRadius)

public:
  explicit NXFoo(QWidget *parent = nullptr);
  ~NXFoo() override;
};

#endif // NXFOO_H
```

### 5.2.2 public cpp 骨架

```cpp
#include "NXFoo.h"

#include "DeveloperComponents/NXFooStyle.h"
#include "private/NXFooPrivate.h"

NXFoo::NXFoo(QWidget *parent)
    : QPushButton(parent)
    , d_ptr(new NXFooPrivate())
{
  Q_D(NXFoo);
  d->q_ptr = this;
  d->_style = new NXFooStyle(style());

  setObjectName("NXFoo");
  setStyle(d->_style);
}

NXFoo::~NXFoo()
{
  Q_D(NXFoo);
  delete d->_style;
}
```

### 5.2.3 style 头文件骨架

```cpp
#ifndef NXFOOSTYLE_H
#define NXFOOSTYLE_H

#include <QProxyStyle>
#include "NXDef.h"

class NXFooStyle : public QProxyStyle
{
  Q_OBJECT
  Q_PROPERTY_CREATE(int, BorderRadius)

public:
  explicit NXFooStyle(QStyle *style = nullptr);
  ~NXFooStyle() override;

  void drawControl(ControlElement element,
                   const QStyleOption *option,
                   QPainter *painter,
                   const QWidget *widget = nullptr) const override;

private:
  NXThemeType::ThemeMode _themeMode { NXThemeType::Light };
};

#endif // NXFOOSTYLE_H
```

### 5.2.4 style cpp 骨架

```cpp
#include "NXFooStyle.h"

#include <QPainter>
#include <QStyleOption>
#include "NXTheme.h"

NXFooStyle::NXFooStyle(QStyle *style)
    : QProxyStyle(style)
{
  _themeMode = nxTheme->getThemeMode();
  connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode)
  {
    _themeMode = themeMode;
  });
}

NXFooStyle::~NXFooStyle() { }

void NXFooStyle::drawControl(ControlElement element,
                             const QStyleOption *option,
                             QPainter *painter,
                             const QWidget *widget) const
{
  switch (element)
  {
  case QStyle::CE_PushButton:
  {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(NXThemeColor(_themeMode, BasicBase));
    painter->drawRoundedRect(option->rect.adjusted(1, 1, -1, -1), _pBorderRadius, _pBorderRadius);
    painter->restore();
    return;
  }
  default:
  {
    break;
  }
  }

  QProxyStyle::drawControl(element, option, painter, widget);
}
```

## 5.3 标准模板 C：Model/View/Delegate 型控件

适合：

- 树状、列表状、建议框、导航、选择器、日历扩展

应创建：

1. `Source/include/NXFoo.h`
2. `Source/NXFoo.cpp`
3. `Source/private/NXFooPrivate.h`
4. `Source/private/NXFooPrivate.cpp`
5. `Source/DeveloperComponents/NXFooModel.h/.cpp`
6. `Source/DeveloperComponents/NXFooDelegate.h/.cpp` 或 `NXFooStyle.h/.cpp`
7. 必要时 `Source/DeveloperComponents/NXFooView.h/.cpp`

### 5.3.1 骨架原则

1. public 控件只负责对外 API 和总装配。
2. model 保存数据。
3. delegate/style 绘制条目。
4. view 负责鼠标、滚动、拖拽、tooltip 等交互。
5. private 负责粘合层逻辑、状态同步、动画、临时缓存。

### 5.3.2 最小 public 组装骨架

```cpp
class NX_EXPORT NXFoo : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXFoo)

public:
  explicit NXFoo(QWidget *parent = nullptr);
  ~NXFoo() override;
};
```

```cpp
NXFoo::NXFoo(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new NXFooPrivate())
{
  Q_D(NXFoo);
  d->q_ptr = this;

  d->_model    = new NXFooModel(this);
  d->_view     = new NXFooView(this);
  d->_delegate = new NXFooDelegate(this);

  d->_view->setModel(d->_model);
  d->_view->setItemDelegate(d->_delegate);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(d->_view);
}
```

## 5.4 标准模板 D：`NXWindow` 页面型控件

适合：

- 要作为 `NXWindow` 的 page 被注册
- 可能被“在新窗口打开”
- 可能被回溯重建

### 5.4.1 头文件骨架

```cpp
#ifndef NXFoopage_H
#define NXFoopage_H

#include <QWidget>
#include "NXDef.h"

class NXFooPagePrivate;

class NX_EXPORT NXFooPage : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXFooPage)

public:
  Q_INVOKABLE explicit NXFooPage(QWidget *parent = nullptr);
  ~NXFooPage() override;
};

#endif // NXFoopage_H
```

### 5.4.2 页面型硬规则

1. 必须 `Q_OBJECT`。
2. 必须有 `Q_INVOKABLE` 构造函数，通常是 `explicit NXFooPage(QWidget *parent = nullptr)`。
3. 不要破坏 `NXPageKey` 动态属性。
4. `windowTitle()` / `windowIcon()` 最好具备明确值，方便新窗口和页签展示。

## 6. 新控件命名规则

### 6.1 类命名

1. 公开类统一使用 `NX` 前缀。
2. private 类命名为 `NXFooPrivate`。
3. style 类命名为 `NXFooStyle`。
4. model 类命名为 `NXFooModel`。
5. delegate 类命名为 `NXFooDelegate`。
6. view 类命名为 `NXFooView`。
7. container / helper 命名应与职责一致，如：
   - `NXFooContainer`
   - `NXFooHeader`
   - `NXFooAnimationWidget`

### 6.2 文件命名

必须一一对应：

- `NXFoo.h`
- `NXFoo.cpp`
- `NXFooPrivate.h`
- `NXFooPrivate.cpp`
- `NXFooStyle.h`
- `NXFooStyle.cpp`

禁止出现：

- 类名和文件名不对应
- `WidgetHelper2` 这类无语义文件名
- `NewControl`, `CustomControl`, `TestControl` 这类临时命名

## 7. 新控件公开属性模板规则

AI 在新增公开属性时，必须优先用 NexUs 的属性宏。

### 7.1 通用属性模板

```cpp
Q_PROPERTY_CREATE_H(bool, IsTransparent)
Q_PROPERTY_CREATE_H(int, BorderRadius)
Q_PROPERTY_CREATE_H(int, Spacing)
Q_PROPERTY_CREATE_H(int, TextPixelSize)
Q_PROPERTY_CREATE_H(NXTextType::TextStyle, TextStyle)
Q_PROPERTY_CREATE_2_H(const QString&, QString, Title)
Q_PROPERTY_CREATE_2_H(const QColor&, QColor, AccentColor)
Q_PROPERTY_CREATE_2_H(const QPixmap&, QPixmap, IconPixmap)
```

### 7.2 属性选择规则

1. UI 可配置且需要外部访问的，放 public 属性。
2. 纯内部状态不要暴露 public 属性。
3. 能复用现有命名的不要发明新词。
4. Bool 命名统一 `IsXxx`。
5. 几何尺寸优先 `Width/Height/Radius/Margin/Spacing`。
6. 文本统一 `Title/SubTitle/Text/...`。
7. 图标优先 `NXIcon` 或 `NXIconType::IconName`，图片才使用 `QPixmap/QImage`。

## 8. 新控件主题接入模板

任何新控件默认都要接入主题。

### 8.1 最小主题模板

```cpp
d->_themeMode = nxTheme->getThemeMode();
connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode)
{
  d->_themeMode = themeMode;
  update();
});
```

### 8.2 如果需要同步 palette

```cpp
connect(nxTheme, &NXTheme::themeModeChanged, d, &NXFooPrivate::onThemeChanged);
```

```cpp
void NXFooPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode) noexcept
{
  Q_Q(NXFoo);
  _themeMode = themeMode;
  QPalette palette = q->palette();
  palette.setColor(QPalette::WindowText, NXThemeColor(_themeMode, BasicText));
  q->setPalette(palette);
}
```

### 8.3 主题实现规则

1. 不允许只支持 Light，不支持 Dark。
2. 不允许把主题变化仅写死在样式表字符串里。
3. 如果图标、背景资源、movie、hover 色会受主题影响，也要联动。
4. 颜色优先取自 `NXThemeColor(...)`。

## 9. 新控件动画模板规则

如果新控件存在 hover、expand、select、open、close、switch 等过渡，优先使用 `QPropertyAnimation`。

### 9.1 动画属性模板

如果动画只影响内部状态，优先将属性挂在 private 或 style 上。

```cpp
Q_PROPERTY_CREATE(qreal, HoverOpacity)
Q_PROPERTY_CREATE(int, ExpandOffset)
Q_PROPERTY_CREATE(qreal, Rotate)
```

### 9.2 动画创建模板

```cpp
QPropertyAnimation *animation = new QPropertyAnimation(d, "pHoverOpacity");
connect(animation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
animation->setDuration(250);
animation->setEasingCurve(QEasingCurve::OutCubic);
animation->setStartValue(0.0);
animation->setEndValue(1.0);
animation->start(QAbstractAnimation::DeleteWhenStopped);
```

### 9.3 动画规则

1. 动画属性归属要明确。
2. 动画结束后如果要恢复临时状态，必须处理 `finished`。
3. 中间 pixmap、临时隐藏控件、overlay 必须清理。
4. 不要同时存在多套重复动画驱动同一个视觉状态。

## 10. 样式型控件开发规则

适用于 `QProxyStyle` 实现。

### 10.1 AI 必须遵守的分工

1. public 控件
   负责安装 style、暴露 API。

2. private
   负责 style 指针所有权、辅助状态。

3. style
   负责：
   - `drawControl`
   - `drawPrimitive`
   - `sizeFromContents`
   - `pixelMetric`
   - `subElementRect`

### 10.2 样式型新增规则

1. 若当前控件已经是 style 驱动，新增视觉功能优先改 style。
2. 不要为 style 驱动控件再额外重写整套 `paintEvent()`，除非你明确要放弃 style 体系。
3. style 中只允许放绘制、度量、与绘制直接相关的动画。
4. 业务数据不要塞进 style。

## 11. 自绘型控件开发规则

适用于 `paintEvent()` 为主的控件。

### 11.1 标准绘制模板

```cpp
void NXFoo::paintEvent(QPaintEvent *event)
{
  Q_D(NXFoo);
  QPainter painter(this);
  painter.save();
  painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

  painter.setPen(Qt::NoPen);
  painter.setBrush(NXThemeColor(d->_themeMode, BasicBase));
  painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), d->_pBorderRadius, d->_pBorderRadius);

  painter.restore();
  QWidget::paintEvent(event);
}
```

### 11.2 自绘型规则

1. 绘制前后要 `save/restore`。
2. 默认开启抗锯齿和文本抗锯齿。
3. 若控件有阴影，优先复用 `nxTheme->drawEffectShadow(...)`。
4. 绘制矩形边框时要统一考虑 `adjusted()`，避免边缘裁切。
5. 文字省略、图标混排要显式计算布局，不要依赖默认 `drawText` 侥幸对齐。

## 12. 容器型控件开发规则

适用于多子控件组合的复合控件。

### 12.1 容器构造标准顺序

1. 初始化 private。
2. 设置 objectName / 透明背景。
3. 创建子控件。
4. 创建 layout。
5. 设置 margin / spacing。
6. connect 子控件信号。
7. connect 主题。
8. 初始化默认状态。

### 12.2 容器型规则

1. 子控件优先复用现有 NexUs 控件。
2. 不要为了一个按钮重新造新按钮类，优先用 `NXPushButton` / `NXToolButton` / `NXIconButton`。
3. 不要把布局构建与复杂行为状态机糅在一起，复杂行为下沉到 private。

## 13. `NXWindow` 页面型模板规则

如果 AI 收到需求是“新增一个可被 `NXWindow` 使用的页面控件”，必须追加以下规则。

### 13.1 页面型硬要求

1. 构造函数可反射创建。
2. 不破坏 `NXPageKey`。
3. 页面内部不要直接依赖特定 `NXWindow` 私有成员。
4. 页面应尽量自包含，可被普通父控件承载。

### 13.2 页面型推荐骨架

```cpp
class NX_EXPORT NXExamplePage : public QWidget
{
  Q_OBJECT
  Q_Q_CREATE(NXExamplePage)

public:
  Q_INVOKABLE explicit NXExamplePage(QWidget *parent = nullptr);
  ~NXExamplePage() override;
};
```

```cpp
NXExamplePage::NXExamplePage(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new NXExamplePagePrivate())
{
  Q_D(NXExamplePage);
  d->q_ptr = this;

  setObjectName("NXExamplePage");
  setWindowTitle(QStringLiteral("Example"));
}
```

## 14. AI 生成代码时的必做检查

生成新增控件代码后，AI 必须自查以下项目。

### 14.1 文件层级自查

1. 是否选对了 Qt 基类。
2. 是否创建了必要的 private 文件。
3. 是否创建了必要的 style/model/delegate 文件。
4. 文件命名是否与类名一致。

### 14.2 主题自查

1. 是否缓存 `_themeMode`。
2. 是否连接 `themeModeChanged`。
3. 是否使用 `NXThemeColor(...)`。
4. 主题变化后是否刷新 UI。

### 14.3 属性自查

1. 公开属性是否使用宏。
2. private 状态是否放到了 private。
3. setter 是否会导致必要刷新。
4. 信号命名是否与现有风格一致。

### 14.4 生命周期自查

1. style 是否有明确所有权。
2. 子控件 parent 是否正确。
3. 动画是否 `DeleteWhenStopped`。
4. 临时资源是否会清理。

### 14.5 页面型自查

1. 是否 `Q_OBJECT`。
2. 是否 `Q_INVOKABLE` 构造。
3. 是否兼容 `metaObject()->newInstance()`。
4. 是否不会破坏 `NXPageKey` 协议。

## 15. AI 生成代码时的禁止事项

以下行为应视为不合格输出：

1. 无脑继承 `QWidget`。
2. 不创建 private 却在 public 类里塞满状态和逻辑。
3. 不接入主题系统。
4. style 型控件却直接重写全部 `paintEvent`。
5. 页面型控件却没有 `Q_INVOKABLE` 构造。
6. public 头文件中暴露大量不必要实现细节。
7. 大量硬编码颜色而不使用主题色。
8. 复用现有 NexUs 控件的地方却重新造轮子。
9. 不明确 style 所有权。
10. 修改现有协议字段名，如 `NXPageKey`、`NXIconType` 等。

## 16. 面向 AI 的推荐输出格式

当 AI 被要求“新增一个 NexUs 控件”时，建议它按如下顺序完成输出和实现：

1. 先声明控件分类和基类选择。
2. 说明采用的实现策略。
3. 列出将创建的文件。
4. 按模板生成代码。
5. 检查主题、属性、析构、动画、页面协议。
6. 最后再补充可选增强项。

## 17. 一份 AI 可直接遵循的最小执行模板

下面这段可以作为 AI 的内部执行提示词模板。

```text
你正在为 NexUs 新增一个控件。

先完成以下判断：
1. 该控件属于哪一类。
2. 应继承哪个 Qt 基类。
3. 应采用自绘、样式代理、Model/View/Delegate、容器组合中的哪一种实现。
4. 是否需要 private 类。
5. 是否需要 style/model/delegate/container 辅助类。
6. 是否需要主题接入。
7. 是否需要动画属性。
8. 是否要作为 NXWindow 页面被 newInstance() 反射创建。

然后按 NexUs 规范创建标准文件骨架：
- include/NXFoo.h
- NXFoo.cpp
- private/NXFooPrivate.h
- private/NXFooPrivate.cpp
- 如有需要，再创建 DeveloperComponents/NXFooStyle.h/.cpp 或 Model/Delegate/View。

生成代码时必须：
- 使用 Q_Q_CREATE / Q_D_CREATE
- 使用 Q_PROPERTY_CREATE_H / CPP / D 宏
- 接入 NXTheme
- 使用 NXThemeColor
- 控制 style 所有权
- 保证页面类可 Q_INVOKABLE 构造
- 不破坏 NXPageKey 等协议
```

## 18. 总结

新增 NexUs 控件时，AI 最重要的能力不是“写出一个能显示的 widget”，而是：

1. 选对基类。
2. 选对实现策略。
3. 选对落层位置。
4. 建立正确文件结构。
5. 接入主题、属性、动画、协议。
6. 让控件自然融入 NexUs 的整体架构。

只要 AI 严格按本模板规则执行，生成的新控件就不会偏离 NexUs 的工程结构，也更适合后续继续扩展、修复和维护。
