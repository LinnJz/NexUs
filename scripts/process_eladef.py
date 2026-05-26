import os
import sys
import re


def process_eladef_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    original_lines = lines[:]

    last_include_idx = -1
    for i, line in enumerate(lines):
        if line.strip().startswith('#include'):
            last_include_idx = i

    if last_include_idx == -1:
        print(f"  [WARN] No #include found, skipping.")
        return

    first_qbec_idx = -1
    for i in range(last_include_idx + 1, len(lines)):
        if re.match(r'^Q_BEGIN_ENUM_CREATE\(', lines[i]):
            first_qbec_idx = i
            break

    if first_qbec_idx == -1:
        print(f"  [WARN] No Q_BEGIN_ENUM_CREATE usage found, skipping.")
        return

    namespace_block = [
        '\n',
        'namespace __ElaDefNamespace_MOC\n',
        '{\n',
        '  Q_NAMESPACE_EXPORT(ELA_EXPORT)\n',
        '} // namespace __ElaDefNamespace_MOC\n',
        '\n',
    ]

    lines = lines[:last_include_idx + 1] + namespace_block + lines[first_qbec_idx:]

    for i in range(len(lines)):
        m = re.match(r'^(Q_BEGIN_ENUM_CREATE\([^)]+)(\))', lines[i])
        if m and ', ELA_EXPORT' not in lines[i]:
            lines[i] = m.group(1) + ', ELA_EXPORT' + m.group(2) + '\n'

    theme_color_enum_found = False
    for i, line in enumerate(lines):
        if re.match(r'^\s*enum ThemeColor\b', line):
            for j in range(i + 1, len(lines)):
                if '{' in lines[j]:
                    insert_values = [
                        '    TabBarBase,\n',
                        '    TabBarSelected,\n',
                        '    TabBarHover,\n',
                        '    TabBarCloseButtonHover,\n',
                        '    TabBarSelectedCloseButtonHover,\n',
                    ]
                    lines = lines[:j + 1] + insert_values + lines[j + 1:]
                    theme_color_enum_found = True
                    break
            break

    if not theme_color_enum_found:
        print(f"  [WARN] enum ThemeColor not found.")

    endif_idx = -1
    for i, line in enumerate(lines):
        stripped = line.strip()
        if stripped.startswith('#endif') and ('ELADEF_H' in stripped.upper()):
            endif_idx = i
            break

    if endif_idx == -1:
        print(f"  [WARN] #endif for ElaDEF_H not found, appending at end.")
        endif_idx = len(lines)

    # 检查新枚举块是否已存在（幂等性检查）
    code_lines_text_raw = ''  # will be set after code_block_text is defined
    # 先把 lines 中已有的内容合并成字符串用于检查
    existing_text = ''.join(lines)

    code_block_text = '''\
/*************************************************************************************
 *
 * @ Description  : Ela Expand Define
 *
 * @ Version      : V1.0
 * @ Author       : Re11a/LinnJz
 *************************************************************************************/
Q_BEGIN_ENUM_CREATE(ElaTabBarType, ELA_EXPORT)

enum TabBarStyle
{
  Google  = 0x00'00,
  Firefox = 0x00'01,
};
Q_ENUM_CREATE(TabBarStyle)

enum TabBarColorRole
{
  Base     = 0x00'00,
  Selected = 0x00'01,
  Hover    = 0x00'02,
};
Q_ENUM_CREATE(TabBarColorRole)
Q_END_ENUM_CREATE(ElaTabBarType)

Q_BEGIN_ENUM_CREATE(ElaColorSchemeType, ELA_EXPORT)

enum ColorSchemeType
{
  Rgba,
  Argb
};
Q_ENUM_CREATE(ColorSchemeType)
Q_END_ENUM_CREATE(ElaColorSchemeType)

Q_BEGIN_ENUM_CREATE(ElaWidgetType, ELA_EXPORT)

enum BorderType
{
  NoBorder     = 0x00'00,
  TopBorder    = 0x00'01,
  BottomBorder = 0x00'02,
  LeftBorder   = 0x00'04,
  RightBorder  = 0x00'08,

  TopLeftBorder     = TopBorder | LeftBorder,
  TopRightBorder    = TopBorder | RightBorder,
  BottomLeftBorder  = BottomBorder | LeftBorder,
  BottomRightBorder = BottomBorder | RightBorder,

  TopBottomBorder = TopBorder | BottomBorder,
  LeftRightBorder = LeftBorder | RightBorder,

  AllBorder = TopBorder | BottomBorder | LeftBorder | RightBorder,
};
Q_ENUM_CREATE(BorderType)
Q_DECLARE_FLAGS(BorderFlags, BorderType)
Q_END_ENUM_CREATE(ElaWidgetType)

Q_BEGIN_ENUM_CREATE(ElaShadowGraphicsEffectType, ELA_EXPORT)

enum ProjectionMode
{
  Inset,
  Outset,
};
Q_ENUM_CREATE(ProjectionMode)

enum RotateMode
{
  Rotate45,
  Rotate135,
  Rotate225,
  Rotate315,
};
Q_ENUM_CREATE(RotateMode)
Q_END_ENUM_CREATE(ElaShadowGraphicsEffectType)

Q_BEGIN_ENUM_CREATE(ElaPivotType, ELA_EXPORT)

enum DisplayMode
{
  IconOnly            = 0x00'00,
  TextOnly            = 0x00'01,
  TextBesideIcon      = 0x00'02,
  TextUnderIcon       = 0x00'03,
  IconWithOverlayText = 0x00'04,
};
Q_ENUM_CREATE(DisplayMode)

enum MarkType
{
  MarkNone            = 0x00'00,
  MarkCheck           = 0x00'01,
  MarkTopIndicator    = 0x00'02,
  MarkBottomIndicator = 0x00'04
};
Q_ENUM_CREATE(MarkType)
Q_DECLARE_FLAGS(MarkFlags, MarkType)
Q_END_ENUM_CREATE(ElaPivotType)
Q_DECLARE_OPERATORS_FOR_FLAGS(ElaPivotType::MarkFlags)

#if defined(__cpp_lib_expected) || (__cplusplus >= 202302L && __has_include(<expected>))
#  include <expected>
template<typename T>
using ElaExpected = std::expected<T, ElaNavigationType::NodeResult>;

template<typename T>
using ElaUnexpected = std::unexpected<ElaNavigationType::NodeResult>;
#else
#  include "expected.hpp"
template<typename T>
using ElaExpected = tl::expected<T, ElaNavigationType::NodeResult>;

template<typename T>
using ElaUnexpected = tl::unexpected<ElaNavigationType::NodeResult>;
#endif // __cplusplus >= 202302L && __has_include(<expected>)
using ElaNodeOperateResult = ElaExpected<QString>;
'''

    # 幂等性检查：如果文件中已包含 ElaTabBarType，说明新枚举块已存在，跳过
    if 'Q_BEGIN_ENUM_CREATE(ElaTabBarType' in existing_text:
        if lines == original_lines:
            print(f"  [INFO] No changes needed.")
        else:
            print(f"  [OK] Enum blocks already present.")
        return

    code_block = code_block_text.splitlines(True)

    lines = lines[:endif_idx] + ['\n'] + code_block + ['\n'] + lines[endif_idx:]

    if lines == original_lines:
        print(f"  [INFO] No changes needed.")
        return

    with open(filepath, 'w', encoding='utf-8') as f:
        f.writelines(lines)

    print(f"  [OK] Processed successfully.")


# ═══════════════════════════════════════════════════════════════════
# ElaThemePrivate.h / .cpp 同步修复
# ═══════════════════════════════════════════════════════════════════

TABBAR_COLOR_BLOCK = '''\
    // ElaTabBar
    _lightThemeColorList[ElaThemeType::TabBarBase] = QColor(0xEA, 0xEA, 0xED);
    _darkThemeColorList[ElaThemeType::TabBarBase] = QColor(0x1C, 0x20, 0x27);

    _lightThemeColorList[ElaThemeType::TabBarSelected] = QColor(0xFF, 0xFF, 0xFF);
    _darkThemeColorList[ElaThemeType::TabBarSelected] = QColor(0x38, 0x3B, 0x43);

    _lightThemeColorList[ElaThemeType::TabBarHover] = QColor(0xD2, 0xD2, 0xD6);
    _darkThemeColorList[ElaThemeType::TabBarHover] = QColor(0x3B, 0x47, 0x5E);

    _lightThemeColorList[ElaThemeType::TabBarCloseButtonHover] = QColor(0xBF, 0xBF, 0xC3);
    _darkThemeColorList[ElaThemeType::TabBarCloseButtonHover] = QColor(0x5A, 0x64, 0x77);

    _lightThemeColorList[ElaThemeType::TabBarSelectedCloseButtonHover] = QColor(0xE7, 0xE7, 0xE8);
    _darkThemeColorList[ElaThemeType::TabBarSelectedCloseButtonHover] = QColor(0x57, 0x5A, 0x60);

'''

def process_elatheme_private_header(filepath):
    """将 ElaThemePrivate.h 中的数组大小 43→48"""
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    original = content
    content = content.replace('QColor _lightThemeColorList[43];', 'QColor _lightThemeColorList[48];')
    content = content.replace('QColor _darkThemeColorList[43];', 'QColor _darkThemeColorList[48];')

    if content != original:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        return True
    return False


def process_elatheme_private_source(filepath):
    """在 ElaThemePrivate.cpp 的 _initThemeColor() 中插入 TabBar 颜色初始化代码"""
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    original = content

    # 如果 TabBar 块已存在，跳过
    if 'ElaThemeType::TabBarBase' in content:
        return False

    # 找到 _initThemeColor 函数体
    init_func_match = re.search(r'void\s+ElaThemePrivate::_initThemeColor\s*\(\s*\)', content)
    if not init_func_match:
        print("  [WARN] ElaThemePrivate::_initThemeColor() not found.")
        return False

    func_body_start = content.find('{', init_func_match.end())
    if func_body_start < 0:
        return False

    # 在函数体内找到第一个赋值语句（ScrollBarHandle）
    first_assign = content.find('_lightThemeColorList', func_body_start)
    if first_assign < 0:
        return False

    # 回退到该行开头
    insert_pos = content.rfind('\n', 0, first_assign) + 1

    content = content[:insert_pos] + TABBAR_COLOR_BLOCK + content[insert_pos:]

    if content != original:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        return True
    return False


def main():
    # 获取脚本所在目录的父目录的父目录
    script_dir = os.path.dirname(os.path.abspath(__file__))
    parent_dir = os.path.dirname(script_dir)        # 上层目录
    grandparent_dir = os.path.dirname(parent_dir)   # 上层目录的上层目录
    base_dir = os.path.join(grandparent_dir, "ElaWidgetTools")

    # 1. 处理 ElaDef.h
    target_file = os.path.join(base_dir, "ElaDef.h")
    if not os.path.exists(target_file):
        print(f"Error: Target file '{target_file}' does not exist.")
        sys.exit(1)
    print(f"Processing: {target_file}")
    process_eladef_file(target_file)

    # 2. 处理 ElaThemePrivate.h (数组大小修正)
    hdr_file = os.path.join(base_dir, "private", "ElaThemePrivate.h")
    if os.path.exists(hdr_file):
        print(f"Processing: {hdr_file}")
        if process_elatheme_private_header(hdr_file):
            print("  [OK] Array size fixed (43→48).")
        else:
            print("  [INFO] Already up to date.")
    else:
        print(f"  [WARN] '{hdr_file}' not found, skipping.")

    # 3. 处理 ElaThemePrivate.cpp (TabBar 颜色初始化)
    src_file = os.path.join(base_dir, "private", "ElaThemePrivate.cpp")
    if os.path.exists(src_file):
        print(f"Processing: {src_file}")
        if process_elatheme_private_source(src_file):
            print("  [OK] TabBar color init inserted.")
        else:
            print("  [INFO] Already up to date.")
    else:
        print(f"  [WARN] '{src_file}' not found, skipping.")

    print("\nDone.")


if __name__ == '__main__':
    main()