import os
import subprocess
import sys

def format_cpp_files():
    # ======================== 配置项（根据你的需求修改） ========================
    # 需要格式化的文件后缀
    cpp_extensions = ['.cc', '.hh', '.h', '.hpp', '.cpp', '.cxx', '.cu']
    # 需要处理的目标文件夹列表
    target_dirs = [
        r'E:\NexUs\NexUs\Source',
        r'E:\NexUs\NexUsExample\Source'
    ]
    # clang-format可执行文件路径
    clang_format_exe = r'E:\Development\LLVM\bin\clang-format.exe'
    # .clang-format配置文件路径
    clang_format_config = r'D:\Code\PDFWiz\.clang-format'
    # ==========================================================================

    # 检查clang-format可执行文件是否存在
    if not os.path.exists(clang_format_exe):
        print(f"错误：找不到clang-format可执行文件，请检查路径：{clang_format_exe}")
        return 1

    # 检查.clang-format配置文件是否存在
    if not os.path.exists(clang_format_config):
        print(f"错误：找不到.clang-format配置文件，请检查路径：{clang_format_config}")
        return 1

    # 统计变量
    total_files = 0
    success_files = 0
    failed_files = []

    # 遍历每个目标文件夹
    for target_dir in target_dirs:
        if not os.path.isdir(target_dir):
            print(f"警告：文件夹不存在，跳过：{target_dir}")
            continue
        
        print(f"\n开始处理文件夹：{target_dir}")
        
        # 递归遍历文件夹中的所有文件
        for root, dirs, files in os.walk(target_dir):
            for file in files:
                # 获取文件后缀（小写）
                file_ext = os.path.splitext(file)[1].lower()
                
                # 筛选出需要格式化的文件
                if file_ext in cpp_extensions:
                    total_files += 1
                    file_path = os.path.join(root, file)
                    
                    try:
                        # 构建clang-format命令
                        # -i：直接修改文件（原地格式化）
                        # --style=file:<配置文件路径>：指定配置文件
                        cmd = [
                            clang_format_exe,
                            '-i',
                            f'--style=file:{clang_format_config}',
                            file_path
                        ]
                        
                        # 执行命令
                        result = subprocess.run(
                            cmd,
                            check=True,
                            capture_output=True,
                            text=True,
                            encoding='utf-8'
                        )
                        
                        success_files += 1
                        print(f"✅ 格式化成功：{file_path}")
                        
                    except subprocess.CalledProcessError as e:
                        failed_files.append(file_path)
                        print(f"❌ 格式化失败：{file_path}")
                        print(f"   错误信息：{e.stderr}")
                    except Exception as e:
                        failed_files.append(file_path)
                        print(f"❌ 处理异常：{file_path}")
                        print(f"   异常信息：{str(e)}")

    # 输出统计结果
    print("\n" + "="*50)
    print(f"格式化完成！")
    print(f"总文件数：{total_files}")
    print(f"成功数：{success_files}")
    print(f"失败数：{len(failed_files)}")
    
    if failed_files:
        print("\n失败的文件列表：")
        for failed_file in failed_files:
            print(f"  - {failed_file}")
    
    return 0 if len(failed_files) == 0 else 1

if __name__ == "__main__":
    sys.exit(format_cpp_files())
