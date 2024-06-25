#include <stdio.h>
#include "../../headers/gui.h"
#include "../../headers/view.h"
#include "../../headers/controller.h"

// console window handlers
HWND hwndConsoleWindow; 
HWND hwndConsoleBtnJoin, hwndConsoleBtnLeave, hwndConsoleBtnHelp, hwndConsoleBtnDir, hwndConsoleBtnRegister;
HWND hwndConsoleBtnBroadcast, hwndConsoleBtnUnicast, hwndConsoleBtnUpload;

// join dialog handlers
HWND hwndJoinDialog, hwndEditIp, hwndEditPort, hwndJoinButton;

// alias change dialog handlers
HWND hwndRegisterDialog, hwndDialogStaticText, hwndDialogTextBox, hwndDialogOkButton;

// flags for open state
int joinDialogOpen = 0;

// main entry point for a windows application
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0}; // initialize a window class structure to zero

    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND; // set the background color for the window
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // set the cursor for the window to the default arrow
    wc.hInstance = hInst; // set the instance handle for the window class
    wc.lpszClassName = L"TestWindowClass"; // set the class name for the window class
    wc.lpfnWndProc = WindowProcedure; // set the window procedure function for the window class

    if (!RegisterClassW(&wc)) // register the window class with the operating system
        return -1; // if registration fails, exit the application

    // create the main application window
    CreateWindowW(
        L"TestWindowClass", 
        L"File Exchange System", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        CENTERED_X, CENTERED_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 
        NULL, 
        NULL, 
        hInst,
        NULL);

    MSG msg = {0}; // initialize a message structure to zero
    while (GetMessage(&msg, NULL, 0, 0)) { // start the message loop
        TranslateMessage(&msg); // translate virtual-key messages into character messages
        DispatchMessage(&msg); // dispatch the message to the window procedure
    }

    return 0; // exit the application
}