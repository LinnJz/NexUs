#!/usr/bin/env python3
"""Deep verification: member count, type preservation, structure preservation."""

import os, re, sys
from collections import defaultdict

_MACRO_RE = re.compile(
    r'(Q_PROPERTY_CREATE_D|Q_PROPERTY_CREATE_2|Q_PROPERTY_CREATE|'
    r'Q_PROPERTY_REF_CREATE|Q_PRIVATE_CREATE_D|Q_PRIVATE_CREATE|'
    r'Q_PRIVATE_REF_CREATE)\s*\('
)

_REF_CREATE_RE = re.compile(
    r'(Q_PROPERTY_REF_CREATE|Q_PRIVATE_REF_CREATE)\s*\('
)

def parse_macro(line):
    # CREATE_2 has 3 args - match first
    m = re.match(r'(Q_\w+)\s*\(\s*(.+?)\s*,\s*(.+?)\s*,\s*(.+?)\s*\)', line)
    if m:
        return m.group(1), m.group(2).strip(), m.group(3).strip(), m.group(4).strip()
    # Regular 2-arg macro
    m = re.match(r'(Q_\w+)\s*\(\s*(.+?)\s*,\s*(.+?)\s*\)', line)
    if m:
        return m.group(1), m.group(2).strip(), m.group(3).strip()
    return None

def normalize_type(t):
    t = t.strip()
    t = re.sub(r'\bconst\b\s*', '', t)
    t = re.sub(r'\s*&\s*', '', t)
    t = re.sub(r'\s*\*\s*', '*', t)
    t = re.sub(r'\s+', ' ', t)
    return t.strip()

def extract_members_from_file(filepath):
    """Return dict: section_name -> list of (kind, raw_line, normalized_key)"""
    with open(filepath, 'r', encoding='utf-8-sig', errors='replace') as f:
        content = f.read()
    
    lines = content.split('\n')
    
    # Find class bodies
    in_class = False
    brace_depth = 0
    class_depth = 0
    sections = defaultdict(list)
    current_section = '_default'
    
    for line in lines:
        stripped = line.strip()
        
        if '{' in stripped and not stripped.startswith('#'):
            brace_depth += 1
            if not in_class:
                in_class = True
                class_depth = brace_depth
            continue
        if '}' in stripped and not stripped.startswith('#'):
            brace_depth -= 1
            if in_class and brace_depth < class_depth:
                in_class = False
            continue
        
        if not in_class:
            continue
        if not stripped or stripped.startswith('#') or stripped.startswith('//'):
            continue
        
        # Access specifier
        if re.match(r'^(public|private|protected|Q_SIGNALS|signals|Q_SLOTS|slots)\s*:', stripped):
            current_section = stripped.split(':')[0].strip()
            continue
        
        if stripped.startswith('friend '):
            continue
        if stripped == 'Q_OBJECT':
            continue
        if re.match(r'Q_D_CREATE\s*\(', stripped):
            continue
        
        # Macro member
        if _MACRO_RE.search(stripped):
            parsed = parse_macro(stripped.rstrip(';'))
            if parsed:
                if _REF_CREATE_RE.search(stripped):
                    key = f"REF:{parsed[1]}:{parsed[2]}"
                    sections[current_section].append(('ref_macro', stripped.rstrip(';'), key))
                elif len(parsed) == 3:
                    key = f"MACRO:{parsed[1]}:{parsed[2]}"
                    sections[current_section].append(('macro', stripped.rstrip(';'), key))
                else:
                    key = f"MACRO2:{parsed[1]}:{parsed[2]}:{parsed[3]}"
                    sections[current_section].append(('macro2', stripped.rstrip(';'), key))
            continue
        
        # Regular member
        rm = re.search(r'^\s*([\w:<>,\s*&]+)\s+(\*?\w+)\s*(?:=|{|;)', line)
        if rm:
            type_str = normalize_type(rm.group(1))
            name_str = rm.group(2).strip()
            if re.match(r'^(explicit|virtual|static|const|inline|Q_SLOT|Q_INVOKABLE|Q_SIGNAL|~)', stripped):
                continue
            if '(' in stripped:
                continue
            key = f"REG:{type_str}:{name_str}"
            sections[current_section].append(('regular', line.rstrip(), key))
    
    return sections


def verify_deep(orig_path, out_path):
    """Deep verification of one file."""
    issues = []
    if not os.path.exists(out_path):
        return [f"MISSING: {out_path}"]
    
    orig = extract_members_from_file(orig_path)
    out = extract_members_from_file(out_path)
    
    # 1. Count total members
    orig_total = sum(len(v) for v in orig.values())
    out_total = sum(len(v) for v in out.values())
    
    if orig_total != out_total:
        issues.append(f"  COUNT MISMATCH: orig={orig_total}, out={out_total}")
    
    # 2. Check all original members exist in output
    orig_keys = set()
    for sec, mems in orig.items():
        for kind, raw, key in mems:
            if kind == 'ref_macro':
                # REF_CREATE(T, Name) -> normalize to key TYPE:Name
                parsed = parse_macro(raw)
                if parsed and len(parsed) >= 3:
                    ntype = normalize_type(parsed[1])
                    nkey = f"{ntype}:{parsed[2]}"
                    orig_keys.add(nkey)
            elif kind == 'macro2':
                parsed = parse_macro(raw)
                if parsed and len(parsed) >= 4:
                    ntype = normalize_type(parsed[2])
                    nkey = f"{ntype}:{parsed[3]}"
                    orig_keys.add(nkey)
            else:
                orig_keys.add(key)
    
    out_keys = set()
    for sec, mems in out.items():
        for kind, raw, key in mems:
            if kind == 'macro2':
                parsed = parse_macro(raw)
                if parsed and len(parsed) >= 4:
                    ntype = normalize_type(parsed[2])
                    nkey = f"{ntype}:{parsed[3]}"
                    out_keys.add(nkey)
            elif kind == 'ref_macro':
                parsed = parse_macro(raw)
                if parsed and len(parsed) >= 3:
                    ntype = normalize_type(parsed[1])
                    nkey = f"{ntype}:{parsed[2]}"
                    out_keys.add(nkey)
            else:
                out_keys.add(key)
    
    missing = orig_keys - out_keys
    extra = out_keys - orig_keys
    
    for m in missing:
        issues.append(f"  MISSING MEMBER: {m}")
    for e in extra:
        issues.append(f"  EXTRA MEMBER: {e}")
    
    # 3. Check no semicolons on macros in output
    for sec, mems in out.items():
        for kind, raw, key in mems:
            if kind in ('macro', 'macro2'):
                if raw.rstrip().endswith(';'):
                    issues.append(f"  SEMICOLON: {raw.strip()}")
    
    return issues


def main():
    sample_dir = os.path.join(os.path.dirname(__file__), 'Sample')
    output_dir = os.path.join(os.path.dirname(__file__), 'Output')
    
    if not os.path.isdir(sample_dir) or not os.path.isdir(output_dir):
        print("ERROR: dirs not found")
        sys.exit(1)
    
    total_files = 0
    files_with_issues = 0
    total_issues = 0
    
    for root, dirs, files in os.walk(sample_dir):
        for fname in sorted(files):
            if not fname.endswith(('.h', '.hpp', '.hxx')):
                continue
            total_files += 1
            orig_path = os.path.join(root, fname)
            rel = os.path.relpath(orig_path, sample_dir)
            out_path = os.path.join(output_dir, rel)
            
            issues = verify_deep(orig_path, out_path)
            if issues:
                files_with_issues += 1
                total_issues += len(issues)
                print(f"FAIL: {rel}")
                for iss in issues:
                    print(iss)
                print()
    
    print(f"\n{'='*60}")
    print(f"Files: {total_files} | Issues: {total_issues} in {files_with_issues} files")
    if total_issues == 0:
        print("ALL CHECKS PASSED")


if __name__ == '__main__':
    main()