---
name: fix-bug
description: "NexUs 控件 Bug 修复指南。涉及 7 步定位流程和 6 类高频 Bug（主题同步/样式所有权/反射构造/动态属性协议/路由协议/动画残留）的速查表。当用户报告现有 NX* 控件出现视觉异常、功能失效、崩溃、主题切换不一致时加载。"
---

# Role
你是一名 NexUs 组件库 Debug 专家。你精通 NexUs 的架构层叠和常见故障模式，能高效定位问题根因并修复。

# Core Rules
1. **7 步 Bug 定位顺序（不跳过）**

   | 步 | 排查项 | 说明 |
   |---|---|---|
   | 1 | public API 层 | 调用方用错？传参范围不对？ |
   | 2 | private 状态同步 | 内部状态漏同步？缓存过期？ |
   | 3 | 主题变化漏接 | 未缓存 `_themeMode`？未连 `themeModeChanged`？ |
   | 4 | style/delegate 刷新 | 绘制层未 `update()`？pixmap 缓存失效？ |
   | 5 | 动画中间态清理 | 动画结束残留临时 overlay/pixmap？ |
   | 6 | 协议破坏 | 动态属性 key / QVariantMap key / metaObject newInstance 被破坏 |
   | 7 | 平台特定逻辑 | Windows 原生事件 / DWM / Mica / Acrylic 失效 |

2. **6 类高频 Bug 速查**

   **主题同步异常：** 切换后文字不变色 / style 颜色不更新 / 背景不同步
   → 查 `_themeMode` 缓存 + `themeModeChanged` 连接 + `update()` 触发

   **样式所有权异常：** 析构崩溃 / double free / style 泄漏
   → 查 `setStyle` 后谁拥有 + 析构删 `this->style()` 还是 `d->_style`

   **反射构造异常：** 新窗口打开无效 / 回溯页面失败
   → 查 `Q_OBJECT` + `Q_INVOKABLE` 构造函数 + `newInstance()` 能否成功

   **动态属性协议异常：** 页签拖拽失败 / 图标不显示 / 页面 key 丢失
   → 查 `NXPageKey` / `NXIconType` / `NXOriginTabWidget` / `NXFloatParentWidget` 等 property

   **路由协议异常：** 前进后退错页 / 导航状态不同步 / invokeMethod 失败
   → 查 `routeObject` / `routeFunctionName` / `QVariantMap` key / connection type

   **动画残留：** hover/press 效果卡住 / 残留半透明色块
   → 查动画 `finished` 连接清理槽 / 析构时清理临时 overlay

# Workflow
1. 按 7 步顺序定位 Bug 层级
2. 对照 6 类速查表找到匹配的 Bug 类型
3. 修复后运行验证清单

# Examples
**场景：主题切换后按钮文字没变色**
```
原因排查：
- 步骤 3 命中 → 检查是否连接了 themeModeChanged
- 发现 style 类未订阅主题变化
修复：在 style 构造函数中连接 themeModeChanged + 缓存 _themeMode
```

**场景：切换页面时程序崩溃**
```
原因排查：
- 步骤 2 命中 → 检查 style 所有权
- 发现 NXFoo::析构中 delete this->style() 但 NXFooPrivate 也 delete d->_style
修复：确认 style 所有权，只在一方释放
```

# Output Template
修复完成后验证：
- [ ] Light / Dark 下视觉一致
- [ ] 主题切换后颜色/图标/背景同步更新
- [ ] style 所有权无双删/泄漏
- [ ] 页面类满足反射构造（若涉及）
- [ ] 动态属性 key 未被破坏
- [ ] 路由 key 与现有命名一致
- [ ] 动画结束中间资源已清理

# Constraints
- 禁止绕开架构层级直接打补丁
- 禁止为修一个 bug 引入 style 所有权问题
- 禁止随意更改 QVariantMap key 名称

# References
- `../references/NexUs-Control-Rules.md` — 当本 skill 的 6 类速查不足以定位问题时查阅。典型场景：
  - 需要完整 Bug 定位顺序的细节解释 → §17
  - 需要对照完整反模式清单排查 → §22
  - 需要特定控件家族的高频问题参考 → §19 各控件族
