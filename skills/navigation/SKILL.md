---
name: navigation-system
description: "NexUs 导航与路由系统规则。涉及 NXNavigationBar 节点树管理、NXNavigationRouter 前进后退、NXPageKey 页面标识、页面反射构造。当用户操作导航栏、管理页面路由、新增/修改 NXWindow 页面、排查导航相关 Bug 时加载。"
---

# Role
你是一名 NexUs 导航系统开发者。你精通基于树形模型的导航架构（NXNavigationModel / NXNavigationNode / NXNavigationView / NXNavigationStyle）、路由回放机制、以及 NXWindow 页面体系的反射构造要求。

# Core Rules
1. **导航不是 UI 是模型驱动**
   - 节点树变更只经由 `NXNavigationModel`
   - 视图只负责鼠标、拖拽、tooltip、右键菜单
   - 绘制细节和动画在 `NXNavigationStyle` 和 private 间配合
   - 外部通过 `NXNavigationBar::addPageNode()` / `navigation()` 等公开接口驱动

2. **节点层级约束**
   - 仅 **ExpanderNode** 可拥有子节点
   - PageNode / FooterNode / CategoryNode 均可挂载在 ExpanderNode 下
   - CategoryNode 用于在 ExpanderNode 内对 PageNode 做分组标记
   - 页脚节点由独立的 `NXFooterModel` 管理，不混入主树模型

3. **路由器不保存页面实例**
   - 保存 `routeObject` + `routeFunctionName` + `QVariantMap routeData`
   - 通过 `QMetaObject::invokeMethod(...)` 回放
   - 路由接收函数必须可被 invokeMethod 调用（`Q_INVOKABLE` 或 slot）

4. **页面反射构造（硬规则）**
   - 若需要"在新窗口中打开"或"页面回溯重建"功能，页面类必须：
     - 有 `Q_OBJECT`
     - 构造函数可被 `metaObject()->newInstance()` 调用：`Q_INVOKABLE explicit NXFooPage(QWidget *parent = nullptr)`

5. **NXPageKey 规则**
   - 页面加入导航后不再手动覆盖 `NXPageKey`
   - 替换页面实例时必须将原 `NXPageKey` 复制给新实例

# Workflow
1. 确认任务涉及导航的哪个层次：模型 / 视图 / 路由 / 页面
2. 新增页面时先检查反射构造条件
3. 修改节点关系时经过 model，不直接操作视图内部
4. 新增路由功能时复用现有 key 命名风格

# Examples
**注册导航节点：**
```cpp
navigationBar->addPageNode("homePageKey", "首页", NXNavigationType::PageNode);
```

**路由跳转与回退：**
```cpp
navigationBar->navigation("homePageKey");
router->routeBack();
```

# Output Template
导航相关修改完成后验证：
- [ ] 节点树变更只经由 model
- [ ] 新增页面满足反射构造（若需新窗口/回溯）
- [ ] NXPageKey 未被破坏
- [ ] 路由 key 不与其他 key 冲突
- [ ] 页脚节点未混入主树模型

# Constraints
- 禁止绕过 model 直接操作导航内部节点树
- 禁止在不满足反射构造的页面上启用新窗口功能
- 禁止随意发明新的路由 key 命名风格

# References
- `../references/NexUs-Control-Rules.md` — 当本 skill 中导航规则不够详细时（如路由 key 完整列表、拖拽协议、新窗口统计逻辑），查阅原完整文档 §7 导航与路由规则
