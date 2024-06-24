#include <windows.h>

#include "../../../headers/view.h"
#include "../../../headers/controller.h"

extern HWND hwndTextBox, hwndButton, hwndHeader;
extern HWND hwndConsoleWindow, hwndConsoleBtnDisconnect, hwndConsoleBtnHelp, hwndConsoleBtnDir, hwndConsoleBtnAlias;
extern HWND hwndDialog, hwndDialogStaticText, hwndDialogTextBox, hwndDialogOkButton;

void CreateGetIpPortWindow(HWND hWnd) {
    hwndHeader = CreateWindowW(
        L"STATIC", // Predefined class; Static control
        L"File Exchange System", // Button text 
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, // Styles; Visible, Child, Centered text, Centered image
        0, // x position; Start from the left edge
        10, // y position; A little down from the top edge
        WINDOW_WIDTH, // Make it as wide as the window
        40, // Height
        hWnd, // Parent window
        (HMENU)0, // No menu.
        NULL, // No instance override
        NULL); // No additional parameters

    // Create a larger font
    HFONT hFont = CreateFontW(
        24, // nHeight
        0, // nWidth
        0, // nEscapement
        0, // nOrientation
        FW_BOLD, // nWeight
        FALSE, // bItalic
        FALSE, // bUnderline
        0, // cStrikeOut
        ANSI_CHARSET, // nCharSet
        OUT_DEFAULT_PRECIS, // nOutPrecision
        CLIP_DEFAULT_PRECIS, // nClipPrecision
        DEFAULT_QUALITY, // nQuality
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
        L"Arial"); // lpszFacename

    // Set the font for the static control
    SendMessage(hwndHeader, WM_SETFONT, (WPARAM)hFont, TRUE);

    hwndTextBox = CreateWindowW(
        L"EDIT", 
        L"", 
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 
        (WINDOW_WIDTH - 400) / 2, // Center horizontally
        250, // Fixed vertical position
        400, // Width
        25, 
        hWnd, 
        (HMENU)1, 
        NULL, 
        NULL); // create a text box control

    SetFocus(hwndTextBox); // set focus to the text box control

    hwndButton = CreateWindowW(
        L"BUTTON", L"Join", 
        WS_CHILD | WS_VISIBLE, 
        (WINDOW_WIDTH - 110) / 2, // Center horizontally
        290, // Below the text box with some spacing
        100, // Width
        30, // Height
        hWnd, (HMENU)2, NULL, NULL); // create a button control
}

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
}

void CreateAliasChangeDialog(HWND hWnd) {
    // Create a modeless dialog or a new window with CreateWindowEx
    hwndDialog = CreateWindowExW(
        0, L"#32770", NULL, // Use the predefined dialog box class "#32770"
        WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
        750, 450, 250, 150,
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
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
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