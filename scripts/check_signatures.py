#!/usr/bin/env python3
"""Validate cross-file signature consistency between .h and .cpp files."""

import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from code_transform import (
    _try_extract_func, find_matching_paren, split_args
)

CXX_KEYWORDS = {
    'alignas','alignof','and','and_eq','asm','auto','bitand','bitor','bool','break',
    'case','catch','char','class','compl','const','constexpr','continue','decltype',
    'default','delete','do','double','else','enum','explicit','export','extern',
    'false','float','for','friend','goto','if','inline','int','long','mutable',
    'namespace','new','noexcept','not','not_eq','nullptr','operator','or','or_eq',
    'private','protected','public','register','reinterpret_cast','return','short',
    'signed','sizeof','static','static_cast','struct','switch','template','this',
    'throw','true','try','typedef','typeid','typename','union','unsigned','using',
    'virtual','void','volatile','while','xor','xor_eq', 'override', 'final',
    'Q_EMIT', 'Q_SIGNAL', 'Q_SLOT', 'Q_INVOKABLE',
}


def is_valid_func_name(name: str) -> bool:
    name = name.strip()
    if not name:
        return False
    if name in CXX_KEYWORDS:
        return False
    if not re.match(r'^[a-zA-Z_][a-zA-Z0-9_]*(::[a-zA-Z_][a-zA-Z0-9_]*)*$', name):
        return False
    return True


def normalize_type_string(t: str) -> str:
    """Normalize a type string: collapse whitespace, normalize ptr/ref spacing."""
    t = t.strip()
    t = re.sub(r'\s+', ' ', t)
    # Only remove space before * or & when it's the token immediately before
    t = re.sub(r' (\*|&)', r'\1', t)
    # Remove space after * or & when followed by word character
    t = re.sub(r'(\*|&) ', r'\1', t)
    t = re.sub(r'<\s*', '<', t)
    t = re.sub(r'\s*>', '>', t)
    t = re.sub(r'\s*,\s*', ', ', t)
    # Re-add space after * or & if it's between two tokens (e.g., int *a -> int*a -> should stay int*a or int *a)
    # Actually for types like `int*` vs `int *` we want them to match
    # For `QPainter *event`, after stripping name → `QPainter*`
    return t


def clean_return_type(raw: str) -> str:
    ret = raw.strip()
    # Remove leading qualifiers
    for q in ('virtual', 'static', 'explicit', 'Q_SIGNAL', 'Q_SLOT', 'Q_INVOKABLE'):
        ret = re.sub(r'^' + re.escape(q) + r'\s+', '', ret)
        ret = re.sub(r'\s+' + re.escape(q) + r'\s+', ' ', ret)
    ret = ret.strip()
    # Remove trailing const
    ret = re.sub(r'\s*\bconst\s*$', '', ret).strip()
    # Remove trailing override
    ret = re.sub(r'\s*\boverride\s*$', '', ret).strip()
    return normalize_type_string(ret)


TYPE_LAST_WORDS = {
    'const', 'int', 'char', 'void', 'bool', 'double', 'float', 'long', 'short',
    'unsigned', 'signed', 'size_t', 'wchar_t', 'char16_t', 'char32_t',
    'int8_t', 'int16_t', 'int32_t', 'int64_t',
    'uint8_t', 'uint16_t', 'uint32_t', 'uint64_t',
    'qint8', 'qint16', 'qint32', 'qint64',
    'quint8', 'quint16', 'quint32', 'quint64',
    'qreal', 'quintptr', 'qintptr',
    'QString', 'QByteArray', 'QVariant', 'QStringList',
    'QModelIndex', 'QPersistentModelIndex', 'QColor',
    'QDateTime', 'QDate', 'QTime', 'QUrl', 'QRect', 'QRectF',
    'QSize', 'QSizeF', 'QPoint', 'QPointF', 'QLine', 'QLineF',
    'QIcon', 'QPixmap', 'QImage', 'QBrush', 'QPen', 'QPalette',
    'QFont', 'QCursor', 'QBitmap', 'QTransform', 'QRegion',
    'QKeySequence', 'QJsonValue', 'QJsonObject', 'QJsonArray', 'QJsonDocument',
    'QEasingCurve', 'QRegularExpression', 'QLocale',
}


def normalize_param_type(ptype: str) -> str:
    ptype = ptype.strip()
    ptype = ptype.split('=')[0].strip() if '=' in ptype else ptype
    # Split into tokens on whitespace
    tokens = ptype.split()
    if not tokens:
        return ''
    # The last token may be param_name or param_name with ptr/ref prefix
    last = tokens[-1]
    # Split last token into ptr/ref prefix + name
    m = re.match(r'^([*&]+)?([a-zA-Z_]\w*)$', last)
    if m:
        ptr_ref = m.group(1) or ''
        name = m.group(2)
        if name not in TYPE_LAST_WORDS:
            # It's a parameter name, strip last token (but keep ptr_ref if separated)
            if ptr_ref:
                # Last token was e.g. "*name", replace with just "* "
                tokens[-1] = ptr_ref
            else:
                tokens = tokens[:-1]
            if not tokens:
                return normalize_type_string(ptr_ref) if ptr_ref else ''
    return normalize_type_string(' '.join(tokens))


def normalize_params_text(params_text: str) -> str:
    if not params_text.strip() or params_text.strip() == 'void':
        return ''
    has_comma = ',' in params_text
    if has_comma:
        params_list = split_args(params_text)
    else:
        params_list = [params_text.strip()] if params_text.strip() else []
    normalized = []
    for p in params_list:
        p = p.strip()
        if not p:
            continue
        p = normalize_param_type(p)
        if p:
            normalized.append(p)
    return normalize_type_string(', '.join(normalized))


def canonical_form_return(ret: str, func_name: str, params_text: str) -> str:
    name = func_name.split('::')[-1]
    cr = clean_return_type(ret)
    cp = normalize_params_text(params_text)
    return f"{cr}|{name}({cp})"


def extract_functions(filepath: str):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            lines = f.readlines()
    except UnicodeDecodeError:
        try:
            with open(filepath, 'r', encoding='utf-8-sig') as f:
                lines = f.readlines()
        except UnicodeDecodeError:
            with open(filepath, 'r', encoding='latin-1') as f:
                lines = f.readlines()
    funcs = []
    i = 0
    while i < len(lines):
        s = lines[i].strip()
        if not s or s.startswith(('#', '//', '/*', '*', '}', '{', ';', 'public:', 'protected:', 'private:')):
            i += 1
            continue
        result = _try_extract_func(lines, i)
        if result:
            fn_start, fn_end, func_name, params_text, return_type, has_override = result
            base_name = func_name.split('::')[-1]
            # Filter: valid identifier
            if not is_valid_func_name(base_name):
                i = fn_end + 1
                continue
            if base_name in CXX_KEYWORDS:
                i = fn_end + 1
                continue
            # Filter: return type must be clean (no braces, no newlines)
            raw_ret = return_type.strip()
            if '\n' in raw_ret or '{' in raw_ret or '}' in raw_ret:
                i = fn_end + 1
                continue
            # Filter: skip Q_EMIT calls / Q_SIGNALS
            if raw_ret.startswith('Q_EMIT') or raw_ret.startswith('Q_SIGNALS:'):
                i = fn_end + 1
                continue
            # Filter: return type containing = or new or ; (function call mistaken as definition)
            if '=' in raw_ret or 'new ' in raw_ret or ';' in raw_ret:
                i = fn_end + 1
                continue
            # Filter: constructor delegation (return type starts with :)
            if raw_ret.startswith(':'):
                i = fn_end + 1
                continue
            # Filter: first word of return type must be a valid C++ type (not a statement keyword)
            cleaned = clean_return_type(raw_ret)
            if not cleaned:
                i = fn_end + 1
                continue
            first_word = cleaned.split()[0] if cleaned.split() else ''
            if first_word in ('return', 'if', 'while', 'for', 'switch', 'case', 'break', 'delete'):
                i = fn_end + 1
                continue
            # Filter: first word shouldn't start with digit
            if first_word and first_word[0].isdigit():
                i = fn_end + 1
                continue
            cf = canonical_form_return(raw_ret, func_name, params_text)
            funcs.append({
                'base_name': base_name,
                'canonical': cf,
                'file': filepath,
                'line': fn_start + 1,
                'raw_name': func_name,
            })
            i = fn_end + 1
        else:
            i += 1
    return funcs


def main():
    root = Path(r'D:\ElaWidgetTools\ElaWidgetTools')
    h_files = {}
    cpp_files = {}
    for h in root.rglob('*.h'):
        stem = h.stem
        h_files.setdefault(stem, []).append(h)
    for cpp in root.rglob('*.cpp'):
        stem = cpp.stem
        cpp_files.setdefault(stem, []).append(cpp)
    all_stems = sorted(set(h_files.keys()) & set(cpp_files.keys()))
    mismatches = []
    for stem in all_stems:
        h_funcs = []
        for hf in h_files[stem]:
            h_funcs.extend(extract_functions(str(hf)))
        s_funcs = []
        for sf in cpp_files[stem]:
            s_funcs.extend(extract_functions(str(sf)))
        if not h_funcs or not s_funcs:
            continue
        h_by_name = {}
        for f in h_funcs:
            h_by_name.setdefault(f['base_name'], []).append(f)
        s_by_name = {}
        for f in s_funcs:
            s_by_name.setdefault(f['base_name'], []).append(f)
        for name in sorted(set(h_by_name.keys()) & set(s_by_name.keys())):
            h_set = {}
            for f in h_by_name[name]:
                h_set.setdefault(f['canonical'], []).append(f)
            s_set = {}
            for f in s_by_name[name]:
                s_set.setdefault(f['canonical'], []).append(f)
            h_cans = set(h_set.keys())
            s_cans = set(s_set.keys())
            if h_cans != s_cans:
                only_h = h_cans - s_cans
                only_s = s_cans - h_cans
                mismatches.append((stem, name, only_h, only_s, h_set, s_set))
    if not mismatches:
        print("All signatures match")
        return
    print(f"Found {len(mismatches)} mismatches:")
    for stem, name, only_h, only_s, h_set, s_set in mismatches:
        print(f"\n  [{stem}] '{name}':")
        if only_h:
            for cf in sorted(only_h):
                locs = [f"{Path(f['file']).name}:{f['line']}" for f in h_set[cf]]
                print(f"    .h: {cf}  ({'; '.join(locs)})")
        if only_s:
            for cf in sorted(only_s):
                locs = [f"{Path(f['file']).name}:{f['line']}" for f in s_set[cf]]
                print(f"    .cpp: {cf}  ({'; '.join(locs)})")


if __name__ == '__main__':
    main()
