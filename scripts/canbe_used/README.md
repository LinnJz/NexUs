# 脚本介绍

## `convert_q_signals.py`

规范`Q_SIGNALS、Q_SIGNAL`，移除`Q_SIGNALS`，统一使用`Q_SIGNAL`

```c++
// 情况1
Q_SIGNALS:
	Q_SIGNAL void clicked();

// 脚本执行后
	Q_SIGNAL void clicked();

// 情况2
Q_SIGNALS:
	void clicked();

// 脚本执行后
	Q_SIGNAL void clicked();
```

## `optimize_qt_params.py`

**作用**
自动优化 Qt/C++ 代码中的函数参数传递方式，遵循两条规则：

1. **小类型 → 值传递**
   移除 `const &`，例如 `const QPoint &p` → `QPoint p`
   （内置类型如 `int` 本就不加引用，此处针对 `QPoint`、`QRect`、`QChar` 等轻量类型）
2. **COW 类型 → const 引用传递**
   将值传递改为 `const &`，例如 `QString s` → `const QString &s`
   （针对 `QString`、`QByteArray`、容器类、`QImage` 等隐式共享类型）

**保护 override 函数**
默认跳过 `override` 函数的参数修改，避免破坏虚函数签名。可通过 `--no-skip-override` 关闭保护。

**使用方法**

```bash
python optimize_qt_params.py <目录> [--no-skip-override]
```

**处理流程**

1. 扫描目录下所有 `.h/.cpp/.c` 等 C++ 文件
2. 提取 `override` 函数签名（如果需要保护）
3. 保护这些区域的代码（替换为占位符）
4. 执行两步参数替换
5. 还原保护区，写回文件

**输出**
显示修改的文件列表和统计信息。

## `process_eladef.py`

给 ElaDef 进行扩展，使用自定义的NXProperty文件定义宏

## `qstring_literal.py`

**作用**  
自动将 C++/Qt 代码中的普通字符串字面量 `"..."` 包装成 `QStringLiteral("...")`，从而避免运行时构造 `QString`，提升性能。

**使用场景**  
Qt 项目中，`QStringLiteral` 可以将字符串字面量转为编译期生成的 `QString` 数据，减少堆分配和拷贝。本脚本帮助批量迁移普通字符串。

**核心规则**

| 情况                                              | 是否包装    | 示例                                  |
| ------------------------------------------------- | ----------- | ------------------------------------- |
| 普通字符串字面量                                  | ✅ 是       | `"hello"` → `QStringLiteral("hello")` |
| 相邻字符串（可跨行、有注释）                      | ✅ 合并包装 | `"a" "b"` → `QStringLiteral("a" "b")` |
| 已有 `QStringLiteral`                             | ❌ 跳过     | `QStringLiteral("x")` 不变            |
| `#include "file.h"` 中的路径                      | ❌ 跳过     | 保留原样                              |
| `setObjectName("...")` 等函数参数                 | ❌ 跳过     | `setObjectName("w")` 不变             |
| `QPropertyAnimation(..., "x")` 参数               | ❌ 跳过     | 动画属性名不变                        |
| 带前缀的字符串（`u8"..."`、`L"..."`、`R"(...)"`） | ❌ 跳过     | 保持原样                              |

**使用方法**

```bash
python qstring_literal.py <目录> [--serial]
```

- 递归扫描目录下 `.h/.cpp/.hpp` 等 C++ 文件
- 默认并行处理（CPU 核心数限制 ≤8）
- 加 `--serial` 使用单线程模式

**输出**  
显示修改的文件数量，直接覆写原文件。

## `remove_override.py`

继承class/struct的析构函数的override标识移除

## `remove_shadow_ifdef.py`

不兼容QT_VERSION_CHECK，将其移除

```
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
int _shadowBorderWidth{ 0 };
#else
int _shadowBorderWidth{ 10 };
#endif

执行脚本
int _shadowBorderWidth{ 10 };
```

## `reorder_includes.py`

**作用**  
自动扫描并重新排列 C/C++ 文件中的 `#include` 指令，按照标准顺序组织，并为关联头文件自动添加目录前缀（如 `DeveloperComponents/`、`private/`），使代码符合项目规范。

**Include 顺序（各部分间用空行分隔）**

| 顺序 | 类别                    | 说明                                                                   |
| ---- | ----------------------- | ---------------------------------------------------------------------- |
| 1    | 自身头文件（仅 `.cpp`） | `#include "ClassName.h"`（与当前 `.cpp` 同名的头文件，不添加任何前缀） |
| 2    | 标准库（非 Qt）         | `#include <utility>`、`#include <vector>` 等                           |
| 3    | Qt 库                   | 头文件名以 `Q` 或 `Qt` 开头的 `<...>` 包含，如 `<QWidget>`、`<QtCore>` |
| 4    | 关联头文件（双引号）    | 其他 `#include "..."`，自动补全相对于项目根目录的路径前缀              |

每个部分内部按头文件名（去掉路径的 basename）字母顺序排序。

**路径前缀规则**

- 若关联头文件与当前文件在同一目录 → 保留原名，不加前缀
- 若头文件在子目录（如 `DeveloperComponents/`、`private/`、`DeveloperComponents/Command/`）→ 自动补全相对路径，使用 `/` 分隔
- 自身头文件（第一部分）永不添加前缀

**示例**

```cpp
// 原始
#include "ElaApplication.h"
#include "ElaTheme.h"
#include "private/ElaApplicationPrivate.h"
#include <QApplication>
#include <utility>

// 调整后（假设 ElaTheme.h 与当前文件同级，ElaWinShadowHelper.h 在 DeveloperComponents/ 下）
#include "ElaApplication.h"

#include <utility>

#include <QApplication>

#include "DeveloperComponents/ElaWinShadowHelper.h"
#include "ElaTheme.h"
#include "private/ElaApplicationPrivate.h"
```

**使用方法**

```bash
python reorder_includes.py <目录> [--dry-run] [--extensions .cpp,.h,.hpp]
```

- `--dry-run`：预览变更，不实际修改文件
- `--extensions`：自定义处理的文件扩展名（默认支持 `.cpp`、`.h`、`.hpp`、`.hxx`、`.mm` 等）

**处理流程**

1. 递归扫描目录，收集所有 C/C++ 文件
2. 构建头文件索引：文件名 → 相对于根目录的路径
3. 对每个文件解析 `#include` 行，保留行号
4. 分类、排序、补全前缀，生成新的 include 块
5. 保持非 include 代码不变，替换原 include 块
6. 若存在条件编译（`#ifdef`）或其他预处理指令包围 includes，则跳过该文件（不自动重排）

**输出**  
显示修改的文件列表及数量，`--dry-run` 时仅预览。

## `reorder_members.py`

**作用**  
自动优化 C++/Qt 头文件中类的成员变量顺序，通过重排减少内存填充（padding），降低类的大小。同时处理特定宏的格式清理和重命名（**引入NXProperty.h必须更新的**）。

**核心功能**

1. **成员变量重排**
   - 以 **8 字节对齐** 为目标，将小尺寸类型（如 `bool`、`char`、`int`、枚举等）尽量连续排列，相同大小的类型紧贴
   - 大尺寸类型（如 `QString`、`QPixmap`）单独分组
   - 优先保持 `Q_PROPERTY_CREATE_*` 等宏定义的成员顺序紧凑，非宏成员仅在必要时与宏成员混合（避免破坏可读性）

2. **宏处理**
   - 识别以下定义成员的宏：`Q_PROPERTY_CREATE_D`、`Q_PROPERTY_CREATE`、`Q_PROPERTY_CREATE_2`、`Q_PRIVATE_CREATE_D`、`Q_PRIVATE_CREATE`、`Q_PRIVATE_REF_CREATE`
   - 自动删除宏调用末尾多余的分号
   - 将 `Q_PROPERTY_REF_CREATE` 转换为 `Q_PROPERTY_CREATE_2`（可根据需求禁用此转换）
   - 将 `_Q_` 宏（如 `Q_PROPERTY_CREATE_Q_H`）重命名为去掉 `_Q_` 的版本（如 `Q_PROPERTY_CREATE_H`），并按类型大小排序

3. **保持语义不变**
   - 保留访问修饰符（`public:`、`private:` 等）及顺序
   - 保留函数声明、`Q_OBJECT`、`Q_D_CREATE`、前置声明、条件编译块等非成员内容
   - 仅重新排列成员变量，不改变任何类型或名称

**使用方法**

```bash
# 单文件处理
python reorder_members.py -i input.h -o output.h

# 目录递归处理（需指定输出目录）
python reorder_members.py -d ./Sample -o ./Output

# 预览变更（不写入）
python reorder_members.py -d ./Sample --dry-run

# 添加自定义类型大小（例如 MyType 占 12 字节）
python reorder_members.py -d ./Sample -o ./Output --add-size "MyType:12"

# 指定头文件扩展名（默认 .h,.hpp,.hxx,.h++）
python reorder_members.py -d ./Sample -o ./Output --extensions .h,.hpp
```

**输出**  
直接修改或输出到指定文件/目录，控制台显示处理进度。

## `virtual_functions.py`

移除继承子类没有必要的virtual

```
virtual void eventFilter(...) const override;

执行脚本
void eventFilter(...) const override;
```

## `content_replace.py`

**作用**
将 C++ 源码中的 ElaWidgetTools 相关标识符批量替换为 NexUs 命名，确保项目重命名后代码引用一致。

**替换规则**

| 规则类型                      | 示例                                                                                                                    |
| ----------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| 全词匹配替换（组1）           | `eApp` → `nxApp`、`eTheme` → `nxTheme`、`getCurrentColorRGB` → `getCurrent4ChannelColor`、`ElaAwesome` → `NXAwesome` 等 |
| 精确子串匹配                  | `":/include` → `":/Resource`                                                                                            |
| 部分匹配（大小写）            | `ELA` → `NX`、`Ela` → `NX`                                                                                              |
| 全词匹配替换（组2，反向修正） | `QNXpsedTimer` → `QElapsedTimer`、`TCP_NODNXY` → `TCP_NODELAY`（修复过度替换）                                          |

**使用方法**

```bash
python content_replace.py <目标根目录>
```

**处理流程**

1. 仅在 `NexUs`、`NexUsExample`、`NXPacketIO` 核心目录下生效
2. 递归处理 `.h/.cpp/.cc/.hh/.hpp` 文件
3. 按顺序执行四步替换（使用正则全词匹配和直接字符串替换）
4. 直接覆写原文件

## `copy_script.py`

**作用**
将指定的源文件或文件夹复制到目标目录。文件覆盖，文件夹合并并覆盖内部文件。

**使用方法**

```bash
python copy_script.py <目标目录> <源1> [源2 源3 ...]
```

**示例**

```bash
python copy_script.py /path/to/dest /path/to/file.txt /path/to/folder
```

## `format_cpp_files.py`

**作用**
使用 `clang-format` 对项目中的 C++ 代码进行批量格式化，统一代码风格。

**使用方法**

```bash
# 格式化指定文件夹（递归）
python format_cpp_files.py --dirs ./NexUs,./NexUsExample,./NXPacketIO

# 指定 clang-format 可执行文件路径
python format_cpp_files.py --exe E:/LLVM/bin/clang-format.exe --dirs ./NexUs

# 直接指定文件列表
python format_cpp_files.py --files ./a.cpp,./b.h
```

**参数说明**
| 参数 | 说明 |
|------|------|
| `--exe` | `clang-format.exe` 完整路径（默认 `E:\Development\LLVM\bin\clang-format.exe`） |
| `--dirs` | 需要格式化的文件夹，多个用英文逗号分隔 |
| `--files` | 直接指定格式化的文件，多个用英文逗号分隔 |

**注意事项**

- 需要当前工作目录下存在 `.clang-format` 配置文件
- 支持的文件后缀：`.cc`、`.hh`、`.h`、`.hpp`、`.cpp`、`.cxx`、`.cu`

## `organize_project.py`

**作用**
整理项目目录结构，将资源文件和源码文件分别归入 `Resource` 和 `Source` 文件夹。

**整理规则**

- `Font`、`Image` 等资源文件夹 → 移动到 `Resource/` 下
- 其他文件夹 → 移动到 `Source/` 下
- `.qrc`、`.cmake`、`.in`、`.txt` 文件 → 保留在原地
- 其他文件 → 移动到 `Source/` 下

**使用方法**

```bash
python organize_project.py <目标路径>
```

**可配置项**（需修改脚本内变量）

- `RESOURCE_FOLDERS`：需要归入 Resource 的文件夹名列表
- `KEEP_SUFFIXES`：保留在原地的文件后缀列表

## `rename_script.py`

**作用**
批量重命名文件和文件夹，支持两种模式。

**模式1：路径模式（递归字符串替换）**
递归遍历目录，将包含指定字符串的文件/文件夹重命名。

```bash
python rename_script.py 1 --path <根目录> --old <旧字符串> --new <新字符串>
```

**模式2：映射模式（CSV批量重命名）**
根据 CSV 映射文件批量重命名，每行格式：`旧路径,新路径`。

```bash
python rename_script.py 2 --mapping <CSV映射文件>
```

**示例**

```bash
# 模式1：将所有包含 "Ela" 的文件/文件夹名替换为 "NX"
python rename_script.py 1 --path ./ElaWidgetTools --old Ela --new NX

# 模式2：根据映射文件重命名
python rename_script.py 2 --mapping ./rename_mapping.csv
```

**映射文件格式**

```csv
旧路径1,新路径1
旧路径2,新路径2
```
