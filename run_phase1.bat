@echo off
setlocal enabledelayedexpansion

set "SCRIPTS=process_eladef.py restructure_dirs.py rename_prefixes.py deploy_framework.py"
set "BASE=%~dp0framework\scripts"

for %%s in (%SCRIPTS%) do (
    echo ^>^>^> Running %%s...
    python "%BASE%\%%s"
    if !ERRORLEVEL! neq 0 (
        echo !!! %%s failed with exit code !ERRORLEVEL!
        exit /b !ERRORLEVEL!
    )
    echo.
)

echo All scripts completed successfully.
