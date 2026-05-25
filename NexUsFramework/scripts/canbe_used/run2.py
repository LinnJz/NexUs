import os
import sys
import subprocess
import argparse

# ======================== Default Configuration ========================
TARGET_DIRS = ["NexUs", "NexUsExample"]

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, "../../.."))
FRAMEWORK_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "../.."))

CLANG_FORMAT_EXE = r"E:\Development\LLVM\bin\clang-format.exe"

DRY_RUN = False
CONTINUE_ON_ERROR = False
# =======================================================================


SCRIPTS = [
    "./convert_q_signals/convert_q_signals.py",
    "./qstring_literal/qstring_literal.py",
    "./remove_override/remove_override.py",
    "./reorder_includes/reorder_includes.py",
    "./virtual_functions/virtual_functions.py",
    "../content_replace.py",
    "../format_cpp_files.py",
]


def run_script(script_rel, target_dir, description, cwd=None, extra_args=None):
    script_path = os.path.normpath(os.path.join(SCRIPT_DIR, script_rel))
    cmd = [sys.executable, script_path]

    if target_dir:
        cmd.append(target_dir)

    if extra_args:
        for key, value in extra_args.items():
            cmd.extend([key, value])

    print(f"\n{'=' * 60}")
    print(f"  {description}")
    print(f"  {' '.join(cmd)}")
    print(f"{'=' * 60}")

    if DRY_RUN:
        print("  [DRY-RUN] skipped")
        return 0

    workdir = cwd if cwd else SCRIPT_DIR
    result = subprocess.run(cmd, cwd=workdir)
    if result.returncode != 0:
        print(f"  WARNING: Script exited with code {result.returncode}")
    return result.returncode

#示例：python run2.py -n NexUs 或 python run2.py --continue-on-error NexUs NexUsExample NXPacketIO
def main():
    global DRY_RUN, CONTINUE_ON_ERROR, CLANG_FORMAT_EXE, TARGET_DIRS

    parser = argparse.ArgumentParser(
        description="Run NexUs code transformation pipeline (simplified)")
    parser.add_argument("-n", "--dry-run", action="store_true",
                        help="Print commands without executing")
    parser.add_argument("--continue-on-error", action="store_true",
                        help="Continue on script failure")
    parser.add_argument("--clang-format-exe", default=CLANG_FORMAT_EXE,
                        help="Path to clang-format.exe")
    parser.add_argument("target_dirs", nargs="*",
                        help="Override target directories (default: NexUs, NexUsExample)")
    args = parser.parse_args()

    DRY_RUN = args.dry_run
    CONTINUE_ON_ERROR = args.continue_on_error
    CLANG_FORMAT_EXE = args.clang_format_exe
    if args.target_dirs:
        TARGET_DIRS = args.target_dirs

    total = len(SCRIPTS)

    for dir_idx, target in enumerate(TARGET_DIRS):
        target_abs = os.path.join(REPO_ROOT, target)
        if not os.path.isdir(target_abs):
            print(f"  WARNING: Target directory '{target_abs}' does not exist, skipping")
            continue

        for i, script in enumerate(SCRIPTS[:-1]):
            step = i + 1
            desc = f"{step}/{total} {script} -> {target}"
            rc = run_script(script, target_abs, desc)
            if rc != 0 and not CONTINUE_ON_ERROR:
                print("  Aborting due to previous error.")
                sys.exit(1)

    format_script = SCRIPTS[-1]
    dirs_arg = ",".join(
        os.path.join(REPO_ROOT, d) for d in TARGET_DIRS
        if os.path.isdir(os.path.join(REPO_ROOT, d))
    )
    if dirs_arg:
        rc = run_script(
            format_script, "",
            f"{total}/{total} {format_script}",
            cwd=FRAMEWORK_DIR,
            extra_args={"--exe": CLANG_FORMAT_EXE, "--dirs": dirs_arg}
        )
        if rc != 0 and not CONTINUE_ON_ERROR:
            sys.exit(1)

    print(f"\n{'=' * 60}")
    print("  All scripts completed!")
    print(f"{'=' * 60}")


if __name__ == "__main__":
    main()
