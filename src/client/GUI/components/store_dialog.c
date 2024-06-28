#include "../../../../headers/components.h"

HTREEITEM g_hRootStore = NULL; // Global variable to store the root item in Store Dialog

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

    g_hRootStore = AddItemToTreeView(hTreeView, NULL, "Client Directory");

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

void PopulateTreeViewWithClientDirectory(HWND hTreeView) {
    // Initialize an integer flag to track the number of files added
    int filesAdded = 0;

    // Find files in the "files" directory
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("./files/*", &findFileData); // Adjust the path as needed

    if (hFind == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, "Failed to read files directory", "Error", MB_OK);
        return;
    }

    do {
        // Skip directories, only list files
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            // Add each file to the TreeView under the "Files Directory" root item
            AddItemToTreeView(hTreeView, g_hRootStore, findFileData.cFileName);
            filesAdded++; // Increment the counter for each file added
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    // Check if no files were added and display a message box if so
    if (filesAdded == 0) {
        MessageBox(NULL, "No files found in the directory.", "Information", MB_OK | MB_ICONINFORMATION);
    }
}

LRESULT CALLBACK StoreDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_NOTIFY: {
            LPNMHDR lpnmh = (LPNMHDR)lParam;
            if (lpnmh->idFrom == 106) { // Assuming 106 is the TreeView control ID in Store Dialog
                switch (lpnmh->code) {
                    case NM_DBLCLK: { // Handle double-click event
                        HTREEITEM hSelectedItem = TreeView_GetSelection(lpnmh->hwndFrom);
                        if (hSelectedItem == g_hRootStore) {
                            HTREEITEM hChildItem = TreeView_GetChild(lpnmh->hwndFrom, g_hRootStore);

                            if (hChildItem == NULL) {
                                // Populate the TreeView with the client directory's files
                                PopulateTreeViewWithClientDirectory(lpnmh->hwndFrom);
                            }
                        }
                        break;
                    }
                    case TVN_ITEMEXPANDEDW: { // Assuming you want to handle the ANSI version
                        NMTREEVIEW* pnmTreeView = (NMTREEVIEW*)lParam;
                        if (pnmTreeView->action == TVE_COLLAPSE) {
                            HTREEITEM hChildItem = TreeView_GetChild(lpnmh->hwndFrom, g_hRootStore);
                            while (hChildItem != NULL) {
                                HTREEITEM hNextItem = TreeView_GetNextSibling(lpnmh->hwndFrom, hChildItem);
                                TreeView_DeleteItem(lpnmh->hwndFrom, hChildItem);
                                hChildItem = hNextItem;
                            }
                        }
                        break;
                    }
                }
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
            PostQuitMessage(0); // Ends the modal loop initiated in CreateStoreDialog
            return TRUE;
    }
    return DefWindowProc(hwnd, message, wParam, lParam); // Default message handling
}