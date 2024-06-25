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
extern HWND hwndJoinDialog, hwndJoinDialogEditIp, hwndJoinDialogEditPort, hwndJoinDialogJoinButton;

// alias change dialog handlers
extern HWND hwndRegisterDialog, hwndRegisterDialogTextBox, hwndRegisterDialogRegisterButton;

// function prototypes
void CreateConsoleWindow(HWND hwndParentWindow);
void CreateConsoleWindowTopButtons(HWND hwndParentWindow);
void CreateConsoleWindowBottomButtons(HWND hwndParentWindow);
void CreateJoinDialog(HWND hwndParentWindow);
void CreateHelpDialog(HWND hwndParentWindow);
void CreateRegisterDialog(HWND hwndParentWindow);


// Custom message for dialog procedure
#define WM_CUSTOM_DIALOG (WM_USER + 1)

#endif // VIEW_H