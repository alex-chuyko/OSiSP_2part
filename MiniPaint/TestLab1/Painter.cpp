#include "Painter.h"


Painter::Painter()
{
}


Painter::~Painter()
{
}

void Painter::drawPencil(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS ptsEnd, double zoom)
{
	MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
	LineTo(hdc, ptsEnd.x, ptsEnd.y);

	MoveToEx(memDC, ptsBegin.x / zoom, ptsBegin.y / zoom, (LPPOINT)NULL);
	LineTo(memDC, ptsEnd.x / zoom, ptsEnd.y / zoom);
}

void Painter::drawLine(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam)
{
	if (fPrevLine)
	{
		SetROP2(hdc, R2_NOTXORPEN); //R2_NOTXORPEN

		MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
		LineTo(hdc, ptsEnd->x, ptsEnd->y);
		StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
			GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
			GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
		SetROP2(hdc, R2_COPYPEN);
	}
	*ptsEnd = MAKEPOINTS(lParam);

	MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
	LineTo(hdc, ptsEnd->x, ptsEnd->y);
}

void Painter::drawRectangle(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam)
{
	if (fPrevLine)
	{
		SetROP2(hdc, R2_NOTXORPEN); //R2_NOTXORPEN
		Rectangle(hdc, ptsBegin.x, ptsBegin.y, ptsEnd->x, ptsEnd->y);
		StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
			GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
			GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
		SetROP2(hdc, R2_COPYPEN);

	}
	*ptsEnd = MAKEPOINTS(lParam);

	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, ptsBegin.x, ptsBegin.y, ptsEnd->x, ptsEnd->y);
}

void Painter::drawEllipse(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam)
{
	if (fPrevLine)
	{
		SetROP2(hdc, R2_NOTXORPEN); //R2_NOTXORPEN

		Ellipse(hdc, ptsBegin.x, ptsBegin.y, ptsEnd->x, ptsEnd->y);
		StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
			GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
			GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
		SetROP2(hdc, R2_COPYPEN);

	}
	*ptsEnd = MAKEPOINTS(lParam);

	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Ellipse(hdc, ptsBegin.x, ptsBegin.y, ptsEnd->x, ptsEnd->y);
}

void Painter::drawCurve(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, double zoom, bool fPrevLine, LPARAM lParam)
{
	if (fPrevLine)
	{
		SetROP2(hdc, R2_NOTXORPEN); //R2_NOTXORPEN
		MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
		LineTo(hdc, ptsEnd->x, ptsEnd->y);
		StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
			GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
			GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
		SetROP2(hdc, R2_COPYPEN);
	}
	*ptsEnd = MAKEPOINTS(lParam);

	MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
	LineTo(hdc, ptsEnd->x, ptsEnd->y);
}

void Painter::drawPoly(HDC hdc, HDC memDC, POINTS ptsBegin, POINTS *ptsEnd, POINTS startPointPoly, double zoom, bool fPrevLine, LPARAM lParam)
{
	if (fPrevLine)
	{
		SetROP2(hdc, R2_NOTXORPEN); //R2_NOTXORPEN
		MoveToEx(hdc, startPointPoly.x, startPointPoly.y, (LPPOINT)NULL);
		LineTo(hdc, ptsEnd->x, ptsEnd->y);
		MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
		LineTo(hdc, ptsEnd->x, ptsEnd->y);
		StretchBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES)*zoom,
			GetDeviceCaps(hdc, VERTRES)*zoom, memDC, 0, 0,
			GetDeviceCaps(memDC, HORZRES), GetDeviceCaps(memDC, VERTRES), SRCCOPY);
		SetROP2(hdc, R2_COPYPEN);
	}
	*ptsEnd = MAKEPOINTS(lParam);

	MoveToEx(hdc, startPointPoly.x, startPointPoly.y, (LPPOINT)NULL);
	LineTo(hdc, ptsEnd->x, ptsEnd->y);
	MoveToEx(hdc, ptsBegin.x, ptsBegin.y, (LPPOINT)NULL);
	LineTo(hdc, ptsEnd->x, ptsEnd->y);
}