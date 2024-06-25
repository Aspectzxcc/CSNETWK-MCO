#include "../../../../headers/components.h"
#include "../../../../headers/commands.h"
#include <stdio.h>

typedef struct {
    const char* command;
    const char* parameters;
    const char* parameterDescription;
} CommandInfo;

CommandInfo commandInfos[] = {
    {COMMAND_JOIN, "2", COMMAND_JOIN_PARAMETERS},
    {COMMAND_LEAVE, "0", "No Parameters"},
    {COMMAND_REGISTER, "1", COMMAND_REGISTER_PARAMETERS},
    {COMMAND_STORE, "1", COMMAND_STORE_PARAMETERS},
    {COMMAND_DIR, "0", "No Parameters"},
    {COMMAND_GET, "1", COMMAND_GET_PARAMETERS},
    {COMMAND_HELP, "0", "No Parameters"},
    {COMMAND_BROADCAST, "1", COMMAND_BROADCAST_PARAMETERS},
    {COMMAND_UNICAST, "2", COMMAND_UNICAST_PARAMETERS}
};

// Function to create and display a custom dialog box for Help
void CreateHelpDialog(HWND parentHwnd, HINSTANCE hInst) {
    // Dialog box dimensions
    int dialogWidth = 400;
    int dialogHeight = 350;

    // Create a basic window to serve as a dialog
    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"STATIC", L"Help Dialog",
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

    // Create a ListView control for displaying help topics
    HWND hList = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
                                 WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
                                 10, 10, 380, 240, // Adjust position and size as needed
                                 hDlg, (HMENU)104, hInst, NULL); // 104 is the control ID for ListView

    // Setup the ListView control with columns and items
    SetupListView(hList);

    // Adjust the "Close" button position
    CreateWindowExW(0, L"BUTTON", L"Close",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    160, 260, 80, 25, // Adjust position and size as needed
                    hDlg, (HMENU)IDCANCEL, hInst, NULL); // Use IDCANCEL as the button ID for standard handling

    // Make the window modal
    EnableWindow(parentHwnd, FALSE);
    SetWindowLongPtr(hDlg, GWLP_WNDPROC, (LONG_PTR)HelpDialogProcedure);

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

void SetupListView(HWND hList) {
    InitializeListViewColumns(hList);
    for (int i = 0; i < sizeof(commandInfos) / sizeof(commandInfos[0]); ++i) {
        char itemText[DEFAULT_BUFLEN];
        if (strlen(commandInfos[i].parameters) > 0) {
            sprintf(itemText, "%s %s", commandInfos[i].command, commandInfos[i].parameters);
        } else {
            sprintf(itemText, "%s", commandInfos[i].command);
        }
        AddItemToListView(hList, i, commandInfos[i].command, commandInfos[i].parameters, commandInfos[i].parameterDescription);
    }
}

void InitializeListViewColumns(HWND hList) {
    // Define columns: "Commands", "Parameters", "Description"
    const char* columnTitles[] = {"Commands", "Parameters", "Parameter Descriptions"};
    int columnWidths[] = {80, 80, 215}; // Adjust widths as needed
    LVCOLUMN lvColumn;
    memset(&lvColumn, 0, sizeof(LVCOLUMN));
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
    lvColumn.fmt = LVCFMT_CENTER;

    for (int i = 0; i < sizeof(columnTitles)/sizeof(columnTitles[0]); ++i) {
        lvColumn.cx = columnWidths[i];
        lvColumn.pszText = (LPSTR)columnTitles[i];
        ListView_InsertColumn(hList, i, &lvColumn);
    }
}

void AddItemToListView(HWND hList, int itemIndex, const char* mainText, const char* paramText, const char* descText) {
    LVITEM lvItem;
    memset(&lvItem, 0, sizeof(LVITEM));
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = itemIndex;
    lvItem.pszText = (LPSTR)mainText;
    ListView_InsertItem(hList, &lvItem);

    ListView_SetItemText(hList, itemIndex, 1, (LPSTR)paramText);
    ListView_SetItemText(hList, itemIndex, 2, (LPSTR)descText);
}

LRESULT CALLBACK HelpDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL: // Close button clicked
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