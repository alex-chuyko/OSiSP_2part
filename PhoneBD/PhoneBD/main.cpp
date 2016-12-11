#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <stdlib.h> 
#include <string.h> 
#include <string> 
#include <tchar.h>
#include <commctrl.h>
#include <vector>
#include "PhoneBook.h"


HINSTANCE hDllInstance = LoadLibraryEx(L"Library.dll", 0, DONT_RESOLVE_DLL_REFERENCES);
typedef void(*functionLoadBDDll)(LPCWSTR);
functionLoadBDDll loadBase = functionLoadBDDll(GetProcAddress(hDllInstance, "loadBD"));
typedef std::vector<std::string>(*functionSearchInBDDll)(std::string, std::string, std::string);
functionSearchInBDDll searchInBD = functionSearchInBDDll(GetProcAddress(hDllInstance, "searchInBD"));

#define ID_OPEN_FILE 1000
#define ID_EXIT 1001
#define PHONE_DB_MAX_LEN 255

LVCOLUMN lvc;
LVITEM lvi;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;
HWND hLabel;
HWND hEdit1;
HWND hEdit2;
HWND hEdit3;
HWND hButton;
HWND hwndList;
_TCHAR* searchLastName;
_TCHAR* searchFirstName;
_TCHAR* searchStreetName;
PhoneBook phB;
bool isLoadBD = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));

	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszClassName = L"My Window Class";
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wClass))
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Класс окна не был создан!", L"Ошибка", MB_ICONERROR);
	}
	hInst = hInstance;
	HMENU MainMenu = CreateMenu();
	HMENU hPopupMenu1 = CreatePopupMenu();
	HMENU hPopupMenu2 = CreatePopupMenu();
	HMENU hPopupMenu3 = CreatePopupMenu();

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu1, L"File");
	{
		AppendMenu(hPopupMenu1, MF_STRING, ID_OPEN_FILE, L"Open file...(Ctrl+O)");
		AppendMenu(hPopupMenu1, MF_STRING, ID_EXIT, L"Exit(Ctrl+W)");
	}

	HWND hWindow = CreateWindowEx(NULL,
		L"My Window Class",
		L"OSiSP lab4",
		WS_OVERLAPPEDWINDOW,
		500,
		250,
		790,
		480,
		NULL,
		MainMenu,
		hInstance,
		NULL);

	hEdit1 = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
		100, 10, 120, 20, hWindow, (HMENU)1004, NULL, NULL);

	hEdit2 = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
		310, 10, 120, 20, hWindow, (HMENU)1005, NULL, NULL);

	hEdit3 = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
		500, 10, 120, 20, hWindow, (HMENU)1006, NULL, NULL);
	//painter.hideScrollBar(hWindow);

	if (!hWindow)
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"ќкно не было создано!", L"ќшибка", MB_ICONERROR);
	}

	ShowWindow(hWindow, nShowCmd);

	UpdateWindow(hWindow);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	std::string str = "";
	int cntText = 0;
	int lenNum = 0;
	int i = 0;

	switch (message)
	{
	case WM_COMMAND:
		if (wParam == 1002)
		{
			lenNum = GetWindowTextLength(hEdit1);
			if (lenNum != 0)
			{
				searchLastName = new _TCHAR[lenNum + 1];
				GetWindowText(hEdit1, searchLastName, lenNum + 1);
			}
			else
			{
				searchLastName = new _TCHAR[1];
				searchLastName = L" ";
			}
			lenNum = GetWindowTextLength(hEdit2);
			if (lenNum != 0)
			{
				searchFirstName = new _TCHAR[lenNum + 1];
				GetWindowText(hEdit2, searchFirstName, lenNum + 1);
			}
			else
			{
				searchFirstName = new _TCHAR[1];
				searchFirstName = L" ";
			}
			lenNum = GetWindowTextLength(hEdit3);
			if (lenNum != 0)
			{
				searchStreetName = new _TCHAR[lenNum + 1];
				GetWindowText(hEdit3, searchStreetName, lenNum + 1);
			}
			else
			{
				searchStreetName = new _TCHAR[1];
				searchStreetName = L" ";
			}

			if (isLoadBD)
			{
				ListView_DeleteAllItems(hwndList);
				std::wstring tempStr1(searchLastName); //convert to wstring
				std::string lastName(tempStr1.begin(), tempStr1.end());
				std::wstring tempStr2(searchFirstName); //convert to wstring
				std::string firstName(tempStr2.begin(), tempStr2.end());
				std::wstring tempStr3(searchStreetName); //convert to wstring
				std::string streetName(tempStr3.begin(), tempStr3.end());

				Person pr(" \t" + lastName + "\t" + firstName + "\t \t" + streetName + "\t \t \t ");
				vector<string> result = phB.find(pr);
				WCHAR buff[PHONE_DB_MAX_LEN * 2];
				if (result.size() > 0)
				{
					for (int i = 0; i < result.size(); i++)
					{
						int it = result[i].find("	", 0);
						while (it != string::npos)
						{
							result[i].replace(it, 1, " | ");
							it = result[i].find("	", it);

						}
						lvi.iItem = i;
						lvi.iSubItem = 0;
						ListView_InsertItem(hwndList, &lvi);
						if (MultiByteToWideChar(CP_ACP, MB_COMPOSITE, result[i].c_str(), -1, buff, PHONE_DB_MAX_LEN * 2) == 0) {
							return EXIT_FAILURE;
						}

						ListView_SetItemText(hwndList, i, 0, buff);
					}
				}
				else
				{
					lvi.iItem = 0;
					lvi.iSubItem = 0;
					ListView_InsertItem(hwndList, &lvi);
					ListView_SetItemText(hwndList, i, 0, L"No matches found");
				}
			}
			else
			{
				MessageBox(hWnd, L"Open BD!", L"Warning", 0);
			}
		}
		else if (wParam == ID_OPEN_FILE)
		{
			isLoadBD = true;
			LPCWSTR fileName = L"C:\\Users\\Александр\\Documents\\Visual Studio 2015\\Projects\\OSiSP\\Lab4\\PhoneBookApp\\very-small_db.txt";
			phB.load(fileName);
			phB.createIndex();
			MessageBox(hWnd, L"BD is opened!", L"Ok", 0);
		}
		else if (wParam == ID_EXIT)
		{

		}
		break;
	case WM_CREATE:
		hwndList = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
			13, 40, 700, 350, hWnd, (HMENU)1003, hInst, 0);

		lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_FMT;
		lvc.fmt = LVCFMT_LEFT;
		lvc.cx = 700;

		lvc.iSubItem = 0;
		lvc.pszText = L"Result";
		ListView_InsertColumn(hwndList, 0, &lvc);
		////////////////////////////////////////

		/*memset(&lvi, 0, sizeof(lvi));

		lvi.mask = LVIF_TEXT | LVIF_TEXT;

		for (int i = 0; i<3; i++)
		{
		lvi.iItem = i;
		lvi.iSubItem = 0;
		ListView_InsertItem(hwndList, &lvi);
		ListView_SetItemText(hwndList, i, 0, L"1111");
		}*/

		hButton = CreateWindow(L"button", L"Найти", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			630, 10, 80, 20, hWnd, (HMENU)1002, NULL, NULL);
		hLabel = CreateWindow(L"static", L"ST_U", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			20, 10, 80, 20, hWnd, (HMENU)1007, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		SetWindowText(hLabel, L"Lastname:");
		hLabel = CreateWindow(L"static", L"ST_U", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			230, 10, 80, 20, hWnd, (HMENU)1008, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		SetWindowText(hLabel, L"Firstname:");
		hLabel = CreateWindow(L"static", L"ST_U", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			440, 10, 60, 20, hWnd, (HMENU)1009, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		SetWindowText(hLabel, L"Street:");
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}