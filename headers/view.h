#ifndef VIEW_H
#define VIEW_H

#include <windows.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CENTERED_X 550
#define CENTERED_Y 200

extern HWND hwndConsoleWindow, hwndConsoleBtnDisconnect, hwndConsoleBtnHelp, hwndConsoleBtnDir, hwndConsoleBtnAlias;
extern HWND hwndMessageBox, hwndUploadBtn;
extern HWND hwndDialog, hwndDialogStaticText, hwndDialogTextBox, hwndDialogOkButton;

// function prototypes
void CreateGetIpPortWindow(HWND hWnd);
void CreateConsoleWindow(HWND hWnd);
void CreateAliasChangeDialog(HWND hWnd);

// Custom message for dialog procedure
#define WM_CUSTOM_DIALOG (WM_USER + 1)

#endif // VIEW_H