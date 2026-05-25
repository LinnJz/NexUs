# NexUs 自定义分支 — 工作区更新差异报告

**生成日期**：2026-05-25  
**Git 版本关系**：

| 里程碑 | 提交哈希 | 说明 |
|--------|---------|------|
| 上游原始项目 | `4ac8498` | [LinnJz/nee](https://github.com/LinnJz/nee) — 原始 NexUs 框架，头文件位于 `Source/include/` |
| 上次提交（update） | `6c591d4` | 你的第一次 fork 修改：头文件移出 include、新增 NXProperty.h/CMakeLists.txt/NexUs.qrc、删除原始 Linn 工具类 |
| **当前工作区** | *(未提交)* | **本次报告主体** — 114 个已修改文件 + 8 个未追踪文件 |

---

## 一、文件清单

### 1.1 已修改文件（共 114 个）

| # | 文件 | +/- | 分类标签 | 合并建议 |
|---|------|-----|---------|---------|
| 1 | `Source/DeveloperComponents/NXCheckBoxStyle.cpp` | +4/-2 | [Style-增强] | auto-merge |
| 2 | `Source/DeveloperComponents/NXCheckBoxStyle.h` | +2/-1 | [Style-增强] | auto-merge |
| 3 | `Source/DeveloperComponents/NXColorValueSliderStyle.cpp` | +13/-3 | [Bug修复][增强] | auto-merge |
| 4 | `Source/DeveloperComponents/NXColorValueSliderStyle.h` | +3/-2 | [Bug修复][增强] | auto-merge |
| 5 | `Source/DeveloperComponents/NXComboBoxStyle.cpp` | +2/-2 | [Bug修复] | auto-merge |
| 6 | `Source/DeveloperComponents/NXFooterModel.cpp` | +9/-8 | [Navigation-重构] | user-confirm (API变更) |
| 7 | `Source/DeveloperComponents/NXFooterModel.h` | +6/-7 | [Navigation-重构] | user-confirm (API变更) |
| 8 | `Source/DeveloperComponents/NXIntValidator.cpp` | +3/-3 | [增强] | auto-merge |
| 9 | `Source/DeveloperComponents/NXLineEditStyle.cpp` | +18/-4 | [Style-增强] | auto-merge |
| 10 | `Source/DeveloperComponents/NXLineEditStyle.h` | +5/-1 | [Style-增强] | auto-merge |
| 11 | `Source/DeveloperComponents/NXListViewStyle.cpp` | +8/-13 | [Style-增强] | auto-merge |
| 12 | `Source/DeveloperComponents/NXMaskWidget.cpp` | +1/-0 | [增强] | auto-merge |
| 13 | `Source/DeveloperComponents/NXMaskWidget.h` | +3/-0 | [增强] | auto-merge |
| 14 | `Source/DeveloperComponents/NXMenuStyle.cpp` | +33/-16 | [Style-增强] | user-confirm (视觉变更) |
| 15 | `Source/DeveloperComponents/NXMenuStyle.h` | +2/-1 | [Style-增强] | auto-merge |
| 16 | `Source/DeveloperComponents/NXMicaBaseInitObject.cpp` | +6/-6 | [重构] | auto-merge |
| 17 | `Source/DeveloperComponents/NXNavigationModel.cpp` | +56/-45 | [Navigation-重构] | user-confirm (API变更) |
| 18 | `Source/DeveloperComponents/NXNavigationModel.h` | +20/-22 | [Navigation-重构] | user-confirm (API变更) |
| 19 | `Source/DeveloperComponents/NXNavigationNode.cpp` | +11/-1 | [Navigation-重构] | auto-merge |
| 20 | `Source/DeveloperComponents/NXNavigationNode.h` | +2/-1 | [Navigation-重构] | auto-merge |
| 21 | `Source/DeveloperComponents/NXNavigationStyle.cpp` | +21/-1 | [Navigation-增强] | auto-merge |
| 22 | `Source/DeveloperComponents/NXNavigationView.cpp` | +22/-1 | [Navigation-增强] | auto-merge |
| 23 | `Source/DeveloperComponents/NXNavigationView.h` | +4/-1 | [Navigation-增强] | auto-merge |
| 24 | `Source/DeveloperComponents/NXTabBarStyle.cpp` | +216/-54 | [Tab-重构] | user-confirm (大幅修改) |
| 25 | `Source/DeveloperComponents/NXTabBarStyle.h` | +15/-1 | [Tab-重构] | user-confirm (大幅修改) |
| 26 | `Source/DeveloperComponents/NXTableViewStyle.cpp` | +155/-28 | [Table-重构] | user-confirm (大幅修改) |
| 27 | `Source/DeveloperComponents/NXTableViewStyle.h` | +15/-3 | [Table-重构] | user-confirm (大幅修改) |
| 28 | `Source/DeveloperComponents/NXTreeViewStyle.cpp` | +52/-11 | [Tree-增强] | user-confirm (大幅修改) |
| 29 | `Source/DeveloperComponents/NXTreeViewStyle.h` | +2/-1 | [Tree-增强] | auto-merge |
| 30 | `Source/NXActionCommander.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 31 | `Source/NXActionCommander.h` | +6/-2 | [清理-Singleton] | auto-merge |
| 32 | `Source/NXApplication.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 33 | `Source/NXApplication.h` | +7/-3 | [清理-Singleton][增强] | auto-merge |
| 34 | `Source/NXCheckBox.cpp` | +131/-4 | [重构-Pimpl] | user-confirm (架构变更) |
| 35 | `Source/NXCheckBox.h` | +9/-3 | [重构-Pimpl] | user-confirm (架构变更) |
| 36 | `Source/NXColorDialog.cpp` | +92/-29 | [ColorDialog-增强] | user-confirm (大幅修改) |
| 37 | `Source/NXColorDialog.h` | +3/-1 | [ColorDialog-增强] | auto-merge |
| 38 | `Source/NXComboBox.cpp` | +21/-30 | [ComboBox-重构] | user-confirm (重构) |
| 39 | `Source/NXContentDialog.cpp` | +41/-159 | [ContentDialog-重写] | user-confirm (API重写) |
| 40 | `Source/NXContentDialog.h` | +10/-14 | [ContentDialog-重写] | user-confirm (API重写) |
| 41 | `Source/NXDef.h` | +138/-45 | [Def-重构] | user-confirm (宏API变更) |
| 42 | `Source/NXDockWidget.cpp` | +18/-5 | [Dock-增强] | auto-merge |
| 43 | `Source/NXDockWidget.h` | +6/-2 | [Dock-增强] | auto-merge |
| 44 | `Source/NXDxgiManager.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 45 | `Source/NXDxgiManager.h` | +6/-2 | [清理-Singleton] | auto-merge |
| 46 | `Source/NXEventBus.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 47 | `Source/NXEventBus.h` | +6/-2 | [清理-Singleton] | auto-merge |
| 48 | `Source/NXExponentialBlur.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 49 | `Source/NXExponentialBlur.h` | +6/-2 | [清理-Singleton] | auto-merge |
| 50 | `Source/NXIcon.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 51 | `Source/NXIcon.h` | +2/-2 | [清理] | auto-merge |
| 52 | `Source/NXKeyBinder.cpp` | +26/-12 | [KeyBinder-增强] | auto-merge |
| 53 | `Source/NXLineEdit.cpp` | +39/-5 | [LineEdit-增强] | auto-merge |
| 54 | `Source/NXLineEdit.h` | +4/-1 | [LineEdit-增强] | auto-merge |
| 55 | `Source/NXLog.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 56 | `Source/NXLog.h` | +6/-2 | [清理-Singleton] | auto-merge |
| 57 | `Source/NXMenu.cpp` | +36/-3 | [Menu-增强] | auto-merge |
| 58 | `Source/NXMenu.h` | +5/-4 | [Menu-增强] | auto-merge |
| 59 | `Source/NXNavigationBar.cpp` | +100/-93 | [Navigation-重构] | user-confirm (API变更) |
| 60 | `Source/NXNavigationBar.h` | +34/-31 | [Navigation-重构] | user-confirm (API变更) |
| 61 | `Source/NXNavigationRouter.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 62 | `Source/NXNavigationRouter.h` | +6/-2 | [清理-Singleton] | auto-merge |
| 63 | `Source/NXProperty.h` | +696/-157 | [Property-重写] | user-confirm (完整重写) |
| 64 | `Source/NXPushButton.cpp` | +89/-33 | [PushButton-增强] | user-confirm (大幅修改) |
| 65 | `Source/NXPushButton.h` | +7/-10 | [PushButton-增强] | auto-merge |
| 66 | `Source/NXRadioButton.cpp` | +98/-1 | [RadioButton-增强] | user-confirm (大幅修改) |
| 67 | `Source/NXRadioButton.h` | +5/-2 | [RadioButton-增强] | auto-merge |
| 68 | `Source/NXRouter.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 69 | `Source/NXRouter.h` | +8/-9 | [Router-重构] | auto-merge |
| 70 | `Source/NXScreenCaptureManager.cpp` | +0/-1 | [清理-Singleton] | auto-merge |
| 71 | `Source/NXScreenCaptureManager.h` | +6/-2 | [清理-Singleton] | auto-merge |
| 72 | `Source/NXScrollPage.cpp` | +6/-3 | [Scroll-增强] | auto-merge |
| 73 | `Source/NXScrollPage.h` | +7/-5 | [Scroll-增强] | auto-merge |
| 74 | `Source/NXSlider.cpp` | +78/-6 | [Slider-重写] | user-confirm (架构变更) |
| 75 | `Source/NXSlider.h` | +9/-1 | [Slider-重写] | user-confirm (架构变更) |
| 76 | `Source/NXTabBar.cpp` | +52/-4 | [TabBar-增强] | user-confirm (大幅修改) |
| 77 | `Source/NXTabBar.h` | +5/-2 | [TabBar-增强] | auto-merge |
| 78 | `Source/NXTabWidget.cpp` | +64/-1 | [TabWidget-增强] | user-confirm (大幅修改) |
| 79 | `Source/NXTabWidget.h` | +5/-2 | [TabWidget-增强] | auto-merge |
| 80 | `Source/NXTableView.cpp` | +97/-4 | [TableView-增强] | user-confirm (大幅修改) |
| 81 | `Source/NXTableView.h` | +10/-1 | [TableView-增强] | auto-merge |
| 82 | `Source/NXText.cpp` | +91/-13 | [NXText-增强] | user-confirm (大幅修改) |
| 83 | `Source/NXText.h` | +12/-5 | [NXText-增强] | auto-merge |
| 84 | `Source/NXTheme.cpp` | +42/-27 | [Theme-增强] | user-confirm (重构) |
| 85 | `Source/NXTheme.h` | +17/-9 | [Theme-增强] | auto-merge |
| 86 | `Source/NXToggleButton.cpp` | +22/-4 | [Toggle-增强] | auto-merge |
| 87 | `Source/NXToggleButton.h` | +4/-2 | [Toggle-增强] | auto-merge |
| 88 | `Source/NXToolTip.cpp` | +12/-2 | [ToolTip-增强] | auto-merge |
| 89 | `Source/NXToolTip.h` | +4/-1 | [ToolTip-增强] | auto-merge |
| 90 | `Source/NXTreeView.cpp` | +8/-1 | [TreeView-增强] | auto-merge |
| 91 | `Source/NXTreeView.h` | +4/-2 | [TreeView-增强] | auto-merge |
| 92 | `Source/NXWidget.cpp` | +9/-2 | [Widget-增强] | auto-merge |
| 93 | `Source/NXWidget.h` | +5/-2 | [Widget-增强] | auto-merge |
| 94 | `Source/NXWindow.cpp` | +25/-29 | [Window-重构] | user-confirm (API变更) |
| 95 | `Source/NXWindow.h` | +33/-32 | [Window-重构] | user-confirm (API变更) |
| 96 | `Source/private/NXColorDialogPrivate.cpp` | +88/-35 | [ColorDialog-增强] | user-confirm (大幅修改) |
| 97 | `Source/private/NXColorDialogPrivate.h` | +9/-2 | [ColorDialog-增强] | auto-merge |
| 98 | `Source/private/NXContentDialogPrivate.cpp` | +20/-6 | [ContentDialog-重写] | user-confirm |
| 99 | `Source/private/NXContentDialogPrivate.h` | +9/-12 | [ContentDialog-重写] | user-confirm |
| 100 | `Source/private/NXLineEditPrivate.h` | +6/-3 | [LineEdit-增强] | auto-merge |
| 101 | `Source/private/NXMessageBarPrivate.cpp` | +8/-8 | [MessageBar-重构] | auto-merge |
| 102 | `Source/private/NXNavigationBarPrivate.cpp` | +13/-1 | [Navigation-重构] | auto-merge |
| 103 | `Source/private/NXNavigationBarPrivate.h` | +4/-3 | [Navigation-重构] | auto-merge |
| 104 | `Source/private/NXPushButtonPrivate.h` | +6/-5 | [PushButton-增强] | auto-merge |
| 105 | `Source/private/NXRadioButtonPrivate.h` | +2/-1 | [RadioButton-增强] | auto-merge |
| 106 | `Source/private/NXRouterPrivate.cpp` | +7/-9 | [Router-重构] | auto-merge |
| 107 | `Source/private/NXTabWidgetPrivate.cpp` | +17/-3 | [TabWidget-增强] | auto-merge |
| 108 | `Source/private/NXTabWidgetPrivate.h` | +3/-3 | [TabWidget-增强] | auto-merge |
| 109 | `Source/private/NXTextPrivate.h` | +3/-3 | [NXText-增强] | auto-merge |
| 110 | `Source/private/NXThemePrivate.h` | +2/-2 | [Theme-增强] | auto-merge |
| 111 | `Source/private/NXToggleButtonPrivate.h` | +4/-3 | [Toggle-增强] | auto-merge |
| 112 | `Source/private/NXToolTipPrivate.cpp` | +78/-39 | [ToolTip-重写] | user-confirm (大幅修改) |
| 113 | `Source/private/NXToolTipPrivate.h` | +18/-4 | [ToolTip-重写] | user-confirm (大幅修改) |
| 114 | `Source/private/NXWindowPrivate.cpp` | +18/-15 | [Window-重构] | auto-merge |

### 1.2 未追踪新文件（共 8 个）

| # | 文件 | 说明 | 合并建议 |
|---|------|------|---------|
| 1 | `Source/LinnFunctionRegistry.h` | 函数注册器工具头文件 | user-confirm（新自定义工具） |
| 2 | `Source/LinnProperty.h` | 属性系统辅助宏（被 NXProperty.h 引用） | user-confirm（新自定义工具） |
| 3 | `Source/LinnSingleton.h` | 单例模式辅助宏（替代上游原始版本） | user-confirm（新自定义工具） |
| 4 | `Source/LinnUtils.h` | 通用工具宏（LINN_CAT, LINN_PAIR 等） | user-confirm（新自定义工具） |
| 5 | `Source/expected.hpp` | std::expected  polyfill（C++17） | user-confirm（新依赖） |
| 6 | `Source/private/NXCheckBoxPrivate.cpp` | NXCheckBox Pimpl 实现（新增架构） | user-confirm（新架构文件） |
| 7 | `Source/private/NXCheckBoxPrivate.h` | NXCheckBox Pimpl 头文件 | user-confirm（新架构文件） |
| 8 | `Source/private/NXSliderPrivate.cpp` | NXSlider Pimpl 实现（新增架构） | user-confirm（新架构文件） |
| 9 | `Source/private/NXSliderPrivate.h` | NXSlider Pimpl 头文件 | user-confirm（新架构文件） |

---

## 二、变更详细分析

### 2.1 全局 BOM 添加（涉及所有文件）

几乎所有 .cpp/.h 文件的首行都被添加了 UTF-8 BOM（`\xEF\xBB\xBF`）：
```diff
-#include "xxx.h"
+#include "xxx.h"
```
这是纯格式变更，不影响功能。但需要注意：BOM 文件在 gcc/clang 下可能会导致编译警告。建议确认项目使用的编译器对此的处理策略。

---

### 2.2 全局 Singleton 移除（14 个文件）

以下文件删除了 `Q_SINGLETON_CREATE_CPP` 宏调用，且这些文件仅含此变更（+0/-1）：

- `Source/NXActionCommander.cpp`
- `Source/NXApplication.cpp`
- `Source/NXDxgiManager.cpp`
- `Source/NXEventBus.cpp`
- `Source/NXExponentialBlur.cpp`
- `Source/NXIcon.cpp`
- `Source/NXLog.cpp`
- `Source/NXNavigationRouter.cpp`
- `Source/NXRouter.cpp`
- `Source/NXScreenCaptureManager.cpp`

对应的 .h 文件中添加了标准单例访问方法（`getInstance()` 等）或改为常规模式。

**合并建议**：auto-merge。属于架构清理，去除对旧 Singleton 宏的依赖。

---

### 2.3 NXProperty.h — 属性系统完整重写（+696/-157）

**范围**：`Source/NXProperty.h`

从简单的 `Q_PROPERTY_CREATE(TYPE, M)` / `Q_PRIVATE_CREATE(TYPE, M)` 宏集合，重写为基于标签派发（tag-dispatch）的策略系统。

**旧系统**（上游原始）：
- 6 个宏：`Q_PROPERTY_CREATE`、`Q_PROPERTY_REF_CREATE`、`Q_PROPERTY_CREATE_Q_H/CPP`、`Q_PRIVATE_CREATE_Q_H/CPP` 等
- 不支持自定义 setter/getter 类型
- 不支持合约检查、noexcept 指定
- 不支持信号参数自定义

**新系统**：
- 统一的标签系统：`QS_TAG_DEFAULT_VALUE`、`QS_TAG_ASSIGN_MOVE`、`QS_TAG_CONTRACT_ALWAYS`、`QS_TAG_NOEXCEPT`、`QS_TAG_SIGNAL_PARAMS`、`QS_TAG_NO_EMIT_SIGNAL`、`QS_TAG_UPDATE` 等
- 支持 `QS_SET_VAL(T)` / `QS_SET_CREF(T)` 自定义 setter/getter 类型
- 使用 `LINN_PAIR` / `LINN_CAT` 等宏实现编译期策略派发
- 新增 `#include "LinnUtils.h"` 依赖

**关键变更**：
```cpp
// 旧用法
Q_PROPERTY_CREATE(int, BorderRadius)

// 新用法（向下兼容）
Q_PROPERTY_CREATE(int, BorderRadius)

// 新能力
Q_PROPERTY_CREATE(QS_SET_CREF(QColor), BaseColor)  // 自定义类型
Q_PROPERTY_CREATE(int, Value, QS_DEFAULT_VALUE(42), QS_NOEXCEPT(true))
```

**合并建议**：user-confirm。这是一个完整重写，如果上游 NexUs 发布了新版本，此文件需要手动合并。新系统依赖 `LinnUtils.h`（未追踪文件）。

---

### 2.4 NXDef.h — 枚举系统重构（+138/-45）

**范围**：`Source/NXDef.h`

**变更内容**：
1. 移除 `#define NXDefVersion "2.0.2"` 
2. 移除 Qt 版本条件编译宏（`Q_BEGIN_ENUM_CREATE` 的 Qt5/Qt6 分支）
3. 新增 `__NXDefNamespace_MOC` 命名空间用于 MOC 处理
4. 所有 `Q_BEGIN_ENUM_CREATE` 改为接受导出宏参数
5. 移除旧的结构——枚举定义被重构为使用统一的 `namespace + Q_NAMESPACE_EXPORT` 模式

```cpp
// 旧
Q_BEGIN_ENUM_CREATE(NXApplicationType)
// 新
Q_BEGIN_ENUM_CREATE(NXApplicationType, NX_EXPORT)
```

**合并建议**：user-confirm。宏 API 签名变更，如果用户代码中使用 `Q_BEGIN_ENUM_CREATE` 旧形式需要同步更新。

---

### 2.5 Navigation 系统重构（涉及 10+ 文件）

**核心变更模式**（所有 Navigation 相关文件一致）：

1. **返回类型变更**：`NXNavigationType::NodeResult` → `NXNodeResultExpected`
2. **Out-parameter 移除**：`QString &expanderKey` / `QString &pageKey` 等输出参数被移除，改为直接返回 `QString` 或 `NXNodeResultExpected`
3. **`expected<T>` 模式引入**：使用 `NXNodeResultExpected`（基于 `expected.hpp` polyfill）处理成功/失败状态

```cpp
// 旧
NXNavigationType::NodeResult addExpanderNode(const QString &title, QString &key, NXIconType::IconName icon);
// 调用者需要先声明 key 变量
QString key;
addExpanderNode("Title", key, NXIconType::Home);

// 新
QString addExpanderNode(const QString &title, NXIconType::IconName icon);
// 或
NXNodeResultExpected addExpanderNode(const QString &title, const QString &targetExpanderKey, NXIconType::IconName icon);
// 调用者直接接收返回值
QString key = addExpanderNode("Title", NXIconType::Home);
```

**涉及文件**：
- `NXFooterModel.cpp/h` — `addFooterNode` 返回类型变更，`getNavigationNode` 改为 const
- `NXNavigationModel.cpp/h` — `addExpanderNode`、`addPageNode` API 重构，新增 `flags()` override（支持拖放）
- `NXNavigationNode.cpp/h` — 节点相关调整
- `NXNavigationBar.cpp/h` — 核心 API 重写，新增 `setIsNodeDragDropEnable`、`setToolTipOffset`
- `NXNavigationView.cpp/h` — 新增拖放支持、compact tooltip
- `NXNavigationStyle.cpp` — 样式调整
- `NXWindow.cpp/h` — 转发 API 同步变更
- `NXNavigationBarPrivate.cpp/h` — 新增 `onNavigationCloseCurrentWindow` 回调

**合并建议**：user-confirm。API 不向下兼容，所有使用这些 API 的用户代码需要更新。

---

### 2.6 ContentDialog — 完整重写（+41/-159）

**范围**：`Source/NXContentDialog.cpp/h`、`Source/private/NXContentDialogPrivate.cpp/h`

**变更**：
- **移除**：三个硬编码按钮（left/middle/right）及对应的信号/槽
- **移除**：`QTimer`、`NXText` include 依赖
- **新增**：动态按钮系统 `addButton(const QString &text)` / `removeButton(NXPushButton *button)`
- **简化**：`setCentralWidget` 改为更健壮的布局管理
- **删除**：`onLeftButtonClicked`、`onMiddleButtonClicked`、`onRightButtonClicked` 虚方法
- **删除**：`setLeftButtonText`、`setMiddleButtonText`、`setRightButtonText` 等方法

```cpp
// 旧 API
dialog->setLeftButtonText("取消");
dialog->setRightButtonText("确定");
connect(dialog, &NXContentDialog::leftButtonClicked, ...);
connect(dialog, &NXContentDialog::rightButtonClicked, ...);

// 新 API
auto *btn = dialog->addButton("确定");
btn->setLightDefaultColor(...);
connect(btn, &NXPushButton::clicked, ...);
```

**合并建议**：user-confirm。API 完全不兼容，外部代码需要重写。

---

### 2.7 NXCheckBox + NXRadioButton — Pimpl 架构引入

**范围**：`NXCheckBox.cpp/h`、`NXRadioButton.cpp/h`、`NXCheckBoxPrivate.cpp/h`(untracked)、`NXRadioButtonPrivate.h`

**变更**：
1. 引入 Pimpl 模式：新增 `d_ptr(new NXCheckBoxPrivate())` / `d_ptr(new NXRadioButtonPrivate())`
2. `NXCheckBoxStyle` 的使用改为通过 private 指针管理
3. 新增公共 API：
   - `setTextPixelSize` / `getTextPixelSize`
   - `setTextPointSize` / `getTextPointSize`
   - `setTextStyle` / `getTextStyle`（支持 Caption/Body/Subtitle/Title 等）
   - `setBorderRadius` / `getBorderRadius`
   - `setCheckIndicatorWidth` / `getCheckIndicatorWidth`
4. `NXRadioButton` 额外添加：`setMouseTracking(true)` + `setCursor(Qt::PointingHandCursor)`

**合并建议**：user-confirm（架构变更，新增了 Pimpl 文件和大量新 API）。

---

### 2.8 NXSlider — 架构重构

**范围**：`NXSlider.cpp/h`、`NXSliderPrivate.cpp/h`(untracked)

**变更**：
1. 引入 Pimpl 模式
2. 第二个构造函数（orientation）改为委托构造
3. 新增 `mousePressEvent` / `mouseMoveEvent` / `mouseReleaseEvent` 重写
4. 集成 `NXToolTip` 显示当前值（滑块拖动时显示 ToolTip 数值）
5. 移除 `QEvent` include，新增 `QMouseEvent`、`QStyleOptionSlider` include

**合并建议**：user-confirm（架构变更）。

---

### 2.9 Style 文件增强（10 个文件）

每个 Style 文件的变化：

| 文件 | 变更摘要 |
|------|---------|
| `NXCheckBoxStyle.cpp/h` | 新增 `_pBorderRadius` 属性，替换硬编码 2px；按下状态添加 brush 填充 |
| `NXColorValueSliderStyle.cpp/h` | 修复内存泄漏（`delete _baseGradient`）；新增 `IsUseAlpha` 属性支持透明度模式 |
| `NXComboBoxStyle.cpp` | 修复 `painter->restore()` 位置错误 |
| `NXLineEditStyle.cpp/h` | 新增 `BorderRadius` / `IconMargin` 属性；添加 `pixelMetric` 重写 |
| `NXListViewStyle.cpp` | 默认 `_pIsTransparent = true`；简化 frame 绘制逻辑 |
| `NXMenuStyle.cpp/h` | 新增 `BorderRadius` 属性；hover 状态下添加颜色标识条、图标/文字抖动效果 |
| `NXTabBarStyle.cpp/h` | 大幅重构（+216/-54），属性化+样式增强 |
| `NXTableViewStyle.cpp/h` | 大幅重构（+155/-28） |
| `NXTreeViewStyle.cpp/h` | 大幅重构（+52/-11） |
| `NXNavigationStyle.cpp` | 导航样式增强（+21） |

**合并建议**：
- 小型增强 → auto-merge
- TabBarStyle / TableViewStyle / TreeViewStyle → user-confirm

---

### 2.10 NXText — 功能增强

**范围**：`NXText.cpp/h`、`NXTextPrivate.h`

**变更**：
1. 新增 `IsClickEnable` 属性 + `clicked()` 信号（通过 `mouseReleaseEvent` / `enterEvent` / `leaveEvent` 实现）
2. 修复 `setIsWrapAnywhere` 使用 `_pIsWrapAnywhere` 而非 `_isWrapAnywhere`
3. 新增 `paintEvent` 改进：支持图标 + 文字混合绘制，`textSpacing` 从成员变量改为常量 0.5
4. `#NXText{background-color:transparent}` 改为 `QStringLiteral` 包装
5. 使用 `QChar` 构造函数而非 `(unsigned short)` 强制转换
6. `setNXIcon` 参数重命名 `elaIcon` → `icon`（清理旧项目命名痕迹）

**合并建议**：user-confirm（新增功能较多）。

---

### 2.11 NXToolTip — 重写

**范围**：`Source/NXToolTip.cpp/h`、`Source/private/NXToolTipPrivate.cpp/h`

大幅重写了 ToolTip 系统，新增：
- `setOffSetX` / `setOffSetY` 偏移配置
- `setIsMoveEnable` 是否随鼠标移动
- 动画/定位逻辑优化

**合并建议**：user-confirm。

---

### 2.12 NXTabWidget / NXTabBar — 新增样式 API

**范围**：`NXTabWidget.cpp/h`、`NXTabWidgetPrivate.cpp/h`、`NXTabBar.cpp/h`

新增 API：
- `setTabBarStyle(TabBarStyle)` / `getTabBarStyle()`
- `setTabCornerRadius(int)` / `getTabCornerRadius()`
- `setIsSelectedIndicatorVisible(bool)` / `getIsSelectedIndicatorVisible()`

**合并建议**：user-confirm。

---

### 2.13 NXWindow — API 同步

**范围**：`NXWindow.cpp/h`、`NXWindowPrivate.cpp`

同步 Navigation 系统的 API 变更：
- `addExpanderNode` — 移除 out-parameter
- `addPageNode` — 返回类型改为 `NXNodeResultExpected`
- `addFooterNode` — 移除 out-parameter
- `addCategoryNode` — 移除 out-parameter
- 移除 `navigationNodeClicked` 信号转发

**合并建议**：user-confirm。

---

### 2.14 其他增强

| 文件 | 变更 |
|------|------|
| `NXKeyBinder.cpp` | 按键绑定逻辑增强（+26/-12） |
| `NXDockWidget.cpp/h` | Dock 窗口增强（+18/-5） |
| `NXMenu.cpp/h` | 菜单功能增强（+36/-3） |
| `NXWidget.cpp/h` | Widget 基础增强（+9/-2） |
| `NXPushButton.cpp/h` | 按钮功能大幅增强（+89/-33） |
| `NXToggleButton.cpp/h` | 开关按钮增强（+22/-4） |
| `NXScrollPage.cpp/h` | 滚动页面增强（+6/-3） |
| `NXMaskWidget.cpp/h` | 新增 `animationFinished()` 信号 |
| `NXIntValidator.cpp` | `_completeInput` 优化（从 while 循环改为直接构造） |
| `NXMicaBaseInitObject.cpp` | `std::move` 优化 + 链式调用合并 |
| `NXComboBox.cpp` | ComboBox 重构（+21/-30） |
| `NXColorDialog.cpp/h` | 颜色对话框增强（+92/-29） |
| `NXTheme.cpp/h` | 主题系统增强（+42/-27） |
| `NXTableView.cpp/h` | 表格视图增强（+97/-4） |

---

## 三、合并决策总表

### 3.1 可直接自动合并（auto-merge）

这些变更属于非破坏性增强、Bug 修复或清理，LLM 可直接合并：

| 文件 | 理由 |
|------|------|
| `NXCheckBoxStyle.cpp/h` | 新增属性，向后兼容 |
| `NXColorValueSliderStyle.cpp/h` | 修复内存泄漏 + 透明度支持 |
| `NXComboBoxStyle.cpp` | 修复 painter restore 位置 |
| `NXIntValidator.cpp` | 性能优化 |
| `NXLineEditStyle.cpp/h` | 新增属性，向后兼容 |
| `NXListViewStyle.cpp` | 透明模式改进 |
| `NXMaskWidget.cpp/h` | 新增信号，向后兼容 |
| `NXMicaBaseInitObject.cpp` | 性能优化 |
| `NXNavigationNode.cpp/h` | 内部调整 |
| `NXNavigationStyle.cpp` | 样式增强 |
| `NXNavigationView.cpp/h` | 新增功能，向后兼容 |
| `NXNavigationBarPrivate.cpp/h` | 新增回调 |
| `NXActionCommander.cpp/h` | Singleton 清理 |
| `NXApplication.cpp/h` | Singleton 清理 + 增强 |
| `NXDockWidget.cpp/h` | 增强 |
| `NXDxgiManager.cpp/h` | Singleton 清理 |
| `NXEventBus.cpp/h` | Singleton 清理 |
| `NXExponentialBlur.cpp/h` | Singleton 清理 |
| `NXIcon.cpp/h` | Singleton 清理 |
| `NXKeyBinder.cpp` | 功能增强 |
| `NXLineEdit.cpp/h` | 功能增强 |
| `NXLog.cpp/h` | Singleton 清理 |
| `NXMenu.cpp/h` | 功能增强 |
| `NXNavigationRouter.cpp/h` | Singleton 清理 |
| `NXPushButtonPrivate.h` | 内部调整 |
| `NXRadioButtonPrivate.h` | 内部调整 |
| `NXRouter.cpp/h` | Singleton 清理 + 重构 |
| `NXScreenCaptureManager.cpp/h` | Singleton 清理 |
| `NXScrollPage.cpp/h` | 增强 |
| `NXToggleButton.cpp/h` | 增强 |
| `NXToolTip.cpp/h` | 增强 |
| `NXTreeView.cpp/h` | 增强 |
| `NXWidget.cpp/h` | 增强 |
| `NXLineEditPrivate.h` | 内部调整 |
| `NXMessageBarPrivate.cpp` | 内部调整 |
| `NXRouterPrivate.cpp` | 内部调整 |
| `NXTabWidgetPrivate.cpp/h` | 内部调整 |
| `NXTextPrivate.h` | 内部调整 |
| `NXThemePrivate.h` | 内部调整 |
| `NXToggleButtonPrivate.h` | 内部调整 |
| `NXWindowPrivate.cpp` | 内部调整 |

### 3.2 需要用户确认（user-confirm）

这些变更涉及 API 破坏、架构变更或大量修改：

| 优先级 | 文件 | 确认要点 |
|--------|------|---------|
| **高** | `NXProperty.h` | 完整重写，依赖 LinnUtils.h |
| **高** | `NXDef.h` | 宏 API 签名变更 |
| **高** | `NXContentDialog.cpp/h` | API 完全重写，破坏兼容性 |
| **高** | `NXNavigationBar.cpp/h` | API 破坏性变更 |
| **高** | `NXNavigationModel.cpp/h` | API 破坏性变更 |
| **高** | `NXFooterModel.cpp/h` | API 破坏性变更 |
| **高** | `NXWindow.cpp/h` | API 破坏性变更 |
| **中** | `NXCheckBox.cpp/h` | 新增 Pimpl 架构 + 大量新 API |
| **中** | `NXRadioButton.cpp/h` | 新增新 API |
| **中** | `NXSlider.cpp/h` | 新增 Pimpl 架构 + ToolTip 集成 |
| **中** | `NXTabBarStyle.cpp/h` | 大幅重构（+216/-54） |
| **中** | `NXTableViewStyle.cpp/h` | 大幅重构（+155/-28） |
| **中** | `NXTreeViewStyle.cpp/h` | 大幅重构（+52/-11） |
| **中** | `NXMenuStyle.cpp` | 视觉样式变更 |
| **中** | `NXTabWidget.cpp/h` | 大量新 API |
| **中** | `NXTabBar.cpp/h` | 大量新 API |
| **中** | `NXPushButton.cpp` | 大幅增强（+89/-33） |
| **中** | `NXText.cpp/h` | 新增点击功能 + 绘制改进 |
| **中** | `NXToolTipPrivate.cpp/h` | 重写（+78/-39） |
| **中** | `NXColorDialog.cpp/h` | 大幅增强（+92/-29） |
| **中** | `NXComboBox.cpp` | 重构 |
| **中** | `NXTheme.cpp/h` | 重构（+42/-27） |
| **中** | `NXTableView.cpp/h` | 大幅增强（+97/-4） |
| **低** | `NXColorDialogPrivate.cpp/h` | 配合 ColorDialog 修改 |
| **低** | `NXContentDialogPrivate.cpp/h` | 配合 ContentDialog 修改 |

---

## 四、附件

本报告附带以下附件文件：

| 文件 | 说明 |
|------|------|
| `full_diff.patch` | `git diff HEAD` 的完整 unified diff 输出（655KB），包含所有 114 个已修改文件的逐行变更 |

---

## 五、注意事项

### 5.1 CRLF/LF 行尾问题
- **`Source/NXProperty.h`** 存在 LF→CRLF 警告，建议统一项目行尾风格
- 建议在项目根目录添加 `.gitattributes`：`* text=auto`

### 5.2 上游依赖变化
- 删除了对 `Q_SINGLETON_CREATE_CPP` 宏的依赖（原定义于 `Source/include/LinnSingleton.h`）
- 新的属性系统依赖 `LinnUtils.h`（位于 `Source/LinnUtils.h`，未追踪）
- 引入了 `expected.hpp` polyfill
- `NXCheckBox` / `NXSlider` 新增了对应的 `*Private.cpp` 实现文件

### 5.3 命名清理
- 部分文件仍有 `ElaWidgetTools` / `elaIcon` 等旧命名残留，建议后续统一清理
- `CMakeLists.txt` 中项目名称为 `ElaWidgetTools`，与 NexUs 项目名不一致

### 5.4 未追踪文件
8 个未追踪文件是新增的依赖/架构文件，不包含在上游原始项目中。合并时需要决定是否纳入版本控制。
