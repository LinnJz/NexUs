@echo off
chcp 65001 > nul
setlocal enabledelayedexpansion

:: 1. 获取当前bat脚本的所在目录（即两个Python脚本的所在目录）
set "SCRIPT_DIR=%~dp0"
:: 去除目录末尾的反斜杠（避免路径拼接异常）
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

:: 2. 获取脚本目录的父目录（即基准递归目录，如E:\NexUs\doc -> E:\NexUs）
for %%i in ("!SCRIPT_DIR!\..") do set "BASE_DIR=%%~fi"

:: 3. 打印路径信息，方便验证
echo =============================================
echo 脚本所在目录：!SCRIPT_DIR!
echo 基准递归目录（父目录）：!BASE_DIR!
echo =============================================
echo.

:: 4. 依次运行两个Python脚本，传递基准目录作为参数
echo 【第一步】运行文件/文件夹重命名脚本：rename_script.py
python "!SCRIPT_DIR!\rename_script.py" "!BASE_DIR!" Ela NX
if errorlevel 1 (
    echo 警告：rename_script.py 运行出错！
    pause
    exit /b 1
)
echo 第一步执行完成！
echo.

echo 【第二步】运行文件内容替换脚本：content_replace_script.py
python "!SCRIPT_DIR!\content_replace_script.py" "!BASE_DIR!"
if errorlevel 1 (
    echo 警告：content_replace_script.py 运行出错！
    pause
    exit /b 1
)
echo 第二步执行完成！
echo.

:: 5. 新增第三步：运行C++文件格式化脚本
echo 【第三步】运行C++文件格式化脚本：format_cpp_files.py
python "!SCRIPT_DIR!\format_cpp_files.py"
if errorlevel 1 (
    echo 警告：format_cpp_files.py 运行出错！
    pause
    exit /b 1
)
echo 第三步执行完成！
echo.

:: 6. 执行完成提示
echo =============================================
echo 所有脚本均已成功执行完毕！
echo 基准递归目录：!BASE_DIR!
echo =============================================
pause
