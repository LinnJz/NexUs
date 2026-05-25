import os
import re
import sys
from pathlib import Path


HEADER_EXTENSIONS = {".h", ".hpp", ".hxx", ".hh", ".h++"}

ACCESS_SPECIFIERS = {
    "public:",
    "protected:",
    "private:",
    "Q_SLOTS:",
    "Q_SIGNALS:",
    "signals:",
    "public slots:",
    "protected slots:",
    "private slots:",
}


def is_header_file(file_path: str) -> bool:
    suffix = Path(file_path).suffix.lower()
    return suffix in HEADER_EXTENSIONS


def read_file_lines(file_path: str) -> list:
    encodings = ["utf-8", "utf-8-sig", "latin-1", "gbk", "cp1252"]
    for enc in encodings:
        try:
            with open(file_path, "r", encoding=enc) as f:
                return f.readlines()
        except (UnicodeDecodeError, UnicodeError):
            continue
    return []


def write_file_lines(file_path: str, lines: list):
    with open(file_path, "w", encoding="utf-8") as f:
        f.writelines(lines)


def is_q_signals_line(line: str) -> bool:
    stripped = line.strip()
    return stripped == "Q_SIGNALS:" or stripped == "signals:"


def is_scope_end_line(line: str) -> bool:
    return line.strip() == "};"


def is_blank_line(line: str) -> bool:
    return line.strip() == ""


def is_access_specifier(line: str) -> bool:
    stripped = line.strip().lower()
    for spec in ACCESS_SPECIFIERS:
        if stripped == spec.lower():
            return True
    return False


def is_function_declaration(line: str) -> bool:
    stripped = line.strip()
    if not stripped:
        return False
    if stripped.startswith("//") or stripped.startswith("/*") or stripped.startswith("*"):
        return False
    if stripped.startswith("#"):
        return False
    if is_q_signals_line(line):
        return False
    if is_scope_end_line(line):
        return False
    if is_access_specifier(line):
        return False
    if "(" in stripped and ")" in stripped and stripped.endswith(";"):
        return True
    return False


def add_q_signal(line: str) -> str:
    stripped = line.lstrip()
    indent = line[: len(line) - len(stripped)]
    if stripped.startswith("Q_SIGNAL "):
        return line
    return f"{indent}Q_SIGNAL {stripped}"


def process_header_file(file_path: str) -> bool:
    lines = read_file_lines(file_path)
    if not lines:
        return False

    modified = False
    i = 0
    output = []

    while i < len(lines):
        line = lines[i]

        if is_q_signals_line(line):
            modified = True

            if output and is_blank_line(output[-1]):
                output.pop()

            i += 1

            while i < len(lines):
                scope_line = lines[i]

                if is_scope_end_line(scope_line):
                    output.append(scope_line)
                    i += 1
                    break

                if is_access_specifier(scope_line):
                    break

                if is_function_declaration(scope_line):
                    output.append(add_q_signal(scope_line))
                else:
                    output.append(scope_line)
                i += 1
        else:
            output.append(line)
            i += 1

    if modified:
        write_file_lines(file_path, output)
        return True
    return False


def main():
    if len(sys.argv) < 2:
        print("Usage: python convert_q_signals.py <target_directory>")
        print("Example: python convert_q_signals.py C:/my_project/src")
        sys.exit(1)

    target_dir = sys.argv[1]

    if not os.path.isdir(target_dir):
        print(f"Error: '{target_dir}' is not a valid directory.")
        sys.exit(1)

    print(f"Scanning directory: {target_dir}")
    print(f"Target extensions: {', '.join(sorted(HEADER_EXTENSIONS))}")

    modified_count = 0

    for dirpath, _dirnames, filenames in os.walk(target_dir):
        for filename in filenames:
            if not is_header_file(filename):
                continue
            file_path = os.path.join(dirpath, filename)
            if process_header_file(file_path):
                modified_count += 1
                print(f"  Modified: {file_path}")

    print(f"\nModified {modified_count} header file(s).")
    print("Done.")


if __name__ == "__main__":
    main()