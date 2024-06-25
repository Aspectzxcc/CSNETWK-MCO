#include "../../../headers/client_gui.h"

// Entry point of a Windows application
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"MyWindowClass";
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
        L"MyWindowClass", 
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
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}