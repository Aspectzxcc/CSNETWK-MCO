#include <windows.h>

// function prototype for the callback function that processes messages for the main window
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// global variables to store handles to the text box and button controls
HWND hwndTextBox, hwndButton;

// CreateWindowW(
//         L"TestWindowClass", // Class name: Specifies the window class name. This class must have been registered with RegisterClassW.
//         L"File Exchange System", // Window name: Text that appears in the title bar of the window.
//         WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Style: Specifies the style of the window being created. WS_OVERLAPPEDWINDOW 
//          creates an overlapped window with a title bar, a window menu, a border, and a minimize/maximize box. WS_VISIBLE makes the window visible.
//         100, // X position: The initial horizontal position of the window. 
//         100, // Y position: The initial vertical position of the window.
//         500, // Width: The width of the window.
//         500, // Height: The height of the window.
//         NULL, // Parent window handle: If this parameter is NULL, the window has no parent.
//         NULL, // Menu handle: If this parameter is NULL, the window is created without a menu.
//         hInst, // Instance handle: A handle to the instance of the module to be associated with the window.
//         NULL // Additional application data: Pointer to a value to be passed to the window through the CREATESTRUCT structure passed in the lParam parameter of the WM_CREATE message.
//     );

// entry point for a Windows application
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    // structure to define various properties of the window class
    WNDCLASSW wc = {0};

    // background color for the window
    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    // cursor used in the window
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    // instance handle for the application
    wc.hInstance = hInst;
    // class name for the window class
    wc.lpszClassName = L"TestWindowClass";
    // pointer to the window procedure function for handling messages for windows of this class
    wc.lpfnWndProc = WindowProcedure;

    // register the window class
    if (!RegisterClassW(&wc))
        return -1; // if registration fails, exit the application

    // create the main application window
    CreateWindowW(
        L"TestWindowClass", L"File Exchange System", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        100, 100, 500, 500, 
        NULL, NULL, hInst, NULL);

    // message loop for the application
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // translates virtual-key messages into character messages
        DispatchMessage(&msg); // dispatches a message to a window procedure
    }

    return 0; // exit the application
}

// Create a text box control
// Parameters:
// - L"EDIT": Specifies the class name for a standard Windows edit control. This creates an edit box where users can input text.
// - L"": The default text to be displayed in the text box. This should be replaced with appropriate placeholder text relevant to the application's context.
// - WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER: Window styles. WS_CHILD makes the control a child window. WS_VISIBLE makes it visible upon creation. WS_BORDER gives it a border. ES_AUTOHSCROLL allows horizontal scrolling. ES_CENTER centers the text.
// - 150, 200: The starting x and y positions of the text box within the parent window, in pixels.
// - 200, 20: The width and height of the text box, in pixels.
// - hWnd: Handle to the parent window.
// - (HMENU)1: The control's identifier.
// - NULL: Points to the window creation data. Not used here, so it is NULL.
// - NULL: Handle to the instance of the module to be associated with the window. Not used here, so it is NULL.

// Create a button control
// Parameters:
// - L"BUTTON": Specifies the class name for a standard Windows button control. This creates a clickable button.
// - L"Join": The text to be displayed on the button.
// - WS_CHILD | WS_VISIBLE: Window styles. WS_CHILD makes the control a child window. WS_VISIBLE makes it visible upon creation.
// - 200, 230: The starting x and y positions of the button within the parent window, in pixels.
// - 100, 30: The width and height of the button, in pixels.
// - hWnd: Handle to the parent window.
// - (HMENU)2: The control's identifier.
// - NULL: Points to the window creation data. Not used here, so it is NULL.
// - NULL: Handle to the instance of the module to be associated with the window. Not used here, so it is NULL.

// window procedure function that processes messages for the main window
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: // message sent when a window is being created
            // create a text box control
            hwndTextBox = CreateWindowW(
                L"EDIT", L"", 
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER, 
                150, 200, 200, 20, 
                hWnd, (HMENU)1, NULL, NULL);

            // create a button control
            hwndButton = CreateWindowW(
                L"BUTTON", L"Join", 
                WS_CHILD | WS_VISIBLE, 
                200, 230, 100, 30, 
                hWnd, (HMENU)2, NULL, NULL);
                
            break;
        case WM_COMMAND: // message sent when a control is interacted with
            if (LOWORD(wp) == 2) { // if the "Join" button was clicked
                wchar_t ipAndPort[256]; // buffer to store the text from the text box
                GetWindowTextW(hwndTextBox, ipAndPort, 256); // get the text from the text box
                // display the text from the text box in a message box
                MessageBoxW(NULL, ipAndPort, L"IP and Port", MB_OK);
            }
            break;
        case WM_DESTROY: // message sent when a window is being destroyed
            PostQuitMessage(0); // posts a quit message and returns exit code 0
            break;
        default: // default case for unhandled messages
            return DefWindowProc(hWnd, msg, wp, lp); // call the default window procedure
    }
    return 0;
}