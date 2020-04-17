#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Menu");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);


	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#define BSIZE 40

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH MyBrush, OldBrush;

	static BOOL selectmap;
	static BOOL ellipse;
	static BOOL rectangle;
	static BOOL ex;
	static BOOL nowDraw;

	static int mapx, mapy;
	static int x, y;
	static int oldx, oldy;

	switch (iMessage) {
	case WM_CREATE:
		mapx = mapy = 0;
		selectmap = FALSE;
		ellipse = FALSE;
		nowDraw = FALSE;
		return 0;
	case WM_COMMAND:
		InvalidateRect(hWnd, NULL, TRUE);
		switch (LOWORD(wParam)) {
		case ID_40001: //4x8
			selectmap = TRUE;
			mapx = 8;
			mapy = 4;
			break;
		case ID_40002: //8x16
			selectmap = TRUE;
			mapx = 16;
			mapy = 8;
			break;
		case ID_40003: //원
			mapx = mapy = 0;
			ellipse = TRUE;
			rectangle = FALSE;
			ex = FALSE;
			break;
		case ID_40004: //직사각형
			mapx = mapy = 0;
			ellipse = FALSE;
			rectangle = TRUE;
			ex = FALSE;
			break;
		case ID_40005: //직선
			mapx = mapy = 0;
			ellipse = FALSE;
			rectangle = FALSE;
			ex = TRUE;
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (selectmap)
		{
			SetMapMode(hdc, MM_ANISOTROPIC);
			SetWindowExtEx(hdc, mapx*BSIZE, mapy*BSIZE, NULL);
			GetClientRect(hWnd, &rect);
			SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);
		}

		for (int i = 0; i < mapx; i++)
		{
			for (int j = 0; j < mapy; j++)
			{
				Rectangle(hdc, i*BSIZE, j*BSIZE, (i + 1)*BSIZE, (j + 1)*BSIZE);
			}
		}

		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		oldx = x;
		oldy = y;

		nowDraw = TRUE;
		return 0;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOT);
		MyBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);

		if (nowDraw)
		{
			if (ellipse)
			{
				Ellipse(hdc, x, y, oldx, oldy);
				oldx = LOWORD(lParam);
				oldy = HIWORD(lParam);
				Ellipse(hdc, x, y, oldx, oldy);
			}

			if (rectangle)
			{
				Rectangle(hdc, x, y, oldx, oldy);
				oldx = LOWORD(lParam);
				oldy = HIWORD(lParam);
				Rectangle(hdc, x, y, oldx, oldy);
			}

			if (ex)
			{
				MoveToEx(hdc, x, y, NULL);
				LineTo(hdc, oldx, oldy);
				oldx = LOWORD(lParam);
				oldy = HIWORD(lParam);
				MoveToEx(hdc, x, y, NULL);
				LineTo(hdc, oldx, oldy);
			}
		}

		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);
		MyBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);

		if (ellipse)
		{
			Ellipse(hdc, x, y, oldx, oldy);
		}

		if (rectangle)
		{
			Rectangle(hdc, x, y, oldx, oldy);
		}

		if (ex)
		{
			MoveToEx(hdc, x, y, NULL);
			LineTo(hdc, oldx, oldy);
		}

		nowDraw = FALSE;
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
