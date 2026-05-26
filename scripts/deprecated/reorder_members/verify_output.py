#!/usr/bin/env python3
"""Verify that reorder_members.py output is correct relative to input."""

import os
import re
import sys
from typing import Dict, List, Set, Tuple

MACRO_NAMES = {
    'Q_PROPERTY_CREATE_D', 'Q_PROPERTY_CREATE', 'Q_PROPERTY_CREATE_2',
    'Q_PROPERTY_REF_CREATE',
    'Q_PRIVATE_CREATE_D', 'Q_PRIVATE_CREATE', 'Q_PRIVATE_REF_CREATE',
    'Q_D_CREATE',
}

_MACRO_RE = re.compile(
    r'(Q_PROPERTY_CREATE_D|Q_PROPERTY_CREATE_2|Q_PROPERTY_CREATE|'
    r'Q_PROPERTY_REF_CREATE|Q_PRIVATE_CREATE_D|Q_PRIVATE_CREATE|'
    r'Q_PRIVATE_REF_CREATE)\s*\('
)

_REF_CREATE_RE = re.compile(
    r'(Q_PROPERTY_REF_CREATE|Q_PRIVATE_REF_CREATE)\s*\('
)

_REGULAR_MEMBER_RE = re.compile(
    r'^\s*([\w:<>,\s*&]+)\s+(\*?\w+)\s*(?:=|{|\s*;)',
)


def find_all_member_vars(content: str) -> Tuple[Set[str], Set[str], Set[str]]:
    """Extract {macro members, regular members, ref_create members} from content."""
    macros = set()
    regulars = set()
    ref_creates = set()

    for line in content.split('\n'):
        stripped = line.strip()

        # REF_CREATE macros (before conversion)
        rm = _REF_CREATE_RE.search(stripped)
        if rm:
            ref_creates.add(stripped.rstrip(';'))
            continue

        # Other macros
        mm = _MACRO_RE.search(stripped)
        if mm:
            macros.add(stripped.rstrip(';'))
            continue

        # Regular member variable
        rm2 = _REGULAR_MEMBER_RE.search(stripped)
        if rm2 and not stripped.startswith('#') and '(' not in stripped and ')' not in stripped:
            # Filter out function declarations
            if not re.match(r'^(explicit|virtual|static|const|inline|Q_SLOT|Q_INVOKABLE|Q_SIGNAL|~)', stripped):
                if not stripped.endswith(')') and not re.search(r'\bnew\b', stripped):
                    regulars.add(line.rstrip())

    return macros, regulars, ref_creates


def find_all_non_member_lines(content: str) -> List[str]:
    """Extract lines that are NOT member variables."""
    lines = []
    for line in content.split('\n'):
        stripped = line.strip()
        if not stripped:
            lines.append(line)
            continue
        if stripped.startswith('#'):
            lines.append(line)
            continue
        if stripped.startswith('//'):
            lines.append(line)
            continue
        if re.match(r'^(public|private|protected|Q_SIGNALS|signals|Q_SLOTS|slots)\s*:', stripped):
            lines.append(line)
            continue
        if stripped == 'Q_OBJECT':
            lines.append(line)
            continue
        if stripped.startswith('friend '):
            lines.append(line)
            continue
        if _MACRO_RE.search(stripped):
            continue
        if _REF_CREATE_RE.search(stripped):
            continue
        rm = _REGULAR_MEMBER_RE.search(stripped)
        if rm and not stripped.startswith(('#', 'explicit', 'virtual', 'static', 'const', 'inline')):
            if not stripped.endswith(')') and not re.search(r'\bnew\b', stripped):
                continue
        lines.append(line)
    return lines


def verify_file(original_path: str, output_path: str) -> List[str]:
    """Verify one file pair. Returns list of issues."""
    issues = []

    if not os.path.exists(output_path):
        return [f"MISSING OUTPUT: {output_path}"]

    with open(original_path, 'r', encoding='utf-8-sig', errors='replace') as f:
        orig = f.read()
    with open(output_path, 'r', encoding='utf-8-sig', errors='replace') as f:
        out = f.read()

    orig_macros, orig_regulars, orig_ref = find_all_member_vars(orig)
    out_macros, out_regulars, out_ref = find_all_member_vars(out)

    # 1. No REF_CREATE should remain in output
    if out_ref:
        issues.append(f"  REF_CREATE not converted: {out_ref}")

    # 2. Count members  
    orig_total = len(orig_macros) + len(orig_regulars) + len(orig_ref)
    out_total = len(out_macros) + len(out_regulars)

    if orig_total != out_total:
        issues.append(f"  MEMBER COUNT MISMATCH: orig={orig_total}, out={out_total}")
        issues.append(f"    orig macros: {orig_macros}")
        issues.append(f"    orig regulars: {orig_regulars}")
        issues.append(f"    out macros: {out_macros}")
        issues.append(f"    out regulars: {out_regulars}")

    # 3. Check CREATE_2 count matches original REF_CREATE count
    orig_ref_count = len(orig_ref)
    out_create2_count = sum(1 for m in out_macros if 'Q_PROPERTY_CREATE_2' in m)
    if orig_ref_count != out_create2_count:
        issues.append(f"  CREATE_2 COUNT MISMATCH: orig_ref={orig_ref_count}, out_create2={out_create2_count}")

    # 4. Check semicolons on macros
    for m in out_macros:
        if m.rstrip().endswith(';'):
            issues.append(f"  SEMICOLON ON MACRO: {m.strip()}")

    # 5. Check for duplicate members (shouldn't have, except preprocessor branches)
    # (skip this for now, preprocessor makes it complex)

    return issues


def main():
    sample_dir = os.path.join(os.path.dirname(__file__), 'Sample')
    output_dir = os.path.join(os.path.dirname(__file__), 'Output')

    if not os.path.isdir(sample_dir):
        print(f"ERROR: Sample dir not found: {sample_dir}")
        sys.exit(1)
    if not os.path.isdir(output_dir):
        print(f"ERROR: Output dir not found: {output_dir}")
        sys.exit(1)

    extensions = ('.h', '.hpp', '.hxx')

    total_files = 0
    files_with_issues = 0
    total_issues = 0

    for root, dirs, files in os.walk(sample_dir):
        for fname in sorted(files):
            if not fname.endswith(extensions):
                continue
            total_files += 1
            orig_path = os.path.join(root, fname)
            rel = os.path.relpath(orig_path, sample_dir)
            out_path = os.path.join(output_dir, rel)

            issues = verify_file(orig_path, out_path)
            if issues:
                files_with_issues += 1
                total_issues += len(issues)
                print(f"ISSUES in {rel}:")
                for iss in issues:
                    print(iss)
                print()

    print(f"\n{'='*60}")
    print(f"Total files: {total_files}")
    print(f"Files with issues: {files_with_issues}")
    print(f"Total issues: {total_issues}")
    if total_issues == 0:
        print("ALL CHECKS PASSED")
    else:
        print("SOME CHECKS FAILED")


if __name__ == '__main__':
    main()