#include "../../../headers/commands.h"
#include "../../../headers/controller.h"
#include "../../../headers/view.h"

// window procedure function that processes messages for the main window
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: // handle window creation
            CreateConsoleWindow(hWnd); // create the initial panel with text box and button
            break;
        case WM_COMMAND: // handle commands, like button clicks
            if (LOWORD(wp) == 1) { 
                CreateJoinDialog(hWnd); 
            }
            if (LOWORD(wp) == 5) {
               CreateRegisterDialog(hWnd); 
            }
            break;
        case WM_DESTROY: // handle window destruction
            PostQuitMessage(0); // post a quit message and return exit code 0
            break;
        default: // default case for unhandled messages
            return DefWindowProc(hWnd, msg, wp, lp); // call the default window procedure
    }
    return 0;
}

LRESULT CALLBACK JoinDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            // Step 2: Handle WM_COMMAND message
            if (LOWORD(wParam) == 3) { 
                handleIpAndPortSent(hwnd);
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            joinDialogOpen = 0;
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK RegisterDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            // Step 2: Handle WM_COMMAND message
            if (LOWORD(wParam) == 2) { 
                handleRegisterAlias();
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            registerDialogOpen = 0;
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void handleIpAndPortSent() {
    wchar_t command[256];
    MultiByteToWideChar(CP_ACP, 0, COMMAND_JOIN, -1, command, 256);
    
    wchar_t ip[256];
    GetWindowTextW(hwndJoinDialogEditIp, ip, 256);

    wchar_t port[256];
    GetWindowTextW(hwndJoinDialogEditPort, port, 256);

    // Prepare the text to be displayed in the console window
    wchar_t consoleText[1024];
    wsprintfW(consoleText, L"%s %s %s\n", command, ip, port);

    // Get the current text length in the console window to append the new text
    int textLength = GetWindowTextLengthW(hwndConsoleWindow);
    SendMessageW(hwndConsoleWindow, EM_SETSEL, (WPARAM)textLength, (LPARAM)textLength); // Move the caret to the end
    SendMessageW(hwndConsoleWindow, EM_REPLACESEL, FALSE, (LPARAM)consoleText); // Append the text

    DestroyWindow(hwndJoinDialog);

    joinDialogOpen = 0;
}

void handleRegisterAlias() {
    wchar_t name[256];
    GetWindowTextW(hwndRegisterDialogTextBox, name, 256);

    // Set the text of the "Name" button to the entered name
    SetWindowTextW(hwndConsoleBtnRegister, name);

    // Close the dialog
    DestroyWindow(hwndRegisterDialog);

    registerDialogOpen = 0;
}