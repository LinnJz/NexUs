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
        if '#endif' in line and 'ELADEF_H' in line:
            endif_idx = i
            break

    if endif_idx == -1:
        print(f"  [WARN] #endif // ELADEF_H not found, appending at end.")
        endif_idx = len(lines)

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

    code_block = code_block_text.splitlines(True)

    lines = lines[:endif_idx] + ['\n'] + code_block + ['\n'] + lines[endif_idx:]

    if lines == original_lines:
        print(f"  [INFO] No changes needed.")
        return

    with open(filepath, 'w', encoding='utf-8') as f:
        f.writelines(lines)

    print(f"  [OK] Processed successfully.")


def main():
    # 获取脚本所在目录的父目录的父目录
    script_dir = os.path.dirname(os.path.abspath(__file__))
    parent_dir = os.path.dirname(script_dir)        # 上层目录
    grandparent_dir = os.path.dirname(parent_dir)   # 上层目录的上层目录
    target_file = os.path.join(grandparent_dir, "ElaWidgetTools", "ElaDef.h")

    if not os.path.exists(target_file):
        print(f"Error: Target file '{target_file}' does not exist.")
        sys.exit(1)

    print(f"Processing: {target_file}")
    process_eladef_file(target_file)
    print("\nDone.")


if __name__ == '__main__':
    main()