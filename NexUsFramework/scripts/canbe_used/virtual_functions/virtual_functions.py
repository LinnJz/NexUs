import os
import re
import sys
from pathlib import Path
from typing import Set


CPP_EXTENSIONS = {
    ".h", ".hpp", ".hxx", ".hh", ".h++",
    ".cpp", ".cxx", ".cc", ".c++", ".c",
    ".m", ".mm",
}

QT_VIRTUAL_FUNCTIONS: Set[str] = {
    # === QObject ===
    "eventFilter", "timerEvent", "customEvent", "childEvent",
    "connectNotify", "disconnectNotify", "event",

    # === QWidget ===
    "paintEvent", "resizeEvent", "moveEvent", "closeEvent",
    "showEvent", "hideEvent", "focusInEvent", "focusOutEvent",
    "enterEvent", "leaveEvent", "mousePressEvent", "mouseReleaseEvent",
    "mouseDoubleClickEvent", "mouseMoveEvent", "wheelEvent",
    "keyPressEvent", "keyReleaseEvent", "changeEvent", "contextMenuEvent",
    "dragEnterEvent", "dragMoveEvent", "dragLeaveEvent", "dropEvent",
    "inputMethodEvent", "tabletEvent", "nativeEvent", "actionEvent",
    "paintEngine", "sizeHint", "minimumSizeHint",
    "hasHeightForWidth", "heightForWidth",
    "metric", "initPainter", "redirectContent", "sharedPainter",
    "devType",

    # === QAbstractItemModel / QAbstractListModel / QAbstractTableModel ===
    "index", "parent", "rowCount", "columnCount",
    "data", "headerData", "flags", "setData", "setHeaderData",
    "insertColumns", "removeColumns", "insertRows", "removeRows",
    "supportedDropActions", "canDropMimeData", "dropMimeData",
    "mimeData", "mimeTypes", "submit", "revert",
    "fetchMore", "canFetchMore", "sort", "roleNames",
    "buddy", "span", "sibling", "hasChildren", "match",
    "resetInternalData", "multiData",

    # === QAbstractItemDelegate / QStyledItemDelegate ===
    "createEditor", "setEditorData", "setModelData",
    "updateEditorGeometry", "editorEvent", "displayText",
    "helpEvent", "destroyEditor", "initStyleOption",

    # === QGraphicsItem / QGraphicsObject ===
    "boundingRect", "shape", "contains",
    "collidesWithItem", "collidesWithPath", "isObscuredBy",
    "opaqueArea", "type", "advance", "itemChange",
    "sceneEvent", "sceneEventFilter",
    "hoverEnterEvent", "hoverLeaveEvent", "hoverMoveEvent",
    "inputMethodQuery",

    # === QThread ===
    "run",

    # === QIODevice ===
    "readData", "writeData", "readLineData", "atEnd",
    "bytesAvailable", "bytesToWrite", "canReadLine",
    "isSequential", "open", "pos", "reset", "seek", "size",
    "waitForBytesWritten", "waitForReadyRead",

    # === QValidator ===
    "validate", "fixup",

    # === QAbstractButton ===
    "hitButton", "checkStateSet", "nextCheckState",

    # === QAbstractScrollArea ===
    "scrollContentsBy", "viewportEvent", "viewportSizeHint",

    # === QListView / QTableView / QTreeView ===
    "visualRect", "scrollTo", "indexAt",
    "selectedIndexes", "selectionChanged", "currentChanged",
    "dataChanged", "rowsInserted", "rowsAboutToBeRemoved",
    "setRootIndex", "updateGeometries", "viewOptions", "startDrag",
    "horizontalOffset", "verticalOffset", "isIndexHidden",
    "setSelection", "visualRegion", "drawRow", "drawBranches",

    # === QHeaderView ===
    "paintSection",

    # === QMenu ===
    "columnSpan", "rowSpan",

    # === QFileSystemModel ===
    "canFetchMore", "fetchMore",

    # === QSortFilterProxyModel ===
    "filterAcceptsRow", "filterAcceptsColumn", "lessThan",

    # === QAbstractProxyModel ===
    "mapFromSource", "mapToSource",
    "mapSelectionFromSource", "mapSelectionToSource",

    # === QIdentityProxyModel ===
    # (inherits from QAbstractProxyModel)

    # === QAbstractSpinBox ===
    "stepBy", "stepEnabled", "clear", "validate", "fixup",
    "textFromValue", "valueFromText",

    # === QComboBox ===
    "showPopup", "hidePopup",

    # === QAbstractSlider ===
    "sliderChange",

    # === QDialog / QMainWindow ===
    "done", "accept", "reject", "open",

    # === QSplashScreen ===
    "drawContents",

    # === QAbstractTextDocumentLayout ===
    "documentChanged", "pageCount", "documentSize",
    "frameBoundingRect", "blockBoundingRect",
    "hitTest", "handlerForObject",

    # === QSyntaxHighlighter ===
    "highlightBlock",

    # === QQuickItem ===
    "geometryChanged", "itemChange", "updatePolish",
    "componentComplete", "isComponentComplete",
    "mouseUngrabEvent", "touchUngrabEvent",
    "keyReleaseEvent", "keyPressEvent",
    "inputMethodEvent", "focusInEvent", "focusOutEvent",
    "hoverEnterEvent", "hoverLeaveEvent", "hoverMoveEvent",
    "dragEnterEvent", "dragLeaveEvent", "dragMoveEvent", "dropEvent",
    "wheelEvent", "releaseResources",

    # === QQmlParserStatus ===
    "classBegin",

    # === QWindow ===
    "exposeEvent", "focusObject",

    # === QOffscreenSurface ===
    # === QSurface ===
    "surfaceType", "format",
}


def is_cpp_file(file_path: str) -> bool:
    suffix = Path(file_path).suffix.lower()
    return suffix in CPP_EXTENSIONS


def read_file_content(file_path: str) -> str:
    encodings = ["utf-8", "utf-8-sig", "latin-1", "gbk", "cp1252"]
    for enc in encodings:
        try:
            with open(file_path, "r", encoding=enc) as f:
                return f.read()
        except (UnicodeDecodeError, UnicodeError):
            continue
    return ""


def extract_all_virtual_names(root_dir: str) -> Set[str]:
    unique_names: Set[str] = set()

    for dirpath, _dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            if not is_cpp_file(filename):
                continue
            file_path = os.path.join(dirpath, filename)
            content = read_file_content(file_path)
            if not content:
                continue

            matches = re.finditer(
                r'virtual\s+'
                r'(?:[\w\s*&:<>,~[\]]+?)\s+'
                r'([~]?\w+)\s*'
                r'\([^)]*\)',
                content
            )
            for m in matches:
                name = m.group(1)
                if not name.startswith("~"):
                    unique_names.add(name)

    return unique_names


def modify_qt_virtuals_in_file(file_path: str, qt_func_names: Set[str]) -> bool:
    content = read_file_content(file_path)
    if not content:
        return False

    original = content
    modified = False

    for func_name in qt_func_names:
        pattern = re.compile(
            rf'virtual\s+'
            rf'('
            rf'(?:(?!\bvirtual\b).)*?'
            rf'\b{re.escape(func_name)}\s*'
            rf'\([^)]*\)'
            rf')'
            rf'([\s\w]*?)'
            rf';',
            re.DOTALL,
        )

        def make_replacement(match, fn=func_name):
            nonlocal modified
            ret_and_params = match.group(1)
            qualifiers = match.group(2)

            new_qualifiers = qualifiers.rstrip()
            has_override = re.search(r'\boverride\b', new_qualifiers)
            has_pure = re.search(r'=\s*0', new_qualifiers)

            if has_pure:
                return f"{ret_and_params}{qualifiers};"

            if not has_override:
                if new_qualifiers and not new_qualifiers.endswith(" "):
                    new_qualifiers += " "
                new_qualifiers += "override"

            modified = True
            return f"{ret_and_params}{new_qualifiers};"

        content = pattern.sub(make_replacement, content)

    if modified:
        with open(file_path, "w", encoding="utf-8") as f:
            f.write(content)
        return True
    return False


def main():
    if len(sys.argv) < 2:
        print("Usage: python extract_virtual_functions.py <target_directory>")
        print("Example: python extract_virtual_functions.py C:/my_project/src")
        sys.exit(1)

    target_dir = sys.argv[1]

    if not os.path.isdir(target_dir):
        print(f"Error: '{target_dir}' is not a valid directory.")
        sys.exit(1)

    print(f"Scanning directory: {target_dir}")
    print(f"Supported extensions: {', '.join(sorted(CPP_EXTENSIONS))}")

    print("\n=== Phase 1: Extracting all virtual function names ===")
    all_virtual_names = extract_all_virtual_names(target_dir)
    print(f"Found {len(all_virtual_names)} unique virtual function names.")

    qt_virtuals = all_virtual_names & QT_VIRTUAL_FUNCTIONS
    custom_virtuals = all_virtual_names - QT_VIRTUAL_FUNCTIONS

    print(f"Qt built-in virtual functions: {len(qt_virtuals)}")
    if qt_virtuals:
        for name in sorted(qt_virtuals):
            print(f"  [Qt] {name}")

    print(f"Custom virtual functions: {len(custom_virtuals)}")
    if custom_virtuals:
        for name in sorted(custom_virtuals):
            print(f"  [Custom] {name}")

    script_dir = os.path.dirname(os.path.abspath(__file__))
    output_path = os.path.join(script_dir, "virtual_functions.txt")

    all_sorted = sorted(all_virtual_names)
    with open(output_path, "w", encoding="utf-8") as f:
        for name in all_sorted:
            tag = "[Qt]" if name in qt_virtuals else "[Custom]"
            f.write(f"{name}\t{tag}\n")

    print(f"\nFunction list written to: {output_path}")

    if not qt_virtuals:
        print("\nNo Qt built-in virtual functions found. Skipping modification phase.")
        return

    print("\n=== Phase 2: Modifying Qt virtual function declarations ===")
    print("Actions: remove 'virtual', add 'override' if missing...")

    modified_count = 0
    for dirpath, _dirnames, filenames in os.walk(target_dir):
        for filename in filenames:
            if not is_cpp_file(filename):
                continue
            file_path = os.path.join(dirpath, filename)
            if modify_qt_virtuals_in_file(file_path, qt_virtuals):
                modified_count += 1
                print(f"  Modified: {file_path}")

    print(f"\nModified {modified_count} file(s).")
    print("Done.")


if __name__ == "__main__":
    main()