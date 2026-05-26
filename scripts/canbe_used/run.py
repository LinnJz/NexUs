#!/usr/bin/env python3
import os
import subprocess
import sys
import tempfile


def run(script_path, args, description, cwd=None):
    cmd = [sys.executable, script_path] + args
    print(f"\n{'=' * 60}")
    print(f"  {description}")
    print(f"  {' '.join(cmd)}")
    print(f"{'=' * 60}")
    result = subprocess.run(cmd, cwd=cwd)
    if result.returncode != 0:
        print(f"  WARNING: Script exited with code {result.returncode}")
    return result.returncode


def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    root_path = os.path.dirname("D:/ElaWidgetTools/")

    ela_tools = os.path.join(root_path, "ElaWidgetTools")
    ela_example = os.path.join(root_path, "ElaWidgetToolsExample")
    ela_packet_io = os.path.join(root_path, "ElaPacketIO")

    nex_us = os.path.join(root_path, "NexUs")
    nex_us_example = os.path.join(root_path, "NexUsExample")
    nx_packet_io = os.path.join(root_path, "NXPacketIO")

    total_steps = 14  # 实际独立步骤总数

    # 1/14 process_eladef
    run(os.path.join(script_dir, "process_eladef", "process_eladef.py"),
        [ela_tools], f"1/{total_steps} process_eladef.py")

    # 2/14 convert_q_signals
    run(os.path.join(script_dir, "convert_q_signals", "convert_q_signals.py"),
        [ela_tools], f"2/{total_steps} convert_q_signals.py")

    # 3/14 qstring_literal
    run(os.path.join(script_dir, "qstring_literal", "qstring_literal.py"),
        [ela_tools], f"3/{total_steps} qstring_literal.py")

    # 4/14 remove_override
    for d in [ela_tools, ela_example]:
        run(os.path.join(script_dir, "remove_override", "remove_override.py"),
            [d], f"4/{total_steps} remove_override.py -> {os.path.basename(d)}")

    # 5/14 remove_shadow_ifdef
    for d in [ela_tools, ela_example]:
        run(os.path.join(script_dir, "remove_shadow_ifdef", "remove_shadow_ifdef.py"),
            [d], f"5/{total_steps} remove_shadow_ifdef.py -> {os.path.basename(d)}")

    # 6/14 reorder_includes
    for d in [ela_tools, ela_example]:
        run(os.path.join(script_dir, "reorder_includes", "reorder_includes.py"),
            [d], f"6/{total_steps} reorder_includes.py -> {os.path.basename(d)}")

    # 7/14 virtual_functions
    run(os.path.join(script_dir, "virtual_functions", "virtual_functions.py"),
        [ela_tools], f"7/{total_steps} virtual_functions.py")

    # 8/14 recognized
    run(os.path.join(script_dir, "recognized.py"),
        [root_path], f"8/{total_steps} recognized.py")

    # 9/14 organize_project
    for d in [ela_tools, ela_example]:
        run(os.path.join(script_dir, "organize_project.py"),
            [d], f"9/{total_steps} organize_project.py -> {os.path.basename(d)}")

    # 10/14 rename_script (pattern mode)
    for d in [ela_tools, ela_example, ela_packet_io]:
        run(os.path.join(script_dir, "rename_script.py"),
            ["1", "--path", d, "--old", "Ela", "--new", "NX"],
            f"10/{total_steps} rename_script.py (pattern) -> {os.path.basename(d)}")

    # 10/14 rename_script (mapping mode)
    mapping_csv = os.path.join(tempfile.gettempdir(), "nx_rename_mapping.csv")
    with open(mapping_csv, "w", encoding="utf-8") as f:
        f.write(f"{ela_tools},{nex_us}\n")
        f.write(f"{ela_example},{nex_us_example}\n")
        f.write(f"{ela_packet_io},{nx_packet_io}\n")
        f.write(f"{nex_us_example}/NXWidgetToolsExample.qrc,{nex_us_example}/NexUsExample.qrc\n")
        f.write(f"{nex_us}/NXWidgetTools.qrc,{nex_us}/NexUs.qrc\n")
        f.write(f"{nex_us}/NXWidgetToolsConfig.cmake.in,{nex_us}/NexUsConfig.cmake.in\n")

    run(os.path.join(script_dir, "rename_script.py"),
        ["2", "--mapping", mapping_csv],
        f"10/{total_steps} rename_script.py (mapping)")

    os.unlink(mapping_csv)

    # 11/14 move_header.py
    run(os.path.join(script_dir, "move_header.py"),
        [os.path.join(nex_us, "Source"), "-d", "include"],
        f"11/{total_steps} move_header.py -> {os.path.basename(nex_us)}/Source")

    # 12/14 copy_script
    parent_dir = os.path.dirname(script_dir)
    run(os.path.join(script_dir, "copy_script.py"),
        [root_path,
         os.path.join(parent_dir, "cmake"),
         os.path.join(parent_dir, "NexUs"),
         os.path.join(parent_dir, "NexUsExample"),
         os.path.join(parent_dir, "NXPacketIO"),
         os.path.join(parent_dir, ".clang-format"),
         os.path.join(parent_dir, "CMakeLists.txt"),
         os.path.join(parent_dir, "CMakeUserPresets.json")],
        f"12/{total_steps} copy_script.py")

    # 13/14 content_replace
    run(os.path.join(script_dir, "content_replace.py"),
        [root_path], f"13/{total_steps} content_replace.py")

    # 14/14 format_cpp_files
    run(os.path.join(script_dir, "format_cpp_files.py"),
        ["--dirs", f"{nex_us},{nex_us_example},{nx_packet_io}"],
        f"14/{total_steps} format_cpp_files.py",
        cwd=root_path)

    print(f"\n{'=' * 60}")
    print("  All scripts completed!")
    print(f"{'=' * 60}")


if __name__ == "__main__":
    main()
