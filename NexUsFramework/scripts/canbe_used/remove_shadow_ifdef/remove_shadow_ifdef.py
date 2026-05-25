#!/usr/bin/env python3
"""
Scan a directory recursively for C++/Qt header files (.h, .hpp, .hxx, .hh, .H, .HPP, .HXX, .HH),
remove the #if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0) / #else / #endif
conditional block around _shadowBorderWidth, keeping only the non-zero branch.

Usage:
    python remove_shadow_ifdef.py <directory>
"""

import os
import re
import sys

PATTERN = re.compile(
    r'[ \t]*#if\s+defined\(Q_OS_WIN\)\s*&&\s*QT_VERSION\s*==\s*QT_VERSION_CHECK\(6\s*,\s*11\s*,\s*0\)[^\n]*\n'
    r'[ \t]*int\s+_shadowBorderWidth\{0\};[^\n]*\n'
    r'[ \t]*#else[^\n]*\n'
    r'([ \t]*)int\s+_shadowBorderWidth\{(\d+)\};[^\n]*\n'
    r'[ \t]*#endif[^\n]*\n?'
)

REPLACEMENT = r'\1int _shadowBorderWidth{\2};\n'

HEADER_EXTS = {'.h', '.hpp', '.hxx', '.hh', '.H', '.HPP', '.HXX', '.HH'}


def process_file(filepath: str) -> bool:
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()

    new_content, count = PATTERN.subn(REPLACEMENT, content)

    if count > 0:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f'[FIXED] {filepath} ({count} occurrence(s))')
        return True
    return False


def main():
    if len(sys.argv) < 2:
        print('Usage: python remove_shadow_ifdef.py <directory>')
        sys.exit(1)

    root_dir = sys.argv[1]
    if not os.path.isdir(root_dir):
        print(f'Error: {root_dir} is not a valid directory')
        sys.exit(1)

    modified_count = 0
    for root, dirs, files in os.walk(root_dir):
        for name in files:
            ext = os.path.splitext(name)[1]
            if ext in HEADER_EXTS:
                filepath = os.path.join(root, name)
                if process_file(filepath):
                    modified_count += 1

    print(f'\nDone. {modified_count} file(s) modified.')


if __name__ == '__main__':
    main()