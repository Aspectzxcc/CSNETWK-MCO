@echo off
taskkill /f /im gui.exe 2>nul
gcc gui.c controller/controller.c view/view.c -o gui.exe -mwindows
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilation successful. Executable created: gui.exe
start gui.exe