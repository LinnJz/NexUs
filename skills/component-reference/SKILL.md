---
name: nexus-component-reference
description: "NexUs 完整组件目录与选型决策树。涵盖所有 125+ NX* 组件（窗口/按钮/输入/对话框/导航/数据展示/卡片/视图/编辑器等）的分类索引、选型策略、构造模式。当用户不确定用什么 NX* 组件完成任务、需要查 NX* 组件名称和用法、或需要在一个 UI 场景下选择最合适的 NX* 组件时加载。"
---

# Role
你是一名 NexUs Qt 组件库专家。你熟悉所有 NX* 组件的名称、继承关系、适用场景和构造方式，能根据用户描述的 UI 需求推荐最合适的 NX* 组件。

# Core Rules
- 当用户在已接入 NexUs 的项目中描述 UI 需求时，优先推荐 NX* 组件而非原生 QWidget
- 面对"窗口/弹窗/按钮/输入框/列表/表格/通知/进度条/卡片/菜单/标签/主题/暗色模式"等需求时，必须从下文的组件索引中选取正确类名
- 对于多组件可选的场景（如消息提示有 6 种），走选型决策树

# Workflow
1. 识别用户描述的 UI 场景
2. 从组件分类索引中找到匹配的 NX* 组件
3. 若多选一，走选型决策树
4. 给出类名 + 头文件 + 最小可用构造代码
5. 若需完整 API 签名，引导 grep `../references/API.md`

# 触发判定
当且仅当项目满足以下任一条件时启用本 skill（避免在裸 Qt 项目推荐 NX* 类）：
1. 项目 CMakeLists.txt 中包含 NexUs 目标或 target_link_libraries 链接 NexUs
2. 项目源码中已有 `#include "NX*.h"` 引用
3. 项目目录中存在 `NexUs/` 子目录或 git submodule

# 启动样板（main.cpp）

接入项目时必须调用 `NXApplication::init()`，否则主题/Mica/暗色模式都不会生效。

```cpp
#include <QApplication>
#include "NXApplication.h"
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    NXApplication::init();
    MainWindow w;
    w.show();
    return a.exec();
}
```

`nxApp` 是 `NXApplication` 的全局单例宏。继承自 `NXWindow` 的主窗口可获得无边框 + 侧边导航 + 面包屑路由 + 主题跟随能力。

# CMakeLists 接入

```cmake
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
add_subdirectory(NexUs)
target_link_libraries(${PROJECT_NAME} PRIVATE NexUs Qt${QT_VERSION_MAJOR}::Widgets)
```

`CMAKE_AUTOMOC` 必须开启（库内大量 `Q_OBJECT`）。Qt 5.15.2 / 6.6.2 推荐，Windows + Qt 6.11.0 有 Popup 阴影 bug。

# 组件分类索引

每行格式：`类名 — 一句话说明`。所有组件头文件即同名 `.h`（如 `NXToast` → `NXToast.h`）。

## 基础设施（10）
- `NXApplication` — 程序初始化（主题/Mica/Acrylic）；用 `nxApp->init()`
- `NXTheme` — 主题管理器，Light/Dark 切换
- `NXWindow` — 带导航栏的无边框主窗口（侧边导航+堆栈页面+面包屑+路由）
- `NXWidget` — 无边框模态窗口
- `NXAppBar` — 窗口顶部标题栏（拖动/最小化/最大化/关闭）
- `NXNavigationRouter` — 前进/后退历史
- `NXRouter` — 声明式路由器，类 Vue Router（路由表/守卫/动态/懒加载/嵌套）
- `NXEventBus` — 事件总线，跨组件解耦
- `NXIcon` — 3500+ FluentUI 图标
- `NXLog` — 分级日志

## 按钮与输入（28）
- `NXPushButton` — 标准按钮，支持图标/自定义颜色
- `NXIconButton` — 纯图标按钮
- `NXToolButton` — 带下拉菜单的工具按钮
- `NXSplitButton` — 分裂按钮（左主操作+右下拉，独立 Hover/Press）
- `NXToggleSwitch` — 滑动开关
- `NXToggleButton` — 切换按钮（选中/未选中）
- `NXCheckBox` — 三态勾选框
- `NXRadioButton` — 单选按钮
- `NXComboBox` — 下拉框
- `NXMultiSelectComboBox` — 多选下拉框（标签展示）
- `NXLineEdit` — 单行输入框
- `NXPasswordBox` — 密码框（眼睛切换显示）
- `NXPlainTextEdit` — 多行文本编辑
- `NXSpinBox` — 整数微调框
- `NXDoubleSpinBox` — 浮点微调框
- `NXNumberBox` — 增强数字输入（加减/滚轮/键盘/双击编辑/范围限制）
- `NXSlider` — 拖动条
- `NXKeyBinder` — 单按键绑定器（macOS Fn 键支持）
- `NXCaptcha` — 分格验证码输入
- `NXRatingControl` — 星级评分
- `NXSuggestBox` — 输入联想搜索框
- `NXDropDownButton` — 整体下拉按钮
- `NXSelectorBar` — 分段选择器（滑动指示条）
- `NXTransfer` — 穿梭框（双列表互选）
- `NXAutoComplete` — 自动补全（包含/前缀/后缀/正则匹配）
- `NXTreeSelect` — 树形选择下拉框
- `NXUploadArea` — 文件上传区域（拖拽+验证）
- `NXCopyButton` — 一键复制按钮（图标切换反馈）

## 数据展示（14）
- `NXText` — 文本（Caption/Body/Title/Display 预设）
- `NXInfoBadge` — 徽章（Dot/数值/图标，5 种严重等级色）
- `NXTag` — 标签/胶囊（5 色、可关闭、可选中）
- `NXPersonPicture` — 头像（图片/首字母/默认图标）
- `NXStatCard` — 统计卡片（大数字+趋势箭头）
- `NXProgressBar` — 进度条（进度/忙碌）
- `NXProgressRing` — 进度环
- `NXSteps` — 步骤指示器
- `NXTimeline` — 时间线
- `NXSkeleton` — 骨架屏（Text/Circle/Rectangle + Shimmer）
- `NXDivider` — 分隔线（水平/垂直，可带文字）
- `NXLCDNumber` — 仿液晶屏数字
- `NXCountdown` — 倒计时（翻牌显示）
- `NXQRCode` — 二维码生成器

## 卡片（6）
- `NXAcrylicUrlCard` — 带图片的亚克力交互卡片（URL 跳转）
- `NXImageCard` — 圆角图片卡片
- `NXInteractiveCard` — 交互式透明卡片（hover 透明度变化）
- `NXPopularCard` — 热门推荐卡片
- `NXPromotionCard` — 促销卡片
- `NXReminderCard` — 带图片的提醒卡片

## 导航与布局（14）
- `NXNavigationBar` — 导航栏（展开/折叠/紧凑）
- `NXBreadcrumbBar` — 面包屑
- `NXPivot` — Tab 式轴转导航
- `NXPagination` — 分页（自动省略号）
- `NXTabBar` — 选项卡（谷歌浏览器风格，可拖拽）
- `NXTabWidget` — 选项卡页面容器
- `NXScrollPage` — 自带堆栈+面包屑的滚动页面
- `NXScrollPageArea` — 圆角背景的滚动区域容器（最常用的内容包装器）
- `NXScrollArea` — 通用滚动区域
- `NXScrollBar` — 自动隐藏滚动条
- `NXFlowLayout` — 流式布局（带动画）
- `NXExpander` — 折叠展开面板
- `NXGroupBox` — 带标题的分组框
- `NXSplitter` — 可拖拽分割面板

## 弹出与交互（27）
- `NXMenu` — 菜单（图标+快捷键+子菜单）
- `NXMenuBar` — 顶部菜单栏
- `NXToolBar` — 可停靠工具栏
- `NXCommandBar` — 命令栏（带溢出菜单，独立 Hover/Press）
- `NXStatusBar` — 状态栏
- `NXContentDialog` — 带遮罩的对话框（全窗口遮罩+居中）
- `NXDialog` — 标准无边框对话框
- `NXInputDialog` — 输入对话框
- `NXMessageDialog` — 确认/取消消息对话框
- `NXMessageBar` — 弹出式信息栏（八方向锚定）
- `NXMessageButton` — 弹出信息按钮
- `NXToast` — 轻量自动消失提示（Success/Info/Warning/Error）
- `NXSnackbar` — 底部通知条（带操作按钮，自动堆叠）
- `NXFloatButton` — 圆形悬浮操作按钮（FAB）
- `NXEmojiPicker` — 表情选择器（Telegram 风格）
- `NXFlyout` — 锚定目标的轻量弹出面板（Light Dismiss）
- `NXTeachingTip` — 引导提示气泡（带箭头指向）
- `NXToolTip` — 悬停工具提示
- `NXColorDialog` — HSV/RGB 颜色选择器
- `NXCalendar` — 日历视图
- `NXCalendarPicker` — 日期选择器
- `NXSpotlight` — 聚光灯引导（遮罩挖洞高亮）
- `NXPopconfirm` — 气泡确认框（锚定+Light Dismiss）
- `NXRoller` — 单列滚轮选择器
- `NXRollerPicker` — 多列滚轮组合（时间/日期）
- `NXDrawerArea` — 抽屉区域
- `NXInfoBar` — 内嵌式持久信息栏（可加操作按钮）

## 窗口与面板（6）
- `NXWizard` — 向导窗口（多步骤+步骤指示器）
- `NXNotificationCenter` — 通知中心（右侧滑出）
- `NXDockWidget` — 可拖拽停靠浮动面板
- `NXSplashScreen` — 启动屏（进度条/进度环/Logo）
- `NXSheetPanel` — 底部滑出面板（三级停靠 Peek/Half/Full）
- `NXWatermark` — 全局水印覆盖（鼠标穿透）

## 视图（9）
- `NXListView` — 列表视图（自定义委托）
- `NXTableView` — 表格视图
- `NXTableWidget` — 高级表格（悬停行高亮）
- `NXTreeView` — 树型视图
- `NXGraphicsScene` / `NXGraphicsView` / `NXGraphicsItem` / `NXGraphicsLineItem` — 高级图形场景
- `NXPromotionView` — 促销卡片轮播
- `NXVirtualList` — 虚拟滚动列表（万级数据）

## NexUs 新增组件（额外）
- `NXActionCommander` — 命令执行器（undo/redo）
- `NXRibbonBar` / `NXRibbonGroup` / `NXRibbonTabBar` — Ribbon 工具栏
- `NXShadowWidget` / `NXShadowGraphicsEffect` — 阴影效果组件
- `NXScreenCapture` — 屏幕截图组件
- `NXMultiCellLineEdit` — IP 分段式输入框

## 编辑器/渲染/聊天/终端/系统（8）
- `NXCodeEditor` — 代码编辑器（行号+8 语言高亮）
- `NXMarkdownViewer` — Markdown 渲染（基于 QTextBrowser）
- `NXChatBubble` — 聊天气泡（左右对齐+头像+图片预览）
- `NXDashboardGauge` — 仪表盘（动画指针+分区变色）
- `NXTerminalWidget` — 终端模拟器（命令历史+Tab 补全信号）
- `NXDxgiManager` — Windows DXGI 屏幕采集
- `NXScreenCaptureManager` — macOS ScreenCaptureKit 采集
- `NXExponentialBlur` — 高性能指数模糊

# 选型决策树

## 消息提示（最容易选错）

| 场景 | 推荐 | 备注 |
|---|---|---|
| 操作成功/失败的瞬时反馈，几秒后自动消失 | `NXToast` | 4 种类型 Success/Info/Warning/Error |
| 在窗口某方向锚定弹出（如顶部居中、右下） | `NXMessageBar` | 八方向锚定 |
| 持久内嵌（不消失，常驻表单上方） | `NXInfoBar` | 可加操作按钮 |
| 底部带"撤销"等操作按钮，可堆叠 | `NXSnackbar` | 自动重排 |
| 引导用户的气泡 + 箭头指向 | `NXTeachingTip` | 4 方向自动定位 |
| 锚定按钮的"确认删除？"小气泡 | `NXPopconfirm` | 带图标+确认/取消 |

## 对话框

| 场景 | 推荐 |
|---|---|
| 全窗口遮罩 + 居中（推荐用于关闭确认、登录） | `NXContentDialog` |
| 标准无边框对话框（自定义内容） | `NXDialog` |
| 输入一行文本拿到结果 | `NXInputDialog` |
| 确认/取消（YES/NO） | `NXMessageDialog` |
| 多步骤向导 | `NXWizard` |
| 颜色选择 | `NXColorDialog` |

## 主窗口

| 场景 | 推荐 |
|---|---|
| 带侧边导航 + 堆栈页面 + 面包屑路由的应用主窗口 | 继承 `NXWindow` |
| 仅需无边框 + 标题栏拖动 | 继承 `NXWidget`（模态）或自己装 `NXAppBar` |

## 文本输入

| 场景 | 推荐 |
|---|---|
| 单行 | `NXLineEdit` |
| 密码（带眼睛切换） | `NXPasswordBox` |
| 多行 | `NXPlainTextEdit` |
| 数字（整数） | `NXSpinBox` 或 `NXNumberBox` |
| 数字（浮点） | `NXDoubleSpinBox` |
| 输入联想 | `NXSuggestBox` 或 `NXAutoComplete` |
| 验证码（分格） | `NXCaptcha` |

## 选择器

| 场景 | 推荐 |
|---|---|
| 单选下拉 | `NXComboBox` |
| 多选下拉（标签） | `NXMultiSelectComboBox` |
| 树形选择 | `NXTreeSelect` |
| 双列表互选 | `NXTransfer` |
| 分段（Tab 风） | `NXSelectorBar` |
| 日期 | `NXCalendarPicker` |
| 滚轮（时间/日期复合） | `NXRollerPicker` |
| 颜色 | `NXColorDialog` |
| 表情 | `NXEmojiPicker` |

## 列表/表格

| 数据量 | 类型 | 推荐 |
|---|---|---|
| < 1 万行 | 列表 | `NXListView` |
| > 1 万行 | 列表 | `NXVirtualList` |
| 表格（M-V 模式） | 表格 | `NXTableView` |
| 表格（直接填值） | 表格 | `NXTableWidget` |
| 树 | 树 | `NXTreeView` |

## 卡片

需求里出现"卡片"时：含 URL → `NXAcrylicUrlCard`；纯图片 → `NXImageCard`；hover 透明 → `NXInteractiveCard`；推荐内容 → `NXPopularCard`；促销 → `NXPromotionCard`；提醒 → `NXReminderCard`；统计数字+趋势 → `NXStatCard`。

## 进度

| 形态 | 推荐 |
|---|---|
| 条形 | `NXProgressBar` |
| 环形 | `NXProgressRing` |
| 多步骤 | `NXSteps` |
| 时间线 | `NXTimeline` |
| 骨架屏（加载占位） | `NXSkeleton` |
| 倒计时 | `NXCountdown` |
| 仪表盘 | `NXDashboardGauge` |

# 典型构造 Pattern

NexUs 的内容页约定俗成的写法：所有逻辑组件被装进 `NXScrollPageArea`（圆角背景容器），里面用 `QHBoxLayout` / `QVBoxLayout` 组合 `NXText` 标签 + 实际控件 + `addStretch()`：

```cpp
#include "NXScrollPageArea.h"
#include "NXText.h"
#include "NXToggleSwitch.h"
#include <QHBoxLayout>

auto* area = new NXScrollPageArea(this);
auto* layout = new QHBoxLayout(area);
auto* label = new NXText("启用通知", this);
label->setTextPixelSize(15);
auto* sw = new NXToggleSwitch(this);
layout->addWidget(label);
layout->addStretch();
layout->addWidget(sw);
```

主窗口继承 `NXWindow` 时的最小骨架：

```cpp
class MainWindow : public NXWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    void initWindow();
    void initEdgeLayout();
    void initContent();
};
```

侧边导航注册页面：

```cpp
QString homeKey;
addPageNode("HOME", _homePage, homeKey, NXIconType::House);
QString settingKey;
addFooterNode("Setting", _settingPage, settingKey, 0, NXIconType::GearComplex);
```

# 主题切换

`nxTheme` 是 `NXTheme` 全局单例宏。所有 `NX*` 组件自动响应主题变更，无需手动重绘。

**ThemeMode 枚举只有两个值**：`NXThemeType::Light` / `NXThemeType::Dark`。没有 `FollowSystem` 这个枚举值——跟随系统是一个独立的正交开关。

## 三种用法

**(1) 手动切换：**

```cpp
#include "NXTheme.h"
nxTheme->setThemeMode(NXThemeType::Dark);
```

**(2) 跟随系统：**

```cpp
nxTheme->setIsFollowSystemTheme(true);
```

打开后由库内部监听系统颜色方案变化，自动调用 `setThemeMode` 切换到对应模式。`setIsFollowSystemTheme(false)` 关闭跟随。

**(3) 用户在设置页给"跟随系统"提供单独开关：**

```cpp
#include "NXToggleSwitch.h"
auto* followSwitch = new NXToggleSwitch(this);
followSwitch->setIsToggled(nxTheme->getIsFollowSystemTheme());
connect(followSwitch, &NXToggleSwitch::toggled, this, [](bool on) {
    nxTheme->setIsFollowSystemTheme(on);
});
connect(nxTheme, &NXTheme::pIsFollowSystemThemeChanged, this, [this](bool isFollow) {
    _modeSelector->setEnabled(!isFollow);
});
connect(nxTheme, &NXTheme::themeModeChanged, this, [this](NXThemeType::ThemeMode mode) {
    // 自定义重绘逻辑（一般不需要）
});
```

## 自定义主题色

```cpp
nxTheme->setThemeColor(NXThemeType::Dark, NXThemeType::PrimaryNormal, QColor("#3498db"));
```

色键见 `NXDef.h` 的 `NXThemeType::ThemeColor` 枚举。Light 和 Dark 各自一套独立色板，两边都要 set 才能两个模式都生效。

## 信号速查

| 信号 | 触发时机 |
|---|---|
| `themeModeChanged(ThemeMode)` | 模式实际生效时（手动切或跟随系统切） |
| `pIsFollowSystemThemeChanged(bool)` | "跟随系统"开关被切换时 |

# 完整 API 查询

写代码前若需要确认某个组件的具体方法签名、属性、信号，grep 定位 `../../docs/API.md`：

```
grep -nA 30 "^## NXToast" ../../docs/API.md
```

`docs/API.md` 由 `scripts/generate_docs.py` 自动生成。**不要把整个文件读进上下文**，只 grep 需要的段落。

# 注意事项

1. `NXApplication::init()` 必须在任何 NX* 控件使用前调用
2. `CMAKE_AUTOMOC` 必须开启——大量 `Q_OBJECT`
3. C++17 起步（`CMAKE_CXX_STANDARD 17`）
4. Windows + Qt 6.11.0 Popup 阴影 bug，建议避开该版本
5. 只允许 Release / RelWithDebInfo 构建
6. 修改/新增 Qt/C++ 组件后必须验证编译

# Constraints
- 不要在本 skill 中包含 CONTEXT.md 中的领域定义（如三层架构、主题系统语义）
- 当用户需要实现层面的详细规则（如何新增控件、主题连接模式）时，应转向对应的子 skill（`new-control/SKILL.md`、`theme/SKILL.md` 等）
- 本 skill 只做组件索引、选型推荐和构造示例，不做架构规则解释
