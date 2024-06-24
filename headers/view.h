#ifndef VIEW_H
#define VIEW_H

#include <windows.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CENTERED_X 550
#define CENTERED_Y 200

HWND hwndConsoleWindow; 
HWND hwndConsoleBtnJoin, hwndConsoleBtnLeave, hwndConsoleBtnHelp, hwndConsoleBtnDir, hwndConsoleBtnRegister;
HWND hwndConsoleBtnBroadcast, hwndConsoleBtnUnicast, hwndConsoleBtnUpload;
extern HWND hwndDialog, hwndDialogStaticText, hwndDialogTextBox, hwndDialogOkButton;

// function prototypes
void CreateConsoleWindow(HWND hWnd);
void CreateConsoleWindowTopButtons(HWND hWnd);
void CreateConsoleWindowBottomButtons(HWND hWnd);
void CreateRegisterDialog(HWND hWnd);

// Custom message for dialog procedure
#define WM_CUSTOM_DIALOG (WM_USER + 1)

#endif // VIEW_H