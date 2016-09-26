#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "Painter.h"
#include "resource.h"


#define ID_OPEN_FILE 1000
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
#define ID_ABOUT 100
#define ID_COLOR_LINE 101
#define ID_COLOR_FILL 102
#define ID_NO_COLOR_FILL 103
#define ID_LINE_1px 1
#define ID_LINE_3px 3
#define ID_LINE_7px 7
#define ID_LINE_10px 10


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int currentId = 0;
bool flag = FALSE, flagPoly = FALSE, firstLine = TRUE, isFill = FALSE, isPrint = FALSE;
LPCWSTR openFileName;
double zoom = 1;
Painter painter;
POINTS startPointPoly;
COLORREF color = RGB(0, 0, 0), colorFill;
int penWidth = 1;
HPEN hPen;
HBRUSH hBrush;
HFONT hfont;
HINSTANCE hInst;
char text[2] = { ' ', '\0' };


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX)); 

	wClass.cbSize = sizeof(WNDCLASSEX); 
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW +1);
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
		AppendMenu(hPopupMenu1, MF_STRING, ID_OPEN_FILE, L"Open file...");
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
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu3, L"Thickness");
	{
		AppendMenu(hPopupMenu3, MF_STRING, ID_LINE_1px, L"1px");
		AppendMenu(hPopupMenu3, MF_STRING, ID_LINE_3px, L"3px");
		AppendMenu(hPopupMenu3, MF_STRING, ID_LINE_7px, L"7px");
		AppendMenu(hPopupMenu3, MF_STRING, ID_LINE_10px, L"10px");
	}
	AppendMenu(MainMenu, MF_STRING, ID_COLOR_LINE, L"Line color");
	AppendMenu(MainMenu, MF_STRING, ID_COLOR_FILL, L"Fill color");
	AppendMenu(MainMenu, MF_STRING, ID_NO_COLOR_FILL, L"No fill color");
	AppendMenu(MainMenu, MF_STRING, ID_ABOUT, L"About");

	HWND hWindow = CreateWindowEx(NULL,
		L"My Window Class", 
		L"Mini Paint", 
		WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
		500, 
		250, 
		640, 
		480, 
		NULL,
		MainMenu,
		hInstance, 
		NULL);

	painter.hideScrollBar(hWindow);

	if (!hWindow)
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Окно не было создано!", L"Ошибка", MB_ICONERROR);
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

void OnDropFiles(HWND hWnd, HDROP hDrop)
{
	TCHAR szFileName[MAX_PATH];
	DWORD dwCount = DragQueryFile(hDrop, 0xFFFFFFFF, szFileName, MAX_PATH);
	for (int i = 0; i < dwCount; i++)
	{
		DragQueryFile(hDrop, i, szFileName, MAX_PATH);
		MessageBox(NULL, szFileName, NULL, 0);
	}
	DragFinish(hDrop);
}

INT_PTR CALLBACK AboutProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		default:
		{
			EndDialog(hwnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
		}
	}

	return (INT_PTR)FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc;		
	static POINTS ptsBegin;		
	static POINTS ptsEnd;			
	static BOOL fPrevLine = FALSE;	
	static int k = 0;
	static HDC memDC;
	static HBITMAP memBM;

	switch (uMsg)
	{
		case WM_CREATE:
		{
			hdc = GetDC(hwnd);
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			DragAcceptFiles(hwnd, TRUE);
			LOGFONT font;
			font.lfHeight = 12;
			font.lfWidth = 5;
			font.lfEscapement = 0;
			font.lfOrientation = 0;
			font.lfWeight = 100;
			font.lfCharSet = RUSSIAN_CHARSET;
			font.lfOutPrecision = 0;
			font.lfClipPrecision = 0;
			font.lfQuality = 100;
			font.lfPitchAndFamily = 5;

			hfont = CreateFontIndirect(&font);
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
			SelectObject(memDC, hPen);
			ReleaseDC(hwnd, hdc);
			break;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_ABOUT:
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutProc);
					break;
				}
				case ID_SAVE_AS_FILE:
				{
					OPENFILENAME ofn;       
					char szFile[260];       

					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hwnd;
					ofn.lpstrFile = (LPWSTR)szFile;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = L"EMF\0*.emf\0";
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


					if (GetSaveFileName(&ofn) == TRUE)
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
						HDC hdcMeta = CreateEnhMetaFile(hdc, ofn.lpstrFile, &rect, L"Example metafile\0");
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
						DeleteEnhMetaFile(meta);
						ReleaseDC(hwnd, hdcMeta);
					}
					break;
				}
				case ID_OPEN_FILE:
				{
					hdc = GetDC(hwnd);
					OPENFILENAME ofn;       
					char szFile[260];       

					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hwnd;
					ofn.lpstrFile = (LPWSTR)szFile;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = L"EMF\0*.emf\0";
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


					if (GetOpenFileName(&ofn) == TRUE)
					{
						openFileName = ofn.lpstrFile;
						//openFileName = ofn.lpstrFile;
						HENHMETAFILE hemf = GetEnhMetaFile(ofn.lpstrFile);
						RECT rect;
						GetClientRect(hwnd, &rect);
						PlayEnhMetaFile(hdc, hemf, &rect);
						StretchBlt(memDC, 0, 0, GetDeviceCaps(hdc, HORZRES),
							GetDeviceCaps(hdc, VERTRES), hdc, 0, 0,
							GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
						DeleteEnhMetaFile(hemf);
					}
					ReleaseDC(hwnd, hdc);
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
					
					HDC hdcMeta = CreateEnhMetaFile(hdc, openFileName, &rect, L"Example metafile\0");
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
					DeleteEnhMetaFile(meta);
					ReleaseDC(hwnd, hdcMeta);
					break;
				}
				case ID_PRINT_FILE:
				{
					if (!isPrint)
					{
						currentId = ID_RECT;
						isPrint = TRUE;
					}
					else
					{
						OPENFILENAME ofn;
						char szFile[260];

						ZeroMemory(&ofn, sizeof(ofn));
						ofn.lStructSize = sizeof(ofn);
						ofn.hwndOwner = hwnd;
						ofn.lpstrFile = (LPWSTR)szFile;
						ofn.lpstrFile[0] = '\0';
						ofn.nMaxFile = sizeof(szFile);
						ofn.lpstrFilter = L"EMF\0*.emf\0";
						ofn.nFilterIndex = 1;
						ofn.lpstrFileTitle = NULL;
						ofn.nMaxFileTitle = 0;
						ofn.lpstrInitialDir = NULL;
						ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


						if (GetSaveFileName(&ofn) == TRUE)
						{
							hdc = GetDC(hwnd);

							RECT rect;
							GetClientRect(hwnd, &rect);
							int iWidthMM = GetDeviceCaps(hdc, HORZSIZE);
							int iHeightMM = GetDeviceCaps(hdc, VERTSIZE);
							int iWidthPels = GetDeviceCaps(hdc, HORZRES);
							int iHeightPels = GetDeviceCaps(hdc, VERTRES);
							rect.left = (0 * iWidthMM * 100) / iWidthPels;
							rect.top = (0 * iHeightMM * 100) / iHeightPels;
							rect.right = (abs(ptsEnd.x - ptsBegin.x) * iWidthMM * 100) / iWidthPels;
							rect.bottom = (abs(ptsEnd.y - ptsBegin.y) * iHeightMM * 100) / iHeightPels;
							/*rect.left = 0;
							rect.top = 0;
							rect.bottom = abs(ptsEnd.y - ptsBegin.y);
							rect.right = abs(ptsEnd.x - ptsBegin.x);*/
							HDC hdcMeta = CreateEnhMetaFile(hdc, ofn.lpstrFile, &rect, L"Example metafile\0");
							if (!hdcMeta)
							{
								MessageBox(NULL, L"CreateEnhMetaFile!", L"Error", MB_ICONERROR);
							}
							StretchBlt(hdcMeta, 0, 0, abs(ptsEnd.x - ptsBegin.x),
								abs(ptsEnd.y - ptsBegin.y), memDC, ptsBegin.x, ptsBegin.y,
								abs(ptsEnd.x - ptsBegin.x), abs(ptsEnd.y - ptsBegin.y), SRCCOPY);
							SetMapMode(hdcMeta, MM_ANISOTROPIC);
							ReleaseDC(hwnd, hdc);
							HENHMETAFILE meta = CloseEnhMetaFile(hdcMeta);
							DeleteEnhMetaFile(meta);
							ReleaseDC(hwnd, hdcMeta);
							currentId = 0;
							isPrint = FALSE;
						}
					}
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
				case ID_LINE_1px:
				{
					penWidth = 1;
					hPen = CreatePen(PS_SOLID, 1, color);
					break;
				}
				case ID_LINE_3px:
				{
					penWidth = 3;
					hPen = CreatePen(PS_SOLID, 3, color);
					break;
				}
				case ID_LINE_7px:
				{
					penWidth = 7;
					hPen = CreatePen(PS_SOLID, 7, color);
					break;
				}
				case ID_LINE_10px:
				{
					penWidth = 10;
					hPen = CreatePen(PS_SOLID, 10, color);
					break;
				}
				case ID_COLOR_LINE:
				{
					CHOOSECOLOR cc = { 0 };
					cc.hwndOwner = hwnd;
					cc.lStructSize = sizeof(cc);
					COLORREF cust_colors[16] = { 0 };
					cc.lpCustColors = cust_colors;

					if (ChooseColor(&cc)) {
						color = cc.rgbResult;
						hPen = CreatePen(PS_SOLID, penWidth, color);
					}
					break;
				}
				case ID_NO_COLOR_FILL:
				{
					isFill = FALSE;
					break;
				}
				case ID_COLOR_FILL:
				{
					CHOOSECOLOR cc = { 0 };
					cc.hwndOwner = hwnd;
					cc.lStructSize = sizeof(cc);
					COLORREF cust_colors[16] = { 0 };
					cc.lpCustColors = cust_colors;

					if (ChooseColor(&cc)) {
						colorFill = cc.rgbResult;
						hBrush = CreateSolidBrush(colorFill);
						isFill = TRUE;

					}
					break;
				}
			}

			break;
		}

		case WM_DESTROY: 
		{
			DeleteObject(memBM);
			PostQuitMessage(0);
			return 0;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			OutputDebugStringW(L"LBUTTONDOWN\n");

			hdc = GetDC(hwnd);
			SelectObject(hdc, hPen);
			SelectObject(memDC, hPen);
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
			break;
		}

		case WM_RBUTTONDOWN:
		{
			OutputDebugStringW(L"RBUTTONDOWN\n");

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
			break;
		}

		case WM_MOUSEMOVE:
		{
			SelectObject(hdc, hPen);
			SelectObject(memDC, hPen);

			switch (currentId)
			{
				case ID_PENCIL:
				{
					if (wParam & MK_LBUTTON)
					{
						hdc = GetDC(hwnd);
						SelectObject(hdc, hPen);
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
						SelectObject(hdc, hPen);
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
						SelectObject(hdc, hPen);
						if (isFill)
							SelectObject(hdc, hBrush);
						painter.drawRectangle(hdc, memDC, ptsBegin, &ptsEnd, zoom, fPrevLine, lParam, isFill);
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
						SelectObject(hdc, hPen);
						if(isFill)
							SelectObject(hdc, hBrush);
						painter.drawEllipse(hdc, memDC, ptsBegin, &ptsEnd, zoom, fPrevLine, lParam, isFill);
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
						SelectObject(hdc, hPen);
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
						SelectObject(hdc, hPen);
						SelectObject(hdc, hBrush);
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
			}
			else if (currentId == ID_RECT)
			{
				if (!isPrint)
				{
					if (!isFill)
						SelectObject(memDC, GetStockObject(NULL_BRUSH));
					else
						SelectObject(memDC, hBrush);
					ptsEnd = MAKEPOINTS(lParam);
					Rectangle(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, ptsEnd.x / zoom, ptsEnd.y / zoom);
				}
				else
				{
					SetROP2(hdc, R2_NOTXORPEN); //R2_NOTXORPEN
					ptsEnd = MAKEPOINTS(lParam);
					Rectangle(hdc, ptsBegin.x / zoom, ptsBegin.y / zoom, ptsEnd.x / zoom, ptsEnd.y / zoom);

					SetROP2(hdc, R2_COPYPEN); 
					SendMessage(hwnd, WM_COMMAND, ID_PRINT_FILE, 0);
				}
			}
			else if (currentId == ID_ELLIPSE)
			{
				if (!isFill)
					SelectObject(memDC, GetStockObject(NULL_BRUSH));
				else
					SelectObject(memDC, hBrush);
				ptsEnd = MAKEPOINTS(lParam);
				Ellipse(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, ptsEnd.x / zoom, ptsEnd.y / zoom);
			}
			else if (currentId == ID_CURVE)
			{
				fPrevLine = FALSE;
				ptsEnd = MAKEPOINTS(lParam);
				MoveToEx(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, (LPPOINT)NULL);
				LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
				ptsBegin = ptsEnd;

			}
			else if (currentId == ID_POLY)
			{
				fPrevLine = FALSE;
				ptsEnd = MAKEPOINTS(lParam);
				if (!isFill)
					SelectObject(memDC, GetStockObject(NULL_BRUSH));
				else
					SelectObject(memDC, hBrush);
				if (firstLine)
				{
					MoveToEx(memDC, startPointPoly.x / zoom, startPointPoly.y / zoom, (LPPOINT)NULL);
					LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
					firstLine = FALSE;
				}
				MoveToEx(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, (LPPOINT)NULL);
				LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
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
				SelectObject(memDC, hfont);
				TextOut(memDC, ptsBegin.x, ptsBegin.y, (LPCWSTR)text, sizeof(text));
				ptsBegin.x += 8;
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
					if (zoom == 1)
						painter.hideScrollBar(hwnd);
					SendMessage(hwnd, WM_PAINT, 0, 0);
				}
				else
				{
					zoom += 0.05; POINT lp;
					RECT rect; GetDCOrgEx(hdc, &lp);
					GetWindowRect(hwnd, &rect);
					HBITMAP hbm = (HBITMAP)GetCurrentObject(memDC, OBJ_BITMAP);
					BITMAP bm;
					GetObject(hbm, sizeof(bm), (LPVOID)&bm);
					int widht = bm.bmWidth; //ширина картинки в 
					int height = bm.bmHeight;//высота картинки
					
					if (zoom <= 1)
						painter.hideScrollBar(hwnd);
					else
					{
						painter.scrollBarSetParams(hwnd, zoom);
						painter.showScrollBar(hwnd);
					}
					SendMessage(hwnd, WM_PAINT, 0, 0);
				}
			}
			else
			{

			}
			break;
		}

		case WM_DROPFILES:
		{
			hdc = GetDC(hwnd);
			static HDROP hdrop;
			RegisterDragDrop(hwnd, (LPDROPTARGET)hdrop);

			OnDropFiles(hwnd, hdrop);
			/*DragQueryFile(hdrop, 0, openFileName, 16);
			HENHMETAFILE hemf = GetEnhMetaFile(openFileName);
			RECT rect;
			GetClientRect(hwnd, &rect);
			PlayEnhMetaFile(hdc, hemf, &rect);
			//EnumEnhMetaFile(hdc, hemf, EnhMetaFileProc, NULL, &rect);
			StretchBlt(memDC, 0, 0, GetDeviceCaps(hdc, HORZRES),
				GetDeviceCaps(hdc, VERTRES), hdc, 0, 0,
				GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
			DeleteEnhMetaFile(hemf);*/
			ReleaseDC(hwnd, hdc);
			break;
		}

		case WM_KEYDOWN:
		{
			if (currentId == ID_TEXT)
			{
				switch (wParam)
				{
					case VK_HOME : case VK_END: case VK_PRIOR:
					case VK_NEXT: case VK_LEFT: case VK_RIGHT:
					case VK_DOWN: case VK_UP: case VK_DELETE:
					case VK_SHIFT: case VK_CONTROL: case VK_CAPITAL:
					case VK_MENU: case VK_TAB: case VK_RETURN:
						break;
					case VK_BACK:
					{
						text[0] = ' ';
						ptsBegin.x -= 9;
						SendMessage(hwnd, WM_PAINT, 0, 0);
						break;
					}
					default:
					{
						text[0] = (char)wParam;
						SendMessage(hwnd, WM_PAINT, 0, 0);
					}
				}
			}
			break;
		}
	
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}