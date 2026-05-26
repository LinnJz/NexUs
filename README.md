# NexUs

**ElaWidgetTools QT开源库的修改封装部分代码**，修改记录见[doc/TrackRecord.md](./doc/TrackRecord.md)、[doc/UPDATE_REPORT.md](./doc/UPDATE_REPORT.md)、

<u>**版权归 ElaWidgetTools 作者[Liniyous](https://github.com/Liniyous)所有**</u>

仓库链接：<u>https://github.com/Liniyous/ElaWidgetTools/commits/main/</u>

部分代码的添加参考以下两位开发者：
- [GitHub - RainbowCandyX/ElaWidgetTools: Add support for Qt 6.10 and macOS](https://github.com/RainbowCandyX/ElaWidgetTools)
- [GitHub - ZongChang-Liu/ElaWidgetTools: Fluent-UI For QWidget](https://github.com/ZongChang-Liu/ElaWidgetTools)

---

# 许可证

**<u>ElaWidgetTools 使用 MIT 许可证授权所有类型项目，但要求所有分发的软件中必须保留本项目的MIT授权许可；所有未保留授权分发的商业行为均将被视为侵权行为</u>**

*版权所有 © 2024 by Liniyous*

**重要提示：** 使用NexUs项目，请务必保留 ***ELA_LICENES许可证***，<u>**尊重Ela作者的知识产权，如有未保留许可证的侵权行为，后果自负**</u>

---

# 使用教程

## 安装插件（适用于Visual Studio解决方案）

1. 打开VS2022 → 扩展 → 管理扩展 → 搜索"Qt"，安装"Qt Visual Studio Tools"
2. 安装完成后，打开：扩展 → Qt VS Tools → Qt Versions
3. 点击"Add"按钮，输入：
   - **Name**: 例如 `6.6.2_msvc2019_64`
   - **Location**: Qt安装路径，例如 `E:\Qt\6.6.2\msvc2019_64`

完成上述操作后，VS2022应该能够识别Qt路径并正确加载对应的头文件资源。

---

## 构建系统选择/项目输出说明

项目支持多种构建系统，可根据需要选择：

### 构建系统选项

| 构建系统                | 状态       | 特点                              |
| ----------------------- | ---------- | --------------------------------- |
| **Sln (Visual Studio)** | ✅ 完整支持 | Windows平台原生支持，编译速度较慢 |
| **CMake**               | ✅ 完整支持 | 跨平台，编译速度快，文件组织灵活  |
| **PreMake**             | 🔧 待完善   | 支持生成多种IDE项目文件           |

### 构建脚本

进入`Build`文件夹，运行对应平台的脚本，系统会自动将构建配置拷贝到相应位置，然后使用IDE打开编译即可。

### 输出文件结构

项目输出文件统一组织到 `Bin\<项目名字>_<构建目标>_<平台架构>` 目录下：

**示例路径：**
```
NexUs\Bin\NexUs_RelWithDebInfo_x64\
```

输出内容包括：
- **动态库/静态库**
- **可执行程序**
- **调试文件**

---

## 中间文件管理

### 1. OBJ文件

#### Sln构建
会在`Bin`目录下生成`Intermediate`文件夹，结构如下：
```
NexUs\Bin\Intermediate\NexUs_Release_x64\
```

#### CMake构建
中间文件保留在标准CMake输出目录：
```
NexUs\out\build\debug\NexUs\CMakeFiles\NexUs.dir\
```

### 2. MOC/RCC/UIC文件

#### Sln构建
在每个项目下生成`GeneratedFiles`文件夹：

```
NexUs\NexUs\GeneratedFiles\
├── qrc_NexUs.cpp
├── RelWithDebInfo\
│   ├── moc_NXAppBar.cpp
│   └── ...
├── Debug\
│   ├── moc_NXAppBar.cpp
│   └── ...
```

#### CMake构建
文件组织在自动生成的`autogen`目录中：
```
NexUs\out\build\debug\NexUs\NexUs_autogen\
├── 5OPOMGIKHL\         # 基础库MOC文件
├── EWIEGA46WW\         # QRC资源文件
├── MDIJTLMGDY\         # DeveloperComponents组件
├── S4JZKUS5AE\         # private内部类
└── mocs_compilation.cpp
```

---

## 构建性能比较

| 构建系统    | 编译速度 | 输出文件大小                          | 特点             |
| ----------- | -------- | ------------------------------------- | ---------------- |
| **Sln**     | 较慢     | Debug/Release稍大，RelWithDebInfo较小 | Windows原生集成  |
| **CMake**   | 很快     | Debug/RelWithDebInfo会包含额外文件    | 跨平台支持优秀   |
| **PreMake** | 中等     | 取决于生成的项目类型                  | 灵活的项目生成器 |

**注意：** CMake在Debug/RelWithDebInfo模式下会生成不必要的文件（如`.exp`、`.idb`、`.ilk`），可手动删除。

### PreMake使用示例

```bash
# Windows (生成Visual Studio项目)
BUILD\premake5.exe vs2022

# Linux (生成Makefile)
./BUILD/premake5 gmake2

# macOS (生成Xcode项目)
./BUILD/premake5 xcode4
```

---

# Example项目适配指南

同步Ela主仓库的Example后，需要进行以下替换以适配已修改的代码：

## 自动替换脚本

运行脚本 `[Script/run_rename_and_replace.bat](./Script/run_rename_and_replace.bat)` 可自动完成以下替换：

| 原内容       | 替换为        | 说明         |
| ------------ | ------------- | ------------ |
| `ELA`        | `NX`          | 预处理宏前缀 |
| `Ela`        | `NX`          | 类名前缀     |
| `T_Ela`      | `T_NX`        | 类型别名     |
| `eTheme`     | `nxTheme`     | 主题枚举宏   |
| `eApp`       | `nxApp`       | 应用实例宏   |
| `":/include` | `":/Resource` | 资源路径     |

## 手动修改部分

### T_Popup.cpp
```cpp
// 修改前
getCurrentColorRGB

// 修改后
getCurrent4ChannelColor
```

### T_NXPacketIO.h/.cpp
需要添加以下头文件包含：

**T_NXPacketIO.h:**
```cpp
#include "XIO/NXXIO_PacketRegistry.h"
#include "Util/UtCallbackHolder.h"
#include <QPixmap>  // 新增
```

**T_NXPacketIO.cpp:**
```cpp
#include "XIO/NXXIO_Connection.h"
#include "XIO/NXXIO_Interface.h"
#include "XIO/NXXIO_PacketRegistry.h"
```

### mainwindow.cpp
需要进行以下关键修改：

```cpp
void
MainWindow::initContent()
{
  _homePage = new T_Home(this);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
  _screenPage = new T_NXScreen(this);
#endif
  _iconPage           = new T_Icon(this);
  _baseComponentsPage = new T_BaseComponents(this);
  _graphicsPage       = new T_Graphics(this);
  _navigationPage     = new T_Navigation(this);
  _popupPage          = new T_Popup(this);
  _cardPage           = new T_Card(this);
  _listViewPage       = new T_ListView(this);
  _tableViewPage      = new T_TableView(this);
  _tableWidgetPage    = new T_TableWidget(this);
  _treeViewPage       = new T_TreeView(this);
  _newComponentsPage  = new T_NewComponents(this);
  _newComponents2Page = new T_NewComponents2(this);
  _codeEditorPage     = new T_CodeEditor(this);
  _routerPage         = new T_Router(this);
  _settingPage        = new T_Setting(this);

  addPageNode(QStringLiteral("HOME"), _homePage, NXIconType::House);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
#  ifdef Q_OS_WIN
  _dxgiKey = addExpanderNode(QStringLiteral("NXDxgi"), NXIconType::TvMusic);
#  else
  _dxgiKey = addExpanderNode(QStringLiteral("NXScreenCapture"), NXIconType::TvMusic);
#  endif
  QString dxgiCategoryKey;
#  ifdef Q_OS_WIN
  dxgiCategoryKey = *addCategoryNode(QStringLiteral("Windows-DXGI"), _dxgiKey);
#  else
  dxgiCategoryKey = *addCategoryNode(QStringLiteral("macOS-ScreenCapture"), _dxgiKey);
#  endif
  addPageNode(QStringLiteral("NXScreen"), _screenPage, _dxgiKey, 3, NXIconType::ObjectGroup);
#endif
  QString controlCategoryKey = addCategoryNode(QStringLiteral("Controls"));
  // navigation(screenWidget->property("NXPageKey").toString());
  addPageNode(QStringLiteral("NXBaseComponents"), _baseComponentsPage, NXIconType::CabinetFiling);

  _viewKey                = addExpanderNode(QStringLiteral("NXView"), NXIconType::CameraViewfinder);
  QString viewCategoryKey = *addCategoryNode(QStringLiteral("View Content"), _viewKey);
  addPageNode(QStringLiteral("NXListView"), _listViewPage, _viewKey, 9, NXIconType::List);
  addPageNode(QStringLiteral("NXTableView"), _tableViewPage, _viewKey, NXIconType::Table);
  addPageNode(QStringLiteral("NXTableWidget"), _tableWidgetPage, _viewKey, NXIconType::TableCells);
  addPageNode(QStringLiteral("NXTreeView"), _treeViewPage, _viewKey, NXIconType::ListTree);
  expandNavigationNode(_viewKey);

  addPageNode(QStringLiteral("NXGraphics"), _graphicsPage, 9, NXIconType::Paintbrush);
  addPageNode(QStringLiteral("NXCard"), _cardPage, NXIconType::Cards);
  QString customKey = addCategoryNode(QStringLiteral("Custom"));
  addPageNode(QStringLiteral("NXNavigation"), _navigationPage, NXIconType::LocationArrow);
  addPageNode(QStringLiteral("NXPopup"), _popupPage, NXIconType::Envelope);
  addPageNode(QStringLiteral("NXNewComponents"), _newComponentsPage, NXIconType::Sparkles);
  addPageNode(QStringLiteral("NXNewComponents2"), _newComponents2Page, NXIconType::StarChristmas);
  addPageNode(QStringLiteral("NXCodeEditor"), _codeEditorPage, NXIconType::Code);
  addPageNode(QStringLiteral("NXRouter"), _routerPage, NXIconType::SignsPost);
  addPageNode(QStringLiteral("NXIcon"), _iconPage, 99, NXIconType::FontCase);
  QString testKey_1 = addExpanderNode(QStringLiteral("TEST_EXPAND_NODE1"), NXIconType::Acorn);
  QString testKey_2 = *addExpanderNode(QStringLiteral("TEST_EXPAND_NODE2"), testKey_1, NXIconType::Acorn);
  addPageNode(QStringLiteral("TEST_NODE3"), new QWidget(this), testKey_2, NXIconType::Acorn);
  for (int i = 0; i < 10; i++)
  {
    addExpanderNode(QString(QStringLiteral("TEST_EXPAND_NODE%1")).arg(i + 4), testKey_2, NXIconType::Acorn);
  }
  addExpanderNode(QStringLiteral("TEST_EXPAND_NODE14"), testKey_1, NXIconType::Acorn);
  addExpanderNode(QStringLiteral("TEST_EXPAND_NODE5"), testKey_1, NXIconType::Acorn);
  addExpanderNode(QStringLiteral("TEST_EXPAND_NODE16"), testKey_1, NXIconType::Acorn);

  _aboutKey  = *addFooterNode(QStringLiteral("About"), nullptr, 0, NXIconType::User);
  _aboutPage = new T_About();

  _aboutPage->hide();
  connect(this, &NXWindow::navigationNodeClicked, this,
          [=](NXNavigationType::NavigationNodeType nodeType, QString nodeKey)
  {
    if (_aboutKey == nodeKey)
    {
      _aboutPage->moveToCenter();
      _aboutPage->show();
    }
  });
  _settingKey = *addFooterNode(QStringLiteral("Setting"), _settingPage, 0, NXIconType::GearComplex);
  connect(this, &MainWindow::userInfoCardClicked, this, [=]()
  {
    this->navigation(_homePage->property("NXPageKey").toString());
  });
#ifdef Q_OS_WIN
  connect(_homePage, &T_Home::screenNavigation, this, [=]()
  {
    this->navigation(_screenPage->property("NXPageKey").toString());
  });
#endif
  connect(_homePage, &T_Home::baseComponentNavigation, this, [=]()
  {
    this->navigation(_baseComponentsPage->property("NXPageKey").toString());
  });
  connect(_homePage, &T_Home::sceneNavigation, this, [=]()
  {
    this->navigation(_graphicsPage->property("NXPageKey").toString());
  });
  connect(_homePage, &T_Home::iconNavigation, this, [=]()
  {
    this->navigation(_iconPage->property("NXPageKey").toString());
  });
  connect(_homePage, &T_Home::cardNavigation, this, [=]()
  {
    this->navigation(_cardPage->property("NXPageKey").toString());
  });

  _windowSuggestBox->addSuggestion(getNavigationSuggestDataList());
  qDebug() << "已注册的事件列表" << NXEventBus::getInstance()->getRegisteredEventsName();
}

```

完成以上修改后，Example项目即可正确编译运行。

