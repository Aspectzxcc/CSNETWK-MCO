#include "../../../../headers/components.h"

// Function to create and display a custom dialog box for Broadcast Message
void CreateBroadcastDialog(HWND parentHwnd, HINSTANCE hInst) {
    // Dialog box dimensions
    int dialogWidth = 400;
    int dialogHeight = 180;
    // Create a basic window to serve as a dialog
    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Broadcast Message",
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
    // Create a static text control for the message label
    CreateWindowExW(0, L"STATIC", L"Message:",
                    WS_VISIBLE | WS_CHILD,
                    10, 10, 380, 20, // Adjust position and size as needed
                    hDlg, NULL, hInst, NULL);
    // Adjusted Edit control for message input with vertical overflow
    CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
                    10, 40, 380, 50, // Position and size
                    hDlg, (HMENU)105, hInst, NULL); // 105 is the control ID for message input
    // Create a "Send" button
    CreateWindowExW(0, L"BUTTON", L"Send",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    160, 100, 80, 25, // Adjust position to fit within the dialog
                    hDlg, (HMENU)IDOK, hInst, NULL);
    // Make the window modal
    EnableWindow(parentHwnd, FALSE);
    SetWindowLongPtrW(hDlg, GWLP_WNDPROC, (LONG_PTR)BroadcastDialogProcedure);
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

LRESULT CALLBACK BroadcastDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK: {
                    wchar_t message[256];
                    GetDlgItemTextW(hwnd, 105, message, 256); // 105 is the Edit control ID for message input
                    if (wcslen(message) > 0) {
                        MessageBoxW(hwnd, L"Message sent successfully", L"Broadcast", MB_OK | MB_ICONINFORMATION);
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
            PostQuitMessage(0); // Ends the modal loop initiated in CreateBroadcastDialog
            return TRUE;
    }
    return DefWindowProc(hwnd, message, wParam, lParam); // Default message handling
}