#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <windows.h>

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK JoinDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RegisterDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void handleIpAndPortSent();
void handleRegisterAlias();
void handleLeave();

#endif // CONTROLLER_H