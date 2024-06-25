#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <windows.h>

// Function prototype for creating the console mimic window
void CreateConsoleOutputWindow(HWND parentHwnd, HINSTANCE hInst);
void CreateConsoleOutputWindowButtons(HWND parentHwnd, HINSTANCE hInst, int posX, int posY, int width, int height); 
void CreateJoinDialog(HWND parentHwnd, HINSTANCE hInst);
void CreateHelpDialog(HWND parentHwnd, HINSTANCE hInst);
void SetupListView(HWND hList);
void InitializeListViewColumns(HWND hList);
void AddItemToListView(HWND hList, int itemIndex, const char* mainText, const char* paramText, const char* descText);

// Window procedure for the console mimic window
LRESULT CALLBACK ConsoleOutputProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK JoinDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HelpDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // COMPONENTS_H