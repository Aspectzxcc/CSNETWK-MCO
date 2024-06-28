#include "../../../../headers/components.h"

// Function to create and display a custom dialog box for Command Builder
void CreateCommandBuilderDialog(HWND parentHwnd, HINSTANCE hInst) {
    // Dialog box dimensions, adjusted to match the Broadcast Dialog
    int dialogWidth = 400;
    int dialogHeight = 220; // Adjusted height to accommodate larger edit window

    // Create a basic window to serve as a dialog
    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Command Builder Dialog",
                                WS_VISIBLE | WS_SYSMENU | WS_CAPTION | DS_MODALFRAME | WS_POPUP,
                                CW_USEDEFAULT, CW_USEDEFAULT, dialogWidth, dialogHeight,
                                parentHwnd, NULL, hInst, NULL);

    // Ensure the window was created
    if (!hDlg) return;

    // Get the dimensions of the parent window
    RECT parentRect;
    GetWindowRect(parentHwnd, &parentRect);

    // Calculate the center position
    int x = parentRect.left + (parentRect.right - parentRect.left - dialogWidth) / 2;
    int y = parentRect.top + (parentRect.bottom - parentRect.top - dialogHeight) / 2;

    // Set the position of the dialog box to be centered on the parent window
    SetWindowPos(hDlg, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    // Create UI elements for command building (e.g., text fields, combo boxes)
    CreateWindowExW(0, L"STATIC", L"Command:", 
                    WS_VISIBLE | WS_CHILD, 
                    10, 10, 380, 20, 
                    hDlg, NULL, hInst, NULL);

    HWND hEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
                    10, 40, 380, 90, // Increased height for the edit window
                    hDlg, (HMENU)107, hInst, NULL); 

    // After creating the console output window
    HFONT hFont = CreateFontW(
        16,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                 // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        FIXED_PITCH | FF_MODERN,   // nPitchAndFamily
        L"Consolas");              // lpszFacename

    SendMessageW(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Adjust the "Execute" button position to match the increased height of the edit window
    CreateWindowExW(0, L"BUTTON", L"Execute", 
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
                    160, 140, 80, 25, // Adjusted Y position for the button
                    hDlg, (HMENU)IDOK, hInst, NULL);

    SetFocus(hEdit); // Set focus to the Edit control

    // Make the window modal
    EnableWindow(parentHwnd, FALSE);
    SetWindowLongPtrW(hDlg, GWLP_WNDPROC, (LONG_PTR)CommandBuilderDialogProcedure);
    
    // Message loop for the dialog
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsDialogMessage(hDlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Re-enable the parent window
    EnableWindow(parentHwnd, TRUE);
    SetForegroundWindow(parentHwnd);
    DestroyWindow(hDlg);
}

LRESULT CALLBACK CommandBuilderDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CTLCOLOREDIT: {
            if ((HWND)lParam == GetDlgItem(hwnd, 107)) { // Ensure it's the edit control
                HDC hdcEdit = (HDC)wParam;
                SetTextColor(hdcEdit, RGB(255, 255, 255)); // Set text color to white
                SetBkColor(hdcEdit, RGB(0, 0, 0)); // Set background color to black

                // Create or use a static brush for the background color
                static HBRUSH hBrushBlack = NULL;
                if (!hBrushBlack) {
                    hBrushBlack = CreateSolidBrush(RGB(0, 0, 0)); // Create a black brush
                }
                return (INT_PTR)hBrushBlack;
            }
        }
        break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK: {
                    wchar_t command[256];
                    GetDlgItemTextW(hwnd, 107, command, 256); // 107 is the Edit control ID for command input
                    
                    HandleCommand(command); // Handle the command
                    
                    DestroyWindow(hwnd); // Close the dialog
                    return TRUE;
                }
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return TRUE;
        case WM_DESTROY:
            PostQuitMessage(0); // Ends the modal loop initiated in CreateCommandBuilderDialog
            return TRUE;
    }
    return DefWindowProc(hwnd, message, wParam, lParam); // Default message handling
}