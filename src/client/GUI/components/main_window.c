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

    // Constants for button layout 
    const int buttonHeight = 30;
    const int gap = 5; // Gap between buttons
    const int buttonYPos = posY - buttonHeight - gap; // Position buttons above the console mimic window
    const int totalGapsWidth = (5 - 1) * gap; // Total width taken by gaps between buttons
    const int buttonWidth = (width - totalGapsWidth) / 5; // Adjusted button width

    // Button labels
    const wchar_t* labels[] = {L"Join", L"Leave", L"Help", L"Directory", L"Register"};

    // Create buttons with adjusted buttonWidth
    for (int i = 0; i < 5; i++) {
        CreateWindowExW(
            0, L"BUTTON", labels[i],
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            posX + (buttonWidth + gap) * i, buttonYPos, buttonWidth, buttonHeight,
            parentHwnd, (HMENU)(INT_PTR)(100 + i), hInst, NULL);
    }

    // Adjust buttonYPos to position buttons under the console mimic window
    const int buttonYPosUnder = posY + height + gap; // New Y position for buttons under the console window

    // Button labels for the new buttons
    const wchar_t* newLabels[] = {L"Broadcast", L"Unicast", L"Upload"};

    // Calculate the width for the three new buttons, considering the total width and gaps
    const int totalGapsWidthUnder = (3 - 1) * gap; // Total width taken by gaps between the new buttons
    const int buttonWidthUnder = (width - totalGapsWidthUnder) / 3; // Adjusted button width for the new buttons

    // Create the new buttons under the console mimic window
    for (int i = 0; i < 3; i++) {
        CreateWindowExW(
            0, L"BUTTON", newLabels[i],
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            posX + (buttonWidthUnder + gap) * i, buttonYPosUnder, buttonWidthUnder, buttonHeight,
            parentHwnd, (HMENU)(INT_PTR)(200 + i), hInst, NULL); // Using 200+i for ID to avoid conflict
    }
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
