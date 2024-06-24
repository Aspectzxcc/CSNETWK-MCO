@echo off
gcc gui_test.c -o gui.exe -mwindows
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilation successful. Executable created: gui.exe
pause