@echo off
setlocal enabledelayedexpansion

set "SCRIPTS_BASE=%~dp0framework\scripts"

echo ^>^>^> Running code_transform.py (NexUs + NexUsExample only)...
python "%SCRIPTS_BASE%\code_transform.py" "%~dp0NexUs" "%~dp0NexUsExample"
if !ERRORLEVEL! neq 0 (
    echo !!! code_transform.py failed with exit code !ERRORLEVEL!
    exit /b !ERRORLEVEL!
)
echo.

echo ^>^>^> Running content_replace.py (NexUs + NexUsExample + NXPacketIO)...
python "%SCRIPTS_BASE%\content_replace.py" "%~dp0."
if !ERRORLEVEL! neq 0 (
    echo !!! content_replace.py failed with exit code !ERRORLEVEL!
    exit /b !ERRORLEVEL!
)
echo.

echo ^>^>^> Running format_cpp_files.py (NexUs + NexUsExample only)...
python "%SCRIPTS_BASE%\format_cpp_files.py" --dirs "%~dp0NexUs,%~dp0NexUsExample"
if !ERRORLEVEL! neq 0 (
    echo !!! format_cpp_files.py failed with exit code !ERRORLEVEL!
    exit /b !ERRORLEVEL!
)
echo.

echo All scripts completed successfully.
