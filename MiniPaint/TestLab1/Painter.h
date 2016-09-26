#pragma once
#include <Windows.h>

class Painter
{
public:
	Painter();
	~Painter();

	void drawPencil(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS ptsEnd, double zoom);
	void drawLine(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam);
	void drawRectangle(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam, bool isFill);
	void drawEllipse(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam, bool isFill);
	void drawCurve(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam);
	void drawPoly(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, POINTS startPointPoly, double zoom, bool fPrevLine, LPARAM lParam);

	void hideScrollBar(HWND hwnd);
	void showScrollBar(HWND hwnd);
	void scrollBarSetParams(HWND hwnd, double zoom);
};

