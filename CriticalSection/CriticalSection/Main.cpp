#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
using namespace std;

#define BUF_SIZE_STR sizeof(LPCTSTR)

HANDLE hSharedMemory;
HANDLE hMutex;
const int BUF_SIZE = 5;


void main()
{
	if ((hSharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUF_SIZE_STR, TEXT("criticalSection"))) == NULL)
	{
		cout << "Error to create file." << endl;
		_getch();
		return;
	}
	hMutex = CreateMutex(NULL, FALSE, L"MyMutex");

	if (GetLastError() != ERROR_ALREADY_EXISTS)
	{
		STARTUPINFO si = { NULL };
		PROCESS_INFORMATION pi = { NULL };
		//TCHAR commandArgs[] = TEXT("CriticalSection.exe"); //C:\\Users\\Александр\\Documents\\Visual Studio 2015\\Projects\\OSiSP\\Lab3\\CriticalSection\\Debug
		TCHAR consoleName[] = TEXT("second process");

		if (!CreateProcess(TEXT("CriticalSection.exe"), consoleName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			cout << "Cannot create process" << endl;
			_getch();
			return;
		}

		LPCTSTR buff;
		for (int i = 0; i < 15; i++)
		{
			WaitForSingleObject(hMutex, INFINITE);
			TCHAR szMsg[5];
			wsprintf(szMsg, L"%d", i);
			buff = (LPTSTR)MapViewOfFile(hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
			CopyMemory((PVOID)buff, szMsg, BUF_SIZE_STR);
			wcout << L"First process write: " << i << endl;
			ReleaseMutex(hMutex);
			Sleep(1000);
		}

		UnmapViewOfFile(buff);
		_getch();
	}
	else
	{
		if (hSharedMemory == NULL)
		{
			cout << "Child Process: Could not open file mapping object" << endl;
			_getch();
			return;
		}

		LPCTSTR buff;
		for (int i = 0; i < 15; i++)
		{
			WaitForSingleObject(hMutex, INFINITE);
			buff = (LPTSTR)MapViewOfFile(hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
			wcout << L"Second process read: " << buff << endl;
			ReleaseMutex(hMutex);
			Sleep(300);
		}
		
		UnmapViewOfFile(buff);
		_getch();
	}

	CloseHandle(hSharedMemory);
	CloseHandle(hMutex);
}