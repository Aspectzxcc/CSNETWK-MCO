@echo off
setlocal enabledelayedexpansion
taskkill /F /IM client_gui.exe /T >nul 2>&1

set "source_file=source_files.txt"
set "object_files="

for /F "tokens=*" %%A in (%source_file%) do (
    set "object_files=!object_files! %%A"
)

gcc %object_files% -o client_gui.exe -mwindows -lcomctl32
if %ERRORLEVEL% neq 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
)
echo Compilation successful. Running the application...
start client_gui.exe