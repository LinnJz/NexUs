#!/usr/bin/env python3
"""
C++ Include Reorder Tool
=========================
Scans a directory for C/C++ files (.cpp, .h, .hpp, etc.) and reorders
#include directives according to a standard ordering, adding directory
prefixes for associated headers when needed.

Include ordering (each part separated by a blank line):
  1. #include "SelfHeader.h"   — self header (.cpp files only)
  2. #include <...>            — standard library (non-Qt)
  3. #include <...>            — Qt library (starts with Q or Qt)
  4. #include "..."            — associated headers (with prefix if applicable)

Within each part, includes are sorted alphabetically.

Usage:
    python reorder_includes.py <directory> [--dry-run] [--extensions .cpp,.h,.hpp]
"""

import re
import os
import sys
import argparse
import glob
from pathlib import Path
from typing import List, Dict, Tuple, Optional, NamedTuple

CppFileExtensions = {'.cpp', '.cc', '.cxx', '.c++', '.h', '.hpp', '.hxx', '.h++', '.inl', '.mm', '.m'}

INCLUDE_RE = re.compile(r'^[ \t]*#include[ \t]+([<"])([^>"]+)([>"])', re.MULTILINE)


class IncludeDirective(NamedTuple):
    full_line: str
    file_path: str
    include_type: str  # 'quote' or 'angle'
    line_number: int


def parse_includes(content: str) -> List[IncludeDirective]:
    includes = []
    for line_number, line in enumerate(content.split('\n'), 1):
        line = line.rstrip('\r')
        m = INCLUDE_RE.match(line)
        if m:
            bracket = m.group(1)
            bracket_close = m.group(3)
            file_path = m.group(2)

            if (bracket == '<' and bracket_close != '>') or (bracket == '"' and bracket_close != '"'):
                continue

            include_type = 'quote' if bracket == '"' else 'angle'
            includes.append(IncludeDirective(
                full_line=line.rstrip(),
                file_path=file_path,
                include_type=include_type,
                line_number=line_number
            ))
    return includes


def is_self_header(include_path: str, current_filename: str) -> bool:
    current_base = os.path.splitext(os.path.basename(current_filename))[0]
    include_base = os.path.splitext(os.path.basename(include_path))[0]

    if os.path.splitext(current_filename)[1].lower() == '.h':
        return False

    return current_base == include_base and include_path.endswith('.h')


def is_qt_include(include_path: str) -> bool:
    basename = os.path.basename(include_path)
    return basename.startswith('Q') or basename.startswith('Qt')


def build_header_index(root_dir: str) -> Dict[str, str]:
    index = {}
    abs_root = os.path.abspath(root_dir)

    for root, dirs, files in os.walk(abs_root):
        for filename in files:
            if os.path.splitext(filename)[1].lower() in CppFileExtensions:
                base = os.path.basename(filename)
                rel_path = os.path.relpath(os.path.join(root, filename), abs_root)
                if base not in index:
                    index[base] = rel_path
    return index


def resolve_prefix(
    include_basename: str,
    current_file: str,
    header_index: Dict[str, str],
    root_dir: str
) -> str:
    abs_root = os.path.abspath(root_dir)
    current_dir = os.path.dirname(os.path.abspath(current_file))

    if include_basename not in header_index:
        return include_basename

    header_rel_path = header_index[include_basename]
    header_abs_dir = os.path.dirname(os.path.join(abs_root, header_rel_path))

    if os.path.normpath(header_abs_dir) == os.path.normpath(current_dir):
        return include_basename

    return header_rel_path.replace(os.sep, '/')


def classify_includes(
    content: str,
    current_file: str,
    header_index: Dict[str, str],
    root_dir: str
) -> Dict[str, List[IncludeDirective]]:
    includes = parse_includes(content)
    current_filename = os.path.basename(current_file)

    result = {
        'self_header': [],
        'standard': [],
        'qt': [],
        'associated': [],
        'unclassified': [],
    }

    for inc in includes:
        if inc.include_type == 'quote' and is_self_header(inc.file_path, current_filename):
            result['self_header'].append(inc)
        elif inc.include_type == 'angle':
            if is_qt_include(inc.file_path):
                result['qt'].append(inc)
            else:
                result['standard'].append(inc)
        elif inc.include_type == 'quote':
            new_path = resolve_prefix(inc.file_path, current_file, header_index, root_dir)
            new_inc = IncludeDirective(
                full_line='#include "{0}"'.format(new_path),
                file_path=new_path,
                include_type=inc.include_type,
                line_number=inc.line_number
            )
            result['associated'].append(new_inc)
        else:
            result['unclassified'].append(inc)

    for key in result:
        result[key].sort(key=lambda x: x.file_path.lower())

    return result


def reorder_includes_in_content(
    content: str,
    current_file: str,
    header_index: Dict[str, str],
    root_dir: str
) -> str:
    parts = classify_includes(content, current_file, header_index, root_dir)
    lines = content.split('\n')

    include_line_numbers = set()
    for key in parts:
        for inc in parts[key]:
            include_line_numbers.add(inc.line_number)

    if not include_line_numbers:
        return content

    min_include = min(include_line_numbers)
    max_include = max(include_line_numbers)

    has_preprocessor = False
    for i in range(min_include - 1, max_include):
        stripped = lines[i].strip()
        if stripped.startswith('#') and not stripped.startswith('#include'):
            has_preprocessor = True
            break

    if has_preprocessor:
        return content

    new_includes = []
    part_order = ['self_header', 'standard', 'qt', 'associated']
    first = True
    for part_key in part_order:
        part = parts[part_key]
        if not part:
            continue
        if not first:
            new_includes.append('')
        for inc in part:
            new_includes.append(inc.full_line)
        first = False

    result_lines = []
    in_include_block = False
    after_include_block = False

    for i, line in enumerate(lines):
        line_num = i + 1

        if line_num == min_include:
            result_lines.extend(new_includes)
            in_include_block = True
            continue

        if in_include_block:
            if line_num > max_include:
                in_include_block = False
                after_include_block = True
            elif line_num not in include_line_numbers and line.strip() == '':
                continue
            elif line_num not in include_line_numbers and line.strip() != '':
                in_include_block = False
                after_include_block = True

        if not in_include_block or after_include_block:
            result_lines.append(line)

    return '\n'.join(result_lines)


def find_cpp_files(root_dir: str) -> List[str]:
    result = []
    abs_root = os.path.abspath(root_dir)
    for root, dirs, files in os.walk(abs_root):
        for filename in files:
            ext = os.path.splitext(filename)[1].lower()
            if ext in CppFileExtensions:
                result.append(os.path.join(root, filename))
    return sorted(result)


def reorder_includes_in_file(
    filepath: str,
    header_index: Dict[str, str],
    root_dir: str,
    dry_run: bool = False
) -> bool:
    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
    except Exception as e:
        print(f"  ERROR reading {filepath}: {e}", file=sys.stderr)
        return False

    new_content = reorder_includes_in_content(content, filepath, header_index, root_dir)

    if new_content == content:
        return False

    if dry_run:
        return True

    try:
        with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
            f.write(new_content)
        return True
    except Exception as e:
        print(f"  ERROR writing {filepath}: {e}", file=sys.stderr)
        return False


def main():
    parser = argparse.ArgumentParser(
        description='Reorder #include directives in C/C++ files according to coding standards.'
    )
    parser.add_argument(
        'directory',
        nargs='?',
        default='.',
        help='Root directory to scan for C/C++ files (default: current directory)'
    )
    parser.add_argument(
        '--dry-run',
        action='store_true',
        help='Show what would be changed without actually modifying files'
    )
    parser.add_argument(
        '--extensions',
        default='',
        help='Comma-separated list of file extensions to process (default: .cpp,.cc,.cxx,.h,.hpp,.hxx,.mm)'
    )

    args = parser.parse_args()

    root_dir = os.path.abspath(args.directory)
    if not os.path.isdir(root_dir):
        print(f"Error: '{args.directory}' is not a valid directory.", file=sys.stderr)
        sys.exit(1)

    global CppFileExtensions
    if args.extensions:
        CppFileExtensions = set(ext.strip() if ext.startswith('.') else '.' + ext.strip()
                                for ext in args.extensions.split(','))

    print(f"Scanning directory: {root_dir}")
    print(f"File extensions: {CppFileExtensions}")

    cpp_files = find_cpp_files(root_dir)
    print(f"Found {len(cpp_files)} C/C++ file(s)")

    print("Building header index...")
    header_index = build_header_index(root_dir)
    print(f"Indexed {len(header_index)} header(s)")

    modified_count = 0
    for filepath in cpp_files:
        rel_path = os.path.relpath(filepath, root_dir)
        changed = reorder_includes_in_file(filepath, header_index, root_dir, dry_run=args.dry_run)
        if changed:
            modified_count += 1
            action = "[DRY-RUN] Would modify" if args.dry_run else "Modified"
            print(f"  {action}: {rel_path}")

    print(f"\nDone. Modified {modified_count} file(s).")
    if args.dry_run:
        print("(Dry-run mode, no files were actually changed)")


if __name__ == '__main__':
    main()