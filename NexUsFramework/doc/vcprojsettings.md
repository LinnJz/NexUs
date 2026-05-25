# 配置属性

## 常规

输出目录

```bash
$(SolutionDir)Bin\$(SolutionName)_$(Configuration)_$(Platform)\

```



中间目录

```bash
$(SolutionDir)Bin\Intermediate\$(ProjectName)_$(Configuration)_$(Platform)\
```



目标文件名

```bash
// DEBUG
$(ProjectName)d

// RELEASE
$(ProjectName)
```



## 高级

字符集

```
使用 Unicode 字符集
```



使用调试库

```bash
// Release
否

// DEBUG
是
```



全程序优化

```
// Release
使用链接时间代码生成

// DEBUG
无全程序优化
```



使用C++动态调试（与全程序优化不兼容，仅在DEBUG下使用）

```
是
```



## Qt Project Settings

Embed .natvis file into PDB

```bash
YES

# TIPs
# 需要给vcproj添加现有项的 qt6.natvis 或 qt6.natvis（根据项目的Qt版本来）
```



## Qt Meta-Object Compiler

### moc

Execution Description

```
Moc'ing %(Identity)...
```



Output Directory

```
.\GeneratedFiles\$(ConfigurationName)
```



Compiler Flavor

```
MSVC
```



## Qt Resource Compiler

### rcc

Execution Description

```
Rcc'ing %(Identity)...
```



Output Directory

```
.\GeneratedFiles
```



### Qt Quick Compiler

Enable Qt Quick Compiler

```
是
```



## Qt UI Compiler



```
Uic'ing %(Identity)...
.\GeneratedFiles
```



## C/C++

### 常规

附加包含目录

```
$(ProjectDir);$(ProjectDir)Source;$(ProjectDir)Source\include;`$(WindowsSDK_IncludePath)
```



支持 C++ 动态调试（与全程序优化不兼容，仅在DEBUG下使用）

```
是
```



多处理器编译

```
是 (/MP)
```



### 优化

DEBUG模式均为默认

以下是RELEASE设置

优化

```
最大优化(优选速度) (/O2)
```



内联函数扩展

```
任何适用项 (/Ob2)
```



启用内部函数

```
是 (/Oi)
```



优选大小或速度

```
代码速度优先 (/Ot)
```



### 代码生成

启用字符串池

```
是 (/GF)
```



运行库

```
// DEBUG
多线程调试 DLL (/MDd)

// RELEASE
多线程 DLL (/MD)
```



结构采用对齐

```
8 字节 (/Zp8)
```



启用函数级链接

```
// DEBUG
空不设置

// RELEASE
是 (/Gy)
```



启用并行代码生成

```
是 (/Qpar)
```



### 语言

强制类型转换规则

```
是 (/Zc:rvalueCast)
```



启用运行时类型信息（RTTI）

```
是 (/GR)
```



OpenMP 支持

```
是 (/openmp)
```



## 链接器

### 调试

生成调试信息

```
// DEBUG
生成调试信息 (/DEBUG)

// RELWITHDEBINFO
生成经过优化以共享和发布的调试信息 (/DEBUG:FULL)

// RELEASE
否
```



支持 C++ 动态调试（与全程序优化不兼容，仅在DEBUG下使用）

```
是
```

