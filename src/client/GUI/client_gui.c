#include "../../../headers/components.h"

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

// Entry point of a Windows application
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"MainWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    // Register the window class
    if (!RegisterClassW(&wc)) {
        return -1;
    }

    // Calculate the screen width and height
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Window size
    int windowWidth = 800;
    int windowHeight = 600;

    // Calculate the top left corner for the window to be centered
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // Create the window centered on the screen
    CreateWindowW(
        L"MainWindowClass", 
        L"File Exchange Client", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        posX, posY, windowWidth, windowHeight, 
        NULL, NULL, hInst, NULL);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Window procedure definition
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            CreateConsoleOutputWindow(hWnd, ((LPCREATESTRUCT)lp)->hInstance);
            break;
        case WM_COMMAND:
            // Handle button clicks
            switch (LOWORD(wp)) {
                case 100: // Join button
                    CreateJoinDialog(hWnd, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
                    break;
                case 101: // Leave button
                    MessageBoxW(hWnd, L"Leave button clicked", L"Button Clicked", MB_OK);
                    break;
                case 102: // Help button
                    CreateHelpDialog(hWnd, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
                    break;
                case 103: // Directory button
                    CreateDirectoryDialog(hWnd, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
                    break;
                case 104: // Register button
                    {
                        HWND hBtnRegister = GetDlgItem(hWnd, 104); // Assuming 104 is the ID for the Register button
                        CreateRegisterDialog(hWnd, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hBtnRegister);
                    }
                    break;
                case 200: // Broadcast button
                    CreateBroadcastDialog(hWnd, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
                    break;
                case 201: // Unicast button
                    CreateUnicastDialog(hWnd, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
                    break;
                case 202: // Upload button
                    CreateStoreDialog(hWnd, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
                    break;
            }
            break;
         case WM_ERASEBKGND:
            {
                HDC hdc = (HDC)wp;
                RECT rect;
                GetClientRect(hWnd, &rect);
                HBRUSH hBrush = CreateSolidBrush(RGB(192, 192, 192)); // A grayish color
                FillRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);
                return 1; // Return non-zero to indicate the message is handled
            }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}