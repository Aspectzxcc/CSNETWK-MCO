#include "../../../../headers/components.h"

// Function to create the console mimic window
void CreateConsoleOutputWindow(HWND parentHwnd, HINSTANCE hInst) {
    // Initialize Rich Edit library
    LoadLibrary(TEXT("Msftedit.dll"));

    // Register the child window class if not already registered
    WNDCLASSW wc = {0};
    wc.lpszClassName = L"ConsoleOutputClass";
    wc.hInstance = hInst;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpfnWndProc = ConsoleOutputProcedure;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if (!GetClassInfoW(hInst, wc.lpszClassName, &wc)) {
        RegisterClassW(&wc);
    }

    // Calculate size and position to be slightly smaller than the parent window
    RECT parentRect;
    GetClientRect(parentHwnd, &parentRect);
    int width = (int)(parentRect.right * 0.8);
    int height = (int)(parentRect.bottom * 0.8);
    int posX = (parentRect.right - width) / 2;
    int posY = (parentRect.bottom - height) / 2;

    // Create the child window as a rich edit control
    g_hConsoleOutput = CreateWindowExW(
        WS_EX_CLIENTEDGE, L"RICHEDIT50W", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
        posX, posY, width, height,
        parentHwnd, NULL, hInst, NULL);

    // Create buttons for the console output window
    CreateConsoleOutputWindowButtons(parentHwnd, hInst, posX, posY, width, height);
}

void CreateConsoleOutputWindowButtons(HWND parentHwnd, HINSTANCE hInst, int posX, int posY, int width, int height) {
    // Constants for button layout 
    const int buttonHeight = 30;
    const int gap = 5; // Gap between buttons
    const int buttonYPos = posY - buttonHeight - gap; // Position buttons above the console mimic window
    const int totalGapsWidth = (5 - 1) * gap; // Total width taken by gaps between buttons
    const int buttonWidth = (width - totalGapsWidth) / 5; // Adjusted button width

    // Button labels
    const wchar_t* topLabels[] = {L"Join", L"Leave", L"Help", L"Directory", L"Register"};
    // Create buttons with adjusted buttonWidth
    for (int i = 0; i < 5; i++) {
        CreateWindowExW(
            0, L"BUTTON", topLabels[i],
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            posX + (buttonWidth + gap) * i, buttonYPos, buttonWidth, buttonHeight,
            parentHwnd, (HMENU)(INT_PTR)(100 + i), hInst, NULL);
    }

    // After creating the console output window
    HFONT hFont = CreateFontW(
        16,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                 // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        FIXED_PITCH | FF_MODERN,   // nPitchAndFamily
        L"Consolas");              // lpszFacename

    SendMessageW(g_hConsoleOutput, WM_SETFONT, (WPARAM)hFont, TRUE);

    // receive EN_PROTECTED notifications
    SendMessageW(g_hConsoleOutput, EM_SETEVENTMASK, 0, ENM_PROTECTED);

    // Adjust buttonYPos to position buttons under the console mimic window
    const int buttonYPosUnder = posY + height + gap; // New Y position for buttons under the console window

    // Button labels for the new buttons
    const wchar_t* bottomLabels[] = {L"Broadcast", L"Unicast", L"Store"};

    // Calculate the width for the three new buttons, considering the total width and gaps
    const int totalGapsWidthUnder = (3 - 1) * gap; // Total width taken by gaps between the new buttons
    const int buttonWidthUnder = (width - totalGapsWidthUnder) / 3; // Adjusted button width for the new buttons

    // Create the new buttons under the console mimic window
    for (int i = 0; i < 3; i++) {
        CreateWindowExW(
            0, L"BUTTON", bottomLabels[i],
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            posX + (buttonWidthUnder + gap) * i, buttonYPosUnder, buttonWidthUnder, buttonHeight,
            parentHwnd, (HMENU)(INT_PTR)(200 + i), hInst, NULL); // Using 200+i for ID to avoid conflict
    }
}

void AppendReadOnlyTextToConsoleOutput(HWND hwndRichEdit, const wchar_t* text) {
    SendMessageW(hwndRichEdit, EM_SETREADONLY, FALSE, 0); // Ensure control is editable

    // Get current text length before appending new text
    int initialTextLength = GetWindowTextLengthW(hwndRichEdit);

    // Move caret to the end and append new text
    SendMessageW(hwndRichEdit, EM_SETSEL, (WPARAM)initialTextLength, (LPARAM)-1); // Move to end
    SendMessageW(hwndRichEdit, EM_REPLACESEL, FALSE, (LPARAM)text); // Append text

    // Get new text length after appending
    int newTextLength = GetWindowTextLengthW(hwndRichEdit);

    // Select from the first character to the new text length to protect the entire range
    CHARRANGE cr = {0, newTextLength - 1};
    SendMessageW(hwndRichEdit, EM_EXSETSEL, 0, (LPARAM)&cr); // Select text to protect

    // Set the selected text as protected
    CHARFORMAT2 cf = {0};
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_PROTECTED;
    cf.dwEffects = CFE_PROTECTED;
    SendMessageW(hwndRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

    // Move caret to the end to allow further input
    SendMessageW(hwndRichEdit, EM_SETSEL, (WPARAM)newTextLength, (LPARAM)-1); // Move to end
}

// Define the child window procedure for the console mimic
LRESULT CALLBACK ConsoleOutputProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
