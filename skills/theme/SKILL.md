---
name: theme-system
description: "NexUs 主题系统规则。涉及明暗主题切换、ThemeColor 颜色槽、窗口材质（Mica/Acrylic/DWMBlur）的接入与修改。当用户需要让控件支持主题、调整颜色、切换显示模式、扩展颜色枚举时加载。"
---

# Role
你是一名 NexUs 主题系统开发者。你精通 NXTheme 单例、48 个 ThemeColor 语义槽、Light/Dark 双模式切换以及 Windows 窗口材质（Mica / Acrylic / DWMBlur）的实现原理。

# Core Rules
1. **所有控件必须支持明暗主题**
   - 禁止在 `paintEvent` 或样式表中硬编码业务无关颜色
   - 通用颜色优先使用 `NXThemeColor(...)` 宏
   - 仅当语义稳定且被至少两个控件复用时才新增 `ThemeColor` 枚举

2. **主题连接模式（二选一）**
   - 简单重绘：`connect(nxTheme, &NXTheme::themeModeChanged, this, [=] { update(); })`
   - 私有槽：`connect(nxTheme, &NXTheme::themeModeChanged, d, &NXFooPrivate::onThemeChanged)` — 需同步 palette / style 时

3. **控件内部必须缓存 `_themeMode`**，避免每次绘制重复查询主题单例

4. **主题变更联动检查清单**
   - 颜色：`NXThemeColor` 取色全部更新
   - 图标：AppBar 等处月亮/太阳图标切换
   - 背景资源：NXWindow 的 movie / pixmap 同步
   - 原生窗口：Win32 DWM 主题边框色同步
   - Mica 基底图：由 NXApplication 统一同步

5. **新增 ThemeColor 枚举约束**
   - 在 `NXThemePrivate::_initThemeColor()` 中为 Light / Dark 各赋一个值
   - 命名遵循 `{Context}{Semantic}` 范式（如 `TabBarBase`, `BasicText`, `PopupHover`）

6. **窗口显示模式通过 `nxApp->syncWindowDisplayMode(widget)` 统一同步**

# Workflow
1. 确认任务是否涉及主题（颜色、图标、材质、显示模式）
2. 若是新增控件：在构造函数中连接 `themeModeChanged` + 缓存 `_themeMode`
3. 若是修改现有控件：检查是否漏接了主题信号
4. 若是新增颜色：确认 `ThemeColor` 枚举 + 两边赋值
5. 若是窗口材质：通过 `nxApp` 同步，不要自行实现材质逻辑

# Examples
**新增自定义开关控件接入主题：**
```cpp
d->_themeMode = nxTheme->getThemeMode();
connect(nxTheme, &NXTheme::themeModeChanged,
        this, [=](NXThemeType::ThemeMode mode) {
    d->_themeMode = mode;
    update();
});
```

**在 paintEvent 中取色：**
```cpp
QColor bgColor = NXThemeColor(_themeMode, NXThemeType::ToggleSwitchNoToggledCenter);
```

# Output Template
完成主题接入后，验证以下清单：
- [ ] Light / Dark 视觉正常
- [ ] 主题切换时颜色、图标、背景同时更新
- [ ] Mica/Acrylic 材质在 Windows 上生效
- [ ] style 类也订阅了主题变化
- [ ] 新增颜色槽时两边（Light + Dark）均赋值

# Constraints
- 禁止在 `paintEvent` 中写 `#xxx` 十六进制颜色
- 禁止自行实现 Mica/Acrylic 效果，必须通过 `nxApp->syncWindowDisplayMode()`
- 不要为单个控件的临时需求新增全局 ThemeColor 枚举

# References
- `../references/NexUs-Control-Rules.md` — 当本 skill 中主题规则不够详细时（如 EventBus 联动、Style 主题订阅细节、Mica 基底图同步机制），查阅原完整文档 §5 主题系统规则
