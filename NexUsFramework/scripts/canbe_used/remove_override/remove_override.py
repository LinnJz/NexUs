#!/usr/bin/env python3
"""
Scan header files recursively and remove 'override' from destructors
of C++ classes/structs that have inheritance (contain ':' in their declaration).

Usage:
    python remove_override.py <folder>              # process files
    python remove_override.py <folder> --dry-run    # preview changes only
    python remove_override.py <folder> --ext .hxx   # add extra extensions
"""

import os
import re
import sys
import argparse

HEADER_EXTS = {'.h', '.hpp', '.hxx', '.h++', '.hh', '.H', '.HPP', '.HXX'}

CLASS_STRUCT_RE = re.compile(r'\b(class|struct)\s+([\w:]+)')

DESTRUCTOR_OVERRIDE_RE = re.compile(
    r'^(\s*(?:virtual\s+)?~\w+\s*\([^)]*\))\s*\boverride\b(.*)',
    re.DOTALL
)


def remove_override_from_file(filepath, dry_run=False):
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()

    modified = False
    new_lines = []
    output_lines = []

    depth = 0
    class_stack = []
    pending = None

    for i, line in enumerate(lines):
        opens = line.count('{')
        closes = line.count('}')

        had_pending = pending is not None

        if pending is not None:
            p_depth, p_text = pending
            if '{' in line:
                brace_idx = line.index('{')
                before_brace_on_line = line[:brace_idx]
                has_inheritance = ':' in p_text or ':' in before_brace_on_line
                opens_before = before_brace_on_line.count('{')
                closes_before = before_brace_on_line.count('}')
                enter_depth = depth + opens_before - closes_before + 1
                class_stack.append((enter_depth, has_inheritance))
                pending = None
            elif ';' in line:
                pending = None
            else:
                pending = (p_depth, p_text + line)

        if not had_pending and pending is None:
            stripped = line.strip()
            m = CLASS_STRUCT_RE.search(stripped)
            if m:
                after_name = stripped[m.end():]
                if '{' in after_name:
                    brace_idx = after_name.index('{')
                    before_brace = after_name[:brace_idx]
                    has_inheritance = ':' in before_brace
                    opens_before = before_brace.count('{')
                    closes_before = before_brace.count('}')
                    enter_depth = depth + opens_before - closes_before + 1
                    class_stack.append((enter_depth, has_inheritance))
                elif ';' in after_name:
                    pass
                else:
                    pending = (depth, after_name + '\n')

        in_inherited = any(ctx[1] for ctx in class_stack)

        if in_inherited:
            m = DESTRUCTOR_OVERRIDE_RE.match(line)
            if m:
                before = m.group(1)
                after = m.group(2)
                if after and after[0] == '{':
                    new_line = before + ' ' + after
                else:
                    new_line = before + after
                if new_line != line:
                    modified = True
                    output_lines.append(new_line)
                    depth = depth + opens - closes
                    while class_stack and depth < class_stack[-1][0]:
                        class_stack.pop()
                    continue

        output_lines.append(line)

        depth = depth + opens - closes
        while class_stack and depth < class_stack[-1][0]:
            class_stack.pop()

    if modified:
        if not dry_run:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.writelines(output_lines)
        print(f"{'[DRY RUN] Would modify' if dry_run else 'Modified'}: {filepath}")
        return 1
    return 0


def main():
    parser = argparse.ArgumentParser(
        description='Remove "override" from destructors in inherited C++ classes/structs.'
    )
    parser.add_argument(
        'folder',
        help='Root folder to scan recursively for header files'
    )
    parser.add_argument(
        '--dry-run', '-n',
        action='store_true',
        help='Preview changes without modifying files'
    )
    parser.add_argument(
        '--ext',
        nargs='+',
        default=[],
        help=f'Additional file extensions to scan (default: {", ".join(sorted(HEADER_EXTS))})'
    )
    args = parser.parse_args()

    exts = set(HEADER_EXTS)
    for e in args.ext:
        exts.add(e if e.startswith('.') else '.' + e)

    folder = os.path.abspath(args.folder)
    if not os.path.isdir(folder):
        print(f"Error: '{folder}' is not a valid directory", file=sys.stderr)
        sys.exit(1)

    total_modified = 0
    for root, dirs, files in os.walk(folder):
        for fname in files:
            ext = os.path.splitext(fname)[1]
            if ext in exts:
                filepath = os.path.join(root, fname)
                total_modified += remove_override_from_file(filepath, dry_run=args.dry_run)

    action = "would be modified" if args.dry_run else "modified"
    print(f"\nTotal files {action}: {total_modified}")


if __name__ == '__main__':
    main()