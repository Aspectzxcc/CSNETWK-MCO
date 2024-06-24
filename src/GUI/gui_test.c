#include <stdio.h>
#include "../../headers/gui_test.h"

// declare global variables to store handles to the console window, text box, and button
HWND hwndTextBox, hwndButton, hwndHeader;

// CreateWindowW documentation
// HWND CreateWindowW(
//   [in, optional]  lpClassName, // class name of the window
//   [in, optional]  lpWindowName, // window name in the title bar
//   [in]            dwStyle, // window styles
//   [in]            x, // x-coordinate of the window
//   [in]            y, // y-coordinate of the window
//   [in]            nWidth, // width of the window
//   [in]            nHeight, // height of the window
//   [in, optional]  hWndParent, // handle to the parent window
//   [in, optional]  hMenu, // handle to the menu
//   [in, optional]  hInstance, // handle to the instance of the module
//   [in, optional]  lpParam // additional parameters
// );

// main entry point for a windows application
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0}; // initialize a window class structure to zero

    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND; // set the background color for the window
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // set the cursor for the window to the default arrow
    wc.hInstance = hInst; // set the instance handle for the window class
    wc.lpszClassName = L"TestWindowClass"; // set the class name for the window class
    wc.lpfnWndProc = WindowProcedure; // set the window procedure function for the window class

    if (!RegisterClassW(&wc)) // register the window class with the operating system
        return -1; // if registration fails, exit the application

    // create the main application window
    CreateWindowW(
        L"TestWindowClass", 
        L"File Exchange System", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        CENTERED_X, CENTERED_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 
        NULL, 
        NULL, 
        hInst,
        NULL);

    MSG msg = {0}; // initialize a message structure to zero
    while (GetMessage(&msg, NULL, 0, 0)) { // start the message loop
        TranslateMessage(&msg); // translate virtual-key messages into character messages
        DispatchMessage(&msg); // dispatch the message to the window procedure
    }

    return 0; // exit the application
}

// window procedure function that processes messages for the main window
// HWND hWnd: handle to the window
// UINT msg: message identifier
// WPARAM wp: additional message information
// LPARAM lp: additional message information
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: // handle window creation
            hwndHeader = CreateWindowW(
                L"STATIC", // Predefined class; Static control
                L"File Exchange System", // Button text 
                WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, // Styles; Visible, Child, Centered text, Centered image
                0, // x position; Start from the left edge
                10, // y position; A little down from the top edge
                WINDOW_WIDTH, // Make it as wide as the window
                40, // Height
                hWnd, // Parent window
                (HMENU)0, // No menu.
                NULL, // No instance override
                NULL); // No additional parameters

            // Create a larger font
            HFONT hFont = CreateFontW(
                24, // nHeight
                0, // nWidth
                0, // nEscapement
                0, // nOrientation
                FW_BOLD, // nWeight
                FALSE, // bItalic
                FALSE, // bUnderline
                0, // cStrikeOut
                ANSI_CHARSET, // nCharSet
                OUT_DEFAULT_PRECIS, // nOutPrecision
                CLIP_DEFAULT_PRECIS, // nClipPrecision
                DEFAULT_QUALITY, // nQuality
                DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
                L"Arial"); // lpszFacename

            // Set the font for the static control
            SendMessage(hwndHeader, WM_SETFONT, (WPARAM)hFont, TRUE);

            hwndTextBox = CreateWindowW(
                L"EDIT", 
                L"", 
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 
                (WINDOW_WIDTH - 400) / 2, // Center horizontally
                250, // Fixed vertical position
                400, // Width
                25, 
                hWnd, 
                (HMENU)1, 
                NULL, 
                NULL); // create a text box control

            SetFocus(hwndTextBox); // set focus to the text box control

            hwndButton = CreateWindowW(
                L"BUTTON", L"Join", 
                WS_CHILD | WS_VISIBLE, 
                (WINDOW_WIDTH - 110) / 2, // Center horizontally
                290, // Below the text box with some spacing
                100, // Width
                30, // Height
                hWnd, (HMENU)2, NULL, NULL); // create a button control
            break;
        case WM_COMMAND: // handle commands, like button clicks
            if (LOWORD(wp) == 2) { // if the "Join" button was clicked
                wchar_t ipAndPort[256]; // buffer to store text from the text box
                GetWindowTextW(hwndTextBox, ipAndPort, 256); // get the text from the text box

                // Hide the text box and button and header
                ShowWindow(hwndTextBox, SW_HIDE);
                ShowWindow(hwndButton, SW_HIDE);
                ShowWindow(hwndHeader, SW_HIDE);
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