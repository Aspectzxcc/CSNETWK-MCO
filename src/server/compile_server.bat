@echo off
gcc server.c -o server -lws2_32
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilation successful. Executable created: server.exe
pause