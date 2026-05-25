# 变更文件清单

### 修改文件 (27)
- `.github/workflows/build-example.yml`
- `CMakeLists.txt`
- `ElaWidgetTools/CMakeLists.txt`
- `ElaWidgetTools/DeveloperComponents/ElaCodeHighlighter.h`
- `ElaWidgetTools/DeveloperComponents/ElaNavigationNode.cpp`
- `ElaWidgetTools/DeveloperComponents/ElaNavigationNode.h`
- `ElaWidgetTools/DeveloperComponents/ElaWinShadowHelper.cpp`
- `ElaWidgetTools/ElaChatBubble.cpp`
- `ElaWidgetTools/ElaDef.h`
- `ElaWidgetTools/ElaDoubleSpinBox.cpp`
- `ElaWidgetTools/ElaLineEdit.cpp`
- `ElaWidgetTools/ElaMessageBar.cpp`
- `ElaWidgetTools/ElaMessageBar.h`
- `ElaWidgetTools/ElaNavigationRouter.h`
- `ElaWidgetTools/ElaPasswordBox.cpp`
- `ElaWidgetTools/ElaPlainTextEdit.cpp`
- `ElaWidgetTools/ElaSpinBox.cpp`
- `ElaWidgetTools/ElaTabBar.cpp`
- `ElaWidgetTools/ElaTeachingTip.cpp`
- `ElaWidgetTools/ElaUploadArea.cpp`
-  `ElaWidgetTools/ElaRouter.h`
- `ElaWidgetTools/ElaMenuBar.cpp`
- `ElaWidgetTools/ElaGraphicsScene.cpp`
- `ElaWidgetTools/private/ElaApplicationPrivate.cpp`
- `ElaWidgetTools/private/ElaScrollPagePrivate.h`
- `ElaWidgetTools/private/ElaWindowPrivate.h`
- `ElaWidgetTools/private/ElaThemePrivate.cpp`
- `ElaWidgetTools/private/ElaThemePrivate.h`
- `ElaPacketIO/ElaPacketIO_Export.h`

### 新增文件 (5)
- `ElaWidgetTools/ElaActionCommander.cpp`
- `ElaWidgetTools/ElaActionCommander.h`
- `ElaWidgetTools/private/ElaActionCommanderPrivate.cpp`
- `ElaWidgetTools/private/ElaActionCommanderPrivate.h`
- `ElaWidgetTools/DeveloperComponents/Command/` (目录)

---

# 差异报告 (Diff.md)

> **对比范围**: HEAD (`0cf4e70`) → 当前工作区 (Working Tree)
>
> **生成时间**: 2026-05-22
>
> **上一提交信息**: `init`

---

## 总览

| 指标       | 数值                                            |
| ---------- | ----------------------------------------------- |
| 变更文件数 | 24 (已追踪) + 7 (新增未追踪)                    |
| 新增行数   | 106 行                                          |
| 删除行数   | 33 行                                           |
| 主要类型   | Qt 6 兼容性修复、新功能(Command 模式)、Bug 修复 |

---

## 一、Qt 6 兼容性适配 (跨版本兼容)

> 大量修改旨在消除 Qt 5 → Qt 6 的 API 废弃 (deprecation) 警告，同时保持对 Qt 5 的向后兼容。

### 1. `QMouseEvent::globalPos()` → `globalPosition().toPoint()`

**涉及文件**:

| 文件                                | 行号     | 说明                    |
| ----------------------------------- | -------- | ----------------------- |
| `ElaWidgetTools/ElaChatBubble.cpp`  | 699, 715 | 拖拽起始点和 delta 计算 |
| `ElaWidgetTools/ElaTeachingTip.cpp` | 484      | 点击外部关闭判断        |

**模式**: `#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)` 条件编译，Qt 6 使用 `event->globalPosition().toPoint()`，Qt 5 保持旧 API。

### 2. `QMouseEvent` 构造函数变更 (Qt 6.4+)

**文件**: `ElaWidgetTools/ElaTabBar.cpp`

- **行 149-162**: `dragEnterEvent` 中构造 pressEvent，Qt 6.4+ 需要 `QPointF(localPos)` 和 `QPointF(globalPos)` 两个位置参数
- **行 165-177**: `dragEnterEvent` 中构造 moveEvent，同上
- **行 177-185**: `dragMoveEvent` 中构造 moveEvent，Qt 6 使用 `event->position().toPoint()` 替代 `event->pos()`

### 3. `qAsConst` → `std::as_const` (Qt 6.6+)

**文件**: `ElaWidgetTools/DeveloperComponents/ElaCodeHighlighter.h` (行 36)

`qAsConst()` 在 Qt 6.6+ 中标记为废弃，迁移至标准库 `std::as_const()`。

### 4. 系统调色板监听方式变更

**文件**: `ElaWidgetTools/private/ElaApplicationPrivate.cpp`

- **删除** (行 19): `connect(qApp, &QApplication::paletteChanged, ...)` — 此信号在 Qt 6 中行为异常
- **新增** (行 100-103): `QEvent::ApplicationPaletteChange` 事件过滤器处理 — 更可靠的事件监听方式

### 5. `::GetProcAddress` 去除全局作用域限定

**文件**: `DeveloperComponents/ElaWinShadowHelper.cpp` (行 15)

**函数**: `ElaWinShadowHelper()` 构造函数

```cpp
// Before
auto pRtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(::GetProcAddress(module, "RtlGetVersion"));
// After
auto pRtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(GetProcAddress(module, "RtlGetVersion"));
```

移除 `::` 全局作用域前缀，消除 MSVC 在 Qt 6 下的编译警告。

---

## 二、新增功能

### 1、Command 模式 (撤销/重做框架)

#### 1. 枚举定义 `ElaActionCommanderType`

**文件**: `ElaWidgetTools/ElaDef.h` (行 187-196)

新增 `CommanderState` 枚举:
| 值       | 名称          | 说明         |
| -------- | ------------- | ------------ |
| `0x0000` | `UndoValid`   | 撤销操作有效 |
| `0x0001` | `UndoInvalid` | 撤销操作无效 |
| `0x0002` | `RedoValid`   | 重做操作有效 |
| `0x0003` | `RedoInvalid` | 重做操作无效 |

通过宏 `Q_BEGIN_ENUM_CREATE` / `Q_ENUM_CREATE` / `Q_END_ENUM_CREATE` 注册到元系统。

#### 2. 新增文件 (未追踪)

| 文件                                                   | 说明                              |
| ------------------------------------------------------ | --------------------------------- |
| `ElaWidgetTools/ElaActionCommander.cpp`                | Commander 实现                    |
| `ElaWidgetTools/ElaActionCommander.h`                  | Commander 头文件                  |
| `ElaWidgetTools/private/ElaActionCommanderPrivate.cpp` | Commander 私有实现                |
| `ElaWidgetTools/private/ElaActionCommanderPrivate.h`   | Commander 私有头文件              |
| `ElaWidgetTools/DeveloperComponents/Command/`          | Command 模式目录 (含具体命令实现) |

#### 3. 相关友元类声明

- `ElaScrollPagePrivate.h` (行 17): `friend class ElaScrollPageRouteCommand;` — 页面路由命令
- `ElaWindowPrivate.h` (行 20): `friend class ElaWindowStackChangeCommand;` — 窗口堆栈切换命令

### 2、`NavigationNodeType`枚举新增，`NXNavigationBar`信号添加

#### 1、`NavigationNodeType`枚举添加`ExpanderNode`

```diff
enum NavigationNodeType
{
  PageNode     = 0x00'00,
  FooterNode   = 0x00'01,
+  ExpanderNode = 0x00'02,
  CategoryNode = 0x00'03
};
```

#### 2、`NXNavigationBar`的`navigationNodeAdded`补全

```diff
QString
NXNavigationBar::addExpanderNode(const QString &expanderTitle, NXIconType::IconName awesome)
{
-  @
+  Q_EMIT navigationNodeAdded(NXNavigationType::ExpanderNode, key, nullptr);
  return key;
}

NXNodeResultExpected
NXNavigationBar::addExpanderNode(const QString &expanderTitle,
                                 const QString &targetExpanderKey,
                                 NXIconType::IconName awesome)
{
-  @
+  Q_EMIT navigationNodeAdded(NXNavigationType::ExpanderNode, *returnType, nullptr);
  }
NXNodeResultExpected
NXNavigationBar::addCategoryNode(const QString &categoryTitle, const QString &targetExpanderKey)
{
-    @
+    Q_EMIT navigationNodeAdded(NXNavigationType::CategoryNode, *result, nullptr);
  }
  return result;
}
```



---

## 三、导航系统增强

### 1. 新增方法 `getRowExceptCategoryNodes`

**文件**:
- `ElaWidgetTools/DeveloperComponents/ElaNavigationNode.h` (行 47) — 声明
- `ElaWidgetTools/DeveloperComponents/ElaNavigationNode.cpp` (行 176-184) — 实现

**功能**: 获取当前节点在父节点中**排除 Category 类型后**的索引位置。调用 `_pParentNode->getExceptCategoryNodes().indexOf(this)` 实现。

**用途**: 用于导航栏中跳过分类标题节点计算正确行号，尤其是在拖拽排序或插入场景中。

### 2. `ElaNavigationRouter.h` 信号位置调整

**文件**: `ElaWidgetTools/ElaNavigationRouter.h`

将信号声明从 `private` 区域移至 `public` 区域 (行 33-35)，确保信号可以被外部对象正确连接。

---

## 四、Bug 修复

### 1. `ElaChatBubble` 拖拽兼容性

**文件**: `ElaWidgetTools/ElaChatBubble.cpp`

鼠标双击事件中的拖拽逻辑适配 Qt 6，使用 `globalPosition()` 替代 `globalPos()`。

### 2. `ElaTeachingTip` 点击外部关闭

**文件**: `ElaWidgetTools/ElaTeachingTip.cpp` (行 484-489)

`eventFilter` 中判断点击是否在提示框外部时，适配 Qt 6 的全局坐标获取方式。

### 3. `ElaUploadArea` 缺少头文件

**文件**: `ElaWidgetTools/ElaUploadArea.cpp` (行 4)

添加 `#include <QApplication>` 修复编译错误。

### 4. `ElaMessageBar` 参数类型修正 (const 引用)

**文件**:
- `ElaWidgetTools/ElaMessageBar.cpp` (行 14)
- `ElaWidgetTools/ElaMessageBar.h` (行 26)

构造函数签名变更:

| 参数    | 修改前     | 修改后           |
| ------- | ---------- | ---------------- |
| `title` | `QString&` | `const QString&` |
| `text`  | `QString&` | `const QString&` |

**原因**: 非常量左值引用无法绑定临时对象，当调用方传入字面量或表达式时会导致编译失败。改为 `const` 引用后兼容左值和右值。

---

## 五、配置与构建变更

### 1. CICD 降级 Qt 版本

**文件**: `.github/workflows/build-example.yml`

| 位置   | 旧值                 | 新值                | 说明              |
| ------ | -------------------- | ------------------- | ----------------- |
| 注释   | `Qt 6.10.3`          | `Qt 6.6.2`          | 版本标注更新      |
| 行 19  | `QT_VERSION: 6.10.3` | `QT_VERSION: 6.6.2` | 构建版本降级      |
| 行 238 | `Qt 6.10.3`          | `Qt 6.6.2`          | Release body 更新 |

**原因**: Qt 6.10.3 可能存在兼容性或可用性问题，降级到更稳定的 Qt 6.6.2 LTS 版本。

### 2. `.gitignore` 新增忽略规则

**文件**: `.gitignore`

新增忽略路径:
```
ElaWidgetToolsExample/Resource
ElaWidgetTools/Image
```

避免资源目录和图片目录被误提交。

### 3. `CMakeLists.txt` (根目录) — 新增 MSVC 编译选项

**文件**: `CMakeLists.txt` (行 23-26)

新增编译选项:
```cmake
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>")
if(MSVC)
    add_compile_options(/wd5103)
endif()
```

- `/Zc:preprocessor` — 启用符合标准的预处理器，Qt 6 需要此选项
- `/wd5103` — 禁用 `5103` 级别警告（宏展开相关杂项警告）

### 4. `ElaWidgetTools/CMakeLists.txt` — 注释 Qt6 WidgetsPrivate 依赖

**文件**: `ElaWidgetTools/CMakeLists.txt` (行 54)

```cmake
# Before
find_package(Qt6 REQUIRED COMPONENTS WidgetsPrivate)
# After
#find_package(Qt6 REQUIRED COMPONENTS WidgetsPrivate)
```

**原因**: Qt6 中 `WidgetsPrivate` 模块不再需要显式查找，注释掉以减少不必要的依赖检查。

### 5. ElaPacketIO新增静态库构建

**文件**: `ElaPacketIO/ElaPacketIO_Export.h`、`ElaPacketIO/CMakeLists.txt`

```
#ifdef ELAPACKETIO_STATIC
#define ELA_PACKETIO_EXPORT
#elif defined(ELAPACKETIO_LIBRARY)
...
#endif
```



```
if(ELAPACKETIO_LIB_TYPE STREQUAL "STATIC")
  target_compile_definitions(${PROJECT_NAME} PUBLIC
		-DELAPACKETIO_STATIC
	)
else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE 
		-DELAPACKETIO_LIBRARY
	)
endif()
```

---

### 6. ThemeColor 枚举扩展与 ElaThemePrivate 同步修复

**涉及文件**:
- `ElaWidgetTools/ElaDef.h`
- `ElaWidgetTools/private/ElaThemePrivate.h`
- `ElaWidgetTools/private/ElaThemePrivate.cpp`
- `framework/scripts/process_eladef.py`

**描述**: `ElaDef.h` 的 `ThemeColor` 枚举新增 5 个 TabBar 颜色条目 (`TabBarBase` ~ `TabBarSelectedCloseButtonHover`)，原有条目索引向后顺移 5 位。

**同步修复**: 因枚举值范围扩大，`ElaThemePrivate.h` 中 `_lightThemeColorList[43]` 和 `_darkThemeColorList[43]` 数组容量不足（最大索引 47），需扩容至 `[48]`；`ElaThemePrivate.cpp` 的 `_initThemeColor()` 中补充 5 组 TabBar 颜色赋值。

**脚本化**: `process_eladef.py` 已集成上述修改，运行时会自动同步更新三个文件，并具备幂等性检查。

```diff
// ElaDef.h
-    ScrollBarHandle,
+    TabBarBase,
+    TabBarSelected,
+    TabBarHover,
+    TabBarCloseButtonHover,
+    TabBarSelectedCloseButtonHover,
+    ScrollBarHandle,   // 原索引 0 → 5

// ElaThemePrivate.h
-    QColor _lightThemeColorList[43];
+    QColor _lightThemeColorList[48];
-    QColor _darkThemeColorList[43];
+    QColor _darkThemeColorList[48];

// ElaThemePrivate.cpp 添加颜色

+ // ElaTabBar
+ _lightThemeColorList[ElaThemeType::TabBarBase] = QColor(0xEA, 0xEA, 0xED);
+ _darkThemeColorList[ElaThemeType::TabBarBase] = QColor(0x1C, 0x20, 0x27);
+ 
+ _lightThemeColorList[ElaThemeType::TabBarSelected] = QColor(0xFF, 0xFF, 0xFF);
+ _darkThemeColorList[ElaThemeType::TabBarSelected] = QColor(0x38, 0x3B, 0x43);
+ 
+ _lightThemeColorList[ElaThemeType::TabBarHover] = QColor(0xD2, 0xD2, 0xD6);
+ _darkThemeColorList[ElaThemeType::TabBarHover] = QColor(0x3B, 0x47, 0x5E);
+ 
+ _lightThemeColorList[ElaThemeType::TabBarCloseButtonHover] = QColor(0xBF, 0xBF, 0xC3);
+ _darkThemeColorList[ElaThemeType::TabBarCloseButtonHover] = QColor(0x5A, 0x64, 0x77);
+ 
+ _lightThemeColorList[ElaThemeType::TabBarSelectedCloseButtonHover] = QColor(0xE7, 0xE7, 0xE8);
+ _darkThemeColorList[ElaThemeType::TabBarSelectedCloseButtonHover] = QColor(0x57, 0x5A, 0x60);

```

------

## 六、与源仓库对齐

### 1、`ElaRouter.h` 使用`Q_Q_CREATE`

```c++
private:
  QScopedPointer<NXRouterPrivate> d_ptr;
  Q_DISABLE_COPY(NXRouter)
  Q_DECLARE_PRIVATE(NXRouter)
//替换为
class NX_EXPORT NXRouter : public QObject
{
  Q_OBJECT
  Q_Q_CREATE(NXRouter)
```

### 2、`ElaMenuBar`、`ElaGraphicsScene`规范代码

```
menu ->qmenu
QAction* ElaMenuBar::addMenu(QMenu* qmenu)
{
    ElaMenu* elaMenu = dynamic_cast<ElaMenu*>(qmenu);
    if (elaMenu)
    {
        elaMenu->setMenuItemHeight(27);
    }
    return QMenuBar::addMenu(qmenu);
}
```



```
间接调用规范
QList<ElaGraphicsItem*> ElaGraphicsScene::getElaItems(QPoint pos)
{
  return getElaItems(pos.toPointF());
}

QList<ElaGraphicsItem*> ElaGraphicsScene::getElaItems(QPointF pos)
{
  QList<ElaGraphicsItem *> result;
  for (QGraphicsItem *qitem : items(pos))
  {
    if (auto item = dynamic_cast<ElaGraphicsItem *>(qitem))
      result.append(item);
  }
  return result;
}

QList<ElaGraphicsItem*> ElaGraphicsScene::getElaItems(QRect rect)
{
  return getElaItems(rect.toRectF());
}

QList<ElaGraphicsItem*> ElaGraphicsScene::getElaItems(QRectF rect)
{
  QList<ElaGraphicsItem *> result;
  for (QGraphicsItem *qitem : items(rect))
  {
    if (auto item = dynamic_cast<ElaGraphicsItem *>(qitem))
      result.append(item);
  }
  return result;
}
```



---

## 总结

本次修改的核心目标:

1. **Qt 6 兼容性** — 系统性地修复 Qt 5→6 过渡期的废弃 API 警告，覆盖鼠标事件、全局坐标、右值引用等关键差异
2. **Command 模式引入** — 新增撤销/重做框架基础设施，包含枚举定义、命令调度器和具体命令类，为后续功能提供可扩展的架构
3. **导航系统增强** — 新增 `getRowExceptCategoryNodes` 方法支持更精确的节点定位
4. **构建系统适配** — CICD 降级至 Qt 6.6.2 保证稳定性，更新 `.gitignore` 清理提交内容
