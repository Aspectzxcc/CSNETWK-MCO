#include "../../../../headers/components.h"

// Function to create and display a custom dialog box
void CreateJoinDialog(HWND parentHwnd, HINSTANCE hInst) {
    // Dialog box dimensions
    int dialogWidth = 300;
    int dialogHeight = 200;

    // Create a basic window to serve as a dialog
    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Join Dialog",
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

    // Create a static text control for IP
    CreateWindowExW(0, L"STATIC", L"IP Address:",
                    WS_VISIBLE | WS_CHILD,
                    10, 10, 280, 20, // Adjust position and size as needed
                    hDlg, NULL, hInst, NULL);

    // Create an Edit control for IP input
    CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                    10, 40, 280, 20, // Adjust position and size as needed
                    hDlg, (HMENU)102, hInst, NULL); // 102 is the control ID for IP

    // Create a static text control for Port
    CreateWindowExW(0, L"STATIC", L"Port:",
                    WS_VISIBLE | WS_CHILD,
                    10, 70, 280, 20, // Adjust position and size as needed
                    hDlg, NULL, hInst, NULL);

    // Create an Edit control for Port input
    CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                    10, 100, 280, 20, // Adjust position and size as needed
                    hDlg, (HMENU)103, hInst, NULL); // 103 is the control ID for Port

    // Adjust the "Join" button position
    CreateWindowExW(0, L"BUTTON", L"Join",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    50, 130, 80, 25, // Adjust position and size as needed
                    hDlg, (HMENU)IDOK, hInst, NULL); // Use IDOK as the button ID for standard handling

    // Adjust the "Cancel" button position
    CreateWindowExW(0, L"BUTTON", L"Cancel",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    160, 130, 80, 25, // Adjust position and size as needed
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
                        DestroyWindow(hwnd); // Close the dialog
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