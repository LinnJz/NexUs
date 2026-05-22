import os
import shutil

ROOT = r"D:\ElaWidgetTools"
LIB_DIR = os.path.join(ROOT, "ElaWidgetTools")
EXAMPLE_DIR = os.path.join(ROOT, "ElaWidgetToolsExample")

EXCLUDE_DIRS = {
    ".git", ".github", ".vs", "out", "Bin", "doc",
    "PySide6Example", "framework", "bindings", "scripts", "skill"
}


def should_exclude(path):
    parts = path.replace(ROOT, "").lstrip("\\").split("\\")
    return bool(set(parts) & EXCLUDE_DIRS)


def rename_items(root_dir, old_prefix, new_prefix, is_dir=False):
    get_func = os.walk if is_dir else (
        lambda top: (
            (dirpath, f)
            for dirpath, dirs, files in os.walk(top)
            if not should_exclude(dirpath)
            for f in (dirs if is_dir else files)
            if f.startswith(old_prefix)
            and not should_exclude(os.path.join(dirpath, f))
        )
    )

    items = []
    for dirpath, dirs, files in os.walk(root_dir):
        dirs[:] = [d for d in dirs if not should_exclude(os.path.join(dirpath, d))]
        if should_exclude(dirpath):
            continue
        names = dirs if is_dir else files
        for name in names:
            if name.startswith(old_prefix) and not should_exclude(os.path.join(dirpath, name)):
                items.append((dirpath, name))

    if is_dir:
        items.sort(key=lambda x: x[0], reverse=True)

    count = 0
    for dirpath, name in items:
        old_path = os.path.join(dirpath, name)
        new_name = new_prefix + name[len(old_prefix):]
        new_path = os.path.join(dirpath, new_name)
        os.rename(old_path, new_path)
        count += 1
    return count


def restructure_library():
    print("[Phase 1] Restructuring ElaWidgetTools library...")

    os.makedirs(os.path.join(LIB_DIR, "include"), exist_ok=True)
    print("  Created include/")

    for ext in (".h", ".hpp"):
        for f in os.listdir(LIB_DIR):
            if f.endswith(ext):
                src = os.path.join(LIB_DIR, f)
                dst = os.path.join(LIB_DIR, "include", f)
                if os.path.isfile(src):
                    shutil.move(src, dst)
    print("  Moved .h/.hpp files -> include/")

    os.makedirs(os.path.join(LIB_DIR, "Resource"), exist_ok=True)
    for folder in ("Font", "Image"):
        src = os.path.join(LIB_DIR, folder)
        dst = os.path.join(LIB_DIR, "Resource", folder)
        if os.path.isdir(src):
            shutil.move(src, dst)
    print("  Moved Font/ Image/ -> Resource/")

    os.makedirs(os.path.join(LIB_DIR, "Source"), exist_ok=True)
    for folder in ("DeveloperComponents", "private", "include"):
        src = os.path.join(LIB_DIR, folder)
        dst = os.path.join(LIB_DIR, "Source", folder)
        if os.path.isdir(src):
            shutil.move(src, dst)
    print("  Moved DeveloperComponents/ private/ include/ -> Source/")

    for ext in (".cpp", ".mm"):
        for f in os.listdir(LIB_DIR):
            if f.endswith(ext):
                src = os.path.join(LIB_DIR, f)
                dst = os.path.join(LIB_DIR, "Source", f)
                if os.path.isfile(src):
                    shutil.move(src, dst)
    print("  Moved .cpp/.mm files -> Source/")


def restructure_example():
    print("[Phase 2] Restructuring ElaWidgetToolsExample...")

    os.makedirs(os.path.join(EXAMPLE_DIR, "Source"), exist_ok=True)
    for folder in ("ExamplePage", "ModelView"):
        src = os.path.join(EXAMPLE_DIR, folder)
        dst = os.path.join(EXAMPLE_DIR, "Source", folder)
        if os.path.isdir(src):
            shutil.move(src, dst)
    print("  Moved ExamplePage/ ModelView/ -> Source/")

    for ext in (".h", ".cpp"):
        for f in os.listdir(EXAMPLE_DIR):
            if f.endswith(ext):
                src = os.path.join(EXAMPLE_DIR, f)
                dst = os.path.join(EXAMPLE_DIR, "Source", f)
                if os.path.isfile(src):
                    shutil.move(src, dst)
    print("  Moved .h/.cpp files -> Source/")


def rename_ela_to_nx():
    print("[Phase 3] Renaming Ela prefix -> NX...")

    file_count = rename_items(ROOT, "Ela", "NX", is_dir=False)
    print(f"  Renamed {file_count} files")

    dir_count = rename_items(ROOT, "Ela", "NX", is_dir=True)
    print(f"  Renamed {dir_count} directories")


def rename_nxwidgettools_to_nexus():
    print("[Phase 4] Renaming NXWidgetTools prefix -> NexUs...")

    file_count = rename_items(ROOT, "NXWidgetTools", "NexUs", is_dir=False)
    print(f"  Renamed {file_count} files")

    dir_count = rename_items(ROOT, "NXWidgetTools", "NexUs", is_dir=True)
    print(f"  Renamed {dir_count} directories")


def main():
    restructure_library()
    restructure_example()
    rename_ela_to_nx()
    rename_nxwidgettools_to_nexus()
    print("\nAll done!")


if __name__ == "__main__":
    main()
