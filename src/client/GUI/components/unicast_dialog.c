#include "../../../../headers/components.h"

// Function to create and display a custom dialog box for Unicast Message
void CreateUnicastDialog(HWND parentHwnd, HINSTANCE hInst) {
    int dialogWidth = 400;
    int dialogHeight = 240; // Increased height to accommodate alias/handle field

    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Unicast Message",
                                WS_VISIBLE | WS_SYSMENU | WS_CAPTION | DS_MODALFRAME | WS_POPUP,
                                CW_USEDEFAULT, CW_USEDEFAULT, dialogWidth, dialogHeight,
                                parentHwnd, NULL, hInst, NULL);
    if (!hDlg) return;

    RECT parentRect;
    GetWindowRect(parentHwnd, &parentRect);
    int x = parentRect.left + (parentRect.right - parentRect.left - dialogWidth) / 2;
    int y = parentRect.top + (parentRect.bottom - parentRect.top - dialogHeight) / 2;
    SetWindowPos(hDlg, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    // Alias/Handle label and field
    CreateWindowExW(0, L"STATIC", L"Alias/Handle:",
                    WS_VISIBLE | WS_CHILD,
                    10, 10, 380, 20,
                    hDlg, NULL, hInst, NULL);
    CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER,
                    10, 30, 380, 25,
                    hDlg, (HMENU)106, hInst, NULL); // 106 is the control ID for alias/handle input

    // Adjusted positions for message label and input
    CreateWindowExW(0, L"STATIC", L"Message:",
                    WS_VISIBLE | WS_CHILD,
                    10, 60, 380, 20,
                    hDlg, NULL, hInst, NULL);
    CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
                    10, 80, 380, 80, // Adjusted position and increased size
                    hDlg, (HMENU)105, hInst, NULL);

    // Adjusted position for "Send" button
    CreateWindowExW(0, L"BUTTON", L"Send",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    160, 170, 80, 25,
                    hDlg, (HMENU)IDOK, hInst, NULL);

    EnableWindow(parentHwnd, FALSE);
    SetWindowLongPtrW(hDlg, GWLP_WNDPROC, (LONG_PTR)UnicastDialogProcedure);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsDialogMessage(hDlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    EnableWindow(parentHwnd, TRUE);
    SetForegroundWindow(parentHwnd);
    DestroyWindow(hDlg);
}

LRESULT CALLBACK UnicastDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK: {
                    wchar_t alias[256], message[256];
                    GetDlgItemTextW(hwnd, 106, alias, 256); // Retrieve alias/handle
                    GetDlgItemTextW(hwnd, 105, message, 256); // Retrieve message
                    if (wcslen(message) > 0) {
                        // Handle sending the unicast message using the alias and message
                        MessageBoxW(hwnd, L"Unicast message sent successfully", L"Unicast", MB_OK | MB_ICONINFORMATION);
                    }
                    DestroyWindow(hwnd);
                    return TRUE;
                }
                case IDCANCEL:
                    DestroyWindow(hwnd);
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