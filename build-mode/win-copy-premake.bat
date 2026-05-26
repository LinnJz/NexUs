@echo off
REM copy-premake.bat - 拷贝PreMake文件夹内容到上级目录

set "SCRIPT_DIR=%~dp0"
set "SOURCE_DIR=%SCRIPT_DIR%PreMake"
set "TARGET_DIR=%SCRIPT_DIR%.."

echo 脚本目录: %SCRIPT_DIR%
echo 源目录: %SOURCE_DIR%
echo 目标目录: %TARGET_DIR%

if exist "%SOURCE_DIR%" (
    echo 正在从 %SOURCE_DIR% 拷贝文件到 %TARGET_DIR%
    
    xcopy "%SOURCE_DIR%" "%TARGET_DIR%" /E /I /Y
    
    if %errorlevel% equ 0 (
        echo PreMake文件夹内容拷贝完成！
    ) else (
        echo 拷贝过程中可能出现问题，请检查文件。
    )
) else (
    echo 错误：PreMake文件夹不存在！
    exit /b 1
)