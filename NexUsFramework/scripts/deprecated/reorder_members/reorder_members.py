#!/usr/bin/env python3
"""
C++ Header Member Variable Reorder Tool
========================================
Optimizes class layout by reordering member variables to reduce padding/size.
Targets class sizes by ensuring types < 8 bytes are grouped adjacently and
same types are grouped together.

Macro patterns handled:
  Q_PROPERTY_CREATE_D, Q_PROPERTY_CREATE, Q_PRIVATE_CREATE_D, Q_PRIVATE_CREATE

Operations:
  1. Reorder members for optimal padding (types < 8 bytes adjacent, same types together)
  2. Remove extra semicolons after macro declarations
"""

import re
import os
import sys
import argparse
from typing import List, Dict, Tuple, Optional, Any

# =============================================================================
# Extensible Type Size Database (64-bit MSVC)
# =============================================================================
TYPE_SIZES: Dict[str, int] = {
    # 1 byte
    'bool': 1, 'char': 1, 'signed char': 1, 'unsigned char': 1,
    'int8_t': 1, 'uint8_t': 1, 'qint8': 1, 'quint8': 1, 'char8_t': 1,

    # 2 bytes
    'short': 2, 'unsigned short': 2, 'wchar_t': 2, 'char16_t': 2,
    'int16_t': 2, 'uint16_t': 2, 'qint16': 2, 'quint16': 2,

    # 4 bytes
    'int': 4, 'unsigned int': 4, 'long': 4, 'unsigned long': 4,
    'int32_t': 4, 'uint32_t': 4, 'qint32': 4, 'quint32': 4,
    'float': 4, 'QTime': 4,
}

# Member-defining macro names (longer patterns first to avoid partial matches)
_MACRO_NAMES = [
    'Q_PROPERTY_CREATE_D',
    'Q_PROPERTY_CREATE',
    'Q_PRIVATE_CREATE_D',
    'Q_PRIVATE_CREATE',
]
_MACRO_NAME_RE = re.compile(r'(' + '|'.join(_MACRO_NAMES) + r')\s*\(')


def _find_matching_paren(s: str, start: int) -> int:
    depth = 0
    for i in range(start, len(s)):
        if s[i] == '(':
            depth += 1
        elif s[i] == ')':
            depth -= 1
            if depth == 0:
                return i
    return -1


def _split_args(s: str) -> List[str]:
    """Split by top-level commas (not inside < > or ( ))."""
    parts = []
    depth = 0
    current = []
    for ch in s:
        if ch in '<(':
            depth += 1
        elif ch in '>)':
            depth -= 1
        if ch == ',' and depth == 0:
            parts.append(''.join(current).strip())
            current = []
        else:
            current.append(ch)
    parts.append(''.join(current).strip())
    return parts


def parse_macro_call(line: str) -> Optional[Tuple[str, str, str]]:
    """Parse a macro call. Returns (macro_name, type, name) or None."""
    m = _MACRO_NAME_RE.match(line)
    if not m:
        return None
    macro_name = m.group(1)
    paren_start = m.end() - 1
    paren_end = _find_matching_paren(line, paren_start)
    if paren_end == -1:
        return None
    args_str = line[paren_start + 1:paren_end]
    args = _split_args(args_str)
    if len(args) < 2:
        return None
    return (macro_name, args[0], args[1])


def normalize_type(t: str) -> str:
    """Normalize type string for comparison."""
    t = t.strip()
    t = re.sub(r'^const\s+', '', t)
    if t.endswith('&'):
        t = t[:-1].strip()
    t = re.sub(r'\s+', ' ', t)
    return t


def get_type_size(type_str: str, extra_sizes: Optional[Dict[str, int]] = None) -> int:
    """Determine sizeof for a C++ type."""
    t = type_str.strip()
    sizes = dict(TYPE_SIZES)
    if extra_sizes:
        sizes.update(extra_sizes)

    if t.endswith('*'):
        return 8
    if t.endswith('&'):
        return get_type_size(t[:-1].strip(), extra_sizes)

    t = re.sub(r'^const\s+', '', t).strip()

    if t in sizes:
        return sizes[t]

    base = re.sub(r'<.*', '', t).strip()
    if base in sizes:
        return sizes[base]

    has_template = '<' in t
    if has_template:
        return 8

    if '::' in t:
        last = t.rsplit('::', 1)[-1].strip()
        if last in sizes:
            return sizes[last]
        return 4

    return 8


# =============================================================================
# MemberVar
# =============================================================================

class MemberVar:
    __slots__ = ('raw_line', 'var_type', 'display_type', 'var_name', 'display_name',
                 'is_macro', 'macro_name', 'original_indent', 'has_semicolon',
                 'line_number', 'size', '_original_index')

    def __init__(self, raw_line: str, var_type: str, var_name: str,
                 is_macro: bool = False, macro_name: str = '',
                 original_indent: str = '', has_semicolon: bool = False,
                 line_number: int = 0, original_index: int = 0,
                 display_type: str = '', display_name: str = ''):
        self.raw_line = raw_line
        self.var_type = var_type
        self.display_type = display_type if display_type else var_type
        self.var_name = var_name
        self.display_name = display_name if display_name else var_name
        self.is_macro = is_macro
        self.macro_name = macro_name
        self.original_indent = original_indent
        self.has_semicolon = has_semicolon
        self.line_number = line_number
        self.size = 0
        self._original_index = original_index

    def compute_size(self, extra_sizes=None):
        self.size = get_type_size(self.var_type, extra_sizes)
        return self.size

    def get_output(self, indent: str = '    ') -> str:
        if not self.is_macro:
            return indent + self.raw_line.strip()
        macro = self.macro_name
        dt = self.display_type
        dn = self.display_name
        # All macros output in their original form; no conversion performed.
        return f'{indent}{macro}({dt}, {dn})'

    def __repr__(self):
        return f'MV({self.var_type} {self.var_name}, macro={self.macro_name}, size={self.size})'


# =============================================================================
# Parsing
# =============================================================================

def is_macro_member(stripped: str) -> bool:
    return bool(_MACRO_NAME_RE.match(stripped))


def parse_regular_member(line: str, line_idx: int) -> Optional[MemberVar]:
    """
    Parse a non-macro member variable like:
      int _value;
      bool _flag{false};
      QWidget* _widget{nullptr};
      ElaThemeType::ThemeMode _themeMode;
      QMap<QString, QWidget*> _routeMap;
    """
    stripped = line.strip()
    if not stripped:
        return None

    # Skip lines that start with C++ keywords (return statements, control flow, etc.)
    _CPP_KEYWORDS = (
        r'return\b|if\b|for\b|while\b|switch\b|case\b|goto\b|continue\b|break\b'
        r'|throw\b|delete\b|new\b|sizeof\b|catch\b|try\b|else\b|do\b|default\b'
        r'|co_await\b|co_return\b|co_yield\b'
    )
    if re.match(_CPP_KEYWORDS, stripped):
        return None

    # Skip non-member lines
    if stripped.startswith('//') or stripped.startswith('/*') or stripped.startswith('*'):
        return None
    if stripped.startswith('#'):
        return None
    if stripped.startswith('friend '):
        return None
    if re.match(r'^(public|private|protected|Q_OBJECT|Q_DECLARE'
                r'|Q_SIGNALS|Q_SLOT|Q_INVOKABLE|signals|slots)\b', stripped):
        return None
    if re.match(r'Q_D_CREATE\s*\(', stripped):
        return None
    if re.match(r'^(explicit|virtual|static|typedef|using|enum\s|class\s|struct\s'
                r'|template|namespace)\b', stripped):
        return None

    # Check for function declarations
    if re.search(r'\(.*\)\s*(?:const\s*)?\s*(?:override\s*)?\s*[;{]?\s*$', stripped):
        return None

    # Try to match: TYPE NAME [initializer];
    # Name: _camelCase starting with underscore
    var_re = re.compile(
        r'^(.*?)\s+'                          # type (non-greedy)
        r'(_[a-zA-Z_]\w*)'                    # variable name
        r'(\s*[=({].*?)?'                     # optional initializer
        r'\s*;?\s*$'                          # optional semicolon
    )
    m = var_re.match(stripped)
    if not m:
        # Try simpler pattern
        m2 = re.search(r'\b(_[a-zA-Z_]\w*)\s*(?:[=({]|$)', stripped)
        if not m2:
            return None
        var_name = m2.group(1)
        name_start = m2.start(1)
        type_str = stripped[:name_start].rstrip()
        if not type_str:
            return None
        # Validate type_str isn't a keyword
        if type_str in ('return', 'delete', 'new', 'sizeof', 'throw', 'case', 'goto'):
            return None
        if re.search(r'\)\s*$', type_str):
            return None
    else:
        type_str = m.group(1).strip()
        var_name = m.group(2)
        if not type_str:
            return None

    indent = line[:len(line) - len(line.lstrip())]
    has_semi = stripped.rstrip().endswith(';')

    return MemberVar(
        raw_line=line.rstrip(),
        var_type=normalize_type(type_str),
        var_name=var_name,
        display_type=type_str,
        display_name=var_name,
        is_macro=False,
        original_indent=indent,
        has_semicolon=has_semi,
        line_number=line_idx,
        original_index=line_idx,
    )


# =============================================================================
# Reordering Algorithm
# =============================================================================

# =============================================================================
# Core class processing
# =============================================================================

def _find_type_def_end(body_lines: List[str], start_idx: int) -> int:
    """If body_lines[start_idx] starts a struct/enum definition, find the closing '};' line."""
    depth = 0
    in_block = False
    for i in range(start_idx, len(body_lines)):
        line = body_lines[i]
        for ch in line:
            if ch == '{':
                depth += 1
                in_block = True
            elif ch == '}':
                depth -= 1
                if depth == 0 and in_block:
                    return i
    return -1


def _find_classes(content: str) -> List[Dict]:
    """Find all class/struct definitions with bodies."""
    lines = content.split('\n')
    classes = []
    i = 0
    while i < len(lines):
        stripped = lines[i].strip()
        cm = re.match(r'^(?:class|struct)\s+(\w+)', stripped)
        if cm:
            class_keyword = stripped.split()[0]
            class_start = i
            brace_idx = i
            while brace_idx < len(lines):
                bline = lines[brace_idx]
                bpos = bline.find('{')
                spos = bline.find(';')
                if bpos != -1:
                    if spos != -1 and spos < bpos:
                        break  # forward declaration
                    break
                if spos != -1:
                    break  # forward declaration
                brace_idx += 1
            else:
                i += 1
                continue

            if brace_idx >= len(lines) or '{' not in lines[brace_idx]:
                i += 1
                continue

            depth = 0
            close_idx = brace_idx
            for k in range(brace_idx, len(lines)):
                for ch in lines[k]:
                    if ch == '{':
                        depth += 1
                    elif ch == '}':
                        depth -= 1
                        if depth == 0:
                            close_idx = k
                            break
                if depth == 0:
                    break

            if close_idx > brace_idx:
                classes.append({
                    'class_start': class_start,
                    'brace_idx': brace_idx,
                    'close_idx': close_idx,
                    'is_struct': class_keyword == 'struct',
                })
            i = close_idx + 1
        else:
            i += 1
    return classes


def _get_indent(line: str) -> str:
    return line[:len(line) - len(line.lstrip())]


def _is_func_decl(stripped: str) -> bool:
    if not stripped:
        return False
    if re.match(r'^(explicit\s+)?(?:virtual\s+)?(?:static\s+)?'
                r'(?:const\s+)?(?:inline\s+)?'
                r'~?\w[\w:<>,*\s&]*\s*\(', stripped):
        return True
    if re.match(r'^(Q_SLOT|Q_INVOKABLE|Q_SIGNAL)\s', stripped):
        return True
    return False


def _process_class(lines: List[str], cls: Dict, extra_sizes: Dict[str, int]) -> List[str]:
    """Reorder members in a single class and return modified lines."""
    is_struct = cls.get('is_struct', False)
    brace_idx = cls['brace_idx']
    close_idx = cls['close_idx']
    body_start = brace_idx + 1
    body_end = close_idx
    if body_start >= body_end:
        return lines

    body = lines[body_start:body_end]

    # --- Pre-scan: identify type definition spans (struct/enum/using) ---
    type_def_spans: Dict[int, Tuple[int, str]] = {}
    current_pre_sec = '_default'
    for line_idx in range(len(body)):
        stripped = body[line_idx].strip()
        if not stripped or stripped.startswith('#'):
            continue
        if re.match(r'^(public|private|protected|Q_SIGNALS|signals|Q_SLOTS|slots)\s*:', stripped):
            current_pre_sec = stripped.split(':')[0].strip()
            continue
        if re.match(r'^using\s+\w+\s*=', stripped):
            type_def_spans[line_idx] = (line_idx, current_pre_sec)
            continue
        td_m = re.match(r'^(struct|enum)\s+(?:class\s+)?(\w+)', stripped)
        if td_m:
            end_idx = _find_type_def_end(body, line_idx)
            if end_idx >= 0:
                type_def_spans[line_idx] = (end_idx, current_pre_sec)

    type_def_indices: set = set()
    section_typedefs: Dict[str, List[List[str]]] = {}
    for start, (end, sec) in type_def_spans.items():
        for j in range(start, end + 1):
            type_def_indices.add(j)
        if sec not in section_typedefs:
            section_typedefs[sec] = []
        section_typedefs[sec].append(body[start:end + 1])

    # --- Pass 1: collect all members with their positions ---
    all_macros: List[MemberVar] = []
    section_regulars: Dict[str, List[MemberVar]] = {}
    section_order: List[str] = []
    current_section = '_default'
    section_order.append(current_section)
    section_regulars[current_section] = []

    line_categories: List[Tuple[int, str, Any]] = []
    raw_lines: Dict[int, str] = {}
    pp_depth = 0

    for line_idx, line in enumerate(body):
        stripped = line.strip()
        raw_lines[line_idx] = line

        if line_idx in type_def_indices:
            line_categories.append((line_idx, 'type_def', None))
            continue

        if not stripped:
            line_categories.append((line_idx, 'empty', None))
            continue
        if stripped.startswith('#'):
            line_categories.append((line_idx, 'preprocessor', None))
            if re.match(r'#\s*if(n?def)?\b', stripped):
                pp_depth += 1
            elif re.match(r'#\s*endif\b', stripped):
                pp_depth = max(0, pp_depth - 1)
            continue
        if stripped.startswith('//'):
            line_categories.append((line_idx, 'comment', None))
            continue

        # Inside preprocessor conditional: treat as opaque
        if pp_depth > 0:
            line_categories.append((line_idx, 'other', None))
            continue

        # Access specifiers
        if re.match(r'^(public|private|protected|Q_SIGNALS|signals|Q_SLOTS|slots)\s*:', stripped):
            line_categories.append((line_idx, 'access_spec', stripped))
            current_section = stripped.split(':')[0].strip()
            if current_section not in section_regulars:
                section_order.append(current_section)
                section_regulars[current_section] = []
            continue

        # Special markers
        if stripped == 'Q_OBJECT':
            line_categories.append((line_idx, 'qobject', None))
            continue
        if re.match(r'Q_D_CREATE\s*\(', stripped):
            line_categories.append((line_idx, 'qd_create', None))
            continue
        if stripped.startswith('friend '):
            line_categories.append((line_idx, 'friend', None))
            continue

        # Macro member
        if is_macro_member(stripped):
            parsed = parse_macro_call(stripped)
            if parsed:
                macro_name, type_str, name_str = parsed
                mv = MemberVar(
                    raw_line=line.rstrip(),
                    var_type=normalize_type(type_str),
                    var_name=name_str,
                    display_type=type_str,
                    display_name=name_str,
                    is_macro=True,
                    macro_name=macro_name,
                    original_indent=_get_indent(line),
                    has_semicolon=stripped.rstrip().endswith(';'),
                    line_number=body_start + line_idx,
                    original_index=len(all_macros) + sum(
                        len(v) for v in section_regulars.values()),
                )
                all_macros.append(mv)
                line_categories.append((line_idx, 'macro', mv))
                continue

        # Function declaration
        if _is_func_decl(stripped):
            line_categories.append((line_idx, 'function', None))
            continue

        # Regular member
        mv = parse_regular_member(line, body_start + line_idx)
        if mv:
            section_regulars[current_section].append(mv)
            line_categories.append((line_idx, 'regular', mv))
            continue

        # Fallthrough
        line_categories.append((line_idx, 'other', None))

    # --- Compute sizes ---
    THRESHOLD = 8
    for m in all_macros:
        m.compute_size(extra_sizes)
    for section in section_regulars:
        for m in section_regulars[section]:
            m.compute_size(extra_sizes)

    # --- Merge default section into private (class) or public (struct) ---
    default_target = 'private' if not is_struct else 'public'
    if '_default' in section_regulars and default_target in section_regulars:
        target_members = section_regulars[default_target]
        default_members = section_regulars['_default']
        max_idx = max((m._original_index for m in target_members), default=0)
        for i, m in enumerate(default_members):
            m._original_index = max_idx + 1 + i
        target_members.extend(default_members)
        del section_regulars['_default']
        if '_default' in section_order:
            section_order.remove('_default')
    elif '_default' in section_regulars:
        section_regulars[default_target] = section_regulars.pop('_default')
        section_order = [default_target if s == '_default' else s for s in section_order]

    # --- Macro block: large by first-occurrence, small grouped by type ---
    large_macros = [m for m in all_macros if m.size >= THRESHOLD]
    small_macros = [m for m in all_macros if m.size < THRESHOLD]

    macro_order = list(large_macros)

    small_type_groups: Dict[str, List[MemberVar]] = {}
    small_type_first: Dict[str, int] = {}
    for m in small_macros:
        if m.var_type not in small_type_groups:
            small_type_groups[m.var_type] = []
            small_type_first[m.var_type] = m._original_index
        small_type_groups[m.var_type].append(m)

    sorted_small_types = sorted(small_type_groups.keys(),
                                key=lambda t: (-small_type_groups[t][0].size, small_type_first[t]))
    for t in sorted_small_types:
        macro_order.extend(small_type_groups[t])

    # --- Reorder each section's regulars: stable sort by ascending size ---
    reordered_sections: Dict[str, List[MemberVar]] = {}
    for section in section_order:
        if section not in section_regulars:
            continue
        members = list(section_regulars[section])
        if not members:
            reordered_sections[section] = members
            continue
        members.sort(key=lambda m: (m.size, m._original_index))
        reordered_sections[section] = members

    # --- Pass 2: reconstruct ---
    template: List[str] = []
    for line_idx, cat, data in line_categories:
        if cat in ('macro', 'regular', 'type_def'):
            continue
        template.append(raw_lines[line_idx])

    final_result: List[str] = []
    macro_emitted = False
    emitted_sections: set = set()

    for line in template:
        stripped = line.strip()
        acm = re.match(
            r'^(public|private|protected|Q_SIGNALS|signals|Q_SLOTS|slots)\s*:', stripped)
        if acm:
            sec = acm.group(1)
            if not macro_emitted:
                for m in macro_order:
                    final_result.append(m.get_output('    '))
                macro_emitted = True
            final_result.append(line)
            if sec not in emitted_sections:
                emitted_sections.add(sec)
                if sec in section_typedefs:
                    for type_lines in section_typedefs[sec]:
                        for tl in type_lines:
                            final_result.append(tl)
                if sec in reordered_sections:
                    for m in reordered_sections[sec]:
                        final_result.append(m.get_output('    '))
            continue
        final_result.append(line)

    if not macro_emitted:
        for m in macro_order:
            final_result.append(m.get_output('    '))
        macro_emitted = True

    if '_default' in reordered_sections:
        for m in reordered_sections['_default']:
            final_result.append(m.get_output('    '))

    new_lines = list(lines)
    new_lines[body_start:body_end] = final_result
    return new_lines


def process_file(filepath: str, extra_sizes: Optional[Dict[str, int]] = None) -> str:
    if extra_sizes is None:
        extra_sizes = {}
    with open(filepath, 'r', encoding='utf-8-sig', errors='replace') as f:
        content = f.read()
    classes = _find_classes(content)
    lines = content.split('\n')
    for cls in reversed(classes):
        lines = _process_class(lines, cls, extra_sizes)
    return '\n'.join(lines)


# =============================================================================
# CLI
# =============================================================================

def _safe_print(text: str) -> None:
    """Print to stdout, replacing unencodable characters for the console."""
    try:
        print(text)
    except UnicodeEncodeError:
        print(text.encode(sys.stdout.encoding or 'utf-8', errors='replace').decode(sys.stdout.encoding or 'utf-8', errors='replace'))


def main():
    parser = argparse.ArgumentParser(
        description='Reorder C++ class member variables to reduce padding/size.',
        epilog="""
Examples:
  python reorder_members.py -i input.h -o output.h
  python reorder_members.py -d ./Sample -o ./Output
  python reorder_members.py -d ./Sample --dry-run
  python reorder_members.py -d ./Sample -o ./Output --add-size "MyType:12"
        """
    )
    parser.add_argument('-i', '--input', help='Single input header file')
    parser.add_argument('-d', '--directory', help='Input directory (recursive)')
    parser.add_argument('-o', '--output', help='Output file or directory')
    parser.add_argument('--dry-run', action='store_true',
                        help='Preview without writing')
    parser.add_argument('--add-size', action='append', default=[],
                        help='Custom type sizes, e.g. "MyType:12"')
    parser.add_argument('--indent', default='    ',
                        help='Indentation (default: 4 spaces)')
    parser.add_argument('--extensions', default='.h,.hpp,.hxx,.h++',
                        help='Header extensions (default: .h,.hpp,.hxx,.h++)')

    args = parser.parse_args()

    extra_sizes = {}
    for item in args.add_size:
        if ':' in item:
            k, v = item.rsplit(':', 1)
            extra_sizes[k.strip()] = int(v.strip())

    extensions = [e.strip() for e in args.extensions.split(',')]

    if args.input:
        if not os.path.isfile(args.input):
            print(f'Error: {args.input} not found', file=sys.stderr)
            sys.exit(1)
        result = process_file(args.input, extra_sizes)
        if args.dry_run:
            _safe_print(result)
        elif args.output:
            os.makedirs(os.path.dirname(os.path.abspath(args.output)) or '.', exist_ok=True)
            with open(args.output, 'w', encoding='utf-8') as f:
                f.write(result)
            print(f'{args.input} -> {args.output}')
        else:
            _safe_print(result)

    elif args.directory:
        if not os.path.isdir(args.directory):
            print(f'Error: {args.directory} not found', file=sys.stderr)
            sys.exit(1)
        if not args.output and not args.dry_run:
            print('Error: --output required for directory mode', file=sys.stderr)
            sys.exit(1)

        processed = 0
        for root, dirs, files in os.walk(args.directory):
            for fname in files:
                if any(fname.endswith(ext) for ext in extensions):
                    fpath = os.path.join(root, fname)
                    try:
                        result = process_file(fpath, extra_sizes)
                        if args.dry_run:
                            print(f'=== {os.path.relpath(fpath, args.directory)} ===')
                            _safe_print(result)
                            print()
                        else:
                            outpath = os.path.join(args.output, os.path.relpath(fpath, args.directory))
                            os.makedirs(os.path.dirname(outpath), exist_ok=True)
                            with open(outpath, 'w', encoding='utf-8') as f:
                                f.write(result)
                        processed += 1
                    except Exception as e:
                        print(f'Error: {fpath}: {e}', file=sys.stderr)
        print(f'Processed {processed} files.')
    else:
        parser.print_help()


if __name__ == '__main__':
    main()