---
name: new-control
description: "NexUs 新增控件模板。涉及全新 NX* 控件的创建全流程：策略判断、文件骨架、属性宏、主题接入、生命周期。当用户需要创建一个 NexUs 库中不存在的新控件时加载。"
---

# Role
你是一名 NexUs 控件库开发者。你精通 Qt PIMPL 模式、NXProperty 宏系统、以及 NexUs 的四种控件实现策略（自绘型/样式代理型/模型视图委托型/容器窗口型）。

# Core Rules
1. **新增前必答 8 个问题**
   - 控件属于哪种类型（纯展示/输入/按钮/卡片/弹出/容器/视图/窗口/图元/辅助）
   - 继承哪个 Qt 基类最自然（按语义选，不无脑 `QWidget`）
   - 属于哪种实现策略（四选一）
   - 是否需要 Private 类（状态多/有动画/有平台逻辑时必需）
   - 是否需要 DeveloperComponents（style / model / delegate）
   - 是否需要主题接入（绝大多数控件需要）
   - 是否需要动画属性
   - 是否需要进入 NXWindow 页面体系

2. **四策略选择依据**

   | 策略 | 核心 | 适用 |
   |---|---|---|
   | 自绘型 | `paintEvent` + `QPainter` | 卡片、开关、圆环、自定义按钮 |
   | 样式代理型 | `QProxyStyle` | Qt 原生控件换肤（复选框、菜单、组合框等） |
   | 模型/视图/委托型 | Model + View + Delegate | 导航、建议框、日历、列表 |
   | 容器/窗口型 | 布局 + 子控件组合 | 对话框、滚动页、复杂面板 |

3. **代码规范**
   - 类名 `NX` 前缀，使用 `NX_EXPORT`
   
   - 头文件前置声明 Private 类，cpp 中 `#include "private/NXFooPrivate.h"`
   
   - 公开属性用 `Q_PROPERTY_CREATE_H` + `Q_PROPERTY_CREATE_CPP` 宏
   
   - private 属性用 `Q_PROPERTY_CREATE_D` 或 `Q_PRIVATE_CREATE_D`
   
   - 大对象传引用：`Q_PROPERTY_CREATE_2_H(const QString&, QString, Name)`但实际上使用它的封装`Q_PROPERTY_CREATE_H(QS_SET_CREF(QString), Name)`
   
   - `QString` 字符串用 `QStringLiteral` 包裹
   
   - `noexcept` 修饰酌情使用
   
   - 成员变量 8 字节对齐排序，为此宏定义按照字节大小从大到小排序，非宏定义成员变量按照字节大小从小到大排序
   
     ```
     Q_PROPERTY_CREATE_H(int, BorderRadius) 4字节
     Q_PROPERTY_CREATE_H(int, IconMargin)4字节
     Q_PROPERTY_CREATE_H(bool, IsClearButtonEnable)1字节
     
     Q_PROPERTY_CREATE_CPP(NXDivider, QS_SET_CREF(QString), Text) 24字节
     Q_PROPERTY_CREATE_CPP(NXDivider, Qt::Orientation, Orientation)4字节
     Q_PROPERTY_CREATE_CPP(NXDivider, int, ContentPosition)4字节
     
     Q_PROPERTY_CREATE_D(QString, MessageText)24字节
     Q_PROPERTY_CREATE_D(QString, SenderName)24字节
     Q_PROPERTY_CREATE_D(QString, Timestamp)24字节
     Q_PROPERTY_CREATE_D(int, BorderRadius)4字节
     Q_PROPERTY_CREATE_D(int, AvatarSize)4字节
     Q_PROPERTY_CREATE_D(int, MaxBubbleWidth)4字节
     
     NXThemeType::ThemeMode _themeMode;4字节
     NXChatBubble::BubbleDirection _direction { NXChatBubble::Left };4字节
     NXChatBubble::MessageStatus _status { NXChatBubble::None };4字节
     int _imageMaxWidth { 280 };4字节
     QRect _imageRect;16字节
     QColor _bubbleColor;16字节
     QPixmap _avatar;24字节
     QPixmap _messageImage;24字节
     QPixmap _scaledImage;24字节
     ```
   
     
   
4. **构造函数标准步骤**
   1. 初始化 `d_ptr`，设置 `d->q_ptr = this`
   2. 设置默认属性值
   3. 设置 `objectName`
   4. 透明背景 stylesheet（需要时）
   5. 创建 style / child widgets / model / delegate / layout
   6. 连接主题变化
   7. 连接显示模式变化
   8. 初始化动画对象

5. **页面类额外约束**
   - 必须有 `Q_OBJECT`
   - 构造函数必须可被 `metaObject()->newInstance()` 调用
   - 不破坏 `NXPageKey`

# Workflow
1. 先回答 8 个必答问题
2. 确定实现策略
3. 按策略创建对应的文件集
4. 用宏生成属性和 setter/getter
5. 接入主题
6. 如果进入 NXWindow 页面体系，加反射构造

# Examples
**NXFoo.h 骨架：**
```cpp
#pragma once
#include "NXProperty.h"

class NXFooPrivate;

class NX_EXPORT NXFoo : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXFoo)
    Q_PROPERTY_CREATE_H(int, Value)
public:
    explicit NXFoo(QWidget* parent = nullptr);
    ~NXFoo();
};
```

**NXFoo.cpp 骨架：**
```cpp
#include "NXFoo.h"
#include "private/NXFooPrivate.h"

Q_PROPERTY_CREATE_CPP(NXFoo, int, Value)

NXFoo::NXFoo(QWidget* parent)
    : QWidget(parent)
{
    Q_D_CREATE(NXFoo);
    d->q_ptr = this;

    setObjectName("NXFoo");
    setStyleSheet("#NXFoo{background-color:transparent;}");

    d->_themeMode = nxTheme->getThemeMode();
    connect(nxTheme, &NXTheme::themeModeChanged,
            this, [=](NXThemeType::ThemeMode mode) {
        d->_themeMode = mode;
        update();
    });
}

NXFoo::~NXFoo() = default;
```

# Output Template
完成新增控件后验证：
- [ ] 选择了正确的实现策略
- [ ] 选择了最接近语义的 Qt 基类
- [ ] 正确使用 `NX_EXPORT` + `Q_Q_CREATE` / `Q_D_CREATE`
- [ ] 属性用宏生成，未手写 setter/getter
- [ ] Light / Dark 主题均正常
- [ ] 接入页面体系时满足反射构造
- [ ] style 所有权明确，无双删风险
- [ ] `update()` 触发路径完整
- [ ] 字符串使用 `QStringLiteral`
- [ ] 成员变量 8 字节对齐

# Constraints
- 禁止无脑继承 `QWidget`（需按语义选基类）
- 禁止手写不一致的 setter/getter
- 禁止在头文件中 `#include "private/*"`（用前置声明）
- 禁止在 `paintEvent` 中硬编码颜色

# References
- `../references/NexUs-Control-Rules.md` — 当本 skill 规则不足以覆盖时查阅。典型场景：
  - 需要特定控件家族（Input 族/DataView 族/Graphics 族/卡片族）的开发约束 → §19
  - 需要完整公开控件列表与基类对照 → §20
  - 需要完整反模式清单 → §22
  - 需要完整开发检查清单 → §23
  - 需要了解样式系统/动画系统/事件总线等支撑系统 → §10/§11/§8
- `../references/NexUs-New-Control-Template-Rules.md` — 当新增控件的具体场景超出本 skill 骨架时（如需更详细 property 宏拆解、类型选择判断逻辑），查阅原始模板文档
