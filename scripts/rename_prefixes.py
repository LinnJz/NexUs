import os

ROOT = r"E:\ElaWidgetTools"

EXCLUDE_DIRS = {
".git", ".github", ".vs", "out", "Bin", "doc",
"PySide6Example", "framework", "bindings", "scripts", "skill"
}


def should_exclude(path):
    parts = path.replace(ROOT, "").lstrip("\\").split("\\")
    return bool(set(parts) & EXCLUDE_DIRS)


def rename_items(root_dir, old_prefix, new_prefix, is_dir=False):
    items = []
    for dirpath, dirs, files in os.walk(root_dir):
        dirs[:] = [d for d in dirs if not should_exclude(os.path.join(dirpath, d))]
        if should_exclude(dirpath):
            continue
        names = dirs if is_dir else files
        for name in names:
            if name.startswith(old_prefix):
                items.append((dirpath, name))

    if is_dir:
        items.sort(key=lambda x: x[0], reverse=True)

    count = 0
    for dirpath, name in items:
        old_path = os.path.join(dirpath, name)
        new_name = new_prefix + name[len(old_prefix):]
        new_path = os.path.join(dirpath, new_name)
        os.rename(old_path, new_path)
        print(f"    Renamed {'dir' if is_dir else 'file'}: {name} -> {new_name}")
        count += 1
    return count


def main():
    print("=" * 50)
    print("[Phase 3] Renaming Ela prefix -> NX...")
    file_count = rename_items(ROOT, "Ela", "NX", is_dir=False)
    print(f"  Renamed {file_count} files")
    dir_count = rename_items(ROOT, "Ela", "NX", is_dir=True)
    print(f"  Renamed {dir_count} directories")

    print("=" * 50)
    print("[Phase 3.5] Renaming T_Ela prefix -> T_NX...")
    file_count = rename_items(ROOT, "T_Ela", "T_NX", is_dir=False)
    print(f"  Renamed {file_count} files")

    print("=" * 50)
    print("[Phase 4] Renaming NXWidgetTools prefix -> NexUs...")
    file_count = rename_items(ROOT, "NXWidgetTools", "NexUs", is_dir=False)
    print(f"  Renamed {file_count} files")
    dir_count = rename_items(ROOT, "NXWidgetTools", "NexUs", is_dir=True)
    print(f"  Renamed {dir_count} directories")

    print("\nAll done!")


if __name__ == "__main__":
    main()
