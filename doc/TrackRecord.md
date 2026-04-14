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

## 代码规范改动

1. override添加
2. virtual删除
3. noexcept尽可能添加
4. QStringLiteral包裹非动画、objectname的字符串
5. include头文件顺序 规范Private Development
6. 前向声明顺序
7. const &添加
8. 槽/信号 标记删除规范

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

#### 输入与颜色编辑控件

**NXCheckBox.cpp / NXCheckBox.h / DeveloperComponents/NXCheckBoxStyle.cpp / DeveloperComponents/NXCheckBoxStyle.h**

- `NXCheckBox` 从仅提供基础勾选能力，扩展为可直接配置 `TextPixelSize`、`TextPointSize`、`TextStyle`、`CheckIndicatorWidth`、`BorderRadius` 的复选框控件，文字字号、文字样式和勾选框尺寸都可以在控件层统一设置。
- `DeveloperComponents/NXCheckBoxStyle` 新增 `BorderRadius` 私有属性，勾选框绘制由固定圆角改为可配置圆角；勾选框矩形和文字矩形改为按垂直居中重新计算，修正大字号和大指示器宽度下的上下偏移问题。
- 选中按压态补充 `BasicPress` 颜色处理，勾选图标字体构造改为 `QStringLiteral("NXAwesome")`，绘制逻辑更稳定。

**NXLineEdit.cpp / NXLineEdit.h / Private/NXLineEditPrivate.cpp / Private/NXLineEditPrivate.h / DeveloperComponents/NXLineEditStyle.cpp / DeveloperComponents/NXLineEditStyle.h**

- `NXLineEdit` 新增圆角、内容边距、图标边距三个直接可配入口：`setBorderRadius`、`setContentsMargins`、`setLineEditIconMargin`，样式层通过 `DeveloperComponents/NXLineEditStyle::pixelMetric` 接管 `PM_LineEditIconMargin`，使清除按钮、前后缀图标与文本区的间距跟随控件属性同步变化。
- `Private/NXLineEditPrivate` 新增文本路由快照机制，记录文本、光标位置、选区起止与长度，形成“局部撤销/恢复 + 整体撤销/恢复”两套历史链路；内部限制快照最大数量，避免长时间输入导致历史无限膨胀。
- `NXLineEdit` 的右键菜单同步接入上述路由机制，新增“撤销 / 恢复 / 整体撤销 / 整体恢复”四项操作，并把 `Ctrl+Z`、`Ctrl+Y`、`Ctrl+Shift+Z`、`Ctrl+Shift+Y` 映射到对应逻辑，不再完全依赖 `QLineEdit` 默认栈。
- 焦点处理仍保留原来的底部高亮动画，但 `Private/NXLineEditPrivate::onWMWindowClickedEvent`、主题刷新和内容边距样式表生成全部改为更明确的状态同步写法。

**NXComboBox.cpp / NXMultiSelectComboBox.cpp / NXMultiSelectComboBox.h / Private/NXMultiSelectComboBoxPrivate.h**

- `NXComboBox` 的弹出容器查找方式从固定 `findChild<QFrame*>()` 扩展为优先取 `view()->parentWidget()`，失败时回退到 `view()->window()`，并在必要时自动补建 `QVBoxLayout`，解决不同 Qt 版本或不同宿主环境下下拉容器初始化不稳定的问题。
- `NXMultiSelectComboBox` 新增 `ShowCheckBox` 属性，允许下拉项在“纯文本多选”与“带复选框多选”两种展示方式之间切换，并通过独立委托统一绘制勾选状态。
- `Private/NXMultiSelectComboBoxPrivate` 的选中状态容器由 `QVector<bool>` 调整为 `QList<bool>`，并补充委托对象、状态刷新、列表长度校正逻辑；外部提供按文本、按索引、按列表批量设置当前选中项的方法，选中项变化后同时刷新显示文本和布尔状态列表信号。

**NXDoubleSpinBox.cpp / NXSpinBox.cpp**

- `NXDoubleSpinBox` 和 `NXSpinBox` 都显式设置为 `QLocale::C`，避免系统区域设置改变小数点/数字格式后影响数值输入、解析和显示。
- `NXDoubleSpinBox::setButtonMode`、`getButtonMode` 改为 `noexcept`，并统一不同按钮模式下行编辑器的对齐与内边距样式，保证内联模式和紧凑模式下文本显示位置一致。

**NXColorDialog.cpp / NXColorDialog.h / Private/NXColorDialogPrivate.cpp / Private/NXColorDialogPrivate.h / DeveloperComponents/NXColorPicker.cpp / DeveloperComponents/NXColorValueSliderStyle.cpp / DeveloperComponents/NXColorValueSliderStyle.h**

- `NXColorDialog` 从三通道颜色编辑器升级为四通道颜色编辑器，新增透明度滑块、第四个数值输入框、四通道十六进制字符串输出，以及基于 `NXColorSchemeType::ColorSchemeType` 的 `ARGB / RGBA / AHSV / HSVA` 展示与输入切换。
- `DeveloperComponents/NXColorValueSliderStyle` 新增 `IsUseAlpha` 属性，同一套纵向渐变滑块既可以继续表示明度，也可以切换为表示透明度；高低端颜色计算逻辑按是否启用 alpha 分支处理，确保透明度渐变不会破坏当前色相与饱和度。
- `Private/NXColorDialogPrivate` 新增 `_transparencyValueSlider`、`_transparencyValueSliderStyle`、`_fourthEdit`、`_fourthText` 及相关同步槽函数，形成“颜色选择板 / 明度滑块 / 透明度滑块 / 四个数值框 / HTML 输入框 / 颜色预览”的全链路双向同步。
- `DeveloperComponents/NXColorPicker` 对 hue 和 saturation 的计算统一使用 `qBound` 约束到 `[0, 1]`，修复鼠标拖到边缘时可能出现的浮点越界问题；`NXColorDialog::getCurrent4ChannelColor()` 改为返回完整四通道 hex 值。

#### 弹窗、菜单与提示类控件

**NXContentDialog.cpp / NXContentDialog.h / Private/NXContentDialogPrivate.cpp / Private/NXContentDialogPrivate.h / NXKeyBinder.cpp**

- `NXContentDialog` 的三按钮模型由“左/中/右三个独立槽函数 + 三个独立点击信号”重构为 `ButtonType` 枚举、统一的 `buttonClicked(ButtonType)` 信号，以及 `leftButton()` / `middleButton()` / `rightButton()` / `button(ButtonType)` 按钮访问接口。
- 三个按钮现在都支持分别设置文本、可见性和完成码，关闭动画最终通过 `Private/NXContentDialogPrivate::_doCloseAnimation(int result)` 调用 `done(result)` 收口，不再强制只能走 accept/reject 两种返回值。
- `setCentralWidget` 改为安全替换中央内容：先移出并销毁旧部件，再把新部件插回布局首位，避免旧实现中直接 `takeAt` 带来的布局顺序不稳定。
- `NXKeyBinder` 同步切换到新的对话框按钮接口，原先依赖 `middleButtonClicked`、`rightButtonClicked` 的重置/保存逻辑，改为通过 `NXContentDialog::buttonClicked` 判断 `MiddleButton`、`RightButton` 分支执行。

**NXMenu.cpp / NXMenu.h / Private/NXMenuPrivate.cpp / Private/NXMenuPrivate.h / DeveloperComponents/NXMenuStyle.h**

- `NXMenu` 新增 `BorderRadius` 和 `AlignParentMenuHeight` 配置，支持对子菜单圆角样式进行统一控制，并可在展示时将子菜单首项高度与父菜单首项进行水平对齐。
- `showEvent` 中的子菜单定位逻辑改为根据父菜单首个 `QAction` 的全局几何位置计算，从而解决级联菜单在不同 DPI 或阴影偏移下的错位问题。
- `Private/NXMenuPrivate` 新增 `AlignParentMenuHeight` 状态；`DeveloperComponents/NXMenuStyle.h` 新增 `BorderRadius` 属性，并调整图标存在标记和图标宽度记录顺序，为后续样式绘制保留统一数据入口。

**NXMessageButton.cpp / NXMessageButton.h / Private/NXMessageButtonPrivate.cpp / Private/NXMessageButtonPrivate.h**

- 原先直接写在 `NXMessageButton` 构造函数中的消息栏弹出逻辑，整体下沉到 `Private/NXMessageButtonPrivate::_showMessage`，把 Success、Warning、Information、Error 四种消息模式的分支集中管理。
- 控件新增 `showMessage` 信号和 `disconnectInternalSignalOfClicked()` 接口，允许外部先断开内部 `clicked -> _showMessage` 的默认绑定，再改由业务代码自行决定何时触发内置消息弹出逻辑。
- `MessageTargetWidget` 的默认值从空指针改为父部件，避免未显式设置目标时消息栏没有挂载容器。

**NXToolTip.cpp / NXToolTip.h / Private/NXToolTipPrivate.cpp / Private/NXToolTipPrivate.h**

- `NXToolTip` 新增 `IsMoveEnabled`、`OffSetX`、`OffSetY` 三个属性，并提供 `updatePos()` 主动刷新接口，使提示框既可以固定显示，也可以跟随鼠标移动并附带自定义偏移量。
- `Private/NXToolTipPrivate` 取消原先多处 `QTimer::singleShot` 的分散写法，改为持有 `_showTimer`、`_hideTimer`、`_autoHideTimer` 三个单次定时器，统一管理显示延迟、隐藏延迟和自动消失时机，避免频繁移入移出时出现显示/隐藏竞争。
- 文本更新后会主动 `adjustSize`、刷新布局并通过 `sizeHint()` 重算控件尺寸，解决长文本或自定义内容切换后提示框大小不刷新的问题。

**NXDockWidget.cpp / NXDockWidget.h**

- 新增 `dockClosed()`、`dockResized(QSize)` 信号，并通过重写 `closeEvent`、`resizeEvent` 在窗口关闭和尺寸变化时对外发送，便于宿主窗口同步面板状态。

#### 按钮、文本与基础显示控件

**NXPushButton.cpp / NXPushButton.h / Private/NXPushButtonPrivate.h**

- `NXPushButton` 新增 `TextPixelSize`、`TextPointSize`、`TextStyle`、`LightTextColor`、`DarkTextColor` 等属性，按钮文字的字号、字重和明暗主题文字颜色不再固定。
- 新增两组 `setNXIcon` 重载，支持为按钮附加 `NXIconType::IconName` 字体图标，并单独控制图标尺寸；`Private/NXPushButtonPrivate` 相应补充 `_icon`、`_iconSize`、`_hasIcon`、`_textStyle` 等状态字段。
- 按钮背景样式表补充 `border:none;outline:none;`，并调整底边线绘制终点，使圆角和阴影边线在高 DPI 下不再错位。

**NXRadioButton.cpp / NXRadioButton.h / Private/NXRadioButtonPrivate.h**

- `NXRadioButton` 新增 `TextPixelSize`、`TextPointSize`、`TextStyle`，内部沿用与 `NXCheckBox`、`NXPushButton` 相同的文字风格映射规则。
- 默认开启 `setMouseTracking(true)` 并设置手型光标，悬停时的交互反馈更明确；`Private/NXRadioButtonPrivate` 补充 `_textStyle` 字段保存当前样式状态。

**NXToggleButton.cpp / NXToggleButton.h / Private/NXToggleButtonPrivate.h**

- `NXToggleButton` 新增 `IsIconVisible` 属性和 `setNXIcon(NXIconType::IconName)` 接口，在原有开关按钮基础上支持“左侧图标 + 右侧文字”的组合绘制模式。
- 私有层新增 `IsIconVisible` 与 `Awesome` 状态字段，绘制阶段会根据是否显示图标切换文本居中绘制或图标文本分栏绘制。

**NXText.cpp / NXText.h / Private/NXTextPrivate.cpp / Private/NXTextPrivate.h**

- `NXText` 新增 `IsAllowClick` 属性和 `clicked()` 信号，可直接作为可点击文本使用；`enterEvent` / `leaveEvent` 会在允许点击时切换为手型光标。
- 图标文本混排逻辑重写：当同时设置 `NXIcon` 和文本时，会分别计算图标宽度、文本宽度和间距，再做整体居中布局，不再简单把图标覆盖绘制到文本区域中央。
- `Private/NXTextPrivate` 新增 `IsAllowClick`、`IsWrapAnywhere` 私有属性，保留换行方式与点击能力的统一状态管理。

**NXTreeView.cpp / NXTreeView.h / DeveloperComponents/NXTreeViewStyle.cpp / DeveloperComponents/NXTreeViewStyle.h**

- `DeveloperComponents/NXTreeViewStyle` 新增 `IconName` 属性，可在树节点文字前绘制一个统一的 `NXIconType::IconName` 字体图标，用于给整棵树附加统一语义图标。
- 样式层重新计算带复选框和不带复选框两种场景下的 `checkRect`、`iconRect`、`textRect`，并对首个被选中节点单独使用 `PrimaryNormal` 强调色，增强层级导航辨识度。
- `NXTreeView` 与样式类的组合后，原生 `QIcon` 装饰和新增字体图标装饰可以共存。

**NXPromotionView.cpp / NXPromotionView.h**

- `NXPromotionView` 新增 `resizeEvent`，会根据当前卡片数量、可用宽度和既定展开/收起宽度自动缩放卡片宽度，并按比例重算控件高度，使轮播卡片在窄窗口下仍能完整显示。
- 固定高度改为最小高度 + 动态缩放策略，原来只能按固定 300px 高度工作的限制被移除。

#### 其他配套补强

**NXScrollPage.cpp / NXScrollPage.h**

- `addCentralWidget` 新增垂直和水平滚动条策略参数，允许页面级滚动容器按场景切换 `AsNeeded`、`AlwaysOff` 等策略，而不再固定关闭水平滚动条。
- 新增 `setPageTitle`，修改标题时会同步更新面包屑第一项和内部 `_centralWidgetMap` 的标题索引映射，避免仅改标题不改路由键值导致页面切换失效。

**DeveloperComponents/NXMicaBaseInitObject.cpp**

- `onInitMicaBase` 改为接收 `const QImage&`，并将缩放与指数模糊串成一次性计算流程，减少中间拷贝。
- 生成的浅色/深色底图最终通过 `std::move` 直接写入应用私有缓存成员，降低大图拷贝成本。

**Private/NXLogPrivate.cpp**

- 日志消息处理不再直接丢弃 `QtCriticalMsg` 以上类型，新增 `QtInfoMsg` 和 `QtFatalMsg` 的格式化输出分支，日志等级覆盖更完整。
- 日志文件清理与落盘路径拼接改为统一使用 `QStringLiteral` 组合，滚动归档时的时间戳生成和文件名拼接更明确。

---

### 1.3 重要功能模块改动

#### 导航系统与窗口联动

**DeveloperComponents/NXFooterModel.cpp / DeveloperComponents/NXFooterModel.h**

- `addFooterNode` 的返回方式由“通过引用参数回写 footerKey + 返回枚举结果”改为直接返回 `NXNodeOperateResult`，成功时携带新建节点 key，失败时返回错误类型，调用链更适合和导航主体模型保持一致。
- `getFooterNodeCount`、`getNavigationNode`、`removeNavigationNode` 全部补齐 `const/noexcept` 语义，页脚节点管理职责更清晰。

**DeveloperComponents/NXNavigationModel.cpp / DeveloperComponents/NXNavigationModel.h**

- 导航节点新增接口全部改成“成功直接返回节点 key，失败返回 `NXUnexpected<QString>` 错误对象”的风格，覆盖 Expander、Category、Page 四类节点添加流程，彻底移除旧版通过 `QString &key` 输出 key 的写法。
- 模型初始化时将根节点直接加入 `_nodesMap`，并把根节点默认展开；同时新增 `getRootNode()`，供上层在不依赖字符串常量的情况下访问真实根节点对象。
- 新增拖拽移动所需的 `flags`、`supportedDragActions`、`supportedDropActions`、`mimeTypes`、`mimeData`、`canDropMimeData`、`dropMimeData`、`swapTwoNodes` 实现，拖拽节点时会把节点 key 和标题打包为 `application/x-nxnavigation-node`，支持同级挪位和跨父节点插入。
- 拖拽完成后会递归修正深度、父子关系和内部索引，保证拖拽后的模型树状态与显示层保持一致。

**DeveloperComponents/NXNavigationNode.cpp / DeveloperComponents/NXNavigationNode.h**

- `NXNavigationNode` 补齐根节点、页脚节点、是否带页脚页面、是否展开、是否可见、节点标题、节点 key、节点深度等状态管理接口，节点自身从“简单数据项”演进为“可参与树结构重排的状态对象”。
- 新增 `insertChildNode`，支持在指定位置插入子节点；新增 `swap`，便于兄弟节点直接交换顺序；`getRowExceptCategoryNodes`、`getExceptCategoryNodes` 进一步区分分类节点和实际可导航节点。

**DeveloperComponents/NXNavigationStyle.cpp / DeveloperComponents/NXNavigationStyle.h**

- 导航项样式层增加 `PressIndex`、选中标记上下边界动画值以及 `ItemHeight` 状态，选中标记不再是一次性静态绘制，而是可跟随点击和展开收起过程平滑移动。
- 绘制逻辑围绕“普通态 / 按压态 / 选中态 / 紧凑态”重新组织，为拖拽、紧凑工具提示和选中标记动画提供统一的视觉基准。

**DeveloperComponents/NXNavigationView.cpp / DeveloperComponents/NXNavigationView.h**

- 导航视图新增 `setNavigationNodeDragAndDropEnable`，真正把 `dragEnabled`、`acceptDrops`、`dropIndicatorShown`、`dragDropMode` 统一封装到视图层，避免外部直接操作多个 `QAbstractItemView` 选项。
- 新增 `navigationPositionSwapped`、`navigationOpenNewWindow`、`navigationCloseCurrentWindow` 三个信号，并在右键菜单中补充“在新窗口中打开”“关闭当前导航窗口”两项动作，扩展了导航节点的上下文操作能力。
- 自定义 `_dropIndicatorPosition`、`_canProceedWithDragDrop` 和 `paintEvent` 中的拖拽指示器绘制逻辑，解决 PageNode 在 Qt 默认拖拽指示器下显示不准确的问题。
- `_doCompactToolTip` 现在按紧凑模式实时管理 `NXToolTip` 生命周期，在紧凑导航栏中悬停节点即可显示节点标题。

**NXNavigationBar.cpp / NXNavigationBar.h / Private/NXNavigationBarPrivate.cpp / Private/NXNavigationBarPrivate.h**

- `NXNavigationBar` 对外新增 `setNavigationPageOpenPolicy`、`setIsLeftButtonPressedToggleNavigation`、`setNavigationNodeDragAndDropEnable`、`setToolTipOffset`、`getNavigationRootKey`、`getPageOpenInNewWindowCount`、`getSuggestDataList` 等接口，导航栏从单纯容器升级为可配置的导航管理入口。
- 页面、分类、展开节点和页脚节点的新增接口全部切换为新的 key/result 返回体系，导航栏不再依赖外部提前准备 `QString &pageKey` 临时变量。
- `Private/NXNavigationBarPrivate` 新增 `_pageMetaMap`、`_pageNewWindowCountMap`、`_compactMenuMap`、`_openPageFunc` 等状态，负责维护页面元对象、新窗口打开计数、紧凑模式菜单缓存以及外部自定义开窗策略。
- `Private/NXNavigationBarPrivate::onNavigationOpenNewWindow`、`onNavigationCloseCurrentWindow`、`_addStackedPage`、`_addFooterPage`、`_initNodeModelIndex`、`_resetNodeSelected`、`_expandSelectedNodeParent` 等内部流程把节点增删改查、页面注册、路由跳转和显示状态恢复真正串成闭环。

**NXWindow.cpp / NXWindow.h / Private/NXWindowPrivate.cpp / Private/NXWindowPrivate.h**

- `NXWindow` 同步对外暴露导航栏新增能力，包括：左键按下即切换导航、节点拖拽开关、导航根节点 key 获取、导航标题读写、页脚节点添加返回值、页面新窗口打开策略等，保证窗口宿主层与导航栏能力一致。
- `Private/NXWindowPrivate::onNavigationNodeClicked` 调整为“先处理页面切换，再决定是否发出外部点击信号”，并允许“无绑定页面的页脚节点”作为纯功能按钮存在，这类节点点击时也会把节点信息原样透出给外部。
- `Private/NXWindowPrivate::onNavigationNodeRemoved` 改为使用 `_routeMap.take(nodeKey)` 取出页面并执行 `deleteLater()`，删除导航节点后会自动回退到当前可显示页面；若已经没有可显示页面，则改为发出根节点信息，避免出现悬空页面指针。
- 主题切换流程中新增“切换时长小于等于 0 时直接反转主题”的分支，宿主窗口可以在禁用过渡动画时立即完成主题切换。

#### Pivot、页签与表格视图模块

**DeveloperComponents/NXPivotModel.cpp / DeveloperComponents/NXPivotModel.h / NXPivot.cpp / NXPivot.h / DeveloperComponents/NXPivotStyle.cpp / DeveloperComponents/NXPivotStyle.h / DeveloperComponents/NXPivotView.cpp / DeveloperComponents/NXPivotView.h**

- `NXPivotModel::PivotItem` 从单纯字符串项扩展为 `text + QPixmap` 组合结构，`NXPivot` 相应新增 `appendPivot`、`insertPivot`、`setPivot` 的图标重载，支持纯文本、纯图标、图文混排三类 pivot 项。
- `DeveloperComponents/NXPivotStyle` 新增 `DisplayMode`、`MarkFlags`、`TextIconSpacing`、`PivotHorizontalSpacing`、`PivotVerticalSpacing`、`OverlayTextOffsetFromCenterAnchor`、`PivotFixedSize`、`IsAutoAdaptivePivotHeight`、`IsPivotFixedSize`、`IsHoverBackgroundEnabled` 等大量视觉与布局参数，Pivot 不再局限于单行文字标签。
- 样式层新增 `_drawTextOnly`、`_drawIconOnly`、`_drawTextBesideIcon`、`_drawTextUnderIcon`、`_drawIconWithOverlayText` 等细分绘制函数，使 Pivot 可以表现为顶部图标、底部标记、悬停背景、高亮勾选等多种组合效果。
- `DeveloperComponents/NXPivotView` 新增 `HoverValidIndexCursor`、按压索引、悬停索引、已提交索引和标记区域同步逻辑；`NXPivot` 在插入、删除、替换项目时会自动修正当前索引，避免删除当前项后出现无效选中状态。

**NXTabBar.cpp / NXTabBar.h / DeveloperComponents/NXTabBarStyle.cpp / DeveloperComponents/NXTabBarStyle.h / NXTabWidget.cpp / NXTabWidget.h / Private/NXTabWidgetPrivate.cpp / Private/NXTabWidgetPrivate.h**

- `NXTabBar` 新增 `IsSelectedIndicatorVisible`、`TabCornerRadius`、`TabBarStyle`、`TabSize` 四项核心属性，页签栏样式从固定绘制演进为可切换风格、可控圆角、可控页签尺寸、可控选中指示器的通用组件。
- 拖拽页签时，`NXTabBar` 会把页签栏样式、圆角、指示器显示状态和页签尺寸一起写入拖拽 `QMimeData`，这样拖出悬浮页签窗口或拖入其他 `NXTabWidget` 时，新容器能够继承原页签栏外观。
- `DeveloperComponents/NXTabBarStyle` 新增 Google / Firefox 两套页签绘制风格，细化选中背景、悬停背景、关闭按钮 hover、选中指示条、文字截断与图标布局处理，页签栏的主题色由 `NXTheme` 新增的 TabBar 颜色项统一供给。
- `NXTabWidget` 新增 `IsTabTransparent`、`IsContainerAcceptDrops`，允许只保留页签本身绘制或允许整个容器接收外部拖入页签；`Private/NXTabWidgetPrivate` 则负责在拖出悬浮页签和拖回容器时把 `TabBarStyle`、`TabCornerRadius`、`IsSelectedIndicatorVisible`、`TabSize` 等信息重新灌入新建页签栏。

**NXTableView.cpp / NXTableView.h / Private/NXTableViewPrivate.cpp / Private/NXTableViewPrivate.h / DeveloperComponents/NXTableViewStyle.cpp / DeveloperComponents/NXTableViewStyle.h**

- `NXTableView` 新增 `HeaderMargin`、`BorderRadius`、`CheckIndicatorWidth`、`IsSelectionEffectsEnabled`、`IsHoverEffectsEnabled`、按列水平内边距设置，以及 `headerCheckIndicatorRect`、`cellCheckIndicatorRect` 两个几何查询接口，为“可点击表头勾选框”“单元格复选框命中测试”这类上层逻辑提供了直接支撑。
- `DeveloperComponents/NXTableViewStyle` 新增列级水平内边距缓存 `_horizontalPaddings`，首列默认额外留白 11 像素，并通过 `syncHorizontalPaddings` 保持与模型列数同步。
- 样式层增加表头复选框绘制、单元格复选框绘制、表头图标与文本共存绘制、整行悬停遮罩、单元格悬停遮罩、整行选中侧边条、单元格选中顶部短条、隔行背景和圆角外框绘制，表格从纯默认视图升级为完整的主题化展示控件。
- `Private/NXTableViewPrivate` 新增对 `modelReset`、`columnsInserted`、`columnsRemoved`、`layoutChanged` 的连接句柄，模型结构变化后会自动同步列 padding 状态，避免列数动态变化时样式数组越界或留白错乱。

**DeveloperComponents/NXToolButtonStyle.cpp**

- `_drawIcon` 针对 `ToolButtonIconOnly`、`ToolButtonTextBesideIcon`、`ToolButtonTextUnderIcon` 以及字体图标场景重新计算图标矩形，菜单指示器和展开箭头的占位也同步调整，保证 Tab、Navigation、普通工具按钮共用同一套样式时图标不发生偏移。

#### 主题、宿主窗口与基础视觉联动

**NXTheme.cpp / NXTheme.h / Private/NXThemePrivate.cpp / Private/NXThemePrivate.h**

- `NXTheme` 的单例实现从旧的 `NXSingleton` 宏切换为 `LinnSingleton` 宏体系，同时把 `setThemeColor`、`getThemeColor`、`setThemeMode`、`getThemeMode` 全部收敛为 `const/noexcept` 语义的稳定接口。
- `drawEffectShadow` 新增 `maxAlpha`、`extendPixels`、`lightColor`、`darkColor` 参数，阴影绘制不再只能使用固定颜色和固定透明度递减规则，TabBar、弹出层和其他浮动控件可以按需复用同一套阴影算法。
- `Private/NXThemePrivate` 的主题色表容量从 43 扩展到 48，并新增 `TabBarBase`、`TabBarSelected`、`TabBarHover`、`TabBarCloseButtonHover`、`TabBarSelectedCloseButtonHover` 五组页签栏专用颜色，为页签系统的大规模样式升级提供主题支持。

**NXWidget.cpp / NXWidget.h**

- `NXWidget` 对外新增 `getAppBar()`、窗口按钮标志获取与设置等更完整的宿主控制接口，上层可以直接访问内嵌 `NXAppBar` 并统一控制关闭、最小化、主题切换等入口。
- 默认 `AppBar` 置顶策略由 `true` 调整为 `false`，使普通宿主控件场景下不会默认占用始终置顶行为。

---

### 1.4 新增文件

#### 库扩展

```
NXWidgetTools/NXCentralStackedWidget.*
NXWidgetTools/private/NXCentralStackedWidgetPrivate.*
NXWidgetTools/private/NXCheckBoxPrivate.*
```

#### 我的引入

```
NXWidgetTools/LinnFunctionRegistry.h
NXWidgetTools/LinnProperty.h
NXWidgetTools/LinnSingleton.h
NXWidgetTools/LinnUtils.h

NXWidgetTools/NXShadowGraphicsEffect.*
NXWidgetTools/NXShadowWidget.*

NXWidgetTools/NXLazyStackedLayout.*
NXWidgetTools/NXLazyStackedWidget.*
NXWidgetTools/private/NXLazyStackedLayoutPrivate.*
NXWidgetTools/private/NXLazyStackedWidgetPrivate.*

NXWidgetTools/DeveloperComponents/NXMultiCellLineEditStyle.*
NXWidgetTools/NXMultiCellLineEdit.*
NXWidgetTools/private/NXMultiCellLineEditPrivate.*

NXWidgetTools/private/NXShadowGraphicsEffectPrivate.*
NXWidgetTools/private/NXShadowWidgetPrivate.*

NXWidgetTools/private/NXSliderPrivate.*
```

#### 第三方库引入

- `include/expected.hpp`
- `include/magic_enum`
- `include/aesni`

#### RainbowCandyX 开发者贡献
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

* 核心文件列表

```
NXCheckBox.cpp
NXCheckBox.h
NXCheckBoxStyle.cpp
NXCheckBoxStyle.h

DeveloperComponents/NXColorPicker.cpp
NXColorDialog.cpp
NXColorDialog.h
NXColorDialogPrivate.cpp
NXColorDialogPrivate.h
NXColorValueSliderStyle.cpp
NXColorValueSliderStyle.h

NXComboBox.cpp

NXContentDialog.cpp
NXContentDialog.h
NXContentDialogPrivate.cpp
NXContentDialogPrivate.h

NXDockWidget.cpp
NXDockWidget.h

NXDoubleSpinBox.cpp

NXFooterModel.cpp
NXFooterModel.h

NXKeyBinder.cpp

NXLineEdit.cpp
NXLineEdit.h
NXLineEditPrivate.cpp
NXLineEditPrivate.h
NXLineEditStyle.cpp
NXLineEditStyle.h

NXLogPrivate.cpp

NXMenu.cpp
NXMenu.h
NXMenuPrivate.cpp
NXMenuPrivate.h
NXMenuStyle.h

NXMessageButton.cpp
NXMessageButton.h
NXMessageButtonPrivate.cpp
NXMessageButtonPrivate.h

NXMicaBaseInitObject.cpp

NXMultiSelectComboBox.cpp
NXMultiSelectComboBox.h
NXMultiSelectComboBoxPrivate.h

NXNavigationBar.cpp
NXNavigationBar.h
NXNavigationModel.cpp
NXNavigationModel.h
NXNavigationNode.cpp
NXNavigationNode.h
NXNavigationBarPrivate.cpp
NXNavigationBarPrivate.h
NXNavigationStyle.cpp
NXNavigationStyle.h
NXNavigationView.cpp
NXNavigationView.h

NXPivotModel.cpp
NXPivotModel.h
NXPivot.cpp
NXPivot.h
NXPivotStyle.cpp
NXPivotStyle.h
NXPivotView.cpp
NXPivotView.h

NXPromotionView.cpp
NXPromotionView.h

NXPushButton.cpp
NXPushButton.h
NXPushButtonPrivate.h

NXRadioButton.cpp
NXRadioButton.h
NXRadioButtonPrivate.h

NXScrollPage.cpp
NXScrollPage.h

NXSlider.cpp
NXSlider.h

NXSpinBox.cpp

NXTabBar.cpp
NXTabBar.h
NXTabBarStyle.cpp
NXTabBarStyle.h

NXTabWidget.cpp
NXTabWidget.h
NXTabWidgetPrivate.cpp
NXTabWidgetPrivate.h

NXTableView.cpp
NXTableView.h
NXTableViewPrivate.cpp
NXTableViewPrivate.h
NXTableViewStyle.cpp
NXTableViewStyle.h

NXText.cpp
NXText.h
NXTextPrivate.cpp
NXTextPrivate.h

NXTheme.cpp
NXTheme.h
NXThemePrivate.cpp
NXThemePrivate.h

NXToggleButton.cpp
NXToggleButton.h
NXToggleButtonPrivate.h

NXToolButtonStyle.cpp

NXToolTip.cpp
NXToolTip.h
NXToolTipPrivate.cpp
NXToolTipPrivate.h

NXTreeView.cpp
NXTreeView.h
NXTreeViewStyle.cpp
NXTreeViewStyle.h

NXWidget.cpp
NXWidget.h

NXWindow.cpp
NXWindow.h
NXWindowPrivate.cpp
NXWindowPrivate.h
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

## 三、改动记录

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

项目格式化

1. **移除单例文件**：删除 `singleton.h`，改用自定义宏工具管理单例。

2. **新增宏工具文件**：添加 `LinnFunctionRegistry.h`、`LinnProperty.h`、`LinnSingleton.h`、`LinnUtils.h`，优化宏定义与单例实现，影响所有单例类的使用。
   *注意：使用宏时，MSVC 需开启编译选项 `/Zc:preprocessor`，库已将其作为 PUBLIC 导出。*

3. **导航接口返回值优化**：为 Navigation 系列的接口重新设计返回值，引入 `tl::expected` 作为错误处理机制，提高代码健壮性。

4. **NXContentDialog 重构**：重构对话框实现，接口设计更通用、易用，方便操作三个按钮。
5. **NXTabBar 增强**：
   - 在 `NXTabBar.cpp` 和 `NXTabBarStyle` 中，`dragEnterEvent` 增加对 Qt 版本坐标事件的兼容性处理。
   - 新增 Firefox 和 Google 两种标签栏样式绘制。

6. **NXTheme 阴影绘制优化**：优化阴影绘制接口，提升视觉效果。

7. **NXApplicationPrivate 改进**：在构造函数中兼容 `qApp` 信号槽连接，增强跨版本稳定性。

8. **NXToolTip 逻辑重构**：

   - 修改原贡献者逻辑，引入布尔变量控制显示行为，使逻辑更清晰。

   - 新增定时器管理，防止延迟显示时长大于延迟隐藏时长时，鼠标快速滑过导致的不当展示。

   - 修复窗口尺寸更新错误（例如大小切换时未能正确调整）。

   - 同步调整 `NXSlider` 中 `NXToolTip` 的调用逻辑。

9. **移除 NXAdvancedToolButton**：删除已废弃的 `NXAdvancedToolButton` 类。
10. **代码清理**：移除 `NXWidget.cpp` 中不必要的背景色设置逻辑。
11.  **引入 NXMultiCellLineEdit**：新增 `NXMultiCellLineEdit` 控件（类似 IP 输入框），相关文件一并添加。
12. **基础控件文本样式调整**：

- 回退 `NXRadioButton`、`NXPushButton`、`NXCheckBox`、`NXText` 等控件的 `setTextStyle` 方法。
- 新增像素大小（pixel size）设置接口，便于精细控制字体。

13. **问题修复**：同步修复若干已知问题，提升整体稳定性。

### Track Record 6：2026年04月09日提交

#### Modify Optize1

* 同步

* NXTheme 阴影绘制逻辑修改统一

* NXTableView表格更新性能优化，降低移动重绘视口开销

  表格添加check指示器位置接口供外部调用

* NXProperty 大改，
  QS_TAG_DEFAULT_VALUE：成员变量支持默认值初始化。
  QS_TAG_ASSIGN_MOVE：支持两种写法
  Q_PROPERTY_CREATE(QString, Text, QS_ASSIGN_MOVE)
  Q_PROPERTY_CREATE(QString, QS_ASSIGN_MOVE(Text))
  且 QString&& 形参也走 std::move。
  QS_TAG_CONTRACT_ALWAYS / QS_TAG_CONTRACT_DEBUG：setter 前置断言，支持 (expr, msg, location)。
  QS_TAG_NOEXCEPT：getter/setter 都可配置，默认 noexcept(true)。
  QS_TAG_NO_EMIT_SIGNAL：setter 不发信号。
  QS_TAG_SIGNAL_PARAMS：信号参数可配置，支持多个参数。
  QS_TAG_UPDATE：setter 中调用 this->update()，且顺序在 emit 前。
  同步修改所有的property、private创建，

* 几乎所有接口方法内置方法都标记为noexcept，非pod对象使用const&传递，QStringLiteral包裹字符串

* CMake设置大改进一步规范

* **RainbowCandyX —— Mar 19, 2026**

#### Modify Optize2

* 同步

* NXLazyStackedLayout、NXLazyStackedWidget引入，应用于NXCentralWidget懒加载

* NXPivot大改，指示器绘制，代码逻辑调整，模式绘制
