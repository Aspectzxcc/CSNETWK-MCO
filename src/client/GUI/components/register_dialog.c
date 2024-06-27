#include "../../../../headers/components.h"

HWND g_registerHwnd; // Global variable to store the handle of the Register window

// Function to create and display a custom dialog box for Register with Alias
void CreateRegisterDialog(HWND parentHwnd, HINSTANCE hInst, HWND registerHwnd) {
    // Dialog box dimensions
    int dialogWidth = 200;
    int dialogHeight = 120;
    // Create a basic window to serve as a dialog
    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Register Dialog",
                                WS_VISIBLE | WS_SYSMENU | WS_CAPTION | DS_MODALFRAME | WS_POPUP,
                                CW_USEDEFAULT, CW_USEDEFAULT, dialogWidth, dialogHeight,
                                parentHwnd, NULL, hInst, NULL);

    g_registerHwnd = registerHwnd; // Store the handle of the Register window

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

    // Adjusted Edit control for inputting the alias
    HWND hEditAlias = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER,
                                    10, 10, 180, 25, // Adjusted size to fit new dialog dimensions
                                    hDlg, (HMENU)106, hInst, NULL);

    // Adjusted "Register" button position and size
    HWND hBtnRegister = CreateWindowExW(0, L"BUTTON", L"Register",
                                        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                        60, 50, 80, 25, // Adjusted position to fit within the dialog
                                        hDlg, (HMENU)IDOK, hInst, NULL);

    SetFocus(hEditAlias); // Set focus to the Edit control for input

    // Make the window modal
    EnableWindow(parentHwnd, FALSE);
    SetWindowLongPtrW(hDlg, GWLP_WNDPROC, (LONG_PTR)RegisterDialogProcedure);

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

LRESULT CALLBACK RegisterDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK: {
                    wchar_t alias[256], command[DEFAULT_BUFLEN];
                    GetDlgItemTextW(hwnd, 106, alias, 256); // 106 is the Edit control ID

                    wsprintfW(command, L"%ls %ls", COMMAND_REGISTER_W, alias);
                    AppendTextToConsoleOutput(g_hConsoleOutput, command);
                    AppendTextToConsoleOutput(g_hConsoleOutput, L"\n");

                    int success = HandleCommand(command); // Handle the command

                    if (success) {
                        SetWindowTextW(g_registerHwnd, alias); // Set the alias as the new text
                    }

                    DestroyWindow(hwnd); // Close the dialog
                    return TRUE;
                }
                case IDCANCEL: // Assuming a "Close" or "Cancel" button with this ID
                    DestroyWindow(hwnd); // Close the dialog
                    return TRUE;
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return TRUE;
        case WM_DESTROY:
            PostQuitMessage(0); // Ends the modal loop initiated in CreateRegisterDialog
            return TRUE;
    }
    return DefWindowProc(hwnd, message, wParam, lParam); // Default message handling
}