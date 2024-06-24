#ifndef GUI_H
#define GUI_H

#include <windows.h>

// function prototypes
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProcedure(HWND, UINT, WPARAM, LPARAM);
void CreateGetIpPortWindow(HWND hWnd);
void CreateConsoleWindow(HWND hWnd);
void CreateAliasChangeDialog(HWND hWnd);

#endif // GUI_H