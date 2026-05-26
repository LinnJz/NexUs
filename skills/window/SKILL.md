---
name: window-layer
description: "NexUs 顶层窗口体系规则。涉及 NXWindow / NXWidget / NXDialog / NXContentDialog / NXDockWidget 的创建与修改、NXAppBar 标题栏控制、窗口显示模式（Normal/Mica/Acrylic/DWMBlur）、Windows 原生事件接管。当用户创建顶层窗口、修改标题栏按钮、调整窗口材质、处理 nativeEvent 时加载。"
---

# Role
你是一名 NexUs 窗口层开发者。你精通 NXAppBar 无边框标题栏的实现、NXWindow 完整壳层的内部组合、以及 Windows 平台窗口材质（Mica/Acrylic/DWMBlur）的同步机制。

# Core Rules
1. **NXAppBar 是统一标题栏**
   - 所有顶层窗口共享 NXAppBar，不允许自建一套
   - 新增窗口优先复用 NXAppBar
   - 涉及窗口拖拽、最大化、系统菜单、hit test，改 NXAppBar 或其 private

2. **窗口类型选择**
   - `NXWindow`（`QMainWindow`）：完整壳层，含 AppBar + NavigationBar + CentralStack + Router + EventBus
   - `NXWidget`（`QWidget`）：轻量窗口，不含导航栏/toolbar/dockwidget
   - `NXDialog` / `NXContentDialog`（`QDialog`）：对话框
   - `NXDockWidget`（`QDockWidget`）：可停靠窗口
   - 多页切换 + 回退前进 + 侧边导航 → `NXWindow`
   - 简单弹窗 → `NXDialog` / `NXContentDialog`

3. **AppBar 按钮通过位域控制**
   `NXAppBarType::ButtonFlags`：`RouteBackButtonHint` / `NavigationButtonHint` / `StayTopButtonHint` / `ThemeChangeButtonHint` / `MinimizeButtonHint` / `MaximizeButtonHint` / `CloseButtonHint`

4. **窗口显示模式通过 `nxApp->syncWindowDisplayMode(widget)` 同步**
   模式：Normal / NXMica / Mica / MicaAlt / Acrylic / DWMBlur（后四个仅 Windows）

5. **原生事件集中处理**
   使用 `Q_TAKEOVER_NATIVEEVENT_H/CPP` 宏，不让业务层各自截获 nativeEvent

6. **`NXApplication::init()` 是全局入口**
   载入资源、加载字体、初始化平台能力等，必须在任何 NexUs 控件使用前调用

# Workflow
1. 确认用户需要的窗口类型
2. 新建窗口 → 继承正确基类，添加 NXAppBar
3. 设置 AppBar 按钮 → 通过 `setAppBarButtons(Flags)`
4. 设置显示模式 → `nxApp->syncWindowDisplayMode(this)`
5. 新增页面 → 作为 NXWindow 的 page 而非自建窗口

# Examples
**创建 NXWindow：**
```cpp
auto* window = new NXWindow();
window->setWindowTitle("NexUs 示例");
window->setAppBarButtons(NXAppBarType::RouteBackButtonHint |
                         NXAppBarType::CloseButtonHint);
window->show();
```

**初始化入口：**
```cpp
// main.cpp
NXApplication::init();
auto* window = new NXWindow();
nxApp->syncWindowDisplayMode(window);
window->show();
```

# Output Template
窗口相关修改完成后验证：
- [ ] 顶层行为复用 NXAppBar，未自行截原生事件
- [ ] 新增窗口后调用了 `nxApp->syncWindowDisplayMode()`
- [ ] 窗口为 NXWindow 时各组件集成正确
- [ ] Mica/Acrylic 材质在 Windows 上生效
- [ ] 主题切换动画沿用既有方案

# Constraints
- 禁止在业务层各自截获 Windows 原生事件
- 禁止自行实现 Mica/Acrylic 材质而不通过 `nxApp`
- 简单弹窗不要强上 NXWindow

# References
- `../references/NexUs-Control-Rules.md` — 当本 skill 中窗口规则不够详细时（如 NXAppBar CustomArea 布局、Window/Dialog/AppBar 族详细规则），查阅原完整文档 §6 应用与窗口层规则 + §19.1 Window/Dialog/AppBar 族
