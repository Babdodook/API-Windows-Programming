#include <windows.h>
#include"resource.h"
#pragma comment(lib, "Msimg32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Dlg1(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("ÀÌÀç¼º_2015032111");


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

#define BSIZE 10

BOOL CALLBACK Dlg1(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	BITMAP bit;

	static int bx, by;
	static int x, y, xspeed, yspeed;
	static bool timer;

	switch (iMessage) {
	case WM_INITDIALOG:
		SetTimer(hDlg, 1, 1, NULL);
		timer = TRUE;
		xspeed = 2;
		yspeed = 2;
		x = 100;
		y = 100;
		return 0;
	case WM_TIMER:
		GetClientRect(hDlg, &rect);
		x += xspeed;
		y += yspeed;
		if (y-BSIZE + by == rect.bottom || y - BSIZE == rect.top)
		{
			yspeed *= -1;
		}
		if (x - BSIZE + bx == rect.right || x - BSIZE == rect.left)
		{
			xspeed *= -1;
		}
		InvalidateRect(hDlg, NULL, TRUE);
		return 0;
	case WM_PAINT:
		GetClientRect(hDlg, &rect);
		hdc = BeginPaint(hDlg, &ps);
		MemDC = CreateCompatibleDC(hdc);
		MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);

		GetObject(MyBitmap, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;

		TransparentBlt(hdc, x - BSIZE, y - BSIZE, bx, by, MemDC, 0, 0, bx, by, RGB(255, 255, 255));
		
		SelectObject(MemDC, OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);
		EndPaint(hDlg, &ps);
		break;
	case WM_COMMAND:
		InvalidateRect(hDlg, NULL, FALSE);
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return 0;
		}
		break;
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	HBITMAP MyBitmap2, OldBitmap2;
	BITMAP bit;

	static bool StartGame;
	static bool Goal;
	static int bx, by;
	static int bx2, by2;
	static int x, y;

	switch (iMessage) {
	case WM_CREATE:
		StartGame = false;
		Goal = false;
		x = y = 0;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (StartGame)
		{
			GetClientRect(hWnd, &rect);
			MemDC = CreateCompatibleDC(hdc);

			MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
			OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);

			GetObject(MyBitmap, sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			//BitBlt(hdc, x-BSIZE, y-BSIZE, bx, by, MemDC, 0, 0, SRCCOPY);
			TransparentBlt(hdc, x, y, bx, by, MemDC, 0, 0, bx, by, RGB(255, 255, 255));

			MyBitmap2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			OldBitmap2 = (HBITMAP)SelectObject(MemDC, MyBitmap2);

			GetObject(MyBitmap2, sizeof(BITMAP), &bit);
			bx2 = bit.bmWidth;
			by2 = bit.bmHeight;

			BitBlt(hdc, rect.right - bx2, rect.bottom - by2, bx2, by2, MemDC, 0, 0, SRCCOPY);

			SelectObject(MemDC, OldBitmap);
			DeleteObject(MyBitmap);
			SelectObject(MemDC, OldBitmap2);
			DeleteObject(MyBitmap2);
			DeleteDC(MemDC);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (Goal)
		{
			TextOut(hdc, 50, 50, TEXT("°ñÀÎ"), 4);
		}

		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, NULL, TRUE);
		switch (wParam)
		{
		case VK_UP:
			y -= 20;
			break;
		case VK_DOWN:
			y += 20;
			break;
		case VK_LEFT:
			x -= 20;
			break;
		case VK_RIGHT:
			x += 20;
			break;
		}

		if (x + bx > rect.right - bx2 && y + by > rect.bottom - by2)
		{
			Goal = true;
			StartGame = false;
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_40001:
			if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg1) == IDOK)
				InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_40002:
			StartGame = true;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

