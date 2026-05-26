import os
import subprocess
import sys
import argparse
import io

# 修复 Windows 控制台 Unicode 输出问题
if sys.platform == 'win32':
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')
    sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8')
    
def format_cpp_files():
    # ======================== 硬编码默认配置（可修改，命令行会覆盖） ========================
    # 需要格式化的文件后缀（无需修改，除非你要新增）
    cpp_extensions = ['.cc', '.hh', '.h', '.hpp', '.cpp', '.cxx', '.cu']
    
    # 默认 clang-format 路径（命令行 --exe 会覆盖）
    default_clang_format_exe = r'E:\Development\LLVM\bin\clang-format.exe'
    
    # 默认需要处理的文件夹（命令行 --dirs 会覆盖，多个用英文逗号分隔）
    default_target_dirs = [
        r'',
    ]
    
    # 默认 .clang-format 配置文件路径
    clang_format_config = r'.clang-format'
    
    # 默认直接指定格式化的文件（命令行 --files 会覆盖，多个用英文逗号分隔）
    default_target_files = []
    # ====================================================================================

    # ======================== 命令行参数解析 ========================
    parser = argparse.ArgumentParser(description='C++ 代码批量格式化工具（基于 clang-format）')
    parser.add_argument('--exe', type=str, default='', help='clang-format.exe 完整路径，例如：E:/LLVM/bin/clang-format.exe')
    parser.add_argument('--dirs', type=str, default='', help='需要格式化的文件夹，多个用英文逗号分隔')
    parser.add_argument('--files', type=str, default='', help='直接指定格式化的文件，多个用英文逗号分隔')
    args = parser.parse_args()

    # 优先级：命令行参数 > 硬编码默认值
    clang_format_exe = args.exe.strip() if args.exe.strip() else default_clang_format_exe
    target_dirs = [d.strip() for d in args.dirs.split(',') if d.strip()] if args.dirs.strip() else default_target_dirs
    target_files = [f.strip() for f in args.files.split(',') if f.strip()] if args.files.strip() else default_target_files

    # 检查核心参数是否为空
    if not clang_format_exe:
        print("错误：未指定 clang-format.exe 路径！请通过 --exe 参数传入或在代码中配置")
        return 1
    if not target_dirs and not target_files:
        print("错误：未指定任何需要处理的文件夹或文件！请通过 --dirs / --files 传入或在代码中配置")
        return 1

    # 检查 clang-format 可执行文件
    if not os.path.exists(clang_format_exe):
        print(f"错误：找不到 clang-format：{clang_format_exe}")
        return 1

    # 检查格式化配置文件
    if not os.path.exists(clang_format_config):
        print(f"错误：找不到 .clang-format 配置文件：{clang_format_config}")
        return 1

    # ======================== 执行格式化 ========================
    total_files = 0
    success_files = 0
    failed_files = []

    # 处理文件夹（递归）
    for target_dir in target_dirs:
        if not os.path.isdir(target_dir):
            print(f"文件夹不存在，跳过：{target_dir}")
            continue

        print(f"\n开始递归处理文件夹：{target_dir}")
        for root, dirs, files in os.walk(target_dir):
            for file in files:
                file_ext = os.path.splitext(file)[1].lower()
                if file_ext in cpp_extensions:
                    total_files += 1
                    file_path = os.path.join(root, file)
                    process_file(file_path, clang_format_exe, clang_format_config, success_files, failed_files)

    # 处理直接指定的文件
    for file_path in target_files:
        if not os.path.isfile(file_path):
            print(f"文件不存在，跳过：{file_path}")
            continue

        file_ext = os.path.splitext(file_path)[1].lower()
        if file_ext not in cpp_extensions:
            print(f"不是支持的格式，跳过：{file_path}")
            continue

        total_files += 1
        print(f"\n开始处理指定文件：{file_path}")
        process_file(file_path, clang_format_exe, clang_format_config, success_files, failed_files)

    # ======================== 结果输出 ========================
    print("\n" + "=" * 60)
    print("📊 格式化任务完成！")
    print(f"总文件数：{total_files}")
    print(f"成功：{success_files}")
    print(f"失败：{len(failed_files)}")

    if failed_files:
        print("\n失败文件列表：")
        for f in failed_files:
            print(f"  - {f}")

    return 0 if len(failed_files) == 0 else 1

def process_file(file_path, clang_format_exe, config_path, success_files, failed_files):
    """单独封装文件处理函数"""
    try:
        cmd = [
            clang_format_exe,
            '-i',
            f'--style=file:{config_path}',
            file_path
        ]
        subprocess.run(
            cmd,
            check=True,
            capture_output=True,
            text=True,
            encoding='utf-8'
        )
        success_files += 1
        print(f"成功：{file_path}")
    except subprocess.CalledProcessError as e:
        failed_files.append(file_path)
        print(f"格式化失败：{file_path}")
        print(f"   错误：{e.stderr.strip()}")
    except Exception as e:
        failed_files.append(file_path)
        print(f"处理异常：{file_path}")
        print(f"   异常：{str(e)}")

if __name__ == "__main__":
    sys.exit(format_cpp_files())
