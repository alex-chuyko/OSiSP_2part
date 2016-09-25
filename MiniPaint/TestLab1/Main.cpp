#include <windows.h>
#include <iostream>

#define SelectPen(hdc, hpen) \
  ((HPEN)SelectObject((hdc), (HGDIOBJ)(HPEN)(hpen)))


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
double x = 0, y = 0, a, b; int lol = 0;
bool flag = FALSE;
bool flagMouseClick = false;
double zoom = 1;


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
		AppendMenu(hPopupMenu1, MF_STRING, 1000, L"New");
		AppendMenu(hPopupMenu1, MF_STRING, 1001, L"Save");
		AppendMenu(hPopupMenu1, MF_STRING, 1002, L"Save as...");
		AppendMenu(hPopupMenu1, MF_STRING, 1003, L"Print");
		AppendMenu(hPopupMenu1, MF_STRING, 1004, L"Exit");
	}
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu2, L"Shape");
	{
		AppendMenu(hPopupMenu2, MF_STRING, 1005, L"Pen");
		AppendMenu(hPopupMenu2, MF_STRING, 1006, L"Line");
		AppendMenu(hPopupMenu2, MF_STRING, 1007, L"Rectangle");
		AppendMenu(hPopupMenu2, MF_STRING, 1008, L"Ellipse");
		AppendMenu(hPopupMenu2, MF_STRING, 1009, L"Curve");
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
	HDC hdc = GetDC(hwnd);		
	static POINTS ptsBegin;		
	static POINTS ptsEnd;			
	static BOOL fPrevLine = FALSE;	
	static int k = 0;
	static HDC memDC = CreateCompatibleDC(hdc);
	static HBITMAP memBM = CreateCompatibleBitmap(hdc, GetDeviceCaps(hdc, HORZRES),
		GetDeviceCaps(hdc, VERTRES));
	if (k == 0)
	{
		SelectObject(memDC, memBM);
		PatBlt(memDC, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), WHITENESS);
		k++;
	}
	ReleaseDC(hwnd, hdc);
	switch (uMsg)
	{
		case WM_CREATE:
		{
			
			// Здесь будем создавать элементы управления окна
			break;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case 1000:
				{

					break;
				}
				case 1005:
				{
					lol = 1005;
					break;
				}
				case 1006:
				{
					lol = 1006;
					break;
				}
				case 1007:
				{
					lol = 1007;
					break;
				}
				case 1008: 
				{
					lol = 1008;
					break;
				}
				case 1009:
				{
					lol = 1009;
					break;
				}
			// Обработка команд (нажатие кнопок, мыши, полей ввода и т.д.)
			}

			break;
		}

		case WM_DESTROY: // Обработка нажатия кнопки закрытия окна
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
			if ((lol == 1009 && !flag))
			{
				ptsBegin = MAKEPOINTS(lParam);
				flag = TRUE;
			}
			else if(lol != 1009)
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
			if (lol == 1009)
			{
				flag = FALSE;
				SendMessage(hwnd, WM_LBUTTONUP, wParam, lParam);
			}
			//SetPixel(memDC, ptsBegin.x, ptsBegin.y, 0);
			//SetPixel(hdc, ptsBegin.x, ptsBegin.y, 0);
			break;
		}

		case WM_MOUSEMOVE:
		{
			switch (lol)
			{
				case 1005:
				{
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);
						ptsEnd = MAKEPOINTS(lParam);


						MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
						LineTo(hdc, ptsEnd.x, ptsEnd.y);

						MoveToEx(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, (LPPOINT)NULL);
						LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);

						fPrevLine = TRUE;
						ptsBegin = ptsEnd;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
				case 1006:
				{
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);

						if (fPrevLine)
						{
							SetROP2(hdc, R2_WHITE); //R2_NOTXORPEN

							MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
							LineTo(hdc, ptsEnd.x, ptsEnd.y);
							StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
								GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
								GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
							SetROP2(hdc, R2_COPYPEN);
						}
						ptsEnd = MAKEPOINTS(lParam);

						MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
						LineTo(hdc, ptsEnd.x, ptsEnd.y);


						/*MoveToEx(memDC, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
						LineTo(memDC, ptsEnd.x, ptsEnd.y);*/

						fPrevLine = TRUE;
						//ptsBegin = ptsEnd;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
				case 1007:
				{
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);
						if (fPrevLine)
						{
							SetROP2(hdc, R2_NOTXORPEN); //R2_NOTXORPEN

							Rectangle(hdc, ptsBegin.x, ptsBegin.y, ptsEnd.x, ptsEnd.y);
							/*StretchBlt(memDC, 0, 0, GetDeviceCaps(hdc, HORZRES),
								GetDeviceCaps(hdc, VERTRES), hdc, 0, 0,
								GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
							StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
								GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
								GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
							SetROP2(hdc, R2_COPYPEN);

						}
						ptsEnd = MAKEPOINTS(lParam);

						SelectObject(hdc, GetStockObject(NULL_BRUSH));
						Rectangle(hdc, ptsBegin.x, ptsBegin.y, ptsEnd.x, ptsEnd.y);

						fPrevLine = TRUE;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
				case 1008:
				{ 
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);
						if (fPrevLine)
						{
							SetROP2(hdc, R2_WHITE); //R2_NOTXORPEN

							Ellipse(hdc, ptsBegin.x, ptsBegin.y, ptsEnd.x, ptsEnd.y);
							StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
								GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
								GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
							SetROP2(hdc, R2_COPYPEN);

						}
						ptsEnd = MAKEPOINTS(lParam);

						SelectObject(hdc, GetStockObject(NULL_BRUSH));
						Ellipse(hdc, ptsBegin.x, ptsBegin.y, ptsEnd.x, ptsEnd.y);

						fPrevLine = TRUE;
						ReleaseDC(hwnd, hdc);
					}
					break;
				}
				case 1009:
				{
					if (flag)
					{
						hdc = GetDC(hwnd);

						if (fPrevLine)
						{
							SetROP2(hdc, R2_WHITE); //R2_NOTXORPEN

							MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
							LineTo(hdc, ptsEnd.x, ptsEnd.y);
							StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
								GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
								GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
							SetROP2(hdc, R2_COPYPEN);
						}
						ptsEnd = MAKEPOINTS(lParam);

						MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
						LineTo(hdc, ptsEnd.x, ptsEnd.y);


						/*MoveToEx(memDC, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
						LineTo(memDC, ptsEnd.x, ptsEnd.y);*/

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

			if (lol < 1007 && lol != 0)
			{
				ptsEnd = MAKEPOINTS(lParam);
				MoveToEx(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, (LPPOINT)NULL);
				LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
				/*StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
					GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
					GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
			}
			else if (lol == 1007)
			{
				SelectObject(memDC, GetStockObject(NULL_BRUSH));
				ptsEnd = MAKEPOINTS(lParam);
				Rectangle(memDC, ptsBegin.x/zoom, ptsBegin.y / zoom, ptsEnd.x / zoom, ptsEnd.y / zoom);
				/*StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
					GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
					GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
			}
			else if (lol == 1008)
			{
				SelectObject(memDC, GetStockObject(NULL_BRUSH));
				ptsEnd = MAKEPOINTS(lParam);
				Ellipse(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, ptsEnd.x / zoom, ptsEnd.y / zoom);
				/*StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
					GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
					GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
			}
			else if (lol == 1009)
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
			fPrevLine = FALSE;
			ClipCursor(NULL);
			ReleaseCapture();

			break;
		}

		case WM_PAINT:
		{
			OutputDebugStringW(L"PAINT\n");
			//hdc = BeginPaint(hwnd, &ps);
			hdc = GetDC(hwnd);
			StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
				GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
				GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
			/*StretchBlt(memDC, 0, 0, GetDeviceCaps(memDC, HORZRES),
				GetDeviceCaps(memDC, VERTRES), hdc, 0, 0,
				GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SRCCOPY);*/
			ReleaseDC(hwnd, hdc);
			//EndPaint(hwnd, &ps);
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
	
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}