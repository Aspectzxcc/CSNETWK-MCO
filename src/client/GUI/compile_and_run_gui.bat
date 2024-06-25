@echo off
taskkill /F /IM client_gui.exe /T >nul 2>&1
gcc client_gui.c components/main_window.c components/join_dialog.c -o client_gui.exe -mwindows -lcomctl32
if %ERRORLEVEL% neq 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
)
echo Compilation successful. Running the application...
start client_gui.exe