import os
import shutil

FRAMEWORK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGET = os.path.dirname(FRAMEWORK)

EXCLUDE = {"prompt", "scripts"}


def copy_overwrite(src_dir, dst_dir):
    for item in os.listdir(src_dir):
        if item in EXCLUDE:
            continue
        src = os.path.join(src_dir, item)
        dst = os.path.join(dst_dir, item)
        if os.path.isdir(src):
            os.makedirs(dst, exist_ok=True)
            copy_overwrite(src, dst)
        else:
            shutil.copy2(src, dst)
            print(f"  Copied: {os.path.relpath(dst, TARGET)}")


def main():
    for item in os.listdir(FRAMEWORK):
        if item in EXCLUDE:
            continue
        src = os.path.join(FRAMEWORK, item)
        dst = os.path.join(TARGET, item)
        if os.path.isdir(src):
            if os.path.exists(dst):
                copy_overwrite(src, dst)
            else:
                shutil.copytree(src, dst, ignore=lambda d, c: [x for x in c if x in EXCLUDE])
                print(f"  Created new dir: {item}")
        else:
            shutil.copy2(src, dst)
            print(f"  Copied file: {item}")

    print(f"\nDone. Overwritten existing files from:\n  {FRAMEWORK}\nto:\n  {TARGET}")


if __name__ == "__main__":
    main()
