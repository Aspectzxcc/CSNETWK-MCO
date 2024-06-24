#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <windows.h>

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // CONTROLLER_H