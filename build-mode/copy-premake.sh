#!/bin/bash
# copy-premake.sh - 拷贝PreMake文件夹内容到上级目录

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$SCRIPT_DIR/PreMake"
TARGET_DIR="$(dirname "$SCRIPT_DIR")"

echo "脚本目录: $SCRIPT_DIR"
echo "源目录: $SOURCE_DIR"
echo "目标目录: $TARGET_DIR"

if [ -d "$SOURCE_DIR" ]; then
    echo "正在从 $SOURCE_DIR 拷贝文件到 $TARGET_DIR"
    
    if command -v rsync >/dev/null 2>&1; then
        rsync -av "$SOURCE_DIR"/ "$TARGET_DIR"/
    else
        cp -R "$SOURCE_DIR"/* "$TARGET_DIR"/ 2>/dev/null || true
        cp -R "$SOURCE_DIR"/.* "$TARGET_DIR"/ 2>/dev/null || true
    fi
    
    echo "PreMake文件夹内容拷贝完成！"
else
    echo "错误：PreMake文件夹不存在！"
    exit 1
fi