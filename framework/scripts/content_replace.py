import os
import re

def replace_file_content(root_dir):
    # 1. 定义需要处理的文件类型（严格按顺序，不可更改）
    target_extensions = ['.h', '.cpp', '.cc', '.hh', '.hpp']
    ext_set = set(target_extensions)

    # 2. 定义文件夹过滤规则（保持你之前的配置）
    mandatory_include_dirs = {'NexUs', 'NexUsExample', 'NXPacketIO'}
    exclude_dirs = {'Bin', 'out', '.vs', 'GeneratedFiles', 'ThirdParty'}

    # 3. 定义替换规则（关键修改：调整":/include的匹配方式）
    full_word_rules_1 = [
        # 移除":/include的全词匹配，后续单独处理；或直接删除该行，移到专属匹配中
        ('eApp', 'nxApp'),
        ('eWinHelper', 'nxWinHelper'),
        ('eTheme', 'nxTheme'),
        ('getCurrentColorRGB', 'getCurrent4ChannelColor'),
        ('ElaAwesome', 'NXAwesome')
    ]

    # 新增：专属匹配":/include（精确子串匹配，无\b边界）
    special_rules = [
        ('":/include', '":/Resource'),
    ]

    partial_rules = [
        ('ElaWidgetTools', 'NexUs'),
        ('ELAWIDGETTOOLS', 'NEXUS'),
        ('ELAWIDGETTOOLSEXAMPLE', 'NEXUSEXAMPLE'),
        ('ELA', 'NX'),
        ('Ela', 'NX'),
    ]

    full_word_rules_2 = [
        ('QNXpsedTimer', 'QElapsedTimer'),
        ('TCP_NODNXY', 'TCP_NODELAY')
    ]

    # 4. 递归遍历目录（保持原有过滤逻辑，不变）
    target_files_by_ext = {ext: [] for ext in target_extensions}

    def recursive_walk(current_dir):
        for item in os.listdir(current_dir):
            item_full_path = os.path.join(current_dir, item)
            if os.path.isdir(item_full_path):
                if item in exclude_dirs:
                    print(f"  跳过排除文件夹：{item_full_path}")
                    continue
                current_dir_name = os.path.basename(current_dir)
                item_dir_name = item
                if (current_dir_name in mandatory_include_dirs) or \
                   (any(inc_dir in current_dir for inc_dir in mandatory_include_dirs)):
                    print(f"  递归处理文件夹：{item_full_path}")
                    recursive_walk(item_full_path)
                else:
                    if item_dir_name not in mandatory_include_dirs:
                        print(f"  跳过非核心文件夹：{item_full_path}")
                        continue
                    recursive_walk(item_full_path)
            elif os.path.isfile(item_full_path):
                file_ext = os.path.splitext(item)[1]
                if file_ext in ext_set:
                    if any(inc_dir in item_full_path for inc_dir in mandatory_include_dirs):
                        target_files_by_ext[file_ext].append(item_full_path)

    for item in os.listdir(root_dir):
        item_full_path = os.path.join(root_dir, item)
        if os.path.isdir(item_full_path) and item in mandatory_include_dirs:
            print(f"\n开始扫描核心文件夹：{item_full_path}")
            recursive_walk(item_full_path)
        else:
            print(f"跳过非核心根目录项：{item_full_path}")

    # 5. 处理文件（关键修改：先执行special_rules匹配":/include）
    for ext in target_extensions:
        file_list = target_files_by_ext[ext]
        file_list = list(set(file_list))
        if not file_list:
            print(f"\n【{ext}】类型无匹配文件，跳过")
            continue
        print(f"\n开始处理【{ext}】类型文件，共{len(file_list)}个")

        for file_path in file_list:
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()

                is_modified = False
                original_content = content

                # 新增步骤：先执行专属规则（精确子串匹配，无\b，确保":/include生效）
                for old_str, new_str in special_rules:
                    new_content = content.replace(old_str, new_str)
                    if new_content != content:
                        is_modified = True
                        content = new_content

                # 第一步：执行全词匹配规则组1（其他规则不变）
                for old_str, new_str in full_word_rules_1:
                    pattern_str = r'\b' + re.escape(old_str) + r'\b'
                    pattern = re.compile(pattern_str)
                    new_content = pattern.sub(new_str, content)
                    if new_content != content:
                        is_modified = True
                        content = new_content

                # 第二步：执行部分匹配规则（不变）
                for old_str, new_str in partial_rules:
                    new_content = content.replace(old_str, new_str)
                    if new_content != content:
                        is_modified = True
                        content = new_content

                # 第三步：执行全词匹配规则组2（不变）
                for old_str, new_str in full_word_rules_2:
                    pattern_str = r'\b' + re.escape(old_str) + r'\b'
                    pattern = re.compile(pattern_str)
                    new_content = pattern.sub(new_str, content)
                    if new_content != content:
                        is_modified = True
                        content = new_content

                if is_modified:
                    with open(file_path, 'w', encoding='utf-8') as f:
                        f.write(content)
                    print(f"  已修改文件：{file_path}")
                else:
                    print(f"  无修改文件：{file_path}")

            except Exception as e:
                print(f"  处理文件失败：{file_path}，错误信息：{str(e)}")

    print("\n所有指定类型文件处理完成！")

def main():
    import sys
    # 检查命令行参数
    if len(sys.argv) != 2:
        print("使用方法：python content_replace.py <目标根目录路径>")
        print("示例：python content_replace.py D:\\project\\test")
        print("示例（Mac/Linux）：python content_replace.py /Users/xxx/project/test")
        sys.exit(1)

    root_dir = sys.argv[1]
    # 检查目录是否存在
    if not os.path.isdir(root_dir):
        print(f"错误：目录 '{root_dir}' 不存在或不是有效目录！")
        sys.exit(1)

    # 执行替换操作
    replace_file_content(root_dir)

if __name__ == "__main__":
    main()
