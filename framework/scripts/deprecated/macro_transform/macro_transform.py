import os
import re

CPP_EXTENSIONS = {
    ".h", ".hpp", ".hxx", ".hh", ".h++",
    ".cpp", ".cxx", ".cc", ".c++", ".c",
    ".m", ".mm",
}

HEADER_EXTENSIONS = {".h", ".hpp", ".hxx", ".hh", ".h++"}

TARGET_MACROS = {
    "Q_PROPERTY_CREATE",
    "Q_PROPERTY_REF_CREATE",
    "Q_PROPERTY_CREATE_Q_H",
    "Q_PROPERTY_REF_CREATE_Q_H",
    "Q_PRIVATE_CREATE_Q_H",
    "Q_PRIVATE_REF_CREATE_Q_H",
    "Q_PROPERTY_CREATE_Q_CPP",
    "Q_PROPERTY_REF_CREATE_Q_CPP",
    "Q_PRIVATE_CREATE_Q_CPP",
    "Q_PRIVATE_REF_CREATE_Q_CPP",
    "Q_PROPERTY_CREATE_D",
    "Q_PRIVATE_CREATE_D",
    "Q_PRIVATE_CREATE",
    "Q_PRIVATE_REF_CREATE",
}

ADD_CONST_REF_TYPES = {
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

PRIMITIVE_TYPES = {
    "bool",
    "char", "signed char", "unsigned char",
    "short", "unsigned short", "short int",
    "int", "unsigned int", "unsigned",
    "long", "unsigned long", "long int",
    "long long", "unsigned long long",
    "float",
    "double", "long double",
    "qreal",
    "qint8", "quint8", "qint16", "quint16",
    "qint32", "quint32", "qint64", "quint64",
    "int8_t", "uint8_t", "int16_t", "uint16_t",
    "int32_t", "uint32_t", "int64_t", "uint64_t",
    "size_t", "ptrdiff_t",
}

TYPE_SIZE_MAP = {
    "bool": 1,
    "char": 1, "signed char": 1, "unsigned char": 1,
    "qint8": 1, "quint8": 1, "int8_t": 1, "uint8_t": 1,
    "short": 2, "unsigned short": 2, "qint16": 2, "quint16": 2,
    "int16_t": 2, "uint16_t": 2,
    "int": 4, "unsigned int": 4, "unsigned": 4,
    "qint32": 4, "quint32": 4, "int32_t": 4, "uint32_t": 4,
    "long": 4, "unsigned long": 4,
    "float": 4,
    "long long": 8, "unsigned long long": 8,
    "qint64": 8, "quint64": 8, "int64_t": 8, "uint64_t": 8,
    "double": 8, "qreal": 8, "size_t": 8, "ptrdiff_t": 8,
    "long double": 16,
    "QSize": 8,
    "QSizeF": 16,
    "QPoint": 8,
    "QPointF": 16,
    "QRect": 16,
    "QRectF": 32,
    "QMargins": 16,
    "QMarginsF": 32,
    "QLine": 8,
    "QLineF": 16,
    "QColor": 4,
    "QImage": 8,
}

MACRO_PATTERN = re.compile(
    r'^(\s*)('
    r'Q_PROPERTY_CREATE_Q_H|'
    r'Q_PROPERTY_REF_CREATE_Q_H|'
    r'Q_PRIVATE_CREATE_Q_H|'
    r'Q_PRIVATE_REF_CREATE_Q_H|'
    r'Q_PROPERTY_CREATE_Q_CPP|'
    r'Q_PROPERTY_REF_CREATE_Q_CPP|'
    r'Q_PRIVATE_CREATE_Q_CPP|'
    r'Q_PRIVATE_REF_CREATE_Q_CPP|'
    r'Q_PROPERTY_CREATE_D|'
    r'Q_PRIVATE_CREATE_D|'
    r'Q_PROPERTY_CREATE|'
    r'Q_PROPERTY_REF_CREATE|'
    r'Q_PRIVATE_CREATE|'
    r'Q_PRIVATE_REF_CREATE'
    r')\s*\((.+?)\)\s*;?\s*$'
)


def parse_macro_line(line):
    m = MACRO_PATTERN.match(line.rstrip('\n'))
    if not m:
        return None
    indent = m.group(1)
    macro_name = m.group(2)
    args_str = m.group(3)
    args = _split_macro_args(args_str)
    result = (indent, macro_name) + tuple(args)
    return result


def _split_macro_args(args_str):
    args = []
    depth = 0
    current = ""
    for ch in args_str:
        if ch in '(<{[（':
            depth += 1
            current += ch
        elif ch in ')>}]）':
            depth -= 1
            current += ch
        elif ch == ',' and depth == 0:
            args.append(current.strip())
            current = ""
        else:
            current += ch
    if current.strip():
        args.append(current.strip())
    return args


def transform_macro_name(name):
    name = name.replace("_Q_", "_")
    name = name.replace("_REF_", "_")
    return name


def categorize_type(type_str):
    base = type_str.rstrip('*').strip()
    if base in PRIMITIVE_TYPES:
        return 0
    if base in ADD_CONST_REF_TYPES:
        return 2
    if '*' in type_str:
        return 1
    if '::' in base:
        return 1
    if base.startswith('Q') and base[0].isupper():
        return 2
    return 1


def get_type_byte_size(type_str):
    base = type_str.rstrip('*').strip()
    if base in TYPE_SIZE_MAP:
        size = TYPE_SIZE_MAP[base]
    elif '*' in type_str:
        size = 8
    else:
        size = 4
    return size


def make_sort_key(macro_name, type_str, property_name):
    cat = categorize_type(type_str)
    if cat == 0:
        size = get_type_byte_size(type_str)
        return (cat, size, property_name)
    elif cat == 1:
        size = get_type_byte_size(type_str)
        return (cat, size, property_name)
    else:
        return (cat, 0, property_name)


def should_wrap_qs_set_cref(type_str):
    base = type_str.rstrip('*').strip()
    return base in ADD_CONST_REF_TYPES


def wrap_type_with_qs_set_cref(type_str):
    if should_wrap_qs_set_cref(type_str):
        return f"QS_SET_CREF({type_str})"
    return type_str


def process_macros_in_content(content):
    lines = content.split('\n')
    result_lines = []
    i = 0
    while i < len(lines):
        line = lines[i]
        parsed = parse_macro_line(line)
        if parsed is None:
            result_lines.append(line)
            i += 1
        else:
            block_entries = []
            j = i
            while j < len(lines):
                p = parse_macro_line(lines[j])
                if p is not None:
                    block_entries.append(p)
                    j += 1
                else:
                    break

            if block_entries:
                base_indent = block_entries[0][0]
                processed = _process_macro_block(block_entries)
                for pline in processed:
                    result_lines.append(base_indent + pline)
                i = j
            else:
                result_lines.append(line)
                i += 1

    return '\n'.join(result_lines)


def _is_h_or_cpp_macro(transformed_name):
    return transformed_name.endswith('_H') or transformed_name.endswith('_CPP')


def _build_macro_line(className, transformed_name, type_str, prop_name):
    if className is not None:
        return f"{transformed_name}({className}, {type_str}, {prop_name})"
    else:
        return f"{transformed_name}({type_str}, {prop_name})"


def _process_macro_block(macro_entries):
    first_transformed = transform_macro_name(macro_entries[0][1])
    if _is_h_or_cpp_macro(first_transformed):
        return _process_h_cpp_block(macro_entries)
    else:
        return _process_non_h_cpp_block(macro_entries)


def _process_h_cpp_block(macro_entries):
    entries = []
    for entry in macro_entries:
        macro_name = entry[1]

        if len(entry) == 4:
            type_str = entry[2]
            prop_name = entry[3]
            className = None
        elif len(entry) == 5:
            className = entry[2]
            type_str = entry[3]
            prop_name = entry[4]
        else:
            continue

        transformed_name = transform_macro_name(macro_name)
        wrapped_type = wrap_type_with_qs_set_cref(type_str)

        entries.append({
            'className': className,
            'transformed_name': transformed_name,
            'type_str': wrapped_type,
            'prop_name': prop_name,
            'sort_key': (transformed_name,) + make_sort_key(transformed_name, type_str, prop_name),
        })

    entries.sort(key=lambda e: e['sort_key'])

    result = []
    for entry in entries:
        line = _build_macro_line(entry['className'], entry['transformed_name'], entry['type_str'], entry['prop_name'])
        result.append(line)

    return result


def _process_non_h_cpp_block(macro_entries):
    result = []
    for entry in macro_entries:
        macro_name = entry[1]

        if len(entry) == 4:
            type_str = entry[2]
            prop_name = entry[3]
            className = None
        elif len(entry) == 5:
            className = entry[2]
            type_str = entry[3]
            prop_name = entry[4]
        else:
            continue

        transformed_name = transform_macro_name(macro_name)
        line = _build_macro_line(className, transformed_name, type_str, prop_name)
        result.append(line)

    return result


def scan_cpp_files(root_dir):
    cpp_files = []
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            ext = os.path.splitext(filename)[1].lower()
            if ext in CPP_EXTENSIONS:
                cpp_files.append(os.path.join(dirpath, filename))
        dirnames[:] = sorted(dirnames)
    return sorted(cpp_files)


def transform_file(file_path):
    """原地修改文件"""
    with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    new_content = process_macros_in_content(content)
    # 仅在内容发生变化时才写回
    if new_content != content:
        with open(file_path, 'w', encoding='utf-8', newline='\n') as f:
            f.write(new_content)


def transform_directory(root_dir):
    """递归处理 root_dir 下的所有 C++ 文件，原地修改"""
    cpp_files = scan_cpp_files(root_dir)
    for file_path in cpp_files:
        transform_file(file_path)
    return len(cpp_files)


if __name__ == "__main__":
    import sys
    if len(sys.argv) >= 2:
        root_dir = sys.argv[1]
    else:
        print("Usage: python script.py <root_directory>")
        sys.exit(1)

    count = transform_directory(root_dir)
    print(f"Transformed {count} files in {root_dir}")