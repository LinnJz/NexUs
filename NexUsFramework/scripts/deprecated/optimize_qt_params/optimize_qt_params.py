import os
import re
import sys
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple


SKIP_OVERRIDE = True

CPP_EXTENSIONS = {
    ".h", ".hpp", ".hxx", ".hh", ".h++",
    ".cpp", ".cxx", ".cc", ".c++", ".c",
    ".m", ".mm",
}

HEADER_EXTENSIONS = {".h", ".hpp", ".hxx", ".hh", ".h++"}


REMOVE_CONST_REF_TYPES: Set[str] = {
    "QPoint", "QPointF",
    "QSize", "QSizeF",
    "QRect", "QRectF",
    "QDate", "QTime", "QChar",
    "QStringView", "QLatin1StringView", "QUtf8StringView",
    "QFlags", "QUuid",
}

ADD_CONST_REF_TYPES: Set[str] = {
    "QString", "QByteArray",

    "QList", "QVector", "QMap", "QHash",
    "QMultiMap", "QMultiHash", "QSet",
    "QStack", "QQueue", "QLinkedList",
    "QVarLengthArray", "QCache", "QContiguousCache",

    "QImage", "QPixmap", "QBrush", "QPen", "QPalette",
    "QFont", "QIcon", "QCursor", "QBitmap",
    "QRegion", "QPolygon", "QPolygonF", "QPainterPath",
    "QTransform", "QPicture",
    "QTextDocument", "QTextBlock", "QTextFrame",
    "QTextList", "QTextTable",
    "QTextFormat", "QTextCharFormat", "QTextBlockFormat",
    "QTextListFormat", "QTextFrameFormat", "QTextTableFormat",
    "QTextImageFormat", "QTextLength",
    "QPageSize", "QPageLayout",
    "QStyleOption", "QStyleHintReturn",

    "QVariant", "QUrl", "QBitArray",
    "QJsonObject", "QJsonArray", "QJsonValue", "QJsonDocument",
    "QRegularExpression", "QLocale", "QEasingCurve",
    "QItemSelection", "QPersistentModelIndex",
    "QCryptographicHash",
    "QCollator", "QCollatorSortKey",
    "QStaticText", "QTextBoundaryFinder",
    "QGlyphRun", "QRawFont",
    "QSurfaceFormat", "QOpenGLVersionProfile",
}


TEMPLATE_GROUP = r'(\s*<(?:[^<>]|<[^<>]*>)*>)?'

_RETURN_TYPE_RE = re.compile(r'^[\w:<>,*\s&]+$')
_LABEL_KEYWORDS = {'public:', 'private:', 'protected:', 'signals:', 'slots:', 'Q_SIGNALS:', 'Q_SLOTS:'}

_override_signatures: Set[str] = set()


def _is_in_comment(content: str, pos: int) -> bool:
    line_start = content.rfind('\n', 0, pos)
    if line_start == -1:
        line_start = 0
    else:
        line_start += 1

    pre_line = content[line_start:pos]
    if '//' in pre_line:
        return True

    last_block_open = content.rfind('/*', 0, pos)
    if last_block_open != -1:
        after_open = content.find('*/', last_block_open + 2)
        if after_open == -1 or after_open > pos:
            return True

    return False


def is_cpp_file(file_path: str) -> bool:
    suffix = Path(file_path).suffix.lower()
    return suffix in CPP_EXTENSIONS


def is_header_file(file_path: str) -> bool:
    suffix = Path(file_path).suffix.lower()
    return suffix in HEADER_EXTENSIONS


def read_file_content(file_path: str) -> str:
    encodings = ["utf-8", "utf-8-sig", "latin-1", "gbk", "cp1252"]
    for enc in encodings:
        try:
            with open(file_path, "r", encoding=enc) as f:
                return f.read()
        except (UnicodeDecodeError, UnicodeError):
            continue
    return ""


def write_file_content(file_path: str, content: str):
    with open(file_path, "w", encoding="utf-8") as f:
        f.write(content)


# ============================================================
# Override signature extraction
# ============================================================

def _extract_override_signature(content: str, override_pos: int) -> Optional[str]:
    if _is_in_comment(content, override_pos):
        return None

    semi = content.find(';', override_pos)
    brace = content.find('{', override_pos)
    if semi == -1 and brace == -1:
        return None

    close_paren = -1
    depth = 0
    for i in range(override_pos - 1, -1, -1):
        ch = content[i]
        if ch == ')':
            if depth == 0:
                close_paren = i
            depth += 1
        elif ch == '(':
            depth -= 1
            if depth == 0 and close_paren != -1:
                open_paren = i
                break
    else:
        return None

    before_paren = content[:open_paren].rstrip()
    func_name_match = re.search(r'(\w+)\s*$', before_paren)
    if not func_name_match:
        return None
    func_name = func_name_match.group(1)

    params_text = content[open_paren + 1:close_paren]
    normalized_params = re.sub(r'\s+', '', params_text)

    between = content[close_paren + 1:override_pos]
    qualifier = 'const' if re.search(r'\bconst\b', between) else ''

    return f"{func_name}||{normalized_params}||{qualifier}"


def _extract_func_def_signature(content: str, func_name_pos: int,
                                 open_paren_pos: int) -> Optional[str]:
    """Extract normalized signature from a function definition in .cpp.
    
    The position points to the start of the function name (after ClassName:: if present).
    Returns: "func_name||normalized_params||qualifier" or None.
    """
    close_paren = -1
    depth = 0
    for i in range(open_paren_pos + 1, len(content)):
        ch = content[i]
        if ch == '(':
            depth += 1
        elif ch == ')':
            if depth == 0:
                close_paren = i
                break
            depth -= 1

    if close_paren == -1:
        return None

    func_name_match = re.match(r'(\w+)', content[func_name_pos:])
    if not func_name_match:
        return None
    func_name = func_name_match.group(1)

    params_text = content[open_paren_pos + 1:close_paren]
    normalized_params = re.sub(r'\s+', '', params_text)

    after_paren = content[close_paren + 1:close_paren + 50]
    qualifier = 'const' if re.match(r'\s*const\b', after_paren) else ''

    return f"{func_name}||{normalized_params}||{qualifier}"


# ============================================================
# Find override regions to protect
# ============================================================

def _find_override_regions_in_header(content: str) -> List[Tuple[int, int]]:
    regions = []
    for m in re.finditer(r'\boverride\b', content):
        override_pos = m.start()
        if _is_in_comment(content, override_pos):
            continue

        end_pos = -1
        semi = content.find(';', override_pos)
        brace = content.find('{', override_pos)
        if semi != -1 and brace != -1:
            end_pos = min(semi, brace) + 1
        elif semi != -1:
            end_pos = semi + 1
        elif brace != -1:
            end_pos = brace
        else:
            continue

        close_paren = -1
        depth = 0
        open_paren = -1
        for i in range(override_pos - 1, -1, -1):
            ch = content[i]
            if ch == ')':
                if depth == 0:
                    close_paren = i
                depth += 1
            elif ch == '(':
                depth -= 1
                if depth == 0 and close_paren != -1:
                    open_paren = i
                    break

        if open_paren == -1:
            continue

        before_paren = content[:open_paren].rstrip()
        func_name_match = re.search(r'(\w+)\s*$', before_paren)
        if not func_name_match:
            continue

        func_name_start = func_name_match.start(1)

        line_start = content.rfind('\n', 0, func_name_start)
        if line_start == -1:
            line_start = 0
        else:
            line_start += 1

        prev_line_end = line_start - 1
        if prev_line_end > 0 and content[prev_line_end] == '\n':
            prev_line_start = content.rfind('\n', 0, prev_line_end)
            if prev_line_start == -1:
                prev_line_start = 0
            else:
                prev_line_start += 1
            prev_line = content[prev_line_start:prev_line_end].strip()
            if prev_line and not prev_line.endswith(',') and not prev_line.endswith('\\'):
                if _RETURN_TYPE_RE.match(prev_line) and prev_line not in _LABEL_KEYWORDS:
                    line_start = prev_line_start

        start_pos = line_start
        regions.append((start_pos, end_pos))

    return regions


def _find_override_regions_in_impl(content: str) -> List[Tuple[int, int]]:
    """Find (start, end) regions of function definitions in .cpp that match override signatures."""
    if not _override_signatures:
        return []

    func_names = set()
    for sig in _override_signatures:
        parts = sig.split('||', 2)
        if parts:
            func_names.add(parts[0])

    regions = []
    for func_name in func_names:
        pattern = re.compile(r'(\w+)::(' + re.escape(func_name) + r')\s*\(')
        for m in pattern.finditer(content):
            short_name_start = m.start(2)
            open_paren = m.end() - 1

            sig = _extract_func_def_signature(content, short_name_start, open_paren)
            if sig and sig in _override_signatures:
                line_start = content.rfind('\n', 0, m.start())
                if line_start == -1:
                    line_start = 0
                else:
                    line_start += 1

                prev_line_end = line_start - 1
                if prev_line_end > 0 and content[prev_line_end] == '\n':
                    prev_line_start = content.rfind('\n', 0, prev_line_end)
                    if prev_line_start == -1:
                        prev_line_start = 0
                    else:
                        prev_line_start += 1
                    prev_line = content[prev_line_start:prev_line_end].strip()
                    if prev_line and not prev_line.endswith(',') and not prev_line.endswith('\\'):
                        if re.match(r'^[\w:<>,*\s&]+$', prev_line):
                            line_start = prev_line_start

                end_pos = content.find('{', open_paren)
                if end_pos == -1:
                    end_pos = content.find(';', open_paren)
                    if end_pos == -1:
                        continue
                    end_pos += 1

                regions.append((line_start, end_pos))

    return regions


# ============================================================
# Placeholder protection
# ============================================================

def _protect_regions(content: str, regions: List[Tuple[int, int]]) -> Tuple[str, Dict[str, str]]:
    """Replace regions with unique placeholders."""
    placeholders: Dict[str, str] = {}
    sorted_regions = sorted(regions, key=lambda r: r[0], reverse=True)
    for i, (start, end) in enumerate(sorted_regions):
        if start >= len(content) or end > len(content) or start >= end:
            continue
        placeholder = f"__QT_OPT_OVERRIDE_PLACEHOLDER_{i}__"
        original = content[start:end]
        content = content[:start] + placeholder + content[end:]
        placeholders[placeholder] = original
    return content, placeholders


def _restore_regions(content: str, placeholders: Dict[str, str]) -> str:
    for placeholder, original in placeholders.items():
        content = content.replace(placeholder, original, 1)
    return content


# ============================================================
# Phase 0: Collect override signatures from headers
# ============================================================

def collect_override_signatures(header_files: List[str]) -> Set[str]:
    """Scan all header files and collect override function signatures."""
    global _override_signatures
    _override_signatures.clear()
    for file_path in header_files:
        content = read_file_content(file_path)
        if not content:
            continue
        for m in re.finditer(r'\boverride\b', content):
            sig = _extract_override_signature(content, m.start())
            if sig:
                _override_signatures.add(sig)
    return _override_signatures


# ============================================================
# Phase 1: Remove const & from small types
# ============================================================

def remove_const_ref_from_small_types(content: str) -> str:
    for type_name in sorted(REMOVE_CONST_REF_TYPES, key=len, reverse=True):
        escaped = re.escape(type_name)
        tmpl = rf'{escaped}{TEMPLATE_GROUP}'

        pattern_a = re.compile(
            rf'\bconst\s+({tmpl})\s*&\s*(\w*)'
        )
        content = pattern_a.sub(r'\1 \3', content)

        pattern_b = re.compile(
            rf'\b({tmpl})\s+const\s*&\s*(\w*)'
        )
        content = pattern_b.sub(r'\1 \3', content)

    return content


# ============================================================
# Phase 2: Add const & to COW types passed by value
# ============================================================

def build_cow_pattern() -> re.Pattern:
    types_sorted = sorted(ADD_CONST_REF_TYPES, key=len, reverse=True)
    alternation = '|'.join(re.escape(t) for t in types_sorted)

    return re.compile(
        rf'([(,]\s*)'
        rf'(const\s+)?'
        rf'({alternation})'
        rf'{TEMPLATE_GROUP}'
        rf'\s+'
        rf'(?!\s*(?:const|[&*]))'
        rf'(\w+)'
    )


def add_const_ref_to_cow_types(content: str) -> str:
    pattern = build_cow_pattern()

    def replace_match(m):
        boundary = m.group(1)
        has_const = m.group(2) or "const "
        type_name = m.group(3)
        tmpl_suffix = m.group(4) or ""
        param_name = m.group(5)

        return f"{boundary}{has_const}{type_name}{tmpl_suffix} &{param_name}"

    return pattern.sub(replace_match, content)


# ============================================================
# Main processing
# ============================================================

def process_file(file_path: str) -> bool:
    original = read_file_content(file_path)
    if not original:
        return False

    content = original

    if SKIP_OVERRIDE:
        if is_header_file(file_path):
            regions = _find_override_regions_in_header(content)
        else:
            regions = _find_override_regions_in_impl(content)

        if regions:
            content, placeholders = _protect_regions(content, regions)
            content = remove_const_ref_from_small_types(content)
            content = add_const_ref_to_cow_types(content)
            content = _restore_regions(content, placeholders)
        else:
            content = remove_const_ref_from_small_types(content)
            content = add_const_ref_to_cow_types(content)
    else:
        content = remove_const_ref_from_small_types(content)
        content = add_const_ref_to_cow_types(content)

    if content != original:
        write_file_content(file_path, content)
        return True
    return False


def main():
    if len(sys.argv) < 2:
        print("Usage: python optimize_qt_params.py <target_directory> [--no-skip-override]")
        print("Example: python optimize_qt_params.py C:/my_project/src")
        print("         python optimize_qt_params.py C:/my_project/src --no-skip-override")
        sys.exit(1)

    target_dir = sys.argv[1]

    global SKIP_OVERRIDE
    if "--no-skip-override" in sys.argv:
        SKIP_OVERRIDE = False

    if not os.path.isdir(target_dir):
        print(f"Error: '{target_dir}' is not a valid directory.")
        sys.exit(1)

    print(f"Scanning directory: {target_dir}")
    print(f"Target extensions: {', '.join(sorted(CPP_EXTENSIONS))}")
    print(f"Skip override functions: {SKIP_OVERRIDE}")

    print(f"\n[Phase 1] Remove const & from small types ({len(REMOVE_CONST_REF_TYPES)} types):")
    print(f"  {', '.join(sorted(REMOVE_CONST_REF_TYPES))}")

    print(f"\n[Phase 2] Add const & to COW types by value ({len(ADD_CONST_REF_TYPES)} types):")
    print(f"  {', '.join(sorted(ADD_CONST_REF_TYPES))}")

    all_files: List[str] = []
    for dirpath, _dirnames, filenames in os.walk(target_dir):
        for filename in filenames:
            if is_cpp_file(filename):
                all_files.append(os.path.join(dirpath, filename))

    if SKIP_OVERRIDE:
        print(f"\n[Phase 0] Collecting override signatures from header files...")
        header_files = [f for f in all_files if is_header_file(f)]
        sigs = collect_override_signatures(header_files)
        print(f"  Found {len(sigs)} override signature(s):")
        for sig in sorted(sigs):
            print(f"    {sig}")

    modified_count = 0
    for file_path in all_files:
        if process_file(file_path):
            modified_count += 1
            print(f"  Modified: {file_path}")

    print(f"\nModified {modified_count} file(s).")
    print("Done.")


if __name__ == "__main__":
    main()