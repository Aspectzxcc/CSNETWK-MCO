@echo off
gcc client.c ../utils/command_handler.c -o client -lws2_32
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilation successful. Executable created: client.exe
pause