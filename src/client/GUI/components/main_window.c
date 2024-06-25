#include <windows.h>
#include "../../../../headers/components.h"

// Function to create the console mimic window
void CreateConsoleOutputWindow(HWND parentHwnd, HINSTANCE hInst) {
    // Register the child window class
    WNDCLASSW wc = {0};
    wc.lpszClassName = L"ConsoleMimicClass";
    wc.hInstance = hInst;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpfnWndProc = ConsoleOutputProcedure;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    // Calculate size and position to be slightly smaller than the parent window
    RECT parentRect;
    GetClientRect(parentHwnd, &parentRect);
    int width = (int)(parentRect.right * 0.8);
    int height = (int)(parentRect.bottom * 0.8);
    int posX = (parentRect.right - width) / 2;
    int posY = (parentRect.bottom - height) / 2;

    // Create the child window
    CreateWindowExW(
        0, L"ConsoleMimicClass", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL,
        posX, posY, width, height,
        parentHwnd, NULL, hInst, NULL);
}

// Define the child window procedure for the console mimic
LRESULT CALLBACK ConsoleOutputProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // Set text color and background
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(0, 0, 0)); // Black background
            // Display some text as an example
            TextOutW(hdc, 5, 5, L"Console Output Mimic", 21);
            EndPaint(hwnd, &ps);
        } break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
