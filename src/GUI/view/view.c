#include <windows.h>

#include "../../../headers/view.h"
#include "../../../headers/controller.h"

// console window handlers
HWND hwndConsoleWindow, hwndConsoleBtnDisconnect, hwndConsoleBtnHelp, hwndConsoleBtnDir, hwndConsoleBtnAlias;
HWND hwndMessageBox, hwndUploadBtn;

// alias change dialog handlers
HWND hwndDialog, hwndDialogStaticText, hwndDialogTextBox, hwndDialogOkButton;

void CreateConsoleWindow(HWND hWnd) {
    int yPos = 35; 
    int buttonGap = 105;

    hwndConsoleWindow = CreateWindowW(
        L"EDIT", NULL,
        WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        (WINDOW_WIDTH - 700) / 2, // Center horizontally, assuming 600px wide
        70, // Start a bit below the top edge of the main window
        700, // Width
        350, // Height, making it pretty big but not occupying the entire window
        hWnd, (HMENU)3, NULL, NULL);

    // set the font of the console output window to a more console-like font
    SendMessage(hwndConsoleWindow, WM_SETFONT, (WPARAM)GetStockObject(ANSI_FIXED_FONT), TRUE);

    hwndConsoleBtnDisconnect = CreateWindowW(
        L"BUTTON",  // Predefined class; Button
        L"Disconnect",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        (WINDOW_WIDTH - 700) / 2,         // x position 
        yPos,         // y position; above the console output window
        100,        // Button width
        30,         // Button height
        hWnd,       // Parent window
        (HMENU)4,   // Button ID
        NULL,
        NULL);

    hwndConsoleBtnHelp = CreateWindowW(
        L"BUTTON",
        L"Help",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        (WINDOW_WIDTH - 700) / 2 + buttonGap,   // x position; next to the Disconnect button
        yPos,         // y position
        100,        // Button width
        30,         // Button height
        hWnd,
        (HMENU)5,
        NULL,
        NULL);

    buttonGap += 105;

    hwndConsoleBtnDir = CreateWindowW(
        L"BUTTON",
        L"Directory",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        (WINDOW_WIDTH - 700) / 2 + buttonGap,   // x position; next to the Request Commands button
        yPos,         // y position
        100,        // Button width
        30,         // Button height
        hWnd,
        (HMENU)6,
        NULL,
        NULL);

    hwndConsoleBtnAlias = CreateWindowW(
        L"BUTTON",  // Button class
        L"Alias",    // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        WINDOW_WIDTH - 150,  // Rightmost position with a 20px margin
        yPos,        // y position
        100,  // Button width
        30,          // Button height
        hWnd,        // Parent window
        (HMENU)7,    // Button ID
        NULL,
        NULL);

    hwndMessageBox = CreateWindowW(
        L"EDIT", NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        (WINDOW_WIDTH - 500) / 2 - 100, // Adjusted for a narrower width, centering it
        450, // Position below the console window
        550, // Reduced width for the message box
        30, // Height remains the same
        hWnd, (HMENU)8, NULL, NULL);

    hwndUploadBtn = CreateWindowW(
        L"BUTTON",  // Button class
        L"Upload ▼",    // No text for a dropdown icon
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles, added BS_SPLITBUTTON for dropdown
        WINDOW_WIDTH - 130,  // Align with the Alias button's x position for consistency
        450,        // Align with the Message Box's y position for visual consistency
        80,  // Button width
        30,          // Button height
        hWnd,        // Parent window
        (HMENU)9,    // Button ID
        NULL,
        NULL);
}

void CreateAliasChangeDialog(HWND hWnd) {
    // Create a modeless dialog or a new window with CreateWindowEx
    hwndDialog = CreateWindowExW(
        0, L"#32770", NULL, // Use the predefined dialog box class "#32770"
        WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
        800, 450, 250, 150,
        hWnd, NULL, NULL, NULL);

    // Create a static text control for "Change Name"
    hwndDialogStaticText = CreateWindowW(
        L"STATIC", L"Change Alias",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 10, 230, 20, // Adjust size and position as needed
        hwndDialog, NULL, NULL, NULL);

    // Create a text box inside the dialog
    hwndDialogTextBox = CreateWindowW(
        L"EDIT", L"",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_CENTER,
        30, 40, 180, 20,
        hwndDialog, (HMENU)8, NULL, NULL);

    // Create an "OK" button inside the dialog
    hwndDialogOkButton = CreateWindowW(
        L"BUTTON", L"OK",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        75, 70, 100, 30,
        hwndDialog, (HMENU)9, NULL, NULL);

    // Show the dialog
    ShowWindow(hwndDialog, SW_SHOW);
    UpdateWindow(hwndDialog);

    SetWindowLongPtr(hwndDialog, GWLP_WNDPROC, (LONG_PTR)DialogProcedure);
}