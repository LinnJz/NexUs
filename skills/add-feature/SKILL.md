---
name: add-feature
description: "NexUs 给现有控件加功能的规则。涉及在现有 NX* 控件上扩展能力的层级定位法：API 层 / Private 状态层 / Style 绘制层 / Model 数据层 / 窗口全局层。当用户需要给一个已有控件新加能力而不是创建全新控件时加载。"
---

# Role
你是一名 NexUs 组件库开发者。你精通 NexUs 的三层架构（公共接口层 → 私有实现层 → 开发支撑层），能根据功能类型精准定位应该在哪个层级做修改。

# Core Rules
1. **先定位改动归属层级**

   | 功能类型 | 目标文件 | 典型改动 |
   |---|---|---|
   | 外部 API | `include/NXFoo.h` + `Source/NXFoo.cpp` | 新增公开方法、Q_PROPERTY、信号 |
   | 内部行为/状态 | `private/NXFooPrivate.h/.cpp` | 新增成员变量、槽函数、动画对象 |
   | 绘制/视觉 | `NXFooStyle` / `NXFooDelegate` / `paintEvent` | 选中态绘制、动画过渡、尺寸度量 |
   | 数据结构 | model / node 类 | 节点字段、树结构、拖拽数据类型 |
   | 窗口/全局 | `NXWindow` / `NXAppBar` / `NXApplication` / `NXNavigationRouter` | 快捷键、显示模式、路由能力 |

2. **严禁补丁式写法**
   - ❌ 不改 style 却在控件里重写整套 `paintEvent`
   - ❌ 新增导航状态却塞进 `NXNavigationBarPrivate` 而不更新 `NXNavigationNode`
   - ❌ 在 `paintEvent` 里硬编码颜色而不接入主题
   - ❌ 绕开 `metaObject()->newInstance()` 而不解决构造函数不可反射的问题

3. **属性修改注意事项**
   - 区分 public 属性（用 `Q_PROPERTY_CREATE` 宏）vs private 状态（用 `Q_PRIVATE_CREATE`）
   - setter 必须保证最终触发 `update()`
   - 确定是否需要 emit 变更信号

4. **主题联动检查**
   - 若新增颜色，使用 `NXThemeColor(...)` 而非硬编码
   - 若新增图标，确认 Light/Dark 两套

5. **导航路由注意事项**
   - 不绕过 `NXNavigationModel` 直接操作内部节点树
   - 路由 key 名与现有模式一致

# Workflow
1. 先回答：这个新功能属于哪一层？
2. 找到对应层级的文件
3. 在不破坏现有架构的前提下扩展
4. 若涉及主题/路由/样式所有权，同步检查

# Examples
**在现有控件上加一个新属性：**
```cpp
// NXFoo.h - 加一行宏
Q_PROPERTY_CREATE_H(int, AnimationDuration)

// NXFoo.cpp - 加一行宏
Q_PROPERTY_CREATE_CPP(NXFoo, int, AnimationDuration)

// 构造函数中设置默认值
d->_pAnimationDuration = 200;
```

**在现有控件的 style 中加绘制逻辑：**
```cpp
// NXFooStyle.cpp
void NXFooStyle::drawControl(ControlElement element, const QStyleOption* option,
                              QPainter* painter, const QWidget* widget) const
{
    // 在既有绘制后追加新效果
    NXBaseStyle::drawControl(element, option, painter, widget);
    // ... 新增绘制逻辑
}
```

# Output Template
完成功能扩展后验证：
- [ ] 改动落到了正确层级
- [ ] 无补丁式绕过架构的写法
- [ ] 新增 API 使用 Property 宏
- [ ] Light/Dark 主题下正常
- [ ] style 所有权未被破坏
- [ ] 新增属性触发必要的 `update()`
- [ ] 若涉及路由，key 名与现有风格一致

# Constraints
- 禁止"在哪方便在哪改"——必须找准真正层级
- 禁止不接入主题直接写硬编码颜色
- 禁止为了小功能绕过既有架构

# References
- `../references/NexUs-Control-Rules.md` — 当本 skill 中层级定位法不够时查阅。典型场景：
  - 需要确认修改涉及的子系统完整约束 → 对应子系统章节（§5 主题 / §7 导航 / §6 窗口）
  - 需要确认是否引入了反模式 → §22 反模式清单
