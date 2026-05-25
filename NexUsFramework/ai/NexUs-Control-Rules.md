# NexUs 控件开发 Rules

## 1. 文档目的

本规则文档用于指导大模型或开发者在 NexUs 中进行以下工作：

1. 新增控件。
2. 为现有控件增加新功能。
3. 修复现有控件 Bug。
4. 对现有控件做主题、样式、交互、动画、路由、窗口行为方面的调整。
5. 判断一个需求应该落在哪一层实现，而不是在错误层面补丁式修改。

这份文档基于 `Source` 目录的实现规律整理，目标不是单纯解释 API，而是给出 NexUs 的真实工程规则、架构边界、常见实现模式、风险点和执行步骤。

## 2. Source 目录的真实结构

`Source` 目录并不是“公开头文件 + 若干 cpp”这么简单，而是一个明显分层的控件框架。

### 2.1 目录分层

1. `Source/include`
   对外公开 API、导出类、枚举、属性定义、宏定义、单例定义、基础工具。

2. `Source/private`
   PIMPL 私有实现层，保存状态、私有槽函数、动画对象、辅助函数、平台相关中转逻辑。

3. `Source/DeveloperComponents`
   开发支撑层，不直接面向最终用户，主要承载：
   - `QProxyStyle` 样式类
   - `QStyledItemDelegate` 委托类
   - `QAbstractItemModel` / `QAbstractListModel` 模型类
   - 容器、浮层、标题栏、遮罩、动画组件
   - Windows 平台辅助

4. `Source/*.cpp`
   公开控件主实现层，负责组装 private、DeveloperComponents、连接主题、构建布局、暴露 API。

### 2.2 当前代码规模

基于 `Source` 统计：

- `TotalFiles = 456`
- `include = 88`
- `private = 154`
- `DeveloperComponents = 130`
- 根目录 `cpp = 81`

这说明 NexUs 不是单层控件库，而是以“公共接口层 + 私有状态层 + 样式/模型/委托支撑层”构成的中大型组件体系。

## 3. NexUs 的总架构结论

### 3.1 最核心的三层模型

绝大多数控件遵循以下架构：

1. 公共控件类
   例如 `NXWindow`、`NXCheckBox`、`NXToolButton`、`NXNavigationBar`。
   这层负责对外接口、Qt 属性、公开信号、公开行为。

2. Private 私有实现类
   例如 `NXWindowPrivate`、`NXCheckBoxPrivate`、`NXToolButtonPrivate`。
   这层负责：
   - 私有状态保存
   - 主题缓存
   - 动画对象
   - 私有槽函数
   - 内部数据结构
   - 复杂逻辑拆分

3. DeveloperComponents 支撑类
   常见形态：
   - `NXCheckBoxStyle`
   - `NXToolButtonStyle`
   - `NXNavigationStyle`
   - `NXNavigationModel`
   - `NXNavigationNode`
   - `NXFooterDelegate`
   - `NXComboBoxView`

### 3.2 不同控件的实现策略并不统一

NexUs 至少有 4 种高频实现形态：

1. 直接自绘型
   基于 `paintEvent` 手工绘制，例如：
   - `NXPushButton`
   - `NXText`
   - `NXToggleSwitch`
   - `NXProgressRing`
   - `NXAcrylicUrlCard`
   - `NXInteractiveCard`

2. 样式代理型
   通过 `QProxyStyle` 接管绘制，例如：
   - `NXCheckBox`
   - `NXToolButton`
   - `NXGroupBox`
   - `NXTableView`
   - `NXTreeView`
   - `NXTabBar`
   - `NXMenu`
   - `NXToolBar`

3. 模型/视图/委托型
   通过 `QAbstractItemModel + View + Delegate/Style` 组合实现，例如：
   - `NXNavigationBar`
   - `NXBreadcrumbBar`
   - `NXCalendar`
   - `NXPivot`
   - `NXSuggestBox`
   - `NXColorDialog` 的颜色展示区

4. 顶层窗口/容器型
   重点在窗口框架、路由、导航、原生事件、动画切换，例如：
   - `NXWindow`
   - `NXWidget`
   - `NXDialog`
   - `NXContentDialog`
   - `NXDockWidget`

结论：新增或改动控件时，第一步不是写代码，而是先判断该控件属于哪一种实现策略。

## 4. 基础设施规则

## 4.1 属性宏系统是第一原则

NexUs 强依赖 `NXProperty.h` 和 `LinnProperty.h` 的宏。

关键宏：

- `Q_Q_CREATE(CLASS)`
- `Q_D_CREATE(CLASS)`
- `Q_PROPERTY_CREATE_H(...)`
- `Q_PROPERTY_CREATE_CPP(...)`
- `Q_PROPERTY_CREATE_D(...)`
- `Q_PRIVATE_CREATE(...)`
- `Q_PRIVATE_CREATE_D(...)`

### 4.1.1 这些宏做了什么

1. 自动生成 Qt 属性。
2. 自动生成 setter/getter。
3. 自动生成变更信号。
4. 在 PIMPL 模式下把真实存储放到 private 类中。
5. 允许通过宏携带默认值、契约、是否触发 `update()`、信号参数等策略。

### 4.1.2 规则

1. 公共类头文件中的公开属性，优先使用 `Q_PROPERTY_CREATE_H`。
2. 对应实现放在 `.cpp` 中，优先使用 `Q_PROPERTY_CREATE_CPP`。
3. private 类中的实际字段，优先使用 `Q_PROPERTY_CREATE_D` 或 `Q_PRIVATE_CREATE_D`。
4. 如果属性需要变更信号，走 `Q_PROPERTY_CREATE_*`。
5. 如果只是 private 内部状态，不需要对外通知，走 `Q_PRIVATE_CREATE_*` 或普通字段。
6. 当 setter 后必须刷新界面时，应保证最终会触发 `update()`，要么在 setter 里手写，要么通过宏策略做。
7. NexUs 现有代码多数 setter 不做“旧值比较后再 emit”，新增代码应根据行为成本决定是否增加比较；如果不比较，必须确认重复触发不会带来动画抖动或重布局风暴。

### 4.1.3 重要含义

大模型在新增属性时，不能只加成员变量。必须同步考虑：

1. 是否需要 Qt 属性暴露。
2. 是否需要通知信号。
3. 是否应该存放在 public 类还是 private 类。
4. 是否影响绘制、布局、主题、动画。

## 4.2 PIMPL 是主实现组织方式

大量控件都遵循：

- 公共类：`NXFoo`
- 私有类：`NXFooPrivate`
- 公开类持有 `d_ptr`
- private 类持有 `q_ptr`
- 通过 `Q_D(NXFoo)` / `Q_Q(NXFoo)` 访问

### 4.2.1 什么时候必须建 private 类

以下场景必须用 private：

1. 状态较多。
2. 有多个动画对象。
3. 有较多内部槽函数。
4. 需要隔离平台相关逻辑。
5. 控件本身承担对外 API，不希望头文件暴露实现细节。

### 4.2.2 什么时候可以不把逻辑塞进 private

如果控件非常薄，仅仅是样式封装，也可以让 public 类逻辑更直接，但 NexUs 现有总体倾向仍然是走 private。

### 4.2.3 规则

1. 新增复杂控件时，默认建 `private/NXFooPrivate.h/.cpp`。
2. 公共类只保留对外 API、生命周期、事件重载入口。
3. internal slot、复杂数据结构、缓存、动画实例放 private。
4. 不要把大段布局构建和行为状态机混在 public 头文件里。

## 4.3 单例基础设施

NexUs 使用 `LinnSingleton.h` 生成单例。

核心单例：

- `NXTheme`
- `NXApplication`
- `NXNavigationRouter`
- `NXEventBus`
- `NXIcon`

### 4.3.1 规则

1. 全局状态只放入真正全局的对象。
2. 控件自身状态不要滥用单例。
3. 如果新需求只是某个窗口、某个控件树内部共享，不要新增全局单例。

## 4.4 基础代码编写要求

1. NexUs库大量方法都是使用了noexcept修饰方法

2. NexUs库对于QString字符串统一使用了QStringLiteral包裹，注意的是`setProperty`、`property`、  `setObjectName`、`QPropertyAnimation构造`等因为不是QString的参数所以不需要包裹

3. NexUs库编码风格是私有方法`下划线开头+驼峰命名`、全局方法`帕斯卡命名`、私有属性`下划线开头+驼峰命名`、由属性宏注册的私有属性`_p+帕斯卡命名`

4. NexUs库对于setter和getter的宏设置要求大对象应该使用属性宏2，setter传入引用设置，如Q_PROPERTY_CREATE_2_H(const QString&, QString, Name)进行设置，pod/枚举类型则使用普通宏Q_PROPERTY_CREATE(int, Name)

5. NexUs库方法传参对于大对象应该使用指针或者const TYPE&传递，避免拷贝

6. NexUs库的成员变量排序都遵循合理的结构布局以统一8字节布局，减小不必要的内存开销，即bool、int和枚举小于8字节的类型都应该紧挨着

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

   

## 5. 主题系统规则

主题系统是 NexUs 中最稳定、最广泛的横切机制。

## 5.1 主题入口

- 单例：`NXTheme`
- 宏：`nxTheme`
- 取色宏：`NXThemeColor(themeMode, ThemeColorName)`
- 主题枚举定义于 `NXDef.h`

### 5.1.1 主题色来源

`NXThemePrivate::_initThemeColor()` 初始化了 Light / Dark 两套颜色。

涉及类别：

- 窗口底色
- 弹出层底色
- 对话框底色
- 基础文字和边框
- Hover / Press / Selected 透明色
- 状态色
- TabBar 色
- Win10 边框色

### 5.1.2 规则

1. 任何新控件都必须支持明暗主题。
2. 禁止在控件绘制中硬编码业务无关颜色，除非是明确状态色并且规则文档有说明。
3. 通用颜色优先使用 `NXThemeColor(...)`。
4. 如果现有主题枚举足够，不要新增颜色枚举。
5. 只有在多个控件复用且语义稳定时，才新增 `NXThemeType::ThemeColor` 条目。

## 5.2 主题连接模式

现有代码主要有两种写法：

1. public 类直接连接：
   `connect(nxTheme, &NXTheme::themeModeChanged, this, [=](...) { ... update(); })`

2. private 类接收主题变化：
   `connect(nxTheme, &NXTheme::themeModeChanged, d, &NXFooPrivate::onThemeChanged)`

### 5.2.1 规则

1. 如果主题变化只影响简单重绘，public lambda 即可。
2. 如果主题变化还要同步 palette、style、子控件状态，写 private slot。
3. 控件内部应缓存 `_themeMode`，避免每次绘制反复查询。
4. 主题变化后，如果 UI 依赖 palette 或 style，除了 `update()`，还要同步 palette/style 状态。

### 5.2.2 典型例子

- `NXTextPrivate::onThemeChanged` 修改 `QPalette::WindowText`
- `NXComboBoxPrivate::onThemeChanged` 同步编辑框配色
- `NXWindowPrivate::onThemeModeChanged` 触发背景 movie 切换与窗口重绘

## 5.3 主题变更不只影响颜色

在 NexUs 中，主题变化还可能影响：

1. 图标选择
   例如 AppBar 在 Light / Dark 间切换月亮/太阳图标。

2. 背景资源
   例如 `NXWindow` 的 movie / pixmap。

3. 窗口显示模式
   例如 Win32 DWM 主题同步。

4. Mica 基底图
   由 `NXApplication` 同步。

结论：新增功能涉及主题时，不要只改颜色；要检查图标、背景资源、原生窗口状态是否也需要联动。

## 6. 应用与窗口层规则

## 6.1 NXApplication 是窗口显示模式和全局初始化入口

`NXApplication::init()` 负责：

1. 载入资源 `Q_INIT_RESOURCE(NexUs)`。
2. 加载字体 `NXAwesome.ttf` 和 `segoe_slboot.ttf`。
3. 设置应用默认字体。
4. 初始化 Windows 平台能力。
5. 与系统主题同步。

### 6.1.1 规则

1. 新控件如果依赖 `NXAwesome` 图标字体，应假设 `nxApp->init()` 已执行。
2. 如果一个新功能依赖应用级主题同步或窗口材质效果，优先挂到 `NXApplication`，不要散落到各控件。
3. 任何顶层窗口都应通过 `nxApp->syncWindowDisplayMode(widget)` 参与显示模式同步。

## 6.2 顶层窗口统一依赖 NXAppBar

以下类都围绕 `NXAppBar` 组织：

- `NXWindow`
- `NXWidget`
- `NXDialog`
- `NXDockWidget`

### 6.2.1 NXAppBar 负责什么

1. 自定义无边框标题栏。
2. 最小化、最大化、关闭、置顶、主题切换、导航按钮。
3. Windows 下原生事件接管。
4. hit test、窗口拖动、系统菜单、边缘 resize。
5. 顶层窗口的内容边距管理。

### 6.2.2 规则

1. 新顶层控件如果需要 NexUs 风格标题栏，优先复用 `NXAppBar`，不要自己另写一套。
2. 如果需求涉及窗口拖拽、最大化、系统菜单、hit test，改 `NXAppBar` 或其 private，而不是在业务控件中绕实现。
3. `Q_TAKEOVER_NATIVEEVENT_H/CPP` 宏是顶层窗口接管 Win32 原生事件的标准入口，不要自行复制一套 `nativeEvent` 模板。

## 6.3 NXWindow 是完整壳层，不只是窗口

`NXWindow` 内部组合：

1. `NXAppBar`
2. `NXNavigationBar`
3. `NXCentralStackedWidget`
4. `NXNavigationRouter`
5. `NXEventBus`
6. `NXWindowStyle`

它是一个“带路由、导航、中心页堆栈、主题动画、可浮动导航”的壳层框架。

### 6.3.1 规则

1. 如果新增页面型业务界面，优先让它作为 `NXWindow` 的 page，而不是自建顶层窗口框架。
2. 如果需求是“窗口内多页切换 + 回退前进 + 侧边导航”，直接走 `NXWindow` 体系。
3. 如果需求只是简单弹窗，不要强上 `NXWindow`，考虑 `NXDialog` / `NXContentDialog`。

## 7. 导航与路由规则

## 7.1 导航不是纯 UI，而是模型驱动

`NXNavigationBar` 由以下几层组成：

1. `NXNavigationModel`
2. `NXNavigationNode`
3. `NXNavigationView`
4. `NXNavigationStyle`
5. `NXFooterModel`
6. `NXFooterDelegate`

### 7.1.1 规则

1. 新增导航节点能力，优先改 `NXNavigationModel` / `NXNavigationNode`。
2. 新增导航显示效果，优先改 `NXNavigationStyle` / `NXNavigationView`。
3. 新增导航条外部 API，改 `NXNavigationBar`。
4. 不要直接从 `NXNavigationBar` 外部操作内部视图选中状态；应通过 `navigation(...)`、`addPageNode(...)` 等公开接口驱动。

## 7.2 导航节点的真实约束

节点类型来自 `NXDef.h`：

- `PageNode`
- `FooterNode`
- `CategoryNode`
- `ExpanderNode` 不是独立枚举，但在模型中是重要行为类型

### 7.2.1 规则

1. 只有 expander 节点可以拥有子节点。
2. 分类节点是展示分组，不是页面。
3. 页脚节点由独立 footer model 管理。
4. 层级深度上限在模型中被限制，新增深层树逻辑要尊重深度规则。

## 7.3 路由器只存“对象 + 函数名 + 数据”

`NXNavigationRouter` 保存的不是页面实例，而是：

- `QObject* routeObject`
- `QString routeFunctionName`
- `QVariantMap routeData`
- `Qt::ConnectionType`

通过 `QMetaObject::invokeMethod(...)` 回放。

### 7.3.1 规则

1. 路由接收函数必须能被 `invokeMethod` 调用，通常用 `Q_INVOKABLE` 或 slot。
2. 路由数据应全部放入 `QVariantMap`。
3. 路由 key 命名要与现有模式一致，例如：
   - `NXBackPageKey`
   - `NXForwardPageKey`
   - `NXBackCentralStackIndex`
   - `NXForwardCentralStackIndex`
   - `NXRouteBackMode`
4. 新增路由功能优先复用现有键命名风格，不要随意发明新的不成体系命名。
5. 新代码优先使用“每窗口独立路由”接口，不再扩散全局兼容接口。

## 7.4 页面类若要支持新窗口打开或回溯重建，必须可反射构造

`NXNavigationBarPrivate::onNavigationOpenNewWindow()` 与 `NXWindow::backtrackNavigationNode()` 都会调用：

- `metaObject()->newInstance()`

这意味着页面类若希望支持：

1. 在新窗口中打开
2. 页面回溯重建

则必须满足：

1. 类有 `Q_OBJECT`
2. 有可被元对象系统调用的构造函数，通常是 `Q_INVOKABLE explicit Class(QWidget *parent = nullptr)` 或兼容的无参可调用构造

### 7.4.1 这是硬规则

如果页面没有可反射构造能力：

- 新窗口打开会失败
- 页面回溯替换会失败

大模型在新增 `NXWindow` 页面时，必须优先检查这一点。

## 7.5 页面识别依赖 `NXPageKey`

导航体系、suggest 数据、新窗口统计、回溯替换都依赖控件动态属性：

- `NXPageKey`

### 7.5.1 规则

1. 页面加入导航后，不要再手动覆盖 `NXPageKey`。
2. 所有涉及页面复用、回溯、悬浮新窗、统计的逻辑都应保留该属性。
3. 若替换页面实例，必须把原 `NXPageKey` 复制给新实例。

## 8. 事件总线规则

## 8.1 NXEventBus 的定位

`NXEventBus` 用于“字符串事件名 + QVariantMap 数据 + invokeMethod 分发”的轻量事件通信。

核心对象：

- `NXEvent`
- `NXEventBus`

### 8.1.1 现有用法

典型事件：`WMWindowClicked`

被用于：

- `NXWindow`
- `NXLineEdit`
- `NXPlainTextEdit`
- `NXMultiCellLineEdit`

多用于窗口点击、焦点外部检测、输入框行为联动。

### 8.1.2 规则

1. 只有跨控件、跨层、弱耦合事件才用 `NXEventBus`。
2. 同一对象树内的直接关系优先用 Qt signal/slot。
3. 事件接收函数需可被 `invokeMethod` 调用。
4. 事件名必须稳定、明确、避免业务临时字符串污染全局空间。

## 9. 图标系统规则

## 9.1 NXAwesome 是统一图标来源

`NXIcon` 通过字体 `NXAwesome` 把 `NXIconType::IconName` 渲染为 `QIcon`。

### 9.1.1 常见用法

1. 直接 `setNXIcon(...)`
2. 在 `QToolButton` / `QMenu` / `QToolBar` 的 `property("NXIconType")` 中保存字符
3. 样式类在绘制时读取 `NXIconType`

### 9.1.2 规则

1. 不要在 NexUs 基础控件中随意引入散乱的 png/svg 图标作为主路径。
2. 优先使用 `NXIconType` 枚举。
3. 如果样式类已通过 `property("NXIconType")` 绘制图标，就不要额外重复设置另一套 icon 渲染逻辑。
4. 只有需要位图内容时，才走 `QPixmap` / `QImage`。

## 10. 样式系统规则

## 10.1 样式类集中放在 DeveloperComponents

大量控件依赖 `QProxyStyle` 子类：

- `NXCheckBoxStyle`
- `NXComboBoxStyle`
- `NXGroupBoxStyle`
- `NXLineEditStyle`
- `NXListViewStyle`
- `NXMenuStyle`
- `NXScrollBarStyle`
- `NXSliderStyle`
- `NXSpinBoxStyle`
- `NXStatusBarStyle`
- `NXTabBarStyle`
- `NXTableViewStyle`
- `NXTableWidgetStyle`
- `NXToolBarStyle`
- `NXToolButtonStyle`
- `NXTreeViewStyle`
- `NXWindowStyle`

### 10.1.1 规则

1. 如果目标控件本质是 Qt 原生控件换皮，优先建 style 类，不要重写整个控件绘制。
2. 若要重写 Qt 控件的 indicator、header、item、branch、toolbutton、menu 等细节，优先通过 `QProxyStyle` 完成。
3. style 类必须自己订阅主题变化，维护 `_themeMode`。
4. style 类只负责绘制和尺寸度量，不要塞业务逻辑。

## 10.2 样式对象所有权要明确

现有代码里存在两种释放方式：

1. `delete this->style();`
2. `delete d->_xxxStyle;`

### 10.2.1 规则

1. 你创建 style 的方式，决定释放方式。
2. 如果 `setStyle(d->_style)` 后 style 只由本控件独占，优先由 private 明确持有并在析构中释放。
3. 不要在同一控件里同时 `delete this->style()` 和 `delete d->_style`，避免双删。
4. 修改现有控件时，先看其当前所有权模式，再保持一致。

## 11. 动画系统规则

NexUs 大量依赖 `QPropertyAnimation`，并且多数动画都基于“暴露一个属性 -> 动画驱动属性 -> `update()` 重绘”。

## 11.1 动画承载位置

常见承载位置：

1. 控件本身的公开属性
2. private 对象属性
3. style 对象属性

### 11.1.1 典型例子

- `NXCentralStackedWidget` 的切页动画属性
- `NXNavigationStyle` 的选中条动画属性
- `NXToolButtonStyle` 的展开图标旋转
- `NXScrollBar` 的平滑滚动
- `NXToggleSwitch` / `NXToggleButton` 的切换动画
- `NXMessageBar` 的出现/消失动画

### 11.1.2 规则

1. 动画属性必须有稳定归属，不要随意挂到 unrelated 对象上。
2. 如果动画只影响绘制，属性应尽量放在 style/private，避免污染 public API。
3. `valueChanged -> update()` 是 NexUs 常态，新增动画时要保证刷新对象是正确的 widget/style。
4. 动画结束后如果有临时 pixmap、overlay、隐藏控件状态，必须清理。

## 12. 模型/视图/委托规则

## 12.1 什么时候应该用 Model/View

满足以下条件优先走 model/view：

1. 项数量动态变化。
2. 需要选中态、hover 态、展开态。
3. 需要拖拽排序。
4. 需要多种节点类型。
5. 需要与 footer/header/分类结构协作。

### 12.1.1 现有代表

- `NXNavigationBar`
- `NXBreadcrumbBar`
- `NXCalendar`
- `NXPivot`
- `NXSuggestBox`
- `NXColorDialog`

## 12.2 视图只做交互，模型只做数据，委托/样式只做画面

### 12.2.1 规则

1. 节点结构、父子关系、唯一 key 放 model/node。
2. 鼠标、拖拽、上下文菜单、tooltip 放 view。
3. 绘制细节和动画 mark 放 style/delegate。
4. 外部总控 API 放包装控件本体。

这是 NexUs 已验证的分层方式，不要混写。

## 13. Graphics 子系统规则

NexUs 有一套独立的 Graphics 画布体系：

- `NXGraphicsView`
- `NXGraphicsScene`
- `NXGraphicsItem`
- `NXGraphicsLineItem`

## 13.1 适用场景

1. 节点编辑器
2. 可拖拽画布
3. 连接线关系
4. 序列化/反序列化图元布局

## 13.2 规则

1. 画布模式切换由 `NXGraphicsSceneType::SceneMode` 管理。
2. 图元连接关系由 scene 管理，不应让 item 自己偷偷维护全局连接表。
3. 如果启用了端口校验，必须通过 `setLinkPortState()` 与 `IsCheckLinkPort` 一致维护端口状态。
4. 连接线图元与业务图元生命周期必须同步清理。
5. 图元唯一性依赖 `ItemUID`，不要自行伪造冲突 UID。
6. 如果新增图元类型，要明确其：
   - `boundingRect()`
   - `paint()`
   - 序列化策略
   - 端口能力

## 14. 控件实现策略选择规则

新增控件前，先按下面分类。

### 14.1 直接自绘型控件适用条件

适合：

- 卡片
- 开关
- 圆环
- 自定义轻量按钮
- 复杂单体视觉控件

规则：

1. 继承最接近交互语义的 Qt 基类。
2. 在 `paintEvent()` 中使用 `QPainter` 完成绘制。
3. 主题变化只需缓存 `_themeMode` 并 `update()`。
4. 阴影优先复用 `nxTheme->drawEffectShadow(...)`。

### 14.2 样式代理型适用条件

适合：

- `QCheckBox`
- `QToolButton`
- `QComboBox`
- `QTableView`
- `QTreeView`
- `QMenu`
- `QToolBar`

规则：

1. 包装类只负责创建并安装 style。
2. style 类负责绘制与度量。
3. 主题订阅放 style 内部。
4. 若需要 hover/press/selected 动画，优先在 style 中暴露动画属性。

### 14.3 容器/组合控件适用条件

适合：

- 对话框
- 滚动页
- 导航条
- 页签容器
- 颜色面板

规则：

1. 组合控件先搭布局，再组装子控件，再绑定主题，再处理动画。
2. 子控件若本身已具备 NexUs 风格，优先复用，不要重复造轮子。
3. 组合控件内部的内部通信优先直接 connect，不用事件总线。

## 15. 新增控件的标准开发流程

以下流程是新增控件时的推荐强制步骤。

## 15.1 先做类型判断

先回答以下问题：

1. 它是顶层窗口、普通控件、容器、视图、图元，还是浮层？
2. 它继承哪个 Qt 基类最自然？
3. 它是自绘，还是 style 驱动，还是 model/view 驱动？
4. 它是否需要主题响应？
5. 它是否需要动画属性？
6. 它是否需要 private 类？
7. 它是否需要 DeveloperComponents 支撑类？

## 15.2 文件结构建议

复杂控件建议同时创建：

1. `Source/include/NXFoo.h`
2. `Source/NXFoo.cpp`
3. `Source/private/NXFooPrivate.h`
4. `Source/private/NXFooPrivate.cpp`

若使用样式/委托/模型，再增加：

5. `Source/DeveloperComponents/NXFooStyle.h/.cpp`
6. 或 `NXFooDelegate.h/.cpp`
7. 或 `NXFooModel.h/.cpp`

## 15.3 头文件规则

1. 类名使用 `NX` 前缀。
2. 导出类使用 `NX_EXPORT`。
3. 使用 `Q_OBJECT`。
4. 使用 `Q_Q_CREATE(NXFoo)`。
5. 对外暴露属性使用 `Q_PROPERTY_CREATE_H`。
6. 信号命名延续现有风格。

## 15.4 构造函数规则

构造函数里通常做这些事，顺序建议如下：

1. 初始化 `d_ptr`，设置 `d->q_ptr = this`。
2. 设置默认属性值。
3. 设置 `objectName`。
4. 必要时设置透明背景 stylesheet。
5. 创建 style / child widgets / model / delegate / layout。
6. 连接主题变化。
7. 连接应用级显示模式变化。
8. 初始化动画对象。

### 15.4.1 高频初始化模式

- `setObjectName("NXFoo")`
- `setStyleSheet("#NXFoo{background-color:transparent;}")`
- `d->_themeMode = nxTheme->getThemeMode();`
- `connect(nxTheme, &NXTheme::themeModeChanged, ...)`

## 15.5 命名和属性暴露规则

1. 布尔属性用 `IsXxx`。
2. 尺寸属性用 `Width/Height/Radius/Margin/Spacing`。
3. 图标属性优先 `NXIconType::IconName`。
4. 文本属性使用 `QString`，名字通常为 `Title/SubTitle/Text/...`。
5. setter/getter 使用宏生成，不要手写一套不一致命名。

## 15.6 新控件如果要进 `NXWindow` 页面体系

必须额外满足：

1. 可被 `newInstance()` 调用。
2. 页面内容能通过 `windowTitle()` / `windowIcon()` 提供标签语义。
3. 不要在页面内部破坏 `NXPageKey`。

## 16. 给现有控件加功能的规则

## 16.1 先定位功能应该落在哪层

### 16.1.1 加的是外部 API

改 public 头文件和 public cpp。

### 16.1.2 加的是内部行为状态

改 private。

### 16.1.3 加的是绘制与视觉

改 style / delegate / paintEvent。

### 16.1.4 加的是数据结构

改 model/node。

### 16.1.5 加的是窗口壳层行为

改 `NXWindow` / `NXAppBar` / `NXApplication` / `NXNavigationRouter`。

## 16.2 严禁“能跑就行”的补丁式写法

错误示例：

1. 为了改 checkbox 视觉，直接在 `NXCheckBox` 里重写整套 `paintEvent`，却保留原 style。
2. 为了新增导航节点状态，把字段临时塞进 `NXNavigationBarPrivate`，却不更新 `NXNavigationNode`。
3. 为了主题切换生效，直接在 `paintEvent` 里写硬编码颜色，而不接入主题。
4. 为了修复浮窗打开失败，绕开 `metaObject()->newInstance()`，却不解决构造函数不可反射的问题。

正确原则：找准真正层级，再修改。

## 17. 修复 Bug 的规则

## 17.1 Bug 定位顺序

建议按以下顺序排查：

1. 先看 public API 层是否用错。
2. 再看 private 状态是否漏同步。
3. 再看主题变化是否漏接。
4. 再看 style/delegate 是否没有刷新。
5. 再看动画开始/结束是否没有清理中间态。
6. 再看对象动态属性、QVariantMap key、metaObject newInstance 等协议是否被破坏。
7. 最后看平台特定逻辑。

## 17.2 NexUs 中的高风险 Bug 区

### 17.2.1 主题同步类 Bug

典型表现：

- 切换 Light/Dark 后部分文字没变色
- style 颜色没更新
- 背景 movie/pixmap 不同步
- Win32 主题边框未同步

优先检查：

- 是否缓存 `_themeMode`
- 是否连接 `themeModeChanged`
- 是否触发 `update()`
- palette 是否需要同步

### 17.2.2 样式所有权 Bug

典型表现：

- 析构崩溃
- double free
- style 泄漏

优先检查：

- `setStyle(...)` 后由谁拥有
- 析构里删的是 `this->style()` 还是 `d->_style`
- 是否两边都删了

### 17.2.3 反射构造 Bug

典型表现：

- 页面“在新窗口中打开”无效
- 回溯页面失败

优先检查：

- 页面类是否 `Q_OBJECT`
- 构造函数是否 `Q_INVOKABLE`
- `metaObject()->newInstance()` 是否能成功

### 17.2.4 动态属性协议 Bug

典型表现：

- 页签拖拽失败
- 图标不显示
- 页面 key 丢失
- 菜单项图标丢失

优先检查这些 property：

- `NXPageKey`
- `NXIconType`
- `NXOriginTabWidget`
- `NXFloatParentWidget`
- `DragType`
- `TabDropIndex`
- `NXBaseClassName`

### 17.2.5 路由协议 Bug

典型表现：

- 前进后退错页
- 页签或导航状态不同步
- `invokeMethod` 无法调用

优先检查：

- `routeObject`
- `routeFunctionName`
- `QVariantMap` key
- connection type
- 是否用了错误的 global / context 路由接口

## 18. 代码编写细则

## 18.1 ObjectName 和透明背景

很多控件会做：

- `setObjectName("NXFoo")`
- `setStyleSheet("#NXFoo{background-color:transparent;}")`

### 18.1.1 规则

1. 需要样式表选择器或透明背景时必须设 objectName。
2. 使用透明背景是 NexUs 普遍视觉前提，尤其在自绘、浮层、标题栏、堆栈容器里。
3. 但不要无脑套透明背景；如果控件本身依赖原生 palette 绘制，要确认透明不会破坏默认行为。

## 18.2 阴影统一复用

已有统一阴影入口：

- `NXTheme::drawEffectShadow(...)`
- `NXShadowGraphicsEffect`
- `NXShadowWidget`

### 18.2.1 规则

1. 普通矩形类轻量阴影优先用 `drawEffectShadow`。
2. 复杂投影效果用 `NXShadowGraphicsEffect`。
3. 不要在多个控件里复制一套阴影 painter 算法。

## 18.3 文本风格统一走 `NXTextType`

`NXText`、`NXPushButton`、`NXCheckBox`、`NXRadioButton` 等都支持 `NXTextType::TextStyle`。

### 18.3.1 规则

1. 如果新控件有主文本显示，优先考虑支持 `TextStyle`。
2. 文本 pixel size 的默认值要与现有体系接近，不要引入突兀字号体系。

## 18.4 图标和文本混排

`NXText`、`NXToolButtonStyle`、`NXMenuStyle` 都有图标与文本协同绘制的模式。

### 18.4.1 规则

1. 若图标是字体图标，尽量在绘制层统一处理。
2. 需要文字省略时使用 `fontMetrics().elidedText(...)`。
3. 文字、图标、indicator 的间距应由单一风格对象控制，不要散落常量。

## 18.5 事件过滤器不是兜底垃圾桶

NexUs 使用了大量 `installEventFilter`，典型用途：

- 顶层窗口尺寸与位置同步
- tooltip 跟随与显示时机
- 浮动窗口统计
- 输入框失焦检测
- 自定义滚动条跟随宿主区域

### 18.5.1 规则

1. eventFilter 只处理跨对象事件，不处理本对象本可直接重载的事件。
2. eventFilter 内要尽量精确判断 `event->type()`。
3. 不要把一个控件所有行为都堆进 eventFilter。

## 19. 特定控件族开发规则

## 19.1 Window / Dialog / AppBar 族

涉及文件：

- `NXWindow`
- `NXWidget`
- `NXDialog`
- `NXContentDialog`
- `NXDockWidget`
- `NXAppBar`

规则：

1. 顶层行为优先复用 `NXAppBar`。
2. 关闭、置顶、固定大小等能力应由 AppBar 驱动。
3. Windows 原生事件处理必须集中，不要让业务层各自截获。
4. 主题切换动画优先沿用 `NXThemeAnimationWidget` + `NXWindowPrivate` 方案。

## 19.2 Navigation 族

涉及文件：

- `NXNavigationBar`
- `NXNavigationModel`
- `NXNavigationNode`
- `NXNavigationView`
- `NXNavigationStyle`
- `NXFooterModel`
- `NXFooterDelegate`

规则：

1. 节点树变更只经由 model。
2. 视图只负责鼠标、拖拽、tooltip、右键菜单。
3. 展开收起动画在 style 和 private 间配合。
4. 页脚节点不要混进主树模型。

## 19.3 Input 族

涉及文件：

- `NXLineEdit`
- `NXPlainTextEdit`
- `NXMultiCellLineEdit`
- `NXKeyBinder`
- `NXComboBox`
- `NXMultiSelectComboBox`
- `NXSpinBox`
- `NXDoubleSpinBox`
- `NXCalendarPicker`
- `NXSuggestBox`

规则：

1. 基于原生输入控件扩展时，优先 style + private，而不是从零写输入法处理。
2. 失焦、清空按钮、浮层搜索结果等联动尽量走 private。
3. 若依赖外部点击关闭，优先复用 EventBus 或 eventFilter 的既有模式。

## 19.4 DataView 族

涉及文件：

- `NXListView`
- `NXTableView`
- `NXTableWidget`
- `NXTreeView`
- `NXTabBar`
- `NXTabWidget`

规则：

1. 样式逻辑下沉到 style。
2. hover index、selection effect、header metrics 等通过 style/private 协作。
3. 页签拖拽是复杂协议，修改时必须保留拖拽 mime property 协议。

## 19.5 Graphics 族

涉及文件：

- `NXGraphicsView`
- `NXGraphicsScene`
- `NXGraphicsItem`
- `NXGraphicsLineItem`

规则：

1. 画布交互由 view + scene 协作，不要把缩放和平移写进 item。
2. 连接关系与端口占用由 scene 统一管理。
3. 序列化相关改动必须保证读写对称。

## 19.6 Card / Promotion / Reminder 族

涉及文件：

- `NXInteractiveCard`
- `NXImageCard`
- `NXPopularCard`
- `NXPromotionCard`
- `NXPromotionView`
- `NXReminderCard`
- `NXAcrylicUrlCard`

规则：

1. 这类控件以自绘为主。
2. 图片圆角、标题字号、间距、hover/press 状态是核心配置项。
3. 统一支持主题文字与边框色。
4. 若增加互动反馈，优先用轻量属性动画，不要引入重型效果栈。

## 20. 公共控件清单与基类选择参考

以下是 `Source/include` 中公开控件的真实基类，可作为新控件选型参考。

### 20.1 顶层与容器

- `NXWindow : QMainWindow`
- `NXWidget : QWidget`
- `NXDialog : QDialog`
- `NXContentDialog : QDialog`
- `NXDockWidget : QDockWidget`
- `NXDrawerArea : QWidget`
- `NXScrollPage : QWidget`
- `NXScrollPageArea : QWidget`
- `NXCentralStackedWidget : QWidget`

### 20.2 基础输入与选择

- `NXCheckBox : QCheckBox`
- `NXRadioButton : QRadioButton`
- `NXPushButton : QPushButton`
- `NXIconButton : QPushButton`
- `NXToolButton : QToolButton`
- `NXLineEdit : QLineEdit`
- `NXPlainTextEdit : QPlainTextEdit`
- `NXComboBox : QComboBox`
- `NXMultiSelectComboBox : QComboBox`
- `NXSpinBox : QSpinBox`
- `NXDoubleSpinBox : QDoubleSpinBox`
- `NXSlider : QSlider`
- `NXToggleSwitch : QWidget`
- `NXToggleButton : QWidget`

### 20.3 数据展示与导航

- `NXListView : QListView`
- `NXTableView : QTableView`
- `NXTableWidget : QTableWidget`
- `NXTreeView : QTreeView`
- `NXTabBar : QTabBar`
- `NXTabWidget : QTabWidget`
- `NXNavigationBar : QWidget`
- `NXPivot : QWidget`
- `NXBreadcrumbBar : QWidget`

### 20.4 图形系统

- `NXGraphicsView : QGraphicsView`
- `NXGraphicsScene : QGraphicsScene`
- `NXGraphicsItem : QGraphicsObject`
- `NXGraphicsLineItem : QGraphicsPathItem`

### 20.5 反馈与浮层

- `NXToolTip : QWidget`
- `NXMessageBar : QWidget`
- `NXMessageDialog : QWidget`
- `NXMenu : QMenu`
- `NXMenuBar : QMenuBar`
- `NXStatusBar : QStatusBar`

### 20.6 视觉控件

- `NXText : QLabel`
- `NXProgressBar : QProgressBar`
- `NXProgressRing : QWidget`
- `NXLCDNumber : QLCDNumber`
- `NXShadowWidget : QWidget`
- `NXShadowGraphicsEffect : QGraphicsEffect`

### 20.7 卡片与特殊控件

- `NXInteractiveCard : QPushButton`
- `NXAcrylicUrlCard : QPushButton`
- `NXReminderCard : QPushButton`
- `NXImageCard : QWidget`
- `NXPopularCard : QWidget`
- `NXPromotionCard : QWidget`
- `NXPromotionView : QWidget`
- `NXRoller : QWidget`
- `NXRollerPicker : QPushButton`
- `NXCalendar : QWidget`
- `NXCalendarPicker : QPushButton`
- `NXKeyBinder : QLabel`

## 21. 大模型执行时的硬性决策表

### 21.1 新增控件

先判断：

1. 基类是什么。
2. 是否需要 private。
3. 是否需要 style/delegate/model。
4. 是否需要主题连接。
5. 是否需要动画属性。
6. 是否会进入 `NXWindow` 页面体系。
7. 是否需要 Windows 原生行为。

### 21.2 给控件加功能

先判断改动归属：

1. API 层
2. private 状态层
3. style/delegate 绘制层
4. model 数据层
5. window/router/application 全局层

### 21.3 修 Bug

先判断 Bug 类型：

1. 主题不同步
2. 样式不刷新
3. 动画中间态未清理
4. property 协议失效
5. 反射构造失败
6. 路由数据错乱
7. 所有权/析构问题
8. 原生窗口行为问题

## 22. 反模式清单

以下做法在 NexUs 中应视为错误方向：

1. 不经过主题系统直接硬编码颜色。
2. 不经过 model 直接操作导航内部节点树。
3. 新页面不提供可反射构造，却要求支持新窗口打开。
4. 把业务逻辑塞进 style/delegate。
5. 把绘制逻辑塞进 model。
6. 把所有事件都丢给 eventFilter。
7. 随意改 `QVariantMap` key 名称导致路由/EventBus/拖拽协议断裂。
8. 不明确 style 所有权导致重复释放。
9. 为了修一个视觉问题绕开整个架构层级。
10. 在顶层窗口行为里跳过 `NXAppBar` 自己截原生事件。

## 23. 建议的开发检查清单

每次提交控件改动前，至少检查：

1. Light / Dark 下是否都正常。
2. 是否使用了正确的层级实现。
3. setter 是否会触发必要的 `update()` / 布局刷新 / palette 同步。
4. style / delegate / model / view 的职责是否清晰。
5. 动画结束后中间资源是否释放。
6. 若控件是页面，是否可被 `newInstance()` 构造。
7. 是否保留了关键动态属性协议。
8. 是否破坏了 Window/AppBar/Router/EventBus 的既有行为。
9. 是否引入 style 所有权问题。
10. 是否影响了滚动、拖拽、hover、selection 等交互细节。

## 24. 一句话总纲

NexUs 的正确开发方式，不是“在某个控件里把需求凑出来”，而是严格按 `公开控件 API -> Private 状态与行为 -> DeveloperComponents 样式/模型/委托 -> Theme/Application/Router/EventBus 横切基础设施` 这条主线落实现。新增控件、加功能、修 Bug，第一步永远是判断应该改哪一层。
