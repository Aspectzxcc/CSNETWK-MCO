@echo off
gcc client.c ../utils/client_command_handler.c ../utils/helpers.c ../utils/commands.c -o client -lws2_32
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilation successful. Executable created: client.exe
pause