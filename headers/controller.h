#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <windows.h>

LRESULT CALLBACK WindowProcedure(HWND hwndParentWindow, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK JoinDialogProcedure(HWND hwndParentWindow, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HelpDialogProcedure(HWND hwndParentWindow, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RegisterDialogProcedure(HWND hwndParentWindow, UINT message, WPARAM wParam, LPARAM lParam);

void handleIpAndPortSent();
void handleRegisterAlias();
void handleLeave();

#endif // CONTROLLER_H