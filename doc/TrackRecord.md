# TrackRecord.md - NexUs 修改记录

# 概述

本文档记录了 NexUs 项目对 ElaWidgetTools 库的修改和优化内容，方便开发者了解具体改动点，便于后续维护和使用。

# 一、修改的文件

## 一、修改的文件

### 1.1 变量替换规则

以下替换操作已应用于指定文件类型：`*.h;*.cpp;*.cc;*.hh;*.hpp`

#### 全词匹配（区分大小写）

| 原内容         | 替换为          | 说明                  |
| -------------- | --------------- | --------------------- |
| `ELA`          | `NX`            | 命名空间前缀          |
| `":/include`   | `":/Resource`   | 资源路径前缀          |
| `eApp`         | `nxApp`         | 应用实例变量名        |
| `eWinHelper`   | `nxWinHelper`   | Windows辅助工具变量名 |
| `eTheme`       | `nxTheme`       | 主题变量名            |
| `ElaAwesome`   | `NXAwesome`     | 字体图标类名          |
| `QNXpsedTimer` | `QElapsedTimer` | 计时器类名修复        |
| `TCP_NODNXY`   | `TCP_NODELAY`   | 网络选项修复          |

#### 部分匹配（区分大小写）

| 原内容 | 替换为 | 说明     |
| ------ | ------ | -------- |
| `Ela`  | `NX`   | 类名前缀 |

#### 函数签名优化

**所有函数签名中的 `QString` 参数已替换为 `const QString&`，以提高性能并避免不必要的拷贝。**

**注意：** 被宏 `Q_PROPERTY_CREATE_Q_H` 包裹的属性声明也进行了相应替换。

## .h文件中的成员变量结构布局改动

```C++
// 调整了成员变量声明先后顺序，减少空间开销使用
```



# TrackRecord.md - NexUs 修改记录

## 概述

本文档记录了 NexUs 项目对 ElaWidgetTools 库的修改和优化内容，方便开发者了解具体改动点，便于后续维护和使用。

---

## 一、修改的文件

### 1.1 变量替换规则

以下替换操作已应用于指定文件类型：`*.h;*.cpp;*.cc;*.hh;*.hpp`

#### 全词匹配（区分大小写）

| 原内容         | 替换为          | 说明                  |
| -------------- | --------------- | --------------------- |
| `ELA`          | `NX`            | 命名空间前缀          |
| `":/include`   | `":/Resource`   | 资源路径前缀          |
| `eApp`         | `nxApp`         | 应用实例变量名        |
| `eWinHelper`   | `nxWinHelper`   | Windows辅助工具变量名 |
| `eTheme`       | `nxTheme`       | 主题变量名            |
| `ElaAwesome`   | `NXAwesome`     | 字体图标类名          |
| `QNXpsedTimer` | `QElapsedTimer` | 计时器类名修复        |
| `TCP_NODNXY`   | `TCP_NODELAY`   | 网络选项修复          |

#### 部分匹配（区分大小写）

| 原内容 | 替换为 | 说明     |
| ------ | ------ | -------- |
| `Ela`  | `NX`   | 类名前缀 |

#### 函数签名优化

**所有函数签名中的 `QString` 参数已替换为 `const QString&`，以提高性能并避免不必要的拷贝。**

**注意：** 被宏 `Q_PROPERTY_CREATE_Q_H` 包裹的属性声明也进行了相应替换。

---

### 1.2 具体文件改动

#### 样式类文件

**DeveloperComponents/NXCheckBoxStyle.cpp**
- 新增 `_pCheckBorderRadius` 成员变量，支持自定义复选框圆角
- `drawControl` 方法逻辑优化，增强绘制效果

**DeveloperComponents/NXListViewStyle.cpp**
- `drawControl` 方法中 `case QStyle::CE_ShapedFrame:` 部分的绘制条件逻辑优化

**DeveloperComponents/NXMicaBaseInitObject.cpp**
- `onInitMicaBase` 方法参数优化为引用传递
- 使用 `std::move` 移动图片数据，避免不必要的拷贝

**DeveloperComponents/NXTreeViewStyle.cpp**
- 为 `NXIconType::IconName` 枚举添加新图标
- `drawControl` 方法逻辑扩展，支持在树节点文字前添加自定义图标
- 为 `NXTreeView` 类新增 `setNXIcon` 接口，支持内置 `NXIconType::IconName` 字体图片渲染

#### 单例模式优化

以下文件中的单例模式实现已使用预处理命令进行统一优化：
- `include/NXApplication.h`
- `include/NXDxgiManager.h`
- `include/NXEventBus.h`
- `include/NXExponentialBlur.h`
- `include/NXLog.h`
- `include/NXNavigationRouter.h`
- `include/NXTheme.h`

#### 控件功能增强

**NXContentDialog.cpp**
- 新增 `appBar()` 接口，提供对应用栏的访问

**NXDockWidget.cpp**
- 新增 `dockClosed`、`dockResized` 信号，增强停靠窗口交互

**NXMessageButton.cpp**
- 消息响应逻辑分离到私有类 `NXMessageButtonPrivate`
- 新增 `disconnected` 接口，允许断开点击后自动发送消息的默认连接
- 可以使用 `showMessage` 方法替代默认行为
- **private/NXMessageButtonPrivate.cpp** - 实现 `_showMessage` 方法

**NXPushButton.cpp**
- 新增字体大小设置接口

**NXRadioButton.cpp**
- 默认启用 `setMouseTracking` 以设置鼠标手势
- 新增字体大小设置接口

**NXSlider.cpp**
- 新增滑动时显示当前值的 ToolTip
- **private/NXSliderPrivate.cpp** (新增) - 实现滑块私有逻辑

**NXToggleButton.cpp**
- 新增对 `NXIconType::IconName` 的支持，可使用内置字体图标
- **private/NXToggleButtonPrivate.h** - 新增相关成员变量

**NXToolButton.cpp**
- 新增 `NXAdvancedToolButton` 类，作为 `NXToolButton` 的父类
- 支持鼠标滚轮和右键点击事件

**NXToolTip.cpp**
- 新增 `OffSetX`、`OffSetY` 变量，可设置提示框偏移
- `updatePos` 接口优化
- **private/NXToolTipPrivate.cpp** - 变量设置初始化

**NXWidget.cpp**
- 新增 `appBar` 接口，提供对应用栏的访问
- 新增自定义背景色接口
- **private/NXWidgetPrivate.h** - 新增相关成员变量

**private/NXCheckBoxPrivate.cpp** (新增) - 复选框私有逻辑实现

---

### 1.3 重要功能模块改动

#### 导航系统增强

**DeveloperComponents/NXFooterModel.cpp**
- 导航栏方法返回值修改：`void` → `NodeOperateReturnTypeWithKey`
- 替换引用参数获取 key，改为返回值传递

**DeveloperComponents/NXNavigationModel.cpp**
- 导航栏方法返回值修改：`void` → `std::expected` 或 `tl::expected`
- 替换引用参数获取 key，改为返回值传递
- 新增拖拽功能支持，包含指示器显示位置
- 新增 `getRootNode()` 接口

**DeveloperComponents/NXNavigationNode.cpp**

- 根节点 `IsExpanded` 属性修改
- 新增节点标题设置接口
- 新增 `insertChildNode` 接口，支持在指定位置插入子节点

**DeveloperComponents/NXNavigationView.cpp**
- 新增拖拽功能支持，包含指示器显示位置
- 新增获取 ToolTip 内容的接口
- `_doCompactToolTip` 方法优化

**NXNavigationBar.cpp**
- 新增 `navigationPositionSwapped` 信号槽连接
- 新增 `navigationCloseCurrentWindow` 信号槽连接
- 新增 `rowsMoved` 信号槽连接，支持拖拽功能
- 新增 `setIsLeftButtonPressedToggleNavigation`、`setNavigationNodeDragAndDropEnable` 接口，支持拖拽设置
- 新增 `setToolTipOffset` 接口，设置提示位置偏移
- 新增 `setNavigationPageOpenPolicy` 接口，支持自定义窗口打开逻辑
- 新增 `getNavigationRootKey` 接口，获取根节点 key
- 导航栏方法返回值修改：`void` → `NodeOperateReturnTypeWithKey`

**private/NXNavigationBarPrivate.cpp**
- 实现 `onNavigationCloseCurrentWindow` 方法
- 调整 `navigationNodeClicked` 信号的发送位置

**NXWindow.cpp**
- 将转发点击信号移动到跳转处理槽函数内
- 新增 `switch` 接口，根据 nodekey 切换页面
- 新增 `setIsLeftButtonPressedToggleNavigation`、`setNavigationNodeDragAndDropEnable` 接口
- 新增 `getNavigationRootKey` 接口
- 新增 `setNavigationPageOpenPolicy` 接口
- 导航栏方法返回值修改：`void` → `NodeOperateReturnTypeWithKey`

**private/NXWindowPrivate.cpp**
- `onNavigationNodeRemoved`、`onNavigationNodeClicked` 逻辑优化

#### 控件功能增强

**DeveloperComponents/NXTableViewStyle.cpp**
- 新增变量：是否启用隔行变色、是否启用选中效果、是否启用悬停效果、圆角半径
- 新增 `adjustHeaderColumnIconRect`、`adjustColummTextRect` 接口，微调文本绘制位置
- `CE_HeaderLabel` 中新增表头图标绘制
- `CE_ItemViewItem` 中为第一列新增复选框绘制逻辑
- 为 `QAbstractItemView::SelectItems` 模式在底部绘制水平条表示选中

**NXTableView.cpp**

- 新增接口：`IsDrawAlternateRowsEnabled`、`IsSelectionEffectsEnabled`、`IsHoverEffectsEnabled`、圆角、Header 字体大小、内容 margin
- 新增 `NXModelIndexWidget`，支持为 TableView 添加小控件

**private/NXTableViewPrivate.h**
- 新增 `NXModelIndexWidgetPrivate` 类定义
- `DrawSelectionBackground` 改为变量控制

**DeveloperComponents/NXToolButtonStyle.cpp**
- `_drawIcon` 逻辑优化，调整不同图标的渲染位置，提升视觉效果

**NXCheckBox.cpp**
- 新增字体大小、圆角、宽度设置接口

**NXColorDialog.cpp**
- 新增 alpha 通道处理
- `getCurrentColorRGB` 接口重命名为 `getCurrent4ChannelColor`

**DeveloperComponents/NXColorValueSliderStyle.cpp**
- 新增 alpha 通道处理

**private/NXColorDialogPrivate.cpp**
- 新增 alpha 通道处理

**NXLineEdit.cpp**
- 新增圆角设置接口、内容边距设置接口
- 新增委托构造函数

**DeveloperComponents/NXLineEditStyle.cpp**
- 新增圆角参数设置
- `drawControl` 方法逻辑优化
- 新增 `pixelMetric` 处理 LineEdit 的 icon margin 显示

**private/NXLineEditPrivate.h**
- 将部分成员变量移至 `NXLineEditStyle`

**NXMenu.cpp**
- 新增圆角设置接口
- `showEvent` 优化，改进子菜单对齐
- 新增 `setAlignParentMenuHeight` 接口，使子菜单与父菜单同一水平对齐
- 新增 `isHasParentMenu` 接口判断是否为父菜单

**DeveloperComponents/NXMenuStyle.cpp**
- 新增圆角参数设置
- `drawControl` 方法逻辑优化，新增悬停背景色绘制、左侧小竖条（表示选中或焦点）
- 悬停时图标和文字有轻微位移效果

**NXText.cpp**
- 新增边框类型选择设置颜色粗细接口
- 字体大小接口优化
- 支持点击发送信号

**private/NXTextPrivate.h**
- 新增相关成员变量

#### 通用工具与定义

**include/NXDef.h**
- 移除枚举宏，分离到 `include/NXProperty.h`
- 新增多个枚举类型

**include/NXProperty.h**
- 新增属性宏
- 优化枚举导出宏设计

**include/singleton.h**
- 单例宏优化

---

### 1.4 新增文件

#### 私有实现类
- `private/NXSliderPrivate.cpp`
- `private/NXCheckBoxPrivate.cpp`
- `private/NXSliderPrivate.cpp`
- `private/NXShadowGraphicsEffectPrivate.cpp`

#### 功能模块
- `NXSharedMemoryManager.cpp`
- `NXAESEncryption.cpp`
- `NXCentralStackedWidget.cpp`（从 DeveloperComponents 提升为导出控件）
- `NXShadowGraphicsEffect.cpp`
- `NXShadowWidget.cpp`

#### 第三方库引入
- `include/expected.hpp`
- `include/magic_enum`
- `include/aesni`

#### RainbowCandyX 开发者贡献
- `NXMessageCard`、`NXMessageCardPrivate`
- `NXMessageDialog`、`NXMessageDialogPrivate`
- `NXMultiSelectComboBoxDelegate`
- `NXGroupBox`、`NXGroupBoxPrivate`、`NXGroupBoxStyle`
- `NXInputDialog`、`NXInputDialogPrivate`
- `NXTableWidget`、`NXTableWidgetStyle`、`NXTableWidgetPrivate`
- `NXScreenCapture`、`NXScreenCaptureManager`、`NXScreenCaptureManagerPrivate`

## 二、总结

### NXWidget

```c++
新增文件
NXWidgetTools/singleton.h
NXWidgetTools/private/NXTableWidgetPrivate.h
NXWidgetTools/private/NXTableWidgetPrivate.cpp
NXWidgetTools/private/NXSliderPrivate.h
NXWidgetTools/private/NXSliderPrivate.cpp
NXWidgetTools/private/NXShadowGraphicsEffectPrivate.h
NXWidgetTools/private/NXShadowGraphicsEffectPrivate.cpp
NXWidgetTools/private/NXScreenCaptureManagerPrivate.h
NXWidgetTools/private/NXScreenCaptureManagerPrivate.cpp
NXWidgetTools/private/NXMessageDialogPrivate.h
NXWidgetTools/private/NXMessageDialogPrivate.cpp
NXWidgetTools/private/NXMessageCardPrivate.h
NXWidgetTools/private/NXMessageCardPrivate.cpp
NXWidgetTools/private/NXInputDialogPrivate.h
NXWidgetTools/private/NXInputDialogPrivate.cpp
NXWidgetTools/private/NXGroupBoxPrivate.h
NXWidgetTools/private/ NXGroupBoxPrivate.cpp
NXWidgetTools/private/NXCheckBoxPrivate.h
NXWidgetTools/private/NXCheckBoxPrivate.cpp
NXWidgetTools/NXTableWidget.h
NXWidaetTools/NXTableWidget.cpp
NXWidgetTools/NXSharedMemoryManager.h
NXWidgetTools/NXSharedMemoryManager.cpp
NXWidgetTools/NXShadowWidget.h
NXWidgetTools/NXShadowWidget.cpp
NXWidgetTools/NXShadowGraphicsEffect.h
NXWidgetTools/NXShadowGraphicsEffect.cpp
NXWidgetTools/NXScreenCaptureManager.h
NXWidgetTools/NXScreenCaptureManager.cpp
NXWidgetTools/NXMessageDialog.h
NXWidgetTools/NXMessageDialog.cpp
NXWidgetTools/NXMessageCard.h
NXWidgetTools/NXMessageCard.cpp
NXWidgetTools/NXInputDialog.h
NXWidgetTools/NXInputDialog.cpp
NXWidgetTools/NXGroupBox.h
NXWidgetTools/NXGroupBox.cpp
NXWidgetTools/NXCentralStackedWidget.h
NXWidgetTools/NXCentralStackedWidget.cpp
NXWidgetTools/NXAESEncryption.h
NXWidgetTools/NXAESEncryption.cpp
NXWidgetTools/DeveloperComponents/NXTableWidgetStyle.h
NXWidgetTools/DeveloperComponents/NXTableWidgetStyle.cpp
NXWidgetTools/DeveloperComponents/NXScreenCapture.h
NXWidgetTools/DeveloperComponents/NXScreenCapture.cpp
NXWidgetTools/DeveloperComponents/NXMultiselectComboBoxDelegate.h
NXWidgetTools/DeveloperComponents/NXMultiSelectComboBoxDelegate.cpp
NXWidgetTools/DeveloperComponents/NXGroupBoxStyle.h
NXWidgetTools/DeveloperComponents/NXGroupBoxStyle.cpp

以下代码比较复杂
NXWidgetTools/DeveloperComponents/NXNavigationView.h
NXWidgetTools/DeveloperComponents/NXNavigationModel.h
NXWidgetTools/DeveloperComponents/NXNavigationNode.cpp
NXWidgetTools/DeveloperComponents/NXNavigationStyle.cpp
NXWidgetTools/private/NXNavigationBarPrivate.h
NXWidgetTools/NXNavigationBar.h
NXWidgetTools/private/NXNavigationBarPrivate.cpp
NXWidgetTools/DeveloperComponents/NXNavigationModel.cpp
NXWidgetTools/DeveloperComponents/NXNavigationView.cpp
NXWidgetTools/NXNavigationBar.cpp

NXWidgetTools/NXWindow.h
NXWidgetTools/NXWindow.cpp
NXWidgetTools/private/NXWindowPrivate.cpp
NXWidgetTools/NXTableView.h
NXWidgetTools/NXTableView.cpp
NXWidgetTools/DeveloperComponents/NXTableViewStyle.cpp
NXWidgetTools/NXLineEdit.cpp
NXWidgetTools/NXSlider.cpp
NXWidgetTools/DeveloperComponents/NXToolButtonStyle.cpp
NXWidgetTools/NXContentDialog.cpp
NXWidgetTools/NXColorDialog.cpp
NXWidgetTools/private/NXColorDialogPrivate.cpp
NXWidgetTools/NXText.cpp


以下代码修改内容不算很多，或者修改地方固定不复杂
NXWidgetTools/NXDef.h
NXWidgetTools/NXProperty.h

NXWidgetTools/DeveloperComponents/NXColorValueSliderStyle.h
NXWidgetTools/DeveloperComponents/NXTreeViewStyle.h
NXWidgetTools/DeveloperComponents/NXLineEditStyle.h
NXWidgetTools/DeveloperComponents/NXMicaBaseInitObject.cpp
NXWidgetTools/DeveloperComponents/NXFooterModel.cpp
NXWidgetTools/DeveloperComponents/NXCheckBoxStyle.cpp
NXWidgetTools/DeveloperComponents/NXTableViewStyle.h
NXWidgetTools/DeveloperComponents/NXColorPicker.cpp
NXWidgetTools/DeveloperComponents/NXNavigationNode.h
NXWidgetTools/DeveloperComponents/NXColorValueSliderStyle.cpp
NXWidgetTools/DeveloperComponents/NXMenuStyle.cpp
NXWidgetTools/DeveloperComponents/NXTreeViewStyle.cpp

NXWidgetTools/private/NXMessageButtonPrivate.h
NXWidgetTools/private/NXToggleButtonPrivate.h
NXWidgetTools/private/NXPushButtonPrivate.h
NXWidgetTools/private/NXToolTipPrivate.cpp
NXWidgetTools/private/NXLineEditPrivate.h
NXWidgetTools/private/NXTableViewPrivate.cpp
NXWidgetTools/private/NXToolTipPrivate.h
NXWidgetTools/private/NXColorDialogPrivate.h
NXWidgetTools/private/NXContentDialogPrivate.h
NXWidgetTools/private/NXMultiSelectComboBoxPrivate.h
NXWidgetTools/private/NXTableViewPrivate.h
NXWidgetTools/private/NXMessageButtonPrivate.cpp
NXWidgetTools/private/NXApplicationPrivate.cpp
NXWidgetTools/private/NXApplicationPrivate.h
NXWidgetTools/private/NXMessageBarPrivate.cpp

NXWidgetTools/NXKeyBinder.cpp
NXWidgetTools/NXLineEdit.h
NXWidgetTools/NXScrollPage.h
NXWidgetTools/NXDockWidget.h
NXWidgetTools/NXRadioButton.h
NXWidgetTools/NXMenu.h
NXWidgetTools/NXPushButton.h
NXWidgetTools/NXScrollPage.cpp
NXWidgetTools/NXToggleButton.h
NXWidgetTools/NXToolTip.cpp
NXWidgetTools/NXToolTip.h
NXWidgetTools/NXMultiSelectComboBox.h
NXWidgetTools/NXColorDialog.h
NXWidgetTools/NXIcon.h
NXWidgetTools/NXPromotionView.h
NXWidgetTools/NXTreeView.h
NXWidgetTools/NXMessageButton.h
NXWidgetTools/NXSlider.h
NXWidgetTools/NXTreeView.cpp
NXWidgetTools/NXWidget.h
NXWidgetTools/NXContentDialog.h
NXWidgetTools/NXMessageButton.cpp
NXWidgetTools/NXText.h
NXWidgetTools/private/NXTextPrivate.h
NXWidgetTools/NXCheckBox.h
NXWidgetTools/DeveloperComponents/NXLineEditStyle.cpp
NXWidgetTools/NXTabBar.cpp
NXWidgetTools/NXDockWidget.cpp
NXWidgetTools/NXToolButton.h
NXWidgetTools/NXComboBox.cpp
NXWidgetTools/NXToggleButton.cpp
NXWidgetTools/NXMultiSelectComboBox.cpp
NXWidgetTools/NXToolButton.cpp
NXWidgetTools/NXMenu.cpp
NXWidgetTools/NXRadioButton.cpp
NXWidgetTools/NXPushButton.cpp
NXWidgetTools/NXCheckBox.cpp


单例
NXWidgetTools/NXDxgiManager.h
NXWidgetTools/NXExponentialBlur.h
NXWidgetTools/NXTheme.h
NXWidgetTools/NXLog.h
NXWidgetTools/NXEventBus.h
NXWidgetTools/NXNavigationRouter.h
NXWidgetTools/NXApplication.h

```

### NXWidgetExamples

```
其他fork作者添加的自定义控件添加展示
NXWidgetToolsExample/ExamplePage/T_BaseComponents.h
NXWidgetToolsExample/ExamplePage/T_BaseComponents.cpp

NXWidgetToolsExample/ExamplePage/T_Popup.h
NXWidgetToolsExample/ExamplePage/T_Popup.cpp

NXWidgetToolsExample/ExamplePage/T_BasePage.cpp

核心修改
NXWidgetToolsExample/mainwindow.h

分离文件
NXWidgetToolsExample/ExamplePage/T_NXScreen.h
NXWidgetToolsExample/ExamplePage/T_NXScreen.cpp

废文件
NXWidgetToolsExample/ExamplePage/T_NXPacketO.cpp
NXWidgetToolsExample/ExamplePage/T_NXPacketO.h
NXWidgetToolsExample/ExamplePage/T_RecvScreen.cpp
NXWidgetToolsExample/ExamplePage/T_RecvScreen.h
NXWidgetToolsExample/ExamplePage/T_TableWidget.cpp
NXWidgetToolsExample/ExamplePage/T_TableWidget.h
```



---

## 二、改动记录

### Track Record 1：2025年3月12日提交(Commits on Mar 12, 2025)

#### Modify Optize1
- 修改了多个基础UI控件：NXWidget、NXWindow、NXColorDialog、NXTableView等
- 优化了宏定义文件：NXDef.h、stdafx.h（现为NXProperty.h）
- **NXColorDialog**：新增Alpha通道支持
- **NXDef.h/NXProperty.h**：重构代码设计，分离枚举和宏定义，新增多个实用宏
- **NXWidget**：新增自定义亮黑主题背景色，新增操作NXAppBar的接口
- **NXWindow**：新增自定义窗口打开逻辑，优化节点方法设计，新增navigationNodeRemoved信号

> **注意事项**：NXDef.h必须是MOC文件，NXProperty.h不是MOC文件，使用VS2022当修改NXDef.h文件的时候，VS2022会错误的修改文件定义为`<ClInclude Include="Source\include\NXDef.h" />`，你需要进行修改NexUs.vcproj保持为`<QtMoc Include="Source\include\NXDef.h" />`

#### Modify Optize2
1. 所有QString参数优化为const QString&引用传递
2. 导航添加方法（如addFooterNode）优化返回值传递ownerKey
3. NXNavigationNode新增自定义标题方法
4. NXNavigationView右键新增删除Node的Action并发送信号，并且发送信号给->Bar->NXWindow
5. NXNavigationBar新增通过key切换Page显示的功能——navigationPageNodeSwitch （`NXWindow一起同步修改`
6. NXMenu新增圆角设置，优化悬停效果，新增父菜单查找方法，添加参数设置子菜单和父菜单左上角同一水平
7. NXSlider新增对齐指定标记Tick的功能
8. NXToolButton优化Checkable状态的显示效果，优化ToolButtonTextUnderIcon布局
9. NXAppBar注释WM_NCCALCSIZE相关代码，解决窗口最大化边缘空缺问题
10. NXScrollPage的addCentralWidget新增滚动条方向选择参数

#### Modify Optize3
- NXContentDialog优化内部按钮信号槽连接，支持外部自定义关闭逻辑
- NXMessageButton新增showMessage方法，外部可以调用，新增disconnect按钮clicked时弹出按钮方法，外部可以取消这个点击事件，然后自行建立
- NXWindow的navigationNodeClicked信号新增QWidget*参数

#### Modify Optize4
1. NXNavigationModel新增拖拽方法
2. NXNavigationNode新增insertChildNode方法
3. NXNavigationStyle新增拖拽指示器绘制逻辑case  QStyle::PE_IndicatorItemViewItemDrop:
4. NXNavigationView新增IsLeftButtonPressedToggleNavigation选择pressed切换pagenode还是released、setNavigationNodeDragAndDropEnable方法设置支持拖拽——NXWindow同步方

   新增自定义dropIndicatorPositionOverride方法（qt内置的不太好用）、拖拽事件的完善、paintEvent指示器的绘制
5. NXNavigationBar增加信号槽连接model的move信号完成节点交换重绘

   新增setNavigationPageOpenPolicy自定义打开逻辑（可以使用内部自定义打开custom，也可以自定义方法打开PageNode的widget）（`NXWindow同步方法`
6. NXWindow新增navigationNodeToggled信号替换原来的navigationNodeClicked作用，navigationNodeClicked变为点击触发即使node没有page，navigationNodeToggled是进行page切换

   新增_currentVisibleWidget获取当前stackedWidget和nodekey、nodetype

#### Modify Optize5
- NXNavigationModel、NXNavigationView拖拽功能优化，改进交换位置逻辑，新增颜色提示

#### Modify Optize6
1. 修改stdafx.h的qt枚举导出宏，进行宏重载方便调用
2. NXTreeView新增设置NXIcon图标作为文字绘制，修改NXTreeViewStyle的文字绘制逻辑，绘制选择时的文本间距及文字颜色（如果采用Model将NXIconType作为QIcon返回NXIcon::getInstance()->getNXIcon间距太宽，图标颜色不好使用QStyle::State进行修改）
3. NXListViewStyle视口绘制逻辑修复
4. NXDockWidget新增dockClosed、dockResized信号

#### Modify Optize7
1. 修复NavigationNodeClicked/Removed的错误逻辑
2. NXSlider新增仅按下拖动功能

---

### Track Record 2：2025年6月30日提交

#### Modify Optize1
1. 清理多个控件中不必要的代码，如NXWidget、NXSlider等
2. 优化整体逻辑
3. 引入NXPacketIO和magic_enum库

---

### Track Record 3：2025年7月8日提交

#### Modify Optize1
1. 引入NXShadowGraphicsEffect、NXShadowWidget阴影效果组件

---

### Track Record 4：2025年12月26日提交

#### Modify Optize1
**主要影响文件：NXWindow.h、NXWindow.cpp、NXWindowPrivate.h**
- 删除currentVisibleWidget()逻辑

- 删除navigationNodeAdded信号参数

- 调整navigationNodeRemoved信号和NXWindowPrivate::onNavigationNodeRemoved的逻辑，删除节点后仍旧存在节点窗口就返回当前节点窗口的key（如，有两个节点1、2，当前处在节点2，删除节点2，那么将会返回节点1窗口的key，底层是QMap按照顺序来的）和窗口指针，如果节点已经删完返回rootkey和空指针

- 调整onNavigationNodeClicked逻辑

- 调整添加节点的返回值，NodeOperateReturnTypeWithKey从QPair替换为struct

- 修复开启拖拽移动NXNavigationNode的时候，以下**拖拽情况1**的情况造成程序中断， `qDeleteAll`双重释放了-窗口节点2，不知道为什么？迭代器失效？但是控制台输出的记录顺序正确，已经提示删除了-窗口节点2，范围越界？可能是，调整逻辑不使用qDeleteAll，在`insertChildNode`添加`childNode->setParent(this);`让qt关联生命周期，不进行手动管理

  > 示例：
  >
  > * **原布局	**				**拖拽情况1**				**拖拽情况2**
  >
  > 根节点 
  >
  > -窗口节点1				  -窗口节点1				-窗口节点1
  >
  > -挂载拓展节点1 		 -挂载拓展节点1		-窗口节点2 
  >
  > --窗口节点2 				-窗口节点2				-挂载拓展节点1
  >
  > -窗口节点3				  -窗口节点3			    -窗口节点3

- 完善导航节点交换逻辑，支持窗口节点直接交换

- 修复  同步 `Ela Commits on Sep 7, 2025` 版本的 NXCustomTabWidget、NXTabWidget、移除窗口逻辑，原版本CMake编译不会出现双重释放情况，但使用VS2022编译后会出现该错误 (错误出现位置 析构~NXTabWidget(), 操作了不该操作被释放的QVariant)

#### Modify Optize2
**同步[ZongChang-Liu开发者](https://github.com/ZongChang-Liu/ElaWidgetTools/commits/Zongchang_Liu?author=ZongChang-Liu)的ElaWidgetTools改进**

1. 修改NXToggleButton、支持添加 NXIcon，

   `NXToggleButton.h、NXToggleButton.cpp、NXToggleButtonPrivate.h`
2. NXToolTip添加显示偏移api,NXSlider添加ToolTip显示值

   `NXToolTip.h、NXToolTip.cpp、NXToolTipPrivate.h、NXToolTipPrivate.cpp、NXSlider.h、NXSlider.cpp、NXSliderPrivate.h(新增)、NXSliderPrivate.cpp(新增)`

3. 为NXNavigationBar添加toolTip显示位置接口

   `NXNavigationBar.h、NXNavigationBar.cpp、NXNavigationView.h、NXNavigationView.cpp`

   修改NXNavigationBar使其更加适合无userCard的显示效果

   `NXNavigationBar.cpp`

   NXNavigationBar显示模式发生改变时会发送信号

4. 修改mac下NXSpinBox焦点异常显示的bug

   `NXSpinBox.cpp`

#### Modify Optize3
1. 优化singleton.h单例模式，减少Qt依赖

2. 修改`NXProperty.h`的setter、getter的宏定义，支持普通类型如int、char直接setter、支持QString自定义cvref限定符setter，支持自定义getter返回类型加&限定，支持大类型的setter移动

   > ```
   > PROPERTY: 需要和信号关联
   > PRIVATE: 不需要和信号关联
   > _H/_CPP: Pimpl版本
   > _EX_H/_EX_CPP: 自定义setter、getter类型限定符
   > _MOVE: setter拷贝大数据开销大，可以使用移动，但请你注意该类型必须实现移动构造/赋值或是POD类型，setter时候可以调用std::move传参数，但请注意生命周期
   > ```

3. 修改NXTableView、NXNavigationView、NXNavigation，优化已有的IndexWidget和指示器代码逻辑

4. NXTabWidget的双重释放问题Ela已经修复，NXCustomTabWidget、NXTabWidget合并Ela代码

5. 优化NXTabBar版本适配

6. 引入tl::expected，支持C++23的std::expected

7. CMake构建系统优化，支持自动打包和库导出

#### Modify Optize4
**同步[RainbowCandyX](https://github.com/RainbowCandyX/ElaWidgetTools)开发者的ElaWidgetTools改进**

1. 更新ElaWidgetToolsExample文件
2. 修复若干问题
3. 引入创意组件：
   - NXMessageCard、NXMessageDialog
   - NXMultiSelectComboBoxDelegate
   - NXGroupBox、NXGroupBoxStyle
   - NXInputDialog
   - NXTableWidget、NXTableWidgetStyle
   - NXScreenCapture、NXScreenCaptureManager

---

### Track Record 5：2026年01月26日提交

#### Modify Optize1

**主要影响文件：

**移除单例文件`singleton.h`，**

**增强逻辑，新增自定义四个`LinnFunctionRegistry.h`、`LinnProperty.h`、`LinnSingleton.h`、`LinnUtils.h`宏工具文件进行使用，影响所有单例类的使用**（如果要是使用宏MSVC需要开启编译选项`/Zc:preprocessor`，库已将它作为PUBLIC导出）

**Navigation系列的接口返回值重设置，引入`tl::expected`作为返回值**

**NXContentDialog重构，接口更通用友好**

**NXTabBar.cpp**：NXTabBar::dragEnterEvent兼容qt版本坐标事件

**NXApplicationPrivate.cpp**：NXApplicationPrivate构造函数qApp信号槽兼容

**NXToolTip的逻辑修改**：修改ZongChang-Liu开发者的贡献逻辑，新增bool变量控制更符合逻辑，新增Timer管理，防止延迟show大于延迟隐藏时间时，快速EnterLeave滑动鼠标，即使鼠标指针不在控件内也被延迟展示，修改窗口尺寸没有更新正确bug（小大小，还是大而不是小的问题），NXAdvancedToolButton移除

**NXMultiCellLineEdit文件添加引入**

**NXTabBar、NXTabWidget**：支持google、firefox样式，添加左右自定义窗口接口

移除NXWidget.cpp背景色设置不必不要逻辑

同步——修复若干问题
