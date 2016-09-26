#include <windows.h>
#include <iostream>
#include "Painter.h"

#define SelectPen(hdc, hpen) \
  ((HPEN)SelectObject((hdc), (HGDIOBJ)(HPEN)(hpen)))

#define ID_NEW_FILE 1000
#define ID_SAVE_FILE 1001
#define ID_SAVE_AS_FILE 1002
#define ID_PRINT_FILE 1003
#define ID_EXIT 1004
#define ID_PENCIL 1005
#define ID_LINE 1006
#define ID_RECT 1007
#define ID_ELLIPSE 1008
#define ID_CURVE 1009
#define ID_POLY 1010
#define ID_TEXT 1011


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int currentId = 0;
bool flag = FALSE, flagPoly = FALSE, firstLine = TRUE;
bool flagMouseClick = false;
double zoom = 1;
Painter painter;
POINTS startPointPoly;
char text[2] = { ' ', '\0' };


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Регистрация класса окна
	// Объявляем переменную типа WNDCLASSEX

	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX)); // Обнуляем память

											 // Заполняем структуру WNDCLASSEX
	wClass.cbSize = sizeof(WNDCLASSEX); // Размер равен размеру структуры
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW +1); // Определяем фон окна
	wClass.hInstance = hInstance; // hInstance window
	wClass.lpfnWndProc = (WNDPROC)WndProc; // Процедура обработки окна
	wClass.lpszClassName = L"My Window Class"; // Имя класса
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.style = CS_HREDRAW | CS_VREDRAW;


	// Если произошла ошибка, то выводим сообщение
	if (!RegisterClassEx(&wClass))
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Класс окна не был создан!", L"Ошибка", MB_ICONERROR);
	}

	HMENU MainMenu = CreateMenu();
	HMENU hPopupMenu1 = CreatePopupMenu();
	HMENU hPopupMenu2 = CreatePopupMenu();

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu1, L"File");
	{
		AppendMenu(hPopupMenu1, MF_STRING, ID_NEW_FILE, L"New");
		AppendMenu(hPopupMenu1, MF_STRING, ID_SAVE_FILE, L"Save");
		AppendMenu(hPopupMenu1, MF_STRING, ID_SAVE_AS_FILE, L"Save as...");
		AppendMenu(hPopupMenu1, MF_STRING, ID_PRINT_FILE, L"Print");
		AppendMenu(hPopupMenu1, MF_STRING, ID_EXIT, L"Exit");
	}
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu2, L"Shape");
	{
		AppendMenu(hPopupMenu2, MF_STRING, ID_PENCIL, L"Pencil");
		AppendMenu(hPopupMenu2, MF_STRING, ID_LINE, L"Line");
		AppendMenu(hPopupMenu2, MF_STRING, ID_RECT, L"Rectangle");
		AppendMenu(hPopupMenu2, MF_STRING, ID_ELLIPSE, L"Ellipse");
		AppendMenu(hPopupMenu2, MF_STRING, ID_CURVE, L"Curve");
		AppendMenu(hPopupMenu2, MF_STRING, ID_POLY, L"Poly");
		AppendMenu(hPopupMenu2, MF_STRING, ID_TEXT, L"Text");
	}
	AppendMenu(MainMenu, MF_STRING, 0, L"About");

	HWND hWindow = CreateWindowEx(NULL,
		L"My Window Class", 
		L"Mini Paint", 
		WS_OVERLAPPEDWINDOW,
		300, 
		200, 
		640, 
		480, 
		NULL,
		MainMenu,
		hInstance, 
		NULL);
	/*RECT rect;
	GetClientRect(hWindow, &rect);
	HWND s1 = CreateWindowEx(0, L"SCROLLBAR", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, rect.right-15, 0, 15, rect.bottom, hWindow, NULL, hInstance, NULL);
	HWND s2 = CreateWindowEx(0, L"SCROLLBAR", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 0, rect.bottom-15, rect.right-15, 15, hWindow, NULL, hInstance, NULL);*/

	/*HWND hWnd_buttonLine = CreateWindow(L"button", L"Прямая", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 10, 80, 30, hWindow, (HMENU)1, hInstance, NULL);

	HWND hWnd_buttonPen = CreateWindow(L"button", L"Карандаш", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 50, 80, 30, hWindow, (HMENU)2, hInstance, NULL);

	HWND hWnd_buttonRectangle = CreateWindow(L"button", L"Прямоугольник", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 100, 80, 30, hWindow, (HMENU)3, hInstance, NULL);

	HWND hWnd_buttonEllipce = CreateWindow(L"button", L"Эллипс", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 150, 80, 30, hWindow, (HMENU)4, hInstance, NULL);*/

	if (!hWindow)
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Окно не было создано!", L"Ошибка", MB_ICONERROR);
	}

	// Показываем окно
	ShowWindow(hWindow, nShowCmd);

	UpdateWindow(hWindow);

	// Объявляем переменную для сообщений типа MSG
	MSG msg;
	// Обнуляем память по размеру структуры MSG
	ZeroMemory(&msg, sizeof(MSG));

	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

// определяем процедуру обратного вызова (WinApi)

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*PAINTSTRUCT paint;
	POINTS ptsBegin = MAKEPOINTS(lParam), ptsEnd = MAKEPOINTS(lParam);
	HDC hdc = GetDC(hwnd);
	double zoom = 1;
	HDC memDC = CreateCompatibleDC(hdc);*/

	PAINTSTRUCT ps;
	HDC hdc;		
	static POINTS ptsBegin;		
	static POINTS ptsEnd;			
	static BOOL fPrevLine = FALSE;	
	static int k = 0;
	static HDC memDC;
	static HBITMAP memBM;
	/*static HDC memDC = CreateCompatibleDC(hdc);
	static HBITMAP memBM = CreateCompatibleBitmap(hdc, GetDeviceCaps(hdc, HORZRES),
		GetDeviceCaps(hdc, VERTRES));
	if (k == 0)
	{
		SelectObject(memDC, memBM);
		PatBlt(memDC, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), WHITENESS);
		k++;
	}
	ReleaseDC(hwnd, hdc);*/
	switch (uMsg)
	{
		case WM_CREATE:
		{
			hdc = GetDC(hwnd);
			k = 0;
			memDC = CreateCompatibleDC(hdc);
			memBM = CreateCompatibleBitmap(hdc, GetDeviceCaps(hdc, HORZRES),
				GetDeviceCaps(hdc, VERTRES));
			if (k == 0)
			{
				SelectObject(memDC, memBM);
				PatBlt(memDC, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), WHITENESS);
				k++;
			}
			ReleaseDC(hwnd, hdc);
			// Здесь будем создавать элементы управления окна
			break;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_NEW_FILE:
				{

					break;
				}
				case ID_SAVE_FILE:
				{
					hdc = GetDC(hwnd);
					
					RECT rect;
					GetClientRect(hwnd, &rect);
					int iWidthMM = GetDeviceCaps(hdc, HORZSIZE);
					int iHeightMM = GetDeviceCaps(hdc, VERTSIZE);
					int iWidthPels = GetDeviceCaps(hdc, HORZRES);
					int iHeightPels = GetDeviceCaps(hdc, VERTRES);
					rect.left = (rect.left * iWidthMM * 100) / iWidthPels;
					rect.top = (rect.top * iHeightMM * 100) / iHeightPels;
					rect.right = (rect.right * iWidthMM * 100) / iWidthPels;
					rect.bottom = (rect.bottom * iHeightMM * 100) / iHeightPels;
					HDC hdcMeta = CreateEnhMetaFile(hdc, L"D:\\testEMF.emf", &rect, L"Example metafile\0");
					if (!hdcMeta)
					{
						MessageBox(NULL, L"CreateEnhMetaFile!", L"Error", MB_ICONERROR);
					}
					StretchBlt(hdcMeta, 0, 0, GetDeviceCaps(hdc, HORZRES),
						GetDeviceCaps(hdc, VERTRES), memDC, 0, 0,
						GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
					SetMapMode(hdcMeta, MM_ANISOTROPIC);
					ReleaseDC(hwnd, hdc);
					HENHMETAFILE meta = CloseEnhMetaFile(hdcMeta);
					//ReleaseDC(hwnd, hdcMeta);
					break;
				}
				case ID_PENCIL:
				{
					currentId = ID_PENCIL;
					break;
				}
				case ID_LINE:
				{
					currentId = ID_LINE;
					break;
				}
				case ID_RECT:
				{
					currentId = ID_RECT;
					break;
				}
				case ID_ELLIPSE:
				{
					currentId = ID_ELLIPSE;
					break;
				}
				case ID_CURVE:
				{
					currentId = ID_CURVE;
					break;
				}
				case ID_POLY:
				{
					currentId = ID_POLY;
					firstLine = TRUE;
					break;
				}
				case ID_TEXT:
				{
					currentId = ID_TEXT;
					break;
				}
			// Обработка команд (нажатие кнопок, мыши, полей ввода и т.д.)
			}

			break;
		}

		case WM_DESTROY: 
		{
			// команда Закрыть окно
			DeleteObject(memBM);
			PostQuitMessage(0);
			return 0;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			OutputDebugStringW(L"LBUTTONDOWN\n");

			// Захватываем мышку.
			//SetCapture(hWnd);
			hdc = GetDC(hwnd);
			if ((currentId == ID_CURVE && !flag))
			{
				ptsBegin = MAKEPOINTS(lParam);
				flag = TRUE;
			}
			else if(currentId == ID_POLY && !flagPoly)
			{
				ptsBegin = MAKEPOINTS(lParam);
				startPointPoly = MAKEPOINTS(lParam);
				flagPoly = TRUE;
			}
			else if (currentId != ID_POLY && currentId != ID_CURVE)
			{
				ptsBegin = MAKEPOINTS(lParam);
			}
			//SetPixel(memDC, ptsBegin.x, ptsBegin.y, 0);
			//SetPixel(hdc, ptsBegin.x, ptsBegin.y, 0);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			OutputDebugStringW(L"RBUTTONDOWN\n");

			// Захватываем мышку.
			//SetCapture(hWnd);
			hdc = GetDC(hwnd);
			if (currentId == ID_CURVE)
			{
				flag = FALSE;
				SendMessage(hwnd, WM_LBUTTONUP, wParam, lParam);
			}
			else if (currentId == ID_POLY)
			{
				flagPoly = FALSE;
				firstLine = TRUE;
				SendMessage(hwnd, WM_LBUTTONUP, wParam, lParam);
			}
			//SetPixel(memDC, ptsBegin.x, ptsBegin.y, 0);
			//SetPixel(hdc, ptsBegin.x, ptsBegin.y, 0);
			break;
		}

		case WM_MOUSEMOVE:
		{
			switch (currentId)
			{
				case ID_PENCIL:
				{
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);
						ptsEnd = MAKEPOINTS(lParam);
						painter.drawPencil(hdc, memDC, ptsBegin, ptsEnd, zoom);
						fPrevLine = TRUE;
						ptsBegin = ptsEnd;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
				case ID_LINE:
				{
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);
						painter.drawLine(hdc, memDC, ptsBegin, &ptsEnd, zoom, fPrevLine, lParam);
						fPrevLine = TRUE;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
				case ID_RECT:
				{
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);
						painter.drawRectangle(hdc, memDC, ptsBegin, &ptsEnd, zoom, fPrevLine, lParam);
						fPrevLine = TRUE;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
				case ID_ELLIPSE:
				{ 
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);
						painter.drawEllipse(hdc, memDC, ptsBegin, &ptsEnd, zoom, fPrevLine, lParam);
						fPrevLine = TRUE;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
				case ID_CURVE:
				{
					if (flag)
					{
						hdc = GetDC(hwnd);
						painter.drawCurve(hdc, memDC, ptsBegin, &ptsEnd, zoom, fPrevLine, lParam);
						fPrevLine = TRUE;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}

				case ID_POLY:
				{
					if (flagPoly)
					{
						hdc = GetDC(hwnd); 
						painter.drawPoly(hdc, memDC, ptsBegin, &ptsEnd, startPointPoly, zoom, fPrevLine, lParam);
						fPrevLine = TRUE;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
			}
			break;
		}

		case WM_LBUTTONUP:
		{
			OutputDebugStringW(L"LBUTTONUP\n");
			hdc = GetDC(hwnd);

			if (currentId < ID_RECT && currentId != 0)
			{
				ptsEnd = MAKEPOINTS(lParam);
				MoveToEx(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, (LPPOINT)NULL);
				LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
				/*StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
					GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
					GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
			}
			else if (currentId == ID_RECT)
			{
				SelectObject(memDC, GetStockObject(NULL_BRUSH));
				ptsEnd = MAKEPOINTS(lParam);
				Rectangle(memDC, ptsBegin.x/zoom, ptsBegin.y / zoom, ptsEnd.x / zoom, ptsEnd.y / zoom);
				/*StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
					GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
					GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
			}
			else if (currentId == ID_ELLIPSE)
			{
				SelectObject(memDC, GetStockObject(NULL_BRUSH));
				ptsEnd = MAKEPOINTS(lParam);
				Ellipse(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, ptsEnd.x / zoom, ptsEnd.y / zoom);
				/*StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
					GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
					GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
			}
			else if (currentId == ID_CURVE)
			{
				fPrevLine = FALSE;
				ptsEnd = MAKEPOINTS(lParam);
				MoveToEx(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, (LPPOINT)NULL);
				LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
				/*StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
					GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
					GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
				ptsBegin = ptsEnd;

			}
			else if (currentId == ID_POLY)
			{
				fPrevLine = FALSE;
				ptsEnd = MAKEPOINTS(lParam);
				if (firstLine)
				{
					MoveToEx(memDC, startPointPoly.x / zoom, startPointPoly.y / zoom, (LPPOINT)NULL);
					LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
					firstLine = FALSE;
				}
				MoveToEx(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, (LPPOINT)NULL);
				LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
				/*StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
				GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
				GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
				ptsBegin = ptsEnd;
			}
			fPrevLine = FALSE;
			ClipCursor(NULL);
			ReleaseCapture();

			break;
		}

		case WM_PAINT:
		{
			OutputDebugStringW(L"PAINT\n");
			hdc = GetDC(hwnd);
			if (currentId == ID_TEXT)
			{
				TextOut(memDC, ptsBegin.x, ptsBegin.y, (LPCWSTR)text, sizeof(text));
				ptsBegin.x += 9;
			}
			StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
				GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
				GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
			ReleaseDC(hwnd, hdc);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			OutputDebugStringW(L"MOUSEWHEEL\n");
			if (LOWORD(wParam) == MK_CONTROL)
			{
				if (((short)HIWORD(wParam)) < 0)
				{
					zoom -= 0.05;
					SendMessage(hwnd, WM_PAINT, 0, 0);
				}
				else
				{
					zoom += 0.05;
					SendMessage(hwnd, WM_PAINT, 0, 0);
				}
			}
			break;
		}

		case WM_KEYDOWN:
		{
			if (currentId == ID_TEXT)
			{
				text[0] = (char)wParam;
				SendMessage(hwnd, WM_PAINT, 0, 0);
			}
			break;
		}
	
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}