#pragma once
#include <Windows.h>

class Painter
{
public:
	Painter();
	~Painter();

	void drawPencil(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS ptsEnd, double zoom);
	void drawLine(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam);
	void drawRectangle(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam);
	void drawEllipse(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam);
	void drawCurve(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam);
	void drawPoly(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, POINTS startPointPoly, double zoom, bool fPrevLine, LPARAM lParam);
};

