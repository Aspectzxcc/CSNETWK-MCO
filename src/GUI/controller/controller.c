#include "../../../headers/controller.h"
#include "../../../headers/view.h"

// window procedure function that processes messages for the main window
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: // handle window creation
            CreateConsoleWindow(hWnd); // create the initial panel with text box and button
            break;
        case WM_COMMAND: // handle commands, like button clicks
            if (LOWORD(wp) == 7) {
               CreateAliasChangeDialog(hWnd); 
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

LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            // Step 2: Handle WM_COMMAND message
            if (LOWORD(wParam) == 9) { 
                // Get the text from the text box in the dialog
                wchar_t name[256];
                GetWindowTextW(hwndDialogTextBox, name, 256);

                // Set the text of the "Name" button to the entered name
                SetWindowTextW(hwndConsoleBtnAlias, name);

                // Close the dialog
                DestroyWindow(hwndDialog);
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}