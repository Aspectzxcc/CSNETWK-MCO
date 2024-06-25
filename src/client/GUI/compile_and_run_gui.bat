@echo off
taskkill /F /IM client_gui.exe /T >nul 2>&1
gcc -o client_gui.exe client_gui.c -mwindows
if %ERRORLEVEL% neq 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
)
echo Compilation successful. Running the application...
start client_gui.exe