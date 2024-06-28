#include "../../../../headers/components.h"

// Function to create and display a custom dialog box for Directory
void CreateDirectoryDialog(HWND parentHwnd, HINSTANCE hInst) {
    // Dialog box dimensions
    int dialogWidth = 400;
    int dialogHeight = 350;

    // Create a basic window to serve as a dialog
    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Directory Dialog",
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

    // Create a TreeView control for displaying directory structure
    HWND hTreeView = CreateWindowExW(WS_EX_CLIENTEDGE, WC_TREEVIEWW, L"",
                                     WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
                                     10, 10, 380, 240, // Adjust position and size as needed
                                     hDlg, (HMENU)105, hInst, NULL); // 105 is the control ID for TreeView

    // Initialize the TreeView with sample items
    InitializeTreeView(hTreeView);

    // Adjust the "Close" button position
    CreateWindowExW(0, L"BUTTON", L"Close",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    160, 260, 80, 25, // Adjust position and size as needed
                    hDlg, (HMENU)IDCANCEL, hInst, NULL); // Use IDCANCEL as the button ID for standard handling

    // Make the window modal
    EnableWindow(parentHwnd, FALSE);
    SetWindowLongPtrW(hDlg, GWLP_WNDPROC, (LONG_PTR)DirectoryDialogProcedure);

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

HTREEITEM g_hRoot = NULL; // Global variable to store the root node handle

void InitializeTreeView(HWND hTreeView) {
    // Add sample items to the TreeView
    g_hRoot = AddItemToTreeView(hTreeView, NULL, "Server Directory");
}

void AddFilesToTreeView(HWND hwndTreeView, HTREEITEM hRoot, const char* dir) {
    char* fileList = strstr(dir, "\n") + 1; // Skip "Server directory\n"
    char* fileStart = fileList;
    char* fileEnd = NULL;
    while ((fileEnd = strstr(fileStart, "\n")) != NULL) {
        *fileEnd = '\0'; // Temporarily terminate the current file name string
        if (fileStart != fileEnd) { // Check if not an empty line
            // Add the file name as a child item of the root
            AddItemToTreeView(hwndTreeView, hRoot, fileStart);
        }
        fileStart = fileEnd + 1; // Move to the start of the next file name
    }
}


HTREEITEM AddItemToTreeView(HWND hTreeView, HTREEITEM hParent, char* text) {
    TVINSERTSTRUCT tvInsert;

    tvInsert.hParent = hParent;
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.item.mask = TVIF_TEXT;
    tvInsert.item.pszText = (LPSTR)text;

    return TreeView_InsertItem(hTreeView, &tvInsert);
}

LRESULT CALLBACK DirectoryDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_NOTIFY: {
            LPNMHDR lpnmh = (LPNMHDR)lParam;
            switch (lpnmh->code) {
                case NM_DBLCLK: { // Handle double-click event
                    HTREEITEM hSelectedItem = TreeView_GetSelection(lpnmh->hwndFrom);
                    if (hSelectedItem == g_hRoot) {
                        HTREEITEM hChildItem = TreeView_GetChild(lpnmh->hwndFrom, g_hRoot);

                        if (hChildItem == NULL) {
                            HandleCommand(COMMAND_DIR_W); // Handle the command
                        }
                        
                        if (client.connectionStatus == DISCONNECTED || client.registrationStatus == REGISTRATION_NOT_REGISTERED) {
                            return TRUE;
                        }
                        
                        AddFilesToTreeView(lpnmh->hwndFrom, g_hRoot, g_serverDir);
                    } else {
                        // Buffer to store the item's name
                        char szItemText[256] = {0}; // Adjust the size as needed

                        // Set up the TVITEM structure to request the text of the selected item
                        TVITEM tvi;
                        tvi.mask = TVIF_TEXT; // We want the text
                        tvi.hItem = hSelectedItem; // The item we're interested in
                        tvi.pszText = szItemText; // Buffer to store the text
                        tvi.cchTextMax = sizeof(szItemText)/sizeof(szItemText[0]); // Size of the buffer

                        // Retrieve the item's information
                        if (TreeView_GetItem(lpnmh->hwndFrom, &tvi)) {
                            // Use the item's name as needed, for example, show it in a message box
                            wchar_t wItemText[256], command[256];
                            MultiByteToWideChar(CP_ACP, 0, szItemText, -1, wItemText, 256);
                            wsprintfW(command, L"%s %s", COMMAND_GET_W, wItemText);

                            HandleCommand(command); // Handle the command
                        } else {
                            MessageBoxW(hwnd, ERROR_FILE_NOT_FOUND_SERVER_W, L"Error", MB_OK | MB_ICONERROR);
                        }
                    }
                    break;
                }
                case TVN_ITEMEXPANDEDW: {
                    NMTREEVIEW* pnmTreeView = (NMTREEVIEW*)lParam;
                    if (pnmTreeView->action == TVE_COLLAPSE) {
                        HTREEITEM hChildItem = TreeView_GetChild(lpnmh->hwndFrom, g_hRoot);
                        while (hChildItem != NULL) {
                            HTREEITEM hNextItem = TreeView_GetNextSibling(lpnmh->hwndFrom, hChildItem);
                            TreeView_DeleteItem(lpnmh->hwndFrom, hChildItem);
                            hChildItem = hNextItem;
                        }
                    }
                    break;
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
            PostQuitMessage(0); // Ends the modal loop initiated in CreateDirectoryDialog
            return TRUE;
    }
    return DefWindowProc(hwnd, message, wParam, lParam); // Default message handling
}