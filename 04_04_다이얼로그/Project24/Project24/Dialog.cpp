#include <windows.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("InfoDlg");


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

TCHAR Red[10];
TCHAR Blue[10];

#define BSIZE 10

BOOL CALLBACK InfoDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hBrush, OldBrush;
	static int x, y, xspeed, yspeed;
	static bool timer;
	static int R, G, B;
	

	switch (iMessage) {
	case WM_INITDIALOG:
		SetTimer(hDlg, 1, 1, NULL);
		timer = FALSE;
		xspeed = 2;
		yspeed = 2;
		x = 100;
		y = 100;
		SetDlgItemText(hDlg, IDC_RADIO1, Red);
		SetDlgItemText(hDlg, IDC_RADIO2, Blue);
		return 0;
	case WM_TIMER:
		GetClientRect(hDlg, &rect);
		if (timer)
		{
			x += xspeed;
			y += yspeed;
			if (y+BSIZE == rect.bottom || y-BSIZE == rect.top)
			{
				yspeed *= -1;
			}
			if (x+BSIZE == rect.right || x-BSIZE == rect.left)
			{
				xspeed *= -1;
			}
			InvalidateRect(hDlg, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		hBrush = CreateSolidBrush(RGB(R, G, B));
		OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);

		EndPaint(hDlg, &ps);
	case WM_COMMAND:
		InvalidateRect(hDlg, NULL, FALSE);
		switch (LOWORD(wParam)) {
		case ID_START:
			timer = TRUE;
			break;
		case ID_STOP:
			timer = FALSE;
			break;
		case ID_QUIT:
			EndDialog(hDlg, ID_QUIT);
			break;
		case IDC_RADIO1:
			R = 255;
			G = B = 0;
			break;
		case IDC_RADIO2:
			R = G = 0;
			B = 255;
			break;
		}
		break;
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		lstrcpy(Red, TEXT("Red"));
		lstrcpy(Blue, TEXT("Blue"));
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_40001:
			if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, InfoDlgProc) == IDOK)
				InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_40002:
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

