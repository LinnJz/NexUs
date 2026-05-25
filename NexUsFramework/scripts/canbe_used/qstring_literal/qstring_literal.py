import re
import os
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed
import multiprocessing

_INCLUDE_RE = re.compile(r'\s*#\s*include\s*$')
_FILTER_FUNCTIONS = frozenset({'setObjectName', 'setProperty', 'property', 'QPropertyAnimation', 'QStringLiteral', 'tr', 'GetProcAddress'})
_QT_DEBUG_MACROS = frozenset({'qDebug', 'qWarning', 'qCritical', 'qFatal', 'qInfo',
                               'qCDebug', 'qCWarning', 'qCCritical', 'qCFatal', 'qCInfo'})
_FILTER_FUNCTIONS = _FILTER_FUNCTIONS | _QT_DEBUG_MACROS
_QSTRING_FROM_RE = re.compile(r'^QString::from\w+$')
_PREFIX_CHARS = frozenset({'R', 'u', 'U', 'L'})
CPP_EXTENSIONS = frozenset({'.h', '.cpp', '.hpp', '.cxx', '.cc', '.c++', '.hxx', '.hh', '.h++', '.c', '.C'})


def _get_prefix_at(content, quote_pos):
    j = quote_pos - 1
    while j >= 0 and content[j] in ' \t':
        j -= 1
    if j < 0:
        return 0

    ch = content[j]
    if ch == 'R':
        if j >= 1 and content[j - 1] in ('u', 'U', 'L'):
            if content[j - 1] == 'u' and j >= 2 and content[j - 2:j] == 'u8':
                return 4
            return 2
        return 1
    if ch == '8' and j >= 1 and content[j - 1] == 'u':
        return 2
    if ch in ('u', 'U', 'L'):
        return 1
    return 0


def _skip_raw_string(content, i, n):
    i += 1
    if i >= n:
        return i
    if content[i] == '(':
        delimiter = ''
    else:
        delim_start = i
        while i < n and content[i] != '(':
            i += 1
        delimiter = content[delim_start:i]
        if i >= n:
            return i
    i += 1
    closing = ')' + delimiter + '"'
    clen = len(closing)
    while i < n:
        if i + clen <= n and content[i:i + clen] == closing:
            return i + clen
        i += 1
    return i


def _skip_regular_string(content, i, n):
    i += 1
    while i < n:
        ch = content[i]
        if ch == '\\':
            i += 2
        elif ch == '"':
            return i + 1
        elif ch == '\n':
            return i + 1
        else:
            i += 1
    return i


def _skip_char_literal(content, i, n):
    i += 1
    while i < n:
        ch = content[i]
        if ch == '\\':
            i += 2
        elif ch == "'":
            return i + 1
        else:
            i += 1
    return i


def _extract_func_name(content, paren_pos):
    j = paren_pos - 1
    while j >= 0 and content[j] in ' \t\n\r':
        j -= 1
    if j < 0:
        return ''
    k = j
    while k >= 0 and (content[k].isalnum() or content[k] == '_' or content[k] == ':'):
        k -= 1
    return content[k + 1:j + 1]


def _is_only_whitespace_or_comments(text):
    i = 0
    n = len(text)
    while i < n:
        ch = text[i]
        if ch in ' \t\n\r':
            i += 1
        elif i + 1 < n and text[i:i + 2] == '/*':
            end = text.find('*/', i + 2)
            if end == -1:
                return False
            i = end + 2
        elif i + 1 < n and text[i:i + 2] == '//':
            newline = text.find('\n', i)
            i = n if newline == -1 else newline + 1
        else:
            return False
    return True


def process_content(content):
    n = len(content)
    i = 0

    literals = []
    paren_stack = []
    in_include = False
    include_line_start = 0
    last_popped_func = None
    in_debug_stream = False

    while i < n:
        ch = content[i]

        if ch == '\n':
            in_include = False
            i += 1
            continue

        if i + 1 < n and content[i:i + 2] == '/*':
            end = content.find('*/', i + 2)
            i = n if end == -1 else end + 2
            continue

        if i + 1 < n and content[i:i + 2] == '//':
            newline = content.find('\n', i)
            i = n if newline == -1 else newline + 1
            continue

        if ch == "'":
            i = _skip_char_literal(content, i, n)
            continue

        if ch == '#':
            line_start = content.rfind('\n', 0, i)
            line_start = 0 if line_start == -1 else line_start + 1
            line_end = content.find('\n', i)
            line_end = n if line_end == -1 else line_end
            directive_line = content[line_start:line_end]
            if _INCLUDE_RE.match(directive_line):
                in_include = True
                include_line_start = line_start
            i = line_end + 1 if line_end < n else n
            continue

        if ch == '(':
            func_name = _extract_func_name(content, i)
            paren_stack.append(func_name)
            i += 1
            continue

        if ch == ')':
            if paren_stack:
                last_popped_func = paren_stack.pop()
            i += 1
            continue

        if ch == ';':
            last_popped_func = None
            in_debug_stream = False
            i += 1
            continue

        if i + 1 < n and content[i:i + 2] == '<<':
            if last_popped_func is not None and last_popped_func in _QT_DEBUG_MACROS:
                in_debug_stream = True
            last_popped_func = None
            i += 2
            continue

        if ch == '"':
            start = i
            prefix_len = _get_prefix_at(content, start)

            if prefix_len >= 1:
                prefix_str = content[start - prefix_len:start].upper()
                if 'R' in prefix_str:
                    i = _skip_raw_string(content, i, n)
                else:
                    i = _skip_regular_string(content, i, n)
            else:
                i = _skip_regular_string(content, i, n)

            needs_wrapping = True
            if prefix_len > 0:
                needs_wrapping = False
            elif in_include:
                needs_wrapping = False
            elif paren_stack and (paren_stack[-1] in _FILTER_FUNCTIONS or _QSTRING_FROM_RE.match(paren_stack[-1])):
                needs_wrapping = False
            elif in_debug_stream:
                needs_wrapping = False
            else:
                j = start - 1
                while j >= 0 and content[j] in ' \t\n\r':
                    j -= 1
                if j >= 14 and content[j - 14:j + 1] == 'QStringLiteral(':
                    needs_wrapping = False

            literals.append((start, i, prefix_len, needs_wrapping))
        else:
            if ch not in ' \t\n\r':
                last_popped_func = None
            i += 1

    if not literals:
        return content

    groups = []
    current_group = [literals[0]]

    for idx in range(1, len(literals)):
        prev = literals[idx - 1]
        curr = literals[idx]
        between = content[prev[1]:curr[0]]

        if _is_only_whitespace_or_comments(between) and prev[3] and curr[3]:
            current_group.append(curr)
        else:
            groups.append(current_group)
            current_group = [curr]
    groups.append(current_group)

    result = []
    last_end = 0

    for group in groups:
        first = group[0]
        last = group[-1]

        if not first[3]:
            result.append(content[last_end:last[1]])
            last_end = last[1]
            continue

        result.append(content[last_end:first[0]])
        result.append('QStringLiteral(')
        result.append(content[first[0]:last[1]])
        result.append(')')
        last_end = last[1]

    result.append(content[last_end:])
    return ''.join(result)


def find_cpp_files(directory):
    cpp_files = []
    for root, dirs, files in os.walk(directory):
        for filename in files:
            ext = os.path.splitext(filename)[1]
            if ext in CPP_EXTENSIONS:
                cpp_files.append(os.path.join(root, filename))
    return cpp_files


def process_file(filepath):
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        original = f.read()
    processed = process_content(original)
    if processed != original:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(processed)
        return True
    return False


def _process_file_worker(args):
    filepath, = args
    return process_file(filepath)


def process_files_parallel(filepaths, max_workers=None):
    if max_workers is None:
        max_workers = min(multiprocessing.cpu_count(), len(filepaths), 8)
    if max_workers <= 1 or len(filepaths) <= 1:
        return sum(1 for fp in filepaths if process_file(fp))

    modified_count = 0
    chunk_size = max(1, len(filepaths) // max_workers)
    with ProcessPoolExecutor(max_workers=max_workers) as executor:
        futures = {executor.submit(process_file, fp): fp for fp in filepaths}
        for future in as_completed(futures):
            try:
                if future.result():
                    modified_count += 1
            except Exception as e:
                print(f"  Error processing {futures[future]}: {e}")
    return modified_count


def main():
    if len(sys.argv) > 1:
        target_dir = sys.argv[1]
    else:
        target_dir = os.path.dirname(os.path.abspath(__file__))

    if not os.path.isdir(target_dir):
        print(f"Error: '{target_dir}' is not a valid directory.")
        sys.exit(1)

    target_dir = os.path.abspath(target_dir)
    print(f"Scanning directory: {target_dir}")

    cpp_files = find_cpp_files(target_dir)
    print(f"Found {len(cpp_files)} C++ file(s)")

    if len(sys.argv) > 2 and sys.argv[2] == '--serial':
        modified_count = sum(1 for fp in cpp_files if process_file(fp))
    else:
        modified_count = process_files_parallel(cpp_files)

    print(f"Done. Modified {modified_count} file(s).")


if __name__ == '__main__':
    main()