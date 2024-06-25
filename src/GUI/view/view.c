#include <windows.h>

#include "../../../headers/view.h"
#include "../../../headers/controller.h"

void CreateConsoleWindow(HWND hWnd) {
    hwndConsoleWindow = CreateWindowW(
        L"EDIT", NULL,
        WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        (WINDOW_WIDTH - 700) / 2, // Center horizontally, assuming 600px wide
        80, // Start a bit below the top edge of the main window
        700, // Width
        400, // Height, making it pretty big but not occupying the entire window
        hWnd, NULL, NULL, NULL);

    // set the font of the console output window to a more console-like font
    SendMessage(hwndConsoleWindow, WM_SETFONT, (WPARAM)GetStockObject(ANSI_FIXED_FONT), TRUE);

    CreateConsoleWindowTopButtons(hWnd);
    CreateConsoleWindowBottomButtons(hWnd);
}

void CreateConsoleWindowTopButtons(HWND hWnd) {
    int yPos = 35; 
    int buttonGap = 105;

    hwndConsoleBtnJoin = CreateWindowW(
        L"BUTTON",  // Predefined class; Button
        L"Join",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        (WINDOW_WIDTH - 700) / 2,         // x position 
        yPos,         // y position; above the console output window
        100,        // Button width
        30,         // Button height
        hWnd,       // Parent window
        (HMENU)1,   // Button ID
        NULL,
        NULL);

    hwndConsoleBtnLeave = CreateWindowW(
        L"BUTTON",  // Predefined class; Button
        L"Leave",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        (WINDOW_WIDTH - 700) / 2 + buttonGap,         // x position 
        yPos,         // y position; above the console output window
        100,        // Button width
        30,         // Button height
        hWnd,       // Parent window
        (HMENU)2,   // Button ID
        NULL,
        NULL);

    buttonGap += 105;

    hwndConsoleBtnHelp = CreateWindowW(
        L"BUTTON",
        L"Help",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        (WINDOW_WIDTH - 700) / 2 + buttonGap,   // x position; next to the Disconnect button
        yPos,         // y position
        100,        // Button width
        30,         // Button height
        hWnd,
        (HMENU)3,
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
        (HMENU)4,
        NULL,
        NULL);

    hwndConsoleBtnRegister = CreateWindowW(
        L"BUTTON",  // Button class
        L"Register",    // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        WINDOW_WIDTH - 150,  // Rightmost position with a 20px margin
        yPos,        // y position
        100,  // Button width
        30,          // Button height
        hWnd,        // Parent window
        (HMENU)5,    // Button ID
        NULL,
        NULL);
}

void CreateConsoleWindowBottomButtons(HWND hWnd) {
    int yPos = 500; 
    int buttonGap = 105;

    // Broadcast button creation
    hwndConsoleBtnBroadcast = CreateWindowW(
        L"BUTTON",
        L"Broadcast",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        (WINDOW_WIDTH - 700) / 2,
        yPos, // Same y position as the Upload button
        100, // Button width
        30,  // Button height
        hWnd,
        (HMENU)6, // Unique Button ID
        NULL,
        NULL);

    // Unicast button creation
    hwndConsoleBtnUnicast = CreateWindowW(
        L"BUTTON",
        L"Unicast",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        (WINDOW_WIDTH - 700) / 2 + buttonGap,
        yPos, // Same y position as the Upload button
        100, // Button width
        30,  // Button height
        hWnd,
        (HMENU)7, // Unique Button ID
        NULL,
        NULL);

    hwndConsoleBtnUpload = CreateWindowW(
        L"BUTTON",  // Button class
        L"Upload ▼",    // No text for a dropdown icon
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles, added BS_SPLITBUTTON for dropdown
        WINDOW_WIDTH - 130,  // Align with the Alias button's x position for consistency
        yPos,        // Align with the Message Box's y position for visual consistency
        80,  // Button width
        30,          // Button height
        hWnd,        // Parent window
        (HMENU)8,    // Button ID
        NULL,
        NULL);
}

void CreateJoinDialog(HWND hWnd) {
    if (IsWindow(hwndJoinDialog)) {
        return;
    }

    hwndJoinDialog = CreateWindowExW(
        0, L"#32770", L"Join Dialog", // Use the predefined dialog box class "#32770"
        WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
        800, 450, 250, 200, // Increased height from 150 to 200
        hWnd, NULL, NULL, NULL);

    // Create a static text control for "IP Address"
    CreateWindowW(
        L"STATIC", L"IP Address:",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 10, 230, 20, // Adjust size and position as needed
        hwndJoinDialog, NULL, NULL, NULL);

    // Create a text box for the IP Address
    hwndJoinDialogEditIp = CreateWindowW(
        L"EDIT", L"",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_CENTER,
        30, 30, 180, 20,
        hwndJoinDialog, (HMENU)1, NULL, NULL);

    // Create a static text control for "Port"
    CreateWindowW(
        L"STATIC", L"Port:",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 70, 230, 20, // Adjust size and position as needed
        hwndJoinDialog, NULL, NULL, NULL);

    // Create a text box for the Port
    hwndJoinDialogEditPort = CreateWindowW(
        L"EDIT", L"",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_CENTER,
        30, 90, 180, 20,
        hwndJoinDialog, (HMENU)2, NULL, NULL);

    // Create an "OK" button inside the dialog
    hwndJoinDialogJoinButton = CreateWindowW(
        L"BUTTON", L"OK",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        75, 130, 100, 30,
        hwndJoinDialog, (HMENU)3, NULL, NULL);

    // Show the dialog
    ShowWindow(hwndJoinDialog, SW_SHOW);
    UpdateWindow(hwndJoinDialog);

    // Set the dialog procedure for custom message handling
    SetWindowLongPtr(hwndJoinDialog, GWLP_WNDPROC, (LONG_PTR)JoinDialogProcedure);
}

void CreateRegisterDialog(HWND hWnd) {
    if (IsWindow(hwndRegisterDialog)) {
        return;
    }

    // Create a modeless dialog or a new window with CreateWindowEx
    hwndRegisterDialog = CreateWindowExW(
        0, L"#32770", NULL, // Use the predefined dialog box class "#32770"
        WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
        800, 450, 250, 150,
        hWnd, NULL, NULL, NULL);

    // Create a static text control for "Change Name"
    CreateWindowW(
        L"STATIC", L"Register Alias",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 10, 230, 20, // Adjust size and position as needed
        hwndRegisterDialog, NULL, NULL, NULL);

    // Create a text box inside the dialog
    hwndRegisterDialogTextBox = CreateWindowW(
        L"EDIT", L"",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_CENTER,
        30, 40, 180, 20,
        hwndRegisterDialog, (HMENU)1, NULL, NULL);

    // Create an "OK" button inside the dialog
    hwndRegisterDialogRegisterButton = CreateWindowW(
        L"BUTTON", L"Register",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        75, 70, 100, 30,
        hwndRegisterDialog, (HMENU)2, NULL, NULL);

    // Show the dialog
    ShowWindow(hwndRegisterDialog, SW_SHOW);
    UpdateWindow(hwndRegisterDialog);

    SetWindowLongPtr(hwndRegisterDialog, GWLP_WNDPROC, (LONG_PTR)RegisterDialogProcedure);
}