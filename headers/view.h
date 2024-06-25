#ifndef VIEW_H
#define VIEW_H

#include <windows.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CENTERED_X 550
#define CENTERED_Y 200

// console window handlers
extern HWND hwndConsoleWindow; 
extern HWND hwndConsoleBtnJoin, hwndConsoleBtnLeave, hwndConsoleBtnHelp, hwndConsoleBtnDir, hwndConsoleBtnRegister;
extern HWND hwndConsoleBtnBroadcast, hwndConsoleBtnUnicast, hwndConsoleBtnUpload;

// join dialog handlers
extern HWND hwndJoinDialog, hwndEditIp, hwndEditPort, hwndJoinButton;

// alias change dialog handlers
extern HWND hwndRegisterDialog, hwndDialogStaticText, hwndDialogTextBox, hwndDialogOkButton;

// flags for open state
extern int joinDialogOpen, registerDialogOpen;

// function prototypes
void CreateConsoleWindow(HWND hWnd);
void CreateConsoleWindowTopButtons(HWND hWnd);
void CreateConsoleWindowBottomButtons(HWND hWnd);
void CreateJoinDialog(HWND hWnd);
void CreateRegisterDialog(HWND hWnd);

// Custom message for dialog procedure
#define WM_CUSTOM_DIALOG (WM_USER + 1)

#endif // VIEW_H