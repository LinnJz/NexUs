#!/usr/bin/env python3
"""
ElaWidgetTools Code Transformation Script
Performs 6 code transformations on C++ headers/sources per task.md.
Usage: python scripts/code_transform.py <path> [--dry-run]
"""

import re
import sys
import argparse
from pathlib import Path

# ═══════════════════════════════════════════════════════════════════
# Configuration
# ═══════════════════════════════════════════════════════════════════

HEADER_EXTS = {'.h', '.hpp', '.hxx', '.hh'}
SOURCE_EXTS = {'.cpp', '.cxx', '.cc', '.c'}

RECOGNIZED_MACROS = sorted([
    'Q_PROPERTY_CREATE_Q_H', 'Q_PRIVATE_CREATE_Q_H',
    'Q_PROPERTY_REF_CREATE_Q_H', 'Q_PRIVATE_REF_CREATE_Q_H',
    'Q_PROPERTY_CREATE', 'Q_PROPERTY_REF_CREATE',
    'Q_PRIVATE_CREATE', 'Q_PRIVATE_REF_CREATE',
    'Q_PROPERTY_CREATE_D', 'Q_PRIVATE_CREATE_D',
    'Q_PROPERTY_CREATE_Q_CPP', 'Q_PROPERTY_REF_CREATE_Q_CPP',
    'Q_PRIVATE_CREATE_Q_CPP', 'Q_PRIVATE_REF_CREATE_Q_CPP',
], key=len, reverse=True)

CPP_MACROS = {
    'Q_PROPERTY_CREATE_Q_CPP', 'Q_PROPERTY_REF_CREATE_Q_CPP',
    'Q_PRIVATE_CREATE_Q_CPP', 'Q_PRIVATE_REF_CREATE_Q_CPP',
}

D_MACROS = {'Q_PROPERTY_CREATE_D', 'Q_PRIVATE_CREATE_D'}

QS_TYPES = {
    "QBitArray", "QBitmap", "QBrush", "QByteArray",
    "QCache", "QCollator", "QCollatorSortKey", "QContiguousCache", "QColor",
    "QCryptographicHash", "QCursor", "QEasingCurve", "QFont",
    "QGlyphRun", "QHash", "QIcon", "QImage",
    "QItemSelection", "QJsonArray", "QJsonDocument", "QJsonObject",
    "QJsonValue", "QLinkedList", "QList", "QLocale",
    "QMap", "QModelIndex", "QMultiHash", "QMultiMap", "QOpenGLVersionProfile",
    "QPageLayout", "QPageSize", "QPainterPath", "QPalette",
    "QPen", "QPersistentModelIndex", "QPicture", "QPixmap",
    "QPolygon", "QPolygonF", "QQueue", "QRawFont",
    "QRegion", "QRegularExpression", "QSet",
    "QStack", "QStaticText", "QString", "QStringList",
    "QStyleHintReturn", "QStyleOption", "QSurfaceFormat",
    "QTextBlock", "QTextBlockFormat", "QTextBoundaryFinder",
    "QTextCharFormat", "QTextDocument", "QTextFormat",
    "QTextFrame", "QTextFrameFormat", "QTextImageFormat",
    "QTextList", "QTextListFormat", "QTextTable",
    "QTextTableFormat", "QTransform", "QUrl", "QVarLengthArray",
    "QVariant", "QVariantMap", "QVector",
}

TYPE_SIZES = {
    'bool': 1, 'char': 1, 'signed char': 1, 'unsigned char': 1,
    'int8_t': 1, 'uint8_t': 1, 'qint8': 1, 'quint8': 1,
    'short': 2, 'unsigned short': 2, 'short int': 2,
    'int16_t': 2, 'uint16_t': 2, 'qint16': 2, 'quint16': 2,
    'int': 4, 'unsigned int': 4, 'unsigned': 4,
    'long': 4, 'unsigned long': 4, 'long int': 4,
    'uint32_t': 4, 'float': 4, 'qint32': 4, 'quint32': 4, 'int32_t': 4,
    'long long': 8, 'unsigned long long': 8, 'double': 8, 'long double': 8,
    'qreal': 8, 'qint64': 8, 'quint64': 8, 'int64_t': 8, 'uint64_t': 8,
    'size_t': 8, 'ptrdiff_t': 8,
    'QLocale': 8, 'QEasingCurve': 8, 'QPersistentModelIndex': 8,
    'QCryptographicHash': 8, 'QCollator': 8, 'QCollatorSortKey': 8,
    'QStaticText': 8, 'QTextBoundaryFinder': 72,
    'QGlyphRun': 8, 'QRawFont': 8,
    'QSurfaceFormat': 8, 'QOpenGLVersionProfile': 8,
    'QBrush': 8, 'QPen': 8, 'QIcon': 8, 'QCursor': 8,
    'QRegion': 8, 'QPainterPath': 8, 'QRegularExpression': 8,
    'QPageSize': 8, 'QPageLayout': 8, 'QStyleHintReturn': 8,
    'QUrl': 8, 'QJsonObject': 8, 'QJsonArray': 8, 'QJsonDocument': 8,
    'QDate': 8, 'QVariantMap': 8, 'QSize': 8, 'QPoint': 8,
    'QRect': 16, 'QSizeF': 16, 'QPointF': 16, 'QColor': 16,
    'QTextImageFormat': 16, 'QTextBlock': 16, 'QFont': 16,
    'QTextLength': 16, 'QPalette': 16,
    'QTextDocument': 16, 'QTextList': 16, 'QTextFrame': 16,
    'QTextTable': 16, 'QTextFormat': 16, 'QTextCharFormat': 16,
    'QTextBlockFormat': 16, 'QTextListFormat': 16, 'QTextFrameFormat': 16,
    'QTextTableFormat': 16,
    'QStringList': 24, 'QModelIndex': 24,
    'QPolygon': 24, 'QPolygonF': 24, 'QString': 24, 'QPicture': 24,
    'QJsonValue': 24, 'QPixmap': 24, 'QItemSelection': 24,
    'QByteArray': 24, 'QImage': 24, 'QBitmap': 24, 'QBitArray': 24,
    'QVariant': 32, 'QRectF': 32, 'QStyleOption': 64, 'QTransform': 80,
}

TYPE_DIFF_LIST = {
    'bool', 'char', 'double', 'float', 'int',
    'int16_t', 'int32_t', 'int64_t', 'int8_t',
    'long', 'long long', 'long double', 'long int',
    'ptrdiff_t', 'qint16', 'qint32', 'qint64', 'qint8', 'qreal',
    'QDate', 'QPoint', 'QPointF', 'QRect', 'QSize', 'QSizeF', 'QTextLength',
    'short', 'short int', 'signed char', 'size_t',
    'uint16_t', 'uint32_t', 'uint64_t', 'uint8_t',
    'unsigned', 'unsigned char', 'unsigned long', 'unsigned long long', 'unsigned short',
}

TEMPLATE_SIZES = {
    'QMap': 8, 'QHash': 8, 'QMultiMap': 8, 'QMultiHash': 16,
    'QSet': 8, 'QContiguousCache': 8,
    'QList': 24, 'QVector': 24, 'QStack': 24, 'QQueue': 24,
    'QLinkedList': 24,
    'QVarLengthArray': 56, 'QCache': 72,
}

FUNC_OVERRIDE_RE = re.compile(
    r'(?:virtual\s+)?'
    r'(\w[\w\s:<>*&,()]*\w)'
    r'\s*\(([^)]*)\)'
    r'\s*(?:const\s+)?'
    r'\s*override\b'
)

# ── Singleton Migration Regexes ──────────────────────────────────

SINGLETON_INCLUDE_OLD = '#include "NXSingleton.h"'
SINGLETON_INCLUDE_NEW = '#include "LinnSingleton.h"'

SINGLETON_H_RE = re.compile(r'Q_SINGLETON_CREATE_H\s*\(\s*(\w+)\s*\)\s*;?')
SINGLETON_CREATE_RE = re.compile(r'Q_SINGLETON_CREATE\s*\(\s*(\w+)\s*\)\s*;?')
SINGLETON_CPP_RE = re.compile(r'Q_SINGLETON_CREATE_CPP\s*\(\s*(\w+)\s*\)\s*;?')
QQ_CREATE_RE = re.compile(r'Q_Q_CREATE\s*\(\s*(\w+)\s*\)')
Q_OBJECT_RE = re.compile(r'Q_OBJECT\b')


# ═══════════════════════════════════════════════════════════════════
# Core Utilities
# ═══════════════════════════════════════════════════════════════════

def _is_pointer_type(ts: str) -> bool:
    """Check if the type is a pointer or reference (* or & outside angle brackets)."""
    depth = 0
    for c in ts:
        if c == '<':
            depth += 1
        elif c == '>':
            depth -= 1
        elif c in ('*', '&') and depth == 0:
            return True
    return False


def get_type_size(type_str: str) -> int:
    ts = type_str.strip()
    if _is_pointer_type(ts):
        return 8
    if ts in TYPE_SIZES:
        return TYPE_SIZES[ts]
    if '<' in ts:
        base = ts.split('<')[0].strip()
        if base in TEMPLATE_SIZES:
            return TEMPLATE_SIZES[base]
        if base in TYPE_SIZES:
            return TYPE_SIZES[base]
    if '::' in ts:
        return 4
    if ts.startswith('QS_SET_CREF(') or ts.startswith('Q_SETTER_CREF('):
        return 8
    return 999999


def needs_qs_set_cref(type_str: str) -> bool:
    base = type_str.split('<')[0].strip() if '<' in type_str else type_str
    base = base.rstrip('*& ')
    return base in QS_TYPES


def rename_macro(name: str) -> str:
    return name.replace('_REF_', '_').replace('_Q_', '_')


def find_matching_paren(text: str, start: int, open_c='(', close_c=')') -> int:
    depth = 1
    i = start
    while i < len(text) and depth > 0:
        c = text[i]
        if c == open_c:
            depth += 1
        elif c == close_c:
            depth -= 1
        elif c in ('"', "'"):
            q = c
            i += 1
            while i < len(text) and text[i] != q:
                if text[i] == '\\':
                    i += 1
                i += 1
        i += 1
    return i if depth == 0 else -1


def split_args(content: str):
    args = []
    depth_paren = 0
    depth_angle = 0
    cur = []
    for c in content:
        if c == '(':
            depth_paren += 1
            cur.append(c)
        elif c == ')':
            depth_paren -= 1
            cur.append(c)
        elif c == '<':
            depth_angle += 1
            cur.append(c)
        elif c == '>':
            depth_angle -= 1
            cur.append(c)
        elif c == ',' and depth_paren == 0 and depth_angle == 0:
            args.append(''.join(cur).strip())
            cur = []
        else:
            cur.append(c)
    if cur:
        args.append(''.join(cur).strip())
    return args


def build_macro_line(name: str, args: list) -> str:
    text = f'{name}({", ".join(args)})'
    return text + '\n'


# ═══════════════════════════════════════════════════════════════════
# Macro Line Parsing
# ═══════════════════════════════════════════════════════════════════

class MacroInfo:
    __slots__ = ('line_idx', 'orig_name', 'args', 'has_semi',
                 'type_arg', 'name_arg', 'size', 'sort_size')
    def __init__(self, line_idx, orig_name, args, has_semi):
        self.line_idx = line_idx
        self.orig_name = orig_name
        self.args = args
        self.has_semi = has_semi
        self.type_arg = ''
        self.name_arg = ''
        self.size = 0
        self.sort_size = 0


def parse_macro_line(line: str):
    stripped = line.strip()
    if not stripped:
        return None
    has_semi = stripped.rstrip().endswith(';')
    clean = stripped.rstrip().rstrip(';').strip()
    for macro in RECOGNIZED_MACROS:
        idx = clean.find(macro)
        if idx < 0:
            continue
        rest = clean[idx + len(macro):].strip()
        if not rest.startswith('('):
            continue
        paren_end = find_matching_paren(clean, idx + len(macro) + 1)
        if paren_end < 0:
            continue
        args_content = clean[idx + len(macro) + 1:paren_end - 1]
        args = split_args(args_content)
        return (macro, args, has_semi)
    return None


def get_type_from_macro(name: str, args: list) -> str:
    is_cpp = name in CPP_MACROS
    if is_cpp and len(args) >= 2:
        return args[1].strip()
    if not is_cpp and len(args) >= 1:
        return args[0].strip()
    return ''


# ═══════════════════════════════════════════════════════════════════
# Phase 0: Override Collection
# ═══════════════════════════════════════════════════════════════════

def collect_overrides_from_text(text: str) -> set:
    overrides = set()
    for m in FUNC_OVERRIDE_RE.finditer(text):
        full = m.group(1).strip()
        name = full.split()[-1] if ' ' in full else full
        name = name.split('::')[-1] if '::' in name else name
        if name and not name.startswith('~'):
            overrides.add(name)
    return overrides


# ═══════════════════════════════════════════════════════════════════
# Phase 1: Functions 1-5 (Header Processing)
# ═══════════════════════════════════════════════════════════════════

def process_header_phase1(lines: list) -> list:
    result = list(lines)
    classes = _find_class_ranges(lines)
    for cls_start, cls_end in reversed(classes):
        _process_single_class(result, cls_start, cls_end)
    return result


def _find_class_ranges(lines: list) -> list:
    ranges = []
    brace_depth = 0
    cls_start = -1
    for i, line in enumerate(lines):
        s = line.strip()
        if cls_start < 0 and re.match(r'\b(class|struct)\b', s):
            # Skip forward declarations (end with ;, no body)
            if s.rstrip().endswith(';'):
                continue
            cls_start = i
            brace_depth = s.count('{') - s.count('}')
            continue
        if cls_start >= 0:
            brace_depth += s.count('{') - s.count('}')
            if brace_depth <= 0:
                ranges.append((cls_start, i))
                cls_start = -1
    return ranges


def _process_single_class(lines: list, cls_start: int, cls_end: int):
    macros = []
    macro_region_end = cls_start
    non_macro_markers = []

    i = cls_start
    while i <= cls_end:
        s = lines[i].strip()
        # Detect nested struct/class — skip its body and process recursively
        if i > cls_start and re.match(r'(struct|class)\b', s) and not s.rstrip().endswith(';'):
            nest_start = i
            brace_depth_nest = s.count('{') - s.count('}')
            j = i + 1
            while j <= cls_end and brace_depth_nest > 0:
                brace_depth_nest += lines[j].count('{') - lines[j].count('}')
                j += 1
            nest_end = j - 1
            _process_single_class(lines, nest_start, nest_end)
            i = nest_end + 1
            continue

        parsed = parse_macro_line(lines[i])
        if parsed:
            name, args, has_semi = parsed
            mi = MacroInfo(i, name, args, has_semi)
            mi.type_arg = get_type_from_macro(name, args)
            mi.size = get_type_size(mi.type_arg)
            macros.append(mi)
            if i >= macro_region_end:
                macro_region_end = i
        elif s in ('public:', 'protected:', 'private:'):
            non_macro_markers.append(i)
        i += 1

    if not macros:
        return

    macro_region_end = macros[-1].line_idx

    # Determine if these are _H/_CPP macros (Func4) or F5 macros (Func5)
    is_h_cpp = False
    for mi in macros:
        new_name = rename_macro(mi.orig_name)
        if new_name.endswith('_H') or new_name.endswith('_CPP'):
            is_h_cpp = True
            break

    # Apply Func1, Func3 per macro line
    for mi in macros:
        orig_line = lines[mi.line_idx]
        indent = orig_line[:len(orig_line) - len(orig_line.lstrip())]
        new_name = rename_macro(mi.orig_name)
        is_d = mi.orig_name in D_MACROS
        new_type = mi.type_arg
        if not is_d and needs_qs_set_cref(mi.type_arg):
            new_type = f'QS_SET_CREF({mi.type_arg})'
        new_args = list(mi.args)
        is_cpp = mi.orig_name in CPP_MACROS
        if not is_d:
            if is_cpp and len(new_args) >= 2:
                new_args[1] = new_type
            elif not is_cpp and len(new_args) >= 1:
                new_args[0] = new_type
        result = indent + build_macro_line(new_name, new_args)
        lines[mi.line_idx] = result
        mi.type_arg = new_type
        mi.sort_size = mi.size
        mi.size = get_type_size(new_type)

    # Get original positional order
    orig_indices = sorted([mi.line_idx for mi in macros])
    orig_texts = [lines[i] for i in orig_indices]

    # All macros: sort descending (large→small) with same-type grouping
    macros.sort(key=lambda m: m.sort_size, reverse=True)
    # Group same-type items together within each size bucket
    grouped = []
    i = 0
    while i < len(macros):
        j = i
        while j < len(macros) and macros[j].size == macros[i].size:
            j += 1
        bucket = macros[i:j]
        seen = {}
        groups = []
        for m in bucket:
            tn = m.type_arg
            if tn not in seen:
                seen[tn] = len(groups)
                groups.append([])
            groups[seen[tn]].append(m)
        for g in groups:
            grouped.extend(g)
        i = j
    macros = grouped

    # Map each sorted macro back to its modified text
    text_map = {mi.line_idx: lines[mi.line_idx] for mi in macros}
    sorted_texts = [text_map[mi.line_idx] for mi in macros]

    # Reorder lines physically (macros are contiguous at class start)
    if orig_indices:
        base = orig_indices[0]
        for j, t in enumerate(sorted_texts):
            lines[base + j] = t
        # Clear original macro positions that are now outside the placed block
        for idx in orig_indices:
            if idx >= base + len(macros):
                lines[idx] = ''

    # Func5: Non-macro region sorting
    if not is_h_cpp and non_macro_markers:
        # Process each access region separately
        for acc_idx in non_macro_markers:
            region_start = acc_idx + 1
            region_end = cls_end
            # Find end of this region (next access specifier or class end)
            for next_acc in non_macro_markers:
                if next_acc > acc_idx:
                    region_end = next_acc - 1
                    break
            _sort_non_macro_region(lines, region_start, region_end)


def _sort_non_macro_region(lines: list, rstart: int, rend: int):
    if rstart > rend:
        return

    # Group by preprocessor blocks
    groups = []
    i = rstart
    while i <= rend:
        s = lines[i].strip()
        if s.startswith('#if') or s.startswith('#ifdef') or s.startswith('#ifndef'):
            pp_start = i
            depth = 1
            i += 1
            while i <= rend and depth > 0:
                si = lines[i].strip()
                if si.startswith('#if') or si.startswith('#ifdef') or si.startswith('#ifndef'):
                    depth += 1
                elif si.startswith('#endif'):
                    depth -= 1
                i += 1
            groups.append(('pp', pp_start, i - 1))
        elif not s or s.startswith(('//', '/*', '*')) or s in ('public:', 'protected:', 'private:'):
            groups.append(('raw', i, i))
            i += 1
        else:
            normal_start = i
            while i <= rend:
                si = lines[i].strip()
                if si.startswith('#if') or si.startswith('#ifdef') or si.startswith('#ifndef') or \
                   si in ('public:', 'protected:', 'private:'):
                    break
                i += 1
            groups.append(('normal', normal_start, i - 1))

    # Collect member vars, type definitions, and other lines
    member_entries = []  # [(size, output_lines)]
    type_def_lines = []  # struct/class/enum blocks, using/typedef/friend
    other_lines = []     # everything else (blanks, comments, functions, etc.)
    for gtype, gs, ge in groups:
        if gtype == 'normal':
            temp_sorted = _sort_member_lines(lines, gs, ge)
            # Track nested struct/class/enum blocks so their inner vars
            # are NOT extracted as outer class member variables
            in_nested = False
            nested_depth = 0
            for line_text in temp_sorted:
                s = line_text.strip()

                if in_nested:
                    nested_depth += line_text.count('{') - line_text.count('}')
                    type_def_lines.append(line_text)
                    if nested_depth <= 0:
                        in_nested = False
                    continue

                if re.match(r'(struct|class|enum)\s+\w+', s) and not (';' in s and '{' not in s):
                    in_nested = True
                    nested_depth = line_text.count('{') - line_text.count('}')
                    type_def_lines.append(line_text)
                    continue

                mv = _is_member_var(s)
                if mv and mv != 'skip':
                    m = re.match(
                        r'(mutable\s+)?'
                        r'(?:static\s+|const\s+|constexpr\s+|inline\s+|virtual\s+)*'
                        r'([\w:]+(?:\s*<[^>]*>)?(?:\s*[*&])?)',
                        s
                    )
                    if m:
                        type_s = m.group(2).strip() if m.group(2) else ''
                        if type_s:
                            member_entries.append((get_type_size(type_s), [line_text]))
                            continue
                # Non-member, non-type-def lines
                if s.startswith(('using ', 'typedef ', 'friend ')):
                    type_def_lines.append(line_text)
                else:
                    other_lines.append(line_text)
        elif gtype == 'pp':
            cond_line = lines[gs].strip() + '\n'
            if_body = []
            else_body = []
            cur = if_body
            for idx in range(gs + 1, ge):
                si = lines[idx].strip()
                if si.startswith('#else') or si.startswith('#elif'):
                    cur = else_body
                    continue
                if si.startswith('#endif'):
                    break
                cur.append(lines[idx])

            if_vars = _extract_pp_vars(if_body)
            else_vars = _extract_pp_vars(else_body)

            if not else_vars:
                for v in if_vars:
                    pp_lines = [cond_line, v['line'], '#endif\n']
                    member_entries.append((get_type_size(v['type']), pp_lines))
            else:
                else_by_name = {v['name']: v for v in else_vars}
                paired = []
                for v in if_vars:
                    paired.append((v, else_by_name.pop(v['name'], None)))
                for v in else_by_name.values():
                    paired.append((None, v))
                for if_v, else_v in paired:
                    lst = [cond_line]
                    if if_v:
                        lst.append(if_v['line'])
                    else:
                        lst.append('\n')
                    if else_v:
                        lst.append('#else\n')
                        lst.append(else_v['line'])
                    lst.append('#endif\n')
                    v = if_v or else_v
                    member_entries.append((get_type_size(v['type']), lst))
        else:
            for idx in range(gs, ge + 1):
                other_lines.append(lines[idx])

    # Sort all member entries by size (ascending)
    member_entries.sort(key=lambda x: x[0])

    # Output: type definitions → sorted member variables → remaining lines
    # This ensures struct/enum/using declarations appear before member vars that use them
    new_section = []
    new_section.extend(type_def_lines)
    for _, text_list in member_entries:
        new_section.extend(text_list)
    new_section.extend(other_lines)

    # Write back (handles expansion/contraction via slice assignment)
    lines[rstart:rend+1] = new_section


def _is_member_var(stripped: str):
    if not stripped or stripped.startswith(('//', '/*', '*', '#endif', '#else')):
        return False
    if '(' in stripped and ')' in stripped:
        return False
    if ';' not in stripped:
        return False
    # Check if it has variable modifier pattern (exclude functions)
    if re.match(r'.*\(.*\).*', stripped):
        return False  # Has parens = function
    m = re.match(
        r'(mutable\s+)?'
        r'(static\s+|const\s+|constexpr\s+|inline\s+|virtual\s+)*'
        r'([\w:]+(?:\s*<[^>]*>)?(?:\s*[*&])?)'
        r'\s+(\w+)'
        r'\s*(?:=\s*[^;]*|{[^}]*})?\s*;',
        stripped
    )
    if m:
        modifiers = (m.group(1) or '') + (m.group(2) or '')
        has_skip = any(kw in modifiers for kw in ('static ', 'const ', 'constexpr ', 'inline '))
        if has_skip:
            return 'skip'
        return True
    return False


def _sort_member_lines(lines: list, gs: int, ge: int, keep_pp=False) -> list:
    member_vars = []
    enum_ranges = []
    nested_structs = []
    prefix_lines = []  # Lines before first member var (friend, comments, etc.)
    other_lines = []   # Non-variable lines after first member var

    i = gs
    seen_member = False
    while i <= ge:
        s = lines[i].strip()
        if keep_pp and s.startswith('#'):
            (prefix_lines if not seen_member else other_lines).append(lines[i])
            i += 1
            continue
        if not s or s.startswith(('//', '/*', '*', '#endif', '#else', 'public:', 'protected:', 'private:')):
            (prefix_lines if not seen_member else other_lines).append(lines[i])
            i += 1
            continue

        # Nested enum (multi-line, block-style)
        if re.match(r'enum\s+\w+', s):
            enum_start = i
            enum_depth = 0
            if '{' in s:
                enum_depth = s.count('{') - s.count('}')
                i += 1
            else:
                i += 1
                while i <= ge and '{' not in lines[i]:
                    i += 1
                if i > ge:
                    continue
                enum_depth = 1
                i += 1
            while i <= ge and enum_depth > 0:
                enum_depth += lines[i].count('{') - lines[i].count('}')
                i += 1
            enum_ranges.append(list(range(enum_start, i)))
            continue

        # Nested struct/class
        if re.match(r'(struct|class)\s+\w+', s):
            struct_start = i
            struct_depth = 0
            if '{' in s:
                struct_depth = s.count('{') - s.count('}')
                i += 1
            else:
                i += 1
                while i <= ge and '{' not in lines[i]:
                    i += 1
                if i > ge:
                    continue
                struct_depth = 1
                i += 1
            while i <= ge and struct_depth > 0:
                struct_depth += lines[i].count('{') - lines[i].count('}')
                i += 1
            nested_structs.append(list(range(struct_start, i)))
            continue

        mv = _is_member_var(s)
        if mv and mv != 'skip':
            seen_member = True
            m = re.match(
                r'(mutable\s+)?'
                r'(?:static\s+|const\s+|constexpr\s+|inline\s+|virtual\s+)*'
                r'([\w:]+(?:\s*<[^>]*>)?(?:\s*[*&])?)'
                r'\s+(\w+)',
                s
            )
            if m:
                type_s = m.group(2).strip() if m.group(2) else ''
                if type_s:
                    member_vars.append((i, type_s, lines[i]))
        else:
            (prefix_lines if not seen_member else other_lines).append(lines[i])
        i += 1

    member_vars.sort(key=lambda x: get_type_size(x[1]))

    # Output order: enum → struct → prefix(using, friend) → member_vars → other
    result = []

    for enum_range in enum_ranges:
        for idx in enum_range:
            result.append(lines[idx])

    for struct_range in nested_structs:
        for idx in struct_range:
            result.append(lines[idx])

    result.extend(prefix_lines)

    for var_idx, _, text in member_vars:
        result.append(text)

    result.extend(other_lines)

    return result


def _extract_pp_vars(lines: list) -> list:
    """Extract member variable info from a list of lines in a pp branch."""
    vars_list = []
    for line in lines:
        s = line.strip()
        if not s or s.startswith(('//', '/*', '*', '#')):
            continue
        mv = _is_member_var(s)
        if mv and mv != 'skip':
            m = re.match(
                r'(mutable\s+)?'
                r'(?:static\s+|const\s+|constexpr\s+|inline\s+|virtual\s+)*'
                r'([\w:]+(?:\s*<[^>]*>)?(?:\s*[*&])?)'
                r'\s+(\w+)',
                s
            )
            if m:
                type_s = m.group(2).strip() if m.group(2) else ''
                name_s = m.group(3).strip() if m.group(3) else ''
                if type_s and name_s:
                    vars_list.append({
                        'name': name_s,
                        'type': type_s,
                        'line': line,
                    })
    return vars_list


def _process_pp_group(lines: list, gs: int, ge: int) -> list:
    """Process a #if/#else/#endif block: split into individual wrapped vars, sort by size."""
    cond_line = lines[gs].strip() + '\n'
    if_body = []
    else_body = []
    cur = if_body
    for i in range(gs + 1, ge):
        s = lines[i].strip()
        if s.startswith('#else') or s.startswith('#elif'):
            cur = else_body
            continue
        if s.startswith('#endif'):
            break
        cur.append(lines[i])

    if_vars = _extract_pp_vars(if_body)
    else_vars = _extract_pp_vars(else_body)

    if not else_vars:
        # Simple #ifndef X / #endif
        result = []
        for v in if_vars:
            result.append(cond_line)
            result.append(v['line'])
            result.append('#endif\n')
        return result

    # Pair by variable name
    else_by_name = {v['name']: v for v in else_vars}
    paired = []
    for v in if_vars:
        paired.append((v, else_by_name.pop(v['name'], None)))
    for v in else_by_name.values():
        paired.append((None, v))

    # Sort by type size (ascending)
    def get_pair_size(pair):
        v = pair[0] or pair[1]
        return get_type_size(v['type'])
    paired.sort(key=get_pair_size)

    result = []
    for if_v, else_v in paired:
        result.append(cond_line)
        if if_v:
            result.append(if_v['line'])
        else:
            result.append('\n')
        if else_v:
            result.append('#else\n')
            result.append(else_v['line'])
        result.append('#endif\n')

    return result


# ═══════════════════════════════════════════════════════════════════
# Phase 1: Source processing (CPP macros, Func4)
# ═══════════════════════════════════════════════════════════════════

def process_source_phase1(lines: list) -> list:
    result = list(lines)
    macros = []
    for i, line in enumerate(lines):
        parsed = parse_macro_line(line)
        if not parsed:
            continue
        name, args, has_semi = parsed
        new_name = rename_macro(name)
        if not new_name.endswith('_CPP'):
            continue
        mi = MacroInfo(i, name, args, has_semi)
        mi.type_arg = get_type_from_macro(name, args)
        mi.size = get_type_size(mi.type_arg)
        macros.append(mi)

    if not macros:
        return result

    # Apply Func1, Func3
    for mi in macros:
        orig_line = lines[mi.line_idx]
        indent = orig_line[:len(orig_line) - len(orig_line.lstrip())]
        new_name = rename_macro(mi.orig_name)
        is_d = mi.orig_name in D_MACROS
        new_type = mi.type_arg
        if not is_d and needs_qs_set_cref(mi.type_arg):
            new_type = f'QS_SET_CREF({mi.type_arg})'
        new_args = list(mi.args)
        if not is_d and mi.orig_name in CPP_MACROS and len(new_args) >= 2:
            new_args[1] = new_type
        result[mi.line_idx] = indent + build_macro_line(new_name, new_args)
        mi.type_arg = new_type
        mi.sort_size = mi.size
        mi.size = get_type_size(new_type)

    # Sort descending (large→small) with same-type grouping
    macros.sort(key=lambda m: m.sort_size, reverse=True)
    # Group same-type items together within each size bucket
    grouped = []
    i = 0
    while i < len(macros):
        j = i
        while j < len(macros) and macros[j].size == macros[i].size:
            j += 1
        bucket = macros[i:j]
        # Stable group by type_arg
        seen = {}
        groups = []
        for m in bucket:
            tn = m.type_arg
            if tn not in seen:
                seen[tn] = len(groups)
                groups.append([])
            groups[seen[tn]].append(m)
        for g in groups:
            grouped.extend(g)
        i = j
    macros = grouped

    texts = [result[mi.line_idx] for mi in macros]
    orig_indices = sorted([mi.line_idx for mi in macros])
    base = orig_indices[0]
    for j, t in enumerate(texts):
        result[base + j] = t
    for idx in orig_indices:
        if idx >= base + len(macros):
            result[idx] = ''

    return result


# ═══════════════════════════════════════════════════════════════════
# Phase 2: Function 6 - const& processing
# ═══════════════════════════════════════════════════════════════════

def process_func6_file(lines: list, global_overrides: set, filepath: str) -> list:
    result = list(lines)
    text = ''.join(lines)

    # Get file-specific override set
    p = Path(filepath)
    file_overrides = set()
    if p.suffix in SOURCE_EXTS:
        stem = p.stem
        # Check same directory for matching header
        for ext in HEADER_EXTS:
            hp = p.with_suffix(ext)
            if hp.exists():
                try:
                    ht = hp.read_text(encoding='utf-8')
                    file_overrides = collect_overrides_from_text(ht)
                except Exception:
                    pass
                break
        # Check private/ directory
        if not file_overrides:
            parent = p.parent
            private_candidates = list(parent.rglob(f'{stem}.h')) + list(parent.rglob(f'{stem}.hpp'))
            for hp in private_candidates:
                try:
                    ht = hp.read_text(encoding='utf-8')
                    file_overrides = collect_overrides_from_text(ht)
                    if file_overrides:
                        break
                except Exception:
                    continue

    local_overrides = file_overrides | global_overrides
    new_text = _process_text_const_ref(text, local_overrides)
    return new_text.splitlines(keepends=True)


def _process_text_const_ref(text: str, override_set: set) -> str:
    lines = text.splitlines(keepends=True)
    result = list(lines)

    # Merge lines into logical function units
    i = 0
    while i < len(lines):
        line = lines[i]
        s = line.strip()
        if not s or s.startswith(('#', '//', '/*', '*', '}', '{', ';', 'public:', 'protected:', 'private:')):
            i += 1
            continue

        func_info = _try_extract_func(lines, i)
        if func_info:
            fn_start, fn_end, func_name, params_text, return_type, has_override = func_info
            base = func_name.split('::')[-1]

            if base.startswith('~'):
                i = fn_end + 1
                continue

            if base not in override_set:
                new_params = _process_params_text(params_text)
                new_return = _process_return_text(return_type)

                if new_params != params_text or new_return != return_type:
                    # Rebuild
                    _patch_func(lines, fn_start, fn_end, params_text, new_params, return_type, new_return)

            i = fn_end + 1
        else:
            i += 1

    return ''.join(lines)


def _try_extract_func(lines: list, start: int):
    """Try to extract a function signature starting at start."""
    block = ''
    line_map = []
    i = start
    paren_depth = 0
    got_paren = False

    while i < len(lines):
        s = lines[i].strip()
        if s.startswith('//') or s.startswith('/*') or s.startswith('*'):
            i += 1
            continue

        # Stop scanning at blank lines, access specifiers, preprocessor,
        # or a new function/macro call, once a complete (...) pair has been found but no { or ;
        if got_paren and paren_depth == 0:
            if not s or s in ('public:', 'protected:', 'private:') or s.startswith('#'):
                break
            if s and not s.startswith(('//', '/*', '*', '{', ';', '}', ':')):
                paren_pos = -1
                for ci, c in enumerate(s):
                    if c == '(':
                        paren_pos = ci
                        break
                    elif c in (';', '{'):
                        break
                if paren_pos > 0:
                    before_paren = s[:paren_pos].rstrip()
                    if before_paren not in ('const', 'override', 'noexcept', 'final'):
                        break

        block += lines[i]
        line_map.append(i)

        for c in lines[i]:
            if c == '(':
                paren_depth += 1
                got_paren = True
            elif c == ')':
                paren_depth = max(0, paren_depth - 1)
            elif c == '{':
                if paren_depth == 0:
                    if not got_paren:
                        return None  # Not a function, reached body before params
                    break  # Body start after closing paren
                # If paren_depth > 0, '{' is inside parameter list (e.g., default value), ignore
            elif c == ';':
                if paren_depth == 0 and got_paren:
                    break

        if paren_depth == 0 and (got_paren and ('{' in lines[i] or ';' in lines[i])):
            break
        i += 1

    if i >= len(lines) and not (got_paren and paren_depth == 0):
        return None

    merged = block
    has_override = 'override' in merged
    has_body = '{' in merged
    is_decl = ';' in merged and not has_body

    # Find param list
    op = merged.find('(')
    if op < 0:
        return None
    cp = find_matching_paren(merged, op + 1)
    if cp < 0:
        return None

    params_text = merged[op + 1:cp - 1]

    # Extract function name and return type
    before = merged[:op].strip()
    if '\n' in before or ';' in before or '}' in before or '=' in before:
        return None
    # Remove qualifiers
    before = before.replace('override', '')
    before = before.replace('noexcept', '|NOSPEC|')
    # Remove trailing const
    before = re.sub(r'\bconst\s*$', '', before).strip()
    before = before.replace('|NOSPEC|', 'noexcept')

    parts = before.rsplit(None, 1)
    if len(parts) < 2:
        # Constructor/destructor with no return type: e.g. Foo::Foo(...) or ~Foo(...)
        func_name = before.strip()
        return_type = ''
    else:
        return_type = parts[0].strip()
        func_name = parts[1].strip()
        while func_name and func_name[0] in ('&', '*'):
            if return_type and return_type[-1] not in (' ', '&', '*'):
                return_type += ' '
            return_type += func_name[0]
            func_name = func_name[1:].strip()

    if not func_name:
        return None

    fn_end = line_map[-1] if line_map else start
    if has_body:
        # Find matching closing brace
        pass

    return (start, fn_end, func_name, params_text, return_type, has_override)


def _process_params_text(params: str) -> str:
    if not params.strip():
        return params
    params_list = split_args(params) if ',' in params else [params.strip()] if params.strip() else []
    new_params = []
    for p in params_list:
        p = p.strip()
        if not p:
            continue

        has_ref = '&' in p
        has_ptr = '*' in p
        is_ref_already = '&' in p.split()[-1] if p.split() else False

        # Check for const & pattern
        const_ref = re.match(r'\bconst\s+(.+?)\s*&(\s*\w+)?\s*(?:=\s*[^,]*)?$', p) or \
                    re.match(r'(.+?)\s+const\s+&(\s*\w+)?\s*(?:=\s*[^,]*)?$', p)

        if const_ref:
            type_part = const_ref.group(1).strip()
            rest = const_ref.group(2) or ''

            if '*' in type_part:
                new_params.append(p)
                continue

            base = type_part.split('<')[0].strip().rstrip('*& ')
            if base in TYPE_DIFF_LIST:
                # Remove const &
                new_p = type_part + ' ' + rest.strip()
                new_p = re.sub(r'\s+', ' ', new_p).strip()
                new_params.append(new_p)
            else:
                new_params.append(p)
        else:
            if has_ptr or is_ref_already:
                new_params.append(p)
                continue

            m = re.match(r'([\w:<>\s*]+)\s+(\w[\w]*)\s*(?:=\s*[^,]*)?$', p)
            if m:
                type_part = m.group(1).strip()
                name_part = m.group(2).strip()
                base = type_part.split('<')[0].strip().rstrip('*& ')
                if base in QS_TYPES and '&' not in type_part and '*' not in type_part:
                    new_p = f'const {type_part} &{name_part}'
                    new_params.append(new_p)
                else:
                    new_params.append(p)
            else:
                new_params.append(p)
    result = ', '.join(new_params)
    norm_orig = re.sub(r'\s+', ' ', params).strip()
    norm_new = re.sub(r'\s+', ' ', result).strip()
    if norm_orig == norm_new:
        return params
    return result


def _process_return_text(ret: str) -> str:
    r = ret.strip()
    # const TYPE &
    m = re.match(r'\bconst\s+(.+?)\s*&$', r)
    if m:
        return m.group(1).strip()
    # TYPE const &
    m = re.match(r'(.+?)\s+const\s+&$', r)
    if m:
        return m.group(1).strip()
    return r


def _patch_func(lines: list, fn_start: int, fn_end: int,
                old_params: str, new_params: str,
                old_ret: str, new_ret: str):
    """Apply changes to function signature in lines."""
    # Build the affected range text
    relevant = ''.join(lines[fn_start:fn_end + 1])

    if old_params != new_params:
        # Replace parameter list in parentheses
        relevant = relevant.replace(f'({old_params})', f'({new_params})', 1)

    if old_ret != new_ret:
        old_stripped = old_ret.strip()
        idx = relevant.find(old_stripped)
        if idx >= 0:
            end = idx + len(old_stripped)
            rest = relevant[end:]
            if rest and not rest[0].isspace():
                rest = ' ' + rest
            relevant = relevant[:idx] + new_ret + rest

    new_lines = relevant.splitlines(keepends=True)
    for j in range(min(len(new_lines), fn_end - fn_start + 1)):
        lines[fn_start + j] = new_lines[j]
    for j in range(fn_start + len(new_lines), fn_end + 1):
        lines[j] = '\n'


# ═══════════════════════════════════════════════════════════════════
# Phase 3: Singleton Migration (NXSingleton.h → LinnSingleton.h)
# ═══════════════════════════════════════════════════════════════════

def _find_outermost_last_include(lines):
    """Find the last #include at outermost preprocessing level."""
    depth = 0
    first_include_depth = -1
    last = -1
    for i, line in enumerate(lines):
        s = line.strip()
        s_clean = s.lstrip('\ufeff')
        if re.match(r'#\s*if', s_clean):
            depth += 1
        elif re.match(r'#\s*endif', s_clean):
            depth -= 1
        elif re.match(r'#\s*include', s_clean):
            if first_include_depth < 0:
                first_include_depth = depth
            if depth == first_include_depth:
                last = i
    return last


def _find_guard_endif_line(lines):
    """Find the header guard #endif (last outermost #endif)."""
    depth = 0
    guard = -1
    for i, line in enumerate(lines):
        s = line.strip()
        s_clean = s.lstrip('\ufeff')
        if re.match(r'#\s*if', s_clean):
            depth += 1
        elif re.match(r'#\s*endif', s_clean):
            depth -= 1
            if depth == 0:
                guard = i
    return guard


def _class_has_qq_create(lines, cls_start, cls_end):
    for i in range(cls_start, cls_end + 1):
        if QQ_CREATE_RE.search(lines[i]):
            return True
    return False


def _class_has_qobject(lines, cls_start, cls_end):
    for i in range(cls_start, cls_end + 1):
        if Q_OBJECT_RE.search(lines[i]):
            return True
    return False


def _find_singleton_insertion_line(lines, cls_start, cls_end, class_name):
    for i in range(cls_start, cls_end + 1):
        if QQ_CREATE_RE.search(lines[i]):
            return i
    for i in range(cls_start, cls_end + 1):
        if Q_OBJECT_RE.search(lines[i]):
            return i
    for i in range(cls_start, cls_end + 1):
        if '{' in lines[i]:
            return i
    return cls_start


def _add_push_pop_macros(lines):
    push_idx = _find_outermost_last_include(lines)
    pop_idx = _find_guard_endif_line(lines)

    if push_idx >= 0:
        to_insert = [
            '#pragma push_macro("Q_DISABLE_COPY")\n',
            '#undef Q_DISABLE_COPY\n',
            '#define Q_DISABLE_COPY(CLASS)\n',
        ]
        for j, l in enumerate(to_insert):
            lines.insert(push_idx + 1 + j, l)
        if pop_idx > push_idx:
            pop_idx += 3

    if pop_idx >= 0:
        lines.insert(pop_idx, '#pragma pop_macro("Q_DISABLE_COPY")\n')
    return lines


def _process_singleton_migration(lines, filepath):
    result = list(lines)
    fp = Path(filepath)
    is_header = fp.suffix in HEADER_EXTS
    is_source = fp.suffix in SOURCE_EXTS
    if not is_header and not is_source:
        return result

    has_singleton_include = False
    for i, line in enumerate(result):
        if '"NXSingleton.h"' in line:
            result[i] = line.replace('"NXSingleton.h"', '"LinnSingleton.h"')
            has_singleton_include = True

    if is_header:
        macro_groups = {}
        for i, line in enumerate(result):
            m = SINGLETON_H_RE.search(line)
            if m:
                cn = m.group(1)
                macro_groups.setdefault(cn, []).append((i, 'H'))
                continue
            m = SINGLETON_CREATE_RE.search(line)
            if m:
                cn = m.group(1)
                macro_groups.setdefault(cn, []).append((i, 'CREATE'))

        if not macro_groups:
            return result

        class_ranges = _find_class_ranges(result)
        needs_push_pop = False

        for class_name, entries in sorted(
            macro_groups.items(),
            key=lambda x: min(idx for idx, _ in x[1]),
            reverse=True
        ):
            macro_indices = [idx for idx, _ in entries]
            min_macro = min(macro_indices)
            max_macro = max(macro_indices)

            own_range = None
            for cr in class_ranges:
                if cr[0] <= min_macro <= cr[1] and cr[0] <= max_macro <= cr[1]:
                    own_range = cr
                    break
            if not own_range:
                continue

            old_indent = result[macro_indices[0]][:len(result[macro_indices[0]]) - len(result[macro_indices[0]].lstrip())]

            for idx in sorted(macro_indices, reverse=True):
                result[idx] = ''

            if _class_has_qq_create(result, own_range[0], own_range[1]):
                needs_push_pop = True

            ins = _find_singleton_insertion_line(result, own_range[0], own_range[1], class_name)

            new_line = f'{old_indent}Q_SINGLETON_CREATE(QS_S_UNIQUE({class_name}))\n'
            result.insert(ins + 1, new_line)

            for j in range(len(class_ranges)):
                if class_ranges[j][0] > ins:
                    class_ranges[j] = (class_ranges[j][0] + 1, class_ranges[j][1] + 1)

        if has_singleton_include and needs_push_pop:
            _add_push_pop_macros(result)

    if is_source:
        for i in range(len(result) - 1, -1, -1):
            if SINGLETON_CPP_RE.search(result[i]):
                result[i] = ''

    return result


# ═══════════════════════════════════════════════════════════════════
# Main
# ═══════════════════════════════════════════════════════════════════

def collect_files(paths, exts):
    files = []
    for ps in paths:
        p = Path(ps)
        if p.is_file() and p.suffix in exts:
            files.append(p)
        elif p.is_dir():
            for ext in exts:
                files.extend(p.rglob(f'*{ext}'))
    return files


def process_file(fp: Path, override_set: set, dry_run: bool) -> int:
    ext = fp.suffix
    is_header = ext in HEADER_EXTS
    is_source = ext in SOURCE_EXTS
    if not is_header and not is_source:
        return 0

    try:
        original = fp.read_text(encoding='utf-8')
    except Exception as e:
        print(f'  Error reading {fp}: {e}')
        return 0

    lines = original.splitlines(keepends=True)

    # Phase 1
    if is_header:
        lines = process_header_phase1(lines)
    if is_source:
        lines = process_source_phase1(lines)

    # Phase 2
    lines = process_func6_file(lines, override_set, str(fp))

    # Phase 3: Singleton migration
    lines = _process_singleton_migration(lines, str(fp))

    new_text = ''.join(lines)
    if new_text == original:
        return 0

    if dry_run:
        print(f'  Would modify: {fp}')
    else:
        fp.write_text(new_text, encoding='utf-8')
        print(f'  Modified: {fp.relative_to(fp.anchor) if fp.is_absolute() else fp}')
    return 1


def main():
    parser = argparse.ArgumentParser(description='ElaWidgetTools Code Transform')
    parser.add_argument('paths', nargs='+', help='Files or directories to process')
    parser.add_argument('--dry-run', action='store_true', help='Preview only')
    args = parser.parse_args()

    all_files = collect_files(args.paths, HEADER_EXTS | SOURCE_EXTS)
    if not all_files:
        print('No matching files found.')
        return

    print(f'Found {len(all_files)} files.')

    # Phase 0: Override collection
    print('Phase 0: Collecting override functions...')
    override_set = set()
    for fp in all_files:
        if fp.suffix in HEADER_EXTS:
            try:
                text = fp.read_text(encoding='utf-8')
                override_set |= collect_overrides_from_text(text)
            except Exception:
                continue
    print(f'  Found {len(override_set)} override names.')

    # Process
    print('Processing files (Phase 1+2+3)...')
    mod = 0
    for fp in all_files:
        mod += process_file(fp, override_set, args.dry_run)

    print(f'\nDone. {mod} files modified.')


if __name__ == '__main__':
    main()
