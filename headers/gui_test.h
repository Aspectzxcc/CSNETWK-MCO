#ifndef GUI_TEST_H
#define GUI_TEST_H

#include <windows.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CENTERED_X 550
#define CENTERED_Y 200

// function prototypes
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProcedure(HWND, UINT, WPARAM, LPARAM);
void CreateGetIpPortPanel(HWND hWnd);
void CreateConsolePanel(HWND hWnd);
void ShowNameChangeDialog(HWND hWnd);

#endif // GUI_TEST_H