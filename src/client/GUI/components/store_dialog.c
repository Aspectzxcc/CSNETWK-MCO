#include "../../../../headers/components.h"

// Function to create and display a custom dialog box for Upload
void CreateStoreDialog(HWND parentHwnd, HINSTANCE hInst) {
    // Dialog box dimensions
    int dialogWidth = 400;
    int dialogHeight = 350;

    // Create a basic window to serve as a dialog
    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Store Dialog",
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

    // Create a TreeView control for displaying client directory structure
    HWND hTreeView = CreateWindowExW(WS_EX_CLIENTEDGE, WC_TREEVIEWW, L"",
                                     WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
                                     10, 10, 380, 240, // Adjust position and size as needed
                                     hDlg, (HMENU)106, hInst, NULL); // 106 is the control ID for TreeView in Store Dialog

    // Initialize the TreeView with sample items for client directory
    InitializeTreeViewForStore(hTreeView);
    // Adjust the "Close" button position
    CreateWindowExW(0, L"BUTTON", L"Close",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    160, 260, 80, 25, // Adjust position and size as needed
                    hDlg, (HMENU)IDCANCEL, hInst, NULL); // Use IDCANCEL as the button ID for standard handling
    // Make the window modal
    EnableWindow(parentHwnd, FALSE);
    SetWindowLongPtrW(hDlg, GWLP_WNDPROC, (LONG_PTR)StoreDialogProcedure);
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

// Function to initialize TreeView with sample items for client directory
void InitializeTreeViewForStore(HWND hTreeView) {
    HTREEITEM g_hRootUpload = AddItemToTreeView(hTreeView, NULL, "Client Directory");
    AddItemToTreeView(hTreeView, g_hRootUpload, "ClientSample1.txt");
    AddItemToTreeView(hTreeView, g_hRootUpload, "ClientSample2.txt");
    AddItemToTreeView(hTreeView, g_hRootUpload, "ClientSample3.txt");
}

// Procedure for handling messages in the Upload Dialog
LRESULT CALLBACK StoreDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_NOTIFY: {
            LPNMHDR lpnmh = (LPNMHDR)lParam;
            if (lpnmh->idFrom == 106 && lpnmh->code == NM_DBLCLK) { // 106 is the TreeView control ID in Upload Dialog
                HTREEITEM hSelectedItem = TreeView_GetSelection(lpnmh->hwndFrom);
                MessageBoxW(hwnd, L"STORE request made", L"Information", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL: // Assuming a "Close" or "Cancel" button with this ID
                    DestroyWindow(hwnd); // Close the dialog
                    return TRUE;
                // Add more cases here for other buttons/commands
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return TRUE;
        case WM_DESTROY:
            PostQuitMessage(0); // Ends the modal loop initiated in CreateUploadDialog
            return TRUE;
    }
    return DefWindowProc(hwnd, message, wParam, lParam); // Default message handling
}