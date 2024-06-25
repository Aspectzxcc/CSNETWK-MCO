#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <windows.h>

// Function prototype for creating the console mimic window
void CreateConsoleOutputWindow(HWND parentHwnd, HINSTANCE hInst);

// Window procedure for the console mimic window
LRESULT CALLBACK ConsoleOutputProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // COMPONENTS_H