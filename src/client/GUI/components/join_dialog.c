#include "../../../../headers/components.h"

// Function to create and display a custom dialog box
void CreateJoinDialog(HWND parentHwnd, HINSTANCE hInst) {
    // Create a basic window to serve as a dialog
    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Join Dialog",
                                WS_VISIBLE | WS_SYSMENU | WS_CAPTION | DS_MODALFRAME | WS_POPUP,
                                CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, // Adjust size as needed
                                parentHwnd, NULL, hInst, NULL);

    // Ensure the window was created
    if (!hDlg) return;

    // Create a static text control
    CreateWindowExW(0, L"STATIC", L"Enter details:",
                    WS_VISIBLE | WS_CHILD,
                    10, 10, 280, 20, // Adjust position and size as needed
                    hDlg, NULL, hInst, NULL);

    // Create an Edit control for input
    CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                    10, 40, 280, 20, // Adjust position and size as needed
                    hDlg, (HMENU)101, hInst, NULL); // 101 is the control ID

    // Create a "Join" button
    CreateWindowExW(0, L"BUTTON", L"Join",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    50, 70, 80, 25, // Adjust position and size as needed
                    hDlg, (HMENU)IDOK, hInst, NULL); // Use IDOK as the button ID for standard handling

    // Create a "Cancel" button
    CreateWindowExW(0, L"BUTTON", L"Cancel",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    170, 70, 80, 25, // Adjust position and size as needed
                    hDlg, (HMENU)IDCANCEL, hInst, NULL); // Use IDCANCEL as the button ID for standard handling

    // Make the window modal
    EnableWindow(parentHwnd, FALSE);

    SetWindowLongPtr(hDlg, GWLP_WNDPROC, (LONG_PTR)JoinDialogProcedure);

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

LRESULT CALLBACK JoinDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK: // Join button clicked
                    {
                        // Get the text from the edit control
                        wchar_t text[256];
                        GetWindowTextW(GetDlgItem(hwnd, 101), text, 256);
                        MessageBoxW(hwnd, text, L"Join Details", MB_OK);
                        return TRUE;
                    }
                case IDCANCEL: // Cancel button clicked
                    DestroyWindow(hwnd); // Close the dialog
                    return TRUE;
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return TRUE;
        case WM_DESTROY:
            PostQuitMessage(0);
            return TRUE;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}