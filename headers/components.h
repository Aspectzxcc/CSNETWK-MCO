#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <windows.h>
#include <commctrl.h>

// console output window
extern HWND g_hConsoleOutput;

// function prototypes for window creation
void CreateConsoleOutputWindow(HWND parentHwnd, HINSTANCE hInst);
void CreateConsoleOutputWindowButtons(HWND parentHwnd, HINSTANCE hInst, int posX, int posY, int width, int height); 
void AppendTextToConsoleOutput(wchar_t *text);
void CreateJoinDialog(HWND parentHwnd, HINSTANCE hInst);
void CreateHelpDialog(HWND parentHwnd, HINSTANCE hInst);
void SetupListView(HWND hList);
void InitializeListViewColumns(HWND hList);
void AddItemToListView(HWND hList, int itemIndex, const char* mainText, const char* paramText, const char* descText);
void CreateDirectoryDialog(HWND parentHwnd, HINSTANCE hInst);
void InitializeTreeView(HWND hTreeView);
HTREEITEM AddItemToTreeView(HWND hTreeView, HTREEITEM hParent, char* text);
void CreateRegisterDialog(HWND parentHwnd, HINSTANCE hInst, HWND registerHwnd);
void CreateBroadcastDialog(HWND parentHwnd, HINSTANCE hInst);
void CreateUnicastDialog(HWND parentHwnd, HINSTANCE hInst);
void CreateStoreDialog(HWND parentHwnd, HINSTANCE hInst);
void InitializeTreeViewForStore(HWND hTreeView);

// function prototypes for window procedure
LRESULT CALLBACK ConsoleOutputProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK JoinDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HelpDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DirectoryDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RegisterDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BroadcastDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK UnicastDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK StoreDialogProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // COMPONENTS_H