# NexUs 领域语言

## 项目标识

- **NexUs**：基于 Qt5/Qt6 的 FluentUI 风格桌面组件库。上游为 ElaWidgetTools，本项目在此基础上规范了代码范式、扩展/新增了组件，供其他开发者开箱即用。
- **FluentUI**：目标视觉设计语言，即微软 MDL2（Microsoft Design Language 2）风格。
- **ElaWidgetTools**：上游开源库（Liniyous），本项目的 fork 起点。

## 架构分层

NexUs 控件体系为三层结构：

| 层级 | 物理位置 | 对外可见 | 职责 |
|---|---|---|---|
| **公共接口层** | `Source/include/*.h` + `Source/*.cpp` | ✅ 仅 `NX_EXPORT` 头文件 | 对外 API、Q_PROPERTY、信号/槽 |
| **私有实现层** | `Source/private/*` | ❌ 不公开 | PIMPL 模式、状态存储、动画对象、平台中转 |
| **开发支撑层** | `Source/DeveloperComponents/*` | ❌ 不公开 | Style、Model、Delegate、容器、辅助组件 |

## 控件实现策略

四种实现形态，决定控件的文件结构和技术路线：

| 策略 | 核心机制 | 适用场景 |
|---|---|---|
| **直接自绘型** | `paintEvent` 手工绘制 | 自定义外观、轻量控件（NXPushButton, NXText, NXToggleSwitch） |
| **样式代理型** | `QProxyStyle` 接管绘制 | 标准 Qt 控件换肤（NXCheckBox, NXToolButton, NXGroupBox） |
| **模型/视图/委托型** | Model + View + Delegate/Style 组合 | 列表、树、导航等数据驱动控件（NXNavigationBar, NXBreadcrumbBar, NXCalendar） |
| **顶层窗口/容器型** | 窗口框架 + 路由 + 原生事件 | 顶级窗口和容器（NXWindow, NXWidget, NXDialog） |

修改或新增控件的第一步：判断属于哪种策略，再照对应模板实现。

## 主题系统

跨所有控件的横切机制，统一管理明暗配色与窗口材质。

| 概念 | 类型 | 说明 |
|---|---|---|
| **ThemeMode** | `NXThemeType::ThemeMode` | `{ Light, Dark }` 明暗模式切换 |
| **ThemeColor** | `NXThemeType::ThemeColor` | 48 个语义化颜色槽，通过 `NXThemeColor(mode, name)` 取色 |
| **DisplayMode** | `NXApplicationType::WindowDisplayMode` | `{ Normal, NXMica, Mica, MicaAlt, Acrylic, DWMBlur }` 窗口材质效果 |

主题变更联动：颜色重绘、图标切换、背景资源、原生窗口 DWM 同步。

## 导航系统

NexUsExample 中的核心交互骨架，基于树形模型的页面路由体系。

| 概念 | 类型 | 说明 |
|---|---|---|
| **NavigationNodeType** | `NXNavigationType::NavigationNodeType` | `{ PageNode, FooterNode, CategoryNode, ExpanderNode }` 节点类型 |
| **NavigationDisplayMode** | `NXNavigationType::NavigationDisplayMode` | `{ Auto, Minimal, Compact, Maximal }` 导航栏展开状态 |
| **NXPageKey** | 动态属性 | 唯一标识每个导航页面的字符串键 |

节点层级规则：仅 **ExpanderNode** 可拥有子节点；PageNode、FooterNode、CategoryNode 均可挂载在 ExpanderNode 下。CategoryNode 用于在 ExpanderNode 内对 PageNode 做分组标记。

## 顶层窗口体系

所有顶层窗口共享 NXAppBar 无边框标题栏，但基类和组件集不同：

| 窗口 | 基类 | 特性 |
|---|---|---|
| **NXWindow** | `QMainWindow` | 完整壳层：AppBar + NavigationBar + CentralStack + Router + EventBus |
| **NXWidget** | `QWidget` | 轻量窗口，不含导航栏/toolbar/dockwidget |
| **NXDialog** | `QDialog` | 对话框 |
| **NXContentDialog** | `QDialog` | 内容弹窗 |
| **NXDockWidget** | `QDockWidget` | 可停靠窗口 |

NXAppBar 是统一标题栏组件，负责：无边框窗口、按钮（最小化/最大化/关闭/置顶/主题切换）、原生事件接管、hit test、边缘 resize。
